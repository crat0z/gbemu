#include "core/basicblock.hpp"

#include <utility>

namespace core {

    std::shared_ptr<basic_block> basic_block::true_branch() { return to_block_true; }

    std::optional<std::shared_ptr<basic_block>> basic_block::false_branch() {
        return to_block_true;
    }

    void basic_block::add_reference(uint16_t from_address, uint16_t to_address) {
        // make sure that this is a valid reference
        for (auto& ins : instructions) {
            if (ins.address == to_address) {
                references.emplace_back(from_address, to_address);
            }
        }
    }

    // append an instruction
    void basic_block::append(uint16_t addr, uint16_t opc) { append(Instruction(addr, opc)); }

    // append an instruction rvalue
    void basic_block::append(Instruction&& rv) {
        if (instructions.empty()) {
            start_address = rv.address;
        }
        end_address = rv.address;
        instructions.emplace_back(rv);
    }

    // "merge" or consume another basic block, appending all of
    // other's instructions into this, and adjusting others appropriately.
    // do not use other after this call
    void basic_block::merge(basic_block&& other) {

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
    std::shared_ptr<basic_block> basic_block::split(uint16_t                     split_address,
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
        new_block->to_block_true = std::move(this_ptr);

        return new_block;
    }
} // namespace core