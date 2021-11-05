#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include "chip8.hpp"
#include "opcodes.hpp"

struct Instruction {
    uint16_t    address;
    uint16_t    opcode;
    op          operation;
    std::string mnemonic;

    Instruction() = default;

    Instruction(uint16_t addr)
            : address{ addr }, opcode{ 0 }, operation{ op::UNKNOWN }, mnemonic{ "" } {}

    Instruction(uint16_t addr, uint16_t opc)
            : address{ addr },
              opcode{ opc },
              operation{ decode(opc) },
              mnemonic{ opcode_instruction(opc) } {}
};

struct basic_block {
    uint16_t                 start_address;
    uint16_t                 end_address;
    std::vector<Instruction> instructions;

    std::vector<std::pair<uint16_t, uint16_t>> references;

    // std::vector<std::shared_ptr<basic_block>> from_blocks;
    /* to_block_true is an unconditional if to_block_false does not have a value */
    std::shared_ptr<basic_block> to_block_true;

    std::optional<std::shared_ptr<basic_block>> to_block_false;

    basic_block() = default;

    bool is_indirect_jump() { return instructions.end()->operation == op::JP_V0; }
    bool end_is_ret() { return instructions.end()->operation == op::RET; }
    bool is_unconditional() const noexcept { return !to_block_false.has_value(); }

    std::shared_ptr<basic_block>                true_branch() { return to_block_true; }
    std::optional<std::shared_ptr<basic_block>> false_branch() { return to_block_true; }

    // append an instruction
    void append(uint16_t addr, uint16_t opc) {
        if (instructions.size() == 0) {
            start_address = addr;
        }
        instructions.emplace_back(addr, opc);
        end_address = addr;
    }

    // "merge" or consume another basic block, appending all of
    // other's instructions into this, and adjusting others appropriately.
    // do not use other after this call
    void merge(basic_block&& other) {

        // add all instructions
        instructions.insert(instructions.end(), std::make_move_iterator(other.instructions.begin()),
                            std::make_move_iterator(other.instructions.end()));

        // add all references
        for (auto& i : other.references) {
            references.emplace_back(std::move(i));
        }
        // adjust end to new end
        end_address = other.end_address;
        // and get outs
        to_block_true  = other.to_block_true;
        to_block_false = other.to_block_false;
    }

    // split THIS block into two, starting from given address
    // returns a basic block, containing the instructions before the split point
    // since this is a member function, splitting requires caller to add reference
    // from new to original.
    std::shared_ptr<basic_block> split(uint16_t split_address) {

        std::shared_ptr new_block = std::make_shared<basic_block>();

        // new block's start is original's
        new_block->start_address = start_address;

        // take all of the instructions before split point
        auto it = instructions.begin();
        while (it->address != split_address) {
            // add
            new_block->instructions.push_back(*it);
            // erase old it
            it = instructions.erase(it);
        }
        // copy all of the necessary references
        for (auto& p : references) {
            if (p.second < split_address) {
                new_block->references.emplace_back(p);
            }
        }

        // delete references in orig now
        references.erase(std::remove_if(references.begin(), references.end(),
                                        [&](auto& pair) { return pair.second < split_address; }),
                         references.end());

        // new_block contains its instructions now, so we can fill out rest
        new_block->end_address = split_address - 2;

        // fix up orig_block now
        start_address = split_address;

        return new_block;
    }
};

class Disassembler {

    std::unordered_map<uint16_t, std::shared_ptr<basic_block>> done;

    Chip8& proc;

    void update_references(std::shared_ptr<basic_block> old, std::shared_ptr<basic_block> now);

    bool is_jump_or_ret(op val);
    bool is_call(op val);

    std::shared_ptr<basic_block> rec_cfg(uint16_t start_address, uint16_t from_address);

public:
    std::vector<std::shared_ptr<basic_block>> control_flow_graph;

    std::unordered_map<uint16_t, Instruction> found_instructions;

    Disassembler(Chip8& p);
    void analyze();
};

#endif