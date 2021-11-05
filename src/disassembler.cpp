#include "disassembler.hpp"
#include <unordered_map>
#include <iostream>

disassembler::disassembler(Chip8& p) : proc(p) {}

bool disassembler::is_call(op val) {
    if (val == op::CALL || val == op::SYS)
        return true;
    return false;
}
// also checks for unknown
bool disassembler::is_jump_or_ret(op val) {
    static std::array<op, 11> jumps = { op::JP,    op::JP_V0, op::SE_I, op::SE_R, op::SNE_I,
                                        op::SNE_R, op::SKP,   op::SKNP, op::RET,  op::UNKNOWN };

    for (auto j : jumps) {
        if (j == val)
            return true;
    }
    return false;
}

void disassembler::update_references(std::shared_ptr<basic_block> old,
                                     std::shared_ptr<basic_block> now) {
    for (auto& bb : result) {
        if (bb->to_block_false == old) {
            bb->to_block_false = now;
        }

        if (bb->to_block_true == old) {
            bb->to_block_true = now;
        }

        if (bb == old) {
            bb = now;
        }
    }
}

std::shared_ptr<basic_block> disassembler::recursive(uint16_t start_address,
                                                     uint16_t from_address) {
    // see if we have already processed this address
    auto found = done.find(start_address);

    // if we havent seen this instruction yet, then we're ok to start a new basic block
    if (found == done.end()) {
        std::shared_ptr curr = std::make_shared<basic_block>();

        result.push_back(curr);

        /*
            when analyzing a new basic block, it is clear that it will at least have a point
            where it came from, unless it is the entry/call
        */
        if (from_address != 0) {
            curr->references.emplace_back(from_address, start_address);
        }
        // start adding instructions to this basic block
        auto current_address = start_address;
        auto current_opcode  = proc.fetch(current_address);

        // current instruction isnt a jump, add it to list for this block
        while (!is_jump_or_ret(decode(current_opcode))) {

            // make sure we arent overrunning into a previous block
            auto check = done.try_emplace(current_address, curr);
            if (check.second) {

                curr->append(current_address, current_opcode);
                /* 
                    add this current address and its associated basic block to our map
                    in case in the future, we find a basic block that jumps to here
                */
                done[current_address] = curr;

                // check to see if it is a call (no branch), process it
                if (is_call(decode(current_opcode))) {
                    auto call_address = current_opcode & 0xFFF;
                    recursive(call_address, current_address);
                }

                // go to next instruction
                current_address += 2;
                current_opcode = proc.fetch(current_address);
            }
            else {
                // we add a reference and end our block
                auto other = done.at(current_address);

                curr->to_block_true = other;
                curr->end_address   = current_address - 2;

                other->references.emplace_back(curr->end_address, other->start_address);

                return curr;
            }
        }

        // now current_opcode is a jump, add it manually

        curr->append(current_address, current_opcode);
        done[current_address] = curr;

        // this is the end for this block
        curr->end_address = current_address;

        switch (decode(current_opcode)) {
        // for unconditional jump, we just process location
        case op::JP: {
            auto jump_address = current_opcode & 0xFFF;

            auto result = recursive(jump_address, current_address);

            done[current_address]->to_block_true = result;
            return done[current_address];
        }
        // indirect jump and unknown opcodes kinda just screw it up..
        case op::UNKNOWN:
        case op::JP_V0:
        case op::RET: {
            return done[current_address];
        }
        /* 
            conditionals all work same way: skip next instruction if true
            then, true path is current_address + 4, false path is + 2
        */
        case op::SE_I:
        case op::SE_R:
        case op::SNE_I:
        case op::SNE_R:
        case op::SKP:
        case op::SKNP: {
            // do in this order, in case false case is not a jump. that way,
            // we split the block

            auto false_result = recursive(current_address + 2, current_address);

            done[current_address]->to_block_false = false_result;

            auto true_result = recursive(current_address + 4, current_address);

            done[current_address]->to_block_true = true_result;

            return done[current_address];
        }
        }
    }
    // otherwise, we're jumping into a basic block already processed/or is being processed
    else {
        auto orig_block = done.at(start_address);

        auto from_block = done.at(from_address);

        // if we're jumping to start of block, just add a reference from_address to start_address
        if (orig_block->start_address == start_address) {
            orig_block->references.emplace_back(from_address, start_address);
            return orig_block;
        }
        else {

            /* 
                otherwise, we must split the block. consider a block like
                
                orig_block
                    INS 1 = orig_block.start_address
                    INS 2
                    INS 3
                    INS 4
                    CONDITIONAL = orig_block.end_address

                suppose our start_address points to INS 4. then, we want to split like

                new_block
                    INS 1 = new_block.start_address
                    INS 2
                    INS 3 = new_block.end_address
                    to_block_true -> orig_block
                    from_blocks = original orig_block.from_blocks
                
                orig_block
                    INS 4 = orig_block.start_address
                    CONDITIONAL = orig_block.end_address
                    from_blocks contains new_block
                    to_block_* are left in original state

                return new_block

                
            */
            auto new_block = orig_block->split(start_address);

            result.push_back(new_block);

            // add reference
            orig_block->references.emplace_back(new_block->end_address, orig_block->start_address);

            // make sure everything in new block points to itself in map
            for (auto& i : new_block->instructions) {
                done[i.address] = new_block;
            }

            return orig_block;
        }
    }
}
/*
    still implementing, in particular calls will ruin things
*/
void disassembler::analyze() {

    /*
        basic algorithm: starting from entry, find all basic blocks, by interpreting
        every instruction, and starting a new basic block when met with a conditional/jump/call

        each basic block can have multiple ins, but only 1 or 2 outs, a struct for this might be
        
        struct basic_block {
            uint16_t start_address;
            uint16_t end_address;
            std::vector<Instruction> instructions;


            std::vector<basic_block&> in;
            // some sort of container that holds either 2 bb& or 1 bb&, not sure yet

        };

        since this is just to have cool call graphs and better disassembly while debugging,
        we might want Instruction struct to look like this
        
        struct Instruction {
            uint16_t address;
            uint16_t ins;
            op operation;
            std::string mnemonic;
        };

        instructions that jump:
        SYS, RET, JP, CALL, SE, SNE, SKP, SKNP

        instructions that are unconditional:
        JP

        instructions that are conditional:
        SE, SNE, SKP, SKNP

        instructons that call:
        SYS, CALL, RET

        JP can jump into the middle of a basic block, which means we'd have to break it
        into two. then, we must contain either a list of every address we've visited, and
        a reference to its basic block, or maybe something recursive idk

        to split a basic block, we iterate over its instructions until we find the split point,
        add the ones we've seen to a new basic block, and remove them from the original, adjusting
        start/end address and ins/outs appropriately
    */

    recursive(0x200, 0);

    std::sort(result.begin(), result.end(),
              [](std::shared_ptr<basic_block> lhs, std::shared_ptr<basic_block> rhs) {
                  return (lhs->start_address < rhs->start_address);
              });
}