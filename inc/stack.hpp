#ifndef STACK_HPP
#define STACK_HPP

#include <vector>
#include <cstdint>
#include <mutex>

// normally i would use a std::stack or std::deque out of laziness, but debugger
// requires viewing the stack of emulator while running, and it would be silly
// to pop/push a million times just to look at its contents

class Stack {

    std::vector<uint16_t> data;

public:
    // with the way imgui works, we'll have to read every frame to draw. it is possible
    // for there to be reading the stack by debugger and resizing of data vector, which
    // could segfault
    std::mutex mut;

    Stack(size_t res = 16);

    size_t size() const noexcept;

    void push_back(uint16_t val) noexcept;

    uint16_t pop_back() noexcept;

    uint16_t& back() noexcept;

    void emplace_back(uint16_t val) noexcept;

    uint16_t& operator[](size_t index) noexcept;

    bool empty() const noexcept;

    std::vector<uint16_t>::iterator       begin() noexcept;
    std::vector<uint16_t>::iterator       end() noexcept;
    std::vector<uint16_t>::const_iterator cbegin() noexcept;
    std::vector<uint16_t>::const_iterator cend() noexcept;

    std::vector<uint16_t>::reverse_iterator       rbegin() noexcept;
    std::vector<uint16_t>::reverse_iterator       rend() noexcept;
    std::vector<uint16_t>::const_reverse_iterator crbegin() noexcept;
    std::vector<uint16_t>::const_reverse_iterator crend() noexcept;
};

#endif