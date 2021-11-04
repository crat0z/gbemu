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
    uint16_t    ins;
    op          operation;
    std::string mnemonic;
};

struct basic_block {
    uint16_t                 start_address;
    uint16_t                 end_address;
    std::vector<Instruction> instructions;

    std::vector<std::shared_ptr<basic_block>> from_blocks;
    /* to_block_true is an unconditional if to_block_false does not have a value */
    std::shared_ptr<basic_block> to_block_true;

    std::optional<std::shared_ptr<basic_block>> to_block_false;

    basic_block() {}
    bool is_indirect_jump() { return instructions.end()->operation == op::JP_V0; }
    bool end_is_ret() { return instructions.end()->operation == op::RET; }
    bool is_unconditional() const noexcept { return !to_block_false.has_value(); }
};

class disassembler {

    std::unordered_map<uint16_t, std::shared_ptr<basic_block>> done;

    Chip8& proc;

    bool is_jump_or_ret(op val);
    bool is_call(op val);

    std::shared_ptr<basic_block> recursive(uint16_t start_address, uint16_t from_address);

public:
    std::vector<std::shared_ptr<basic_block>> result;

    disassembler(Chip8& p);
    void analyze();
};

#endif