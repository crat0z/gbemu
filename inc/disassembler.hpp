#ifndef DISASSEMBLER_HPP
#define DISASSEMBLER_HPP
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include "debugger.hpp"
#include "opcodes.hpp"

struct Instruction {
    uint16_t    address;
    uint16_t    opcode;
    op          operation;
    uint8_t     length;
    std::string mnemonic;

    Instruction() = default;

    Instruction(const Instruction& rhs) = default;
    Instruction& operator=(const Instruction& rhs) = default;

    Instruction(Instruction&& rhs) = default;
    Instruction& operator=(Instruction&& rhs) = default;

    ~Instruction() = default;

    Instruction(uint16_t addr)
            : address{ addr }, opcode{ 0 }, operation{ op::UNKNOWN }, length{ 0 }, mnemonic{ "" } {}

    // length = 2 always at this point, change in future
    Instruction(uint16_t addr, uint16_t opc)
            : address{ addr },
              opcode{ opc },
              operation{ decode(opc) },
              length{ 2 },
              mnemonic{ opcode_mnemonic(opc) } {}
};

struct basic_block {
    uint16_t start_address;
    uint16_t end_address;

    std::vector<Instruction> instructions;
    // references are pairs of from_address, to_address where to_address
    // is always within the range of this basic block
    std::vector<std::pair<uint16_t, uint16_t>> references;

    /* to_block_true is an unconditional if to_block_false does not have a value */
    std::shared_ptr<basic_block>                to_block_true;
    std::optional<std::shared_ptr<basic_block>> to_block_false;

    basic_block() = default;

    std::shared_ptr<basic_block>                true_branch() { return to_block_true; }
    std::optional<std::shared_ptr<basic_block>> false_branch() { return to_block_true; }

    void add_reference(uint16_t from_address, uint16_t to_address) {
        // make sure that this is a valid reference
        for (auto& ins : instructions) {
            if (ins.address == to_address) {
                references.emplace_back(from_address, to_address);
            }
        }
    }

    // append an instruction
    void append(uint16_t addr, uint16_t opc) { append(Instruction(addr, opc)); }

    // append an instruction rvalue
    void append(Instruction&& rv) {
        if (instructions.size() == 0) {
            start_address = rv.address;
        }
        end_address = rv.address;
        instructions.emplace_back(rv);
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
    // returns a basic block, containing the instructions before the split point.
    // second parameter is necessary so split can link new block to this block post split
    std::shared_ptr<basic_block> split(uint16_t                     split_address,
                                       std::shared_ptr<basic_block> this_ptr) {

        std::shared_ptr new_block = std::make_shared<basic_block>();

        // new block's start is original's
        new_block->start_address = this->start_address;

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
                new_block->add_reference(p.first, p.second);
            }
        }

        // delete references in orig now
        references.erase(std::remove_if(references.begin(), references.end(),
                                        [&](auto& pair) { return pair.second < split_address; }),
                         references.end());

        // new_block contains its instructions now, so we can fill out rest
        // FIXME LOGIC hardcoded difference of 2
        new_block->end_address = split_address - 2;

        // fix up orig_block now
        this->start_address = split_address;

        // add a reference from new_block to orig_block
        this->add_reference(new_block->end_address, this->start_address);

        // our new_block now has unconditional jump
        new_block->to_block_true = this_ptr;

        return new_block;
    }
};

class Disassembler {

    Debugger& proc;

    std::shared_ptr<basic_block> rec_cfg(uint16_t start_address, uint16_t from_address);

public:
    std::vector<std::shared_ptr<basic_block>> control_flow_graph;
    std::unordered_map<uint16_t, Instruction> found_instructions;

    Disassembler(Debugger& p);
    void analyze();
};

#endif