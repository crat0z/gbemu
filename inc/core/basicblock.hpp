#ifndef BASICBLOCK_HPP
#define BASICBLOCK_HPP

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include "core/instruction.hpp"

namespace core {

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

        std::shared_ptr<basic_block>                true_branch();
        std::optional<std::shared_ptr<basic_block>> false_branch();

        void add_reference(uint16_t from_address, uint16_t to_address);

        // append an instruction
        void append(uint16_t addr, uint16_t opc);

        // append an instruction rvalue
        void append(Instruction&& rv);
        // "merge" or consume another basic block, appending all of
        // other's instructions into this, and adjusting others appropriately.
        // do not use other after this call
        void merge(basic_block&& other);

        // split THIS block into two, starting from given address
        // returns a basic block, containing the instructions before the split point.
        // second parameter is necessary so split can link new block to this block post split
        std::shared_ptr<basic_block> split(uint16_t                     split_address,
                                           std::shared_ptr<basic_block> this_ptr);
    };
} // namespace core

#endif