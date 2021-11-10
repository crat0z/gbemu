#ifndef STACK_HPP
#define STACK_HPP

#include <vector>
#include <cstdint>
#include <mutex>

// normally i would use a std::stack or std::deque out of laziness, but debugger
// requires viewing the stack of emulator while running, and it would be silly
// to pop/push a million times just to look at its contents

template<typename T>
class Stack {

    std::vector<T> data;

public:
    // with the way imgui works, we'll have to read every frame to draw. it is possible
    // for there to be reading the stack by debugger and resizing of data vector, which
    // could segfault
    std::mutex mut;

    Stack(size_t res = 16) { data.reserve(res); }

    size_t size() const noexcept { return data.size(); }

    void push_back(T val) noexcept { data.push_back(val); }

    T pop_back() noexcept {
        auto ret = data.back();
        data.pop_back();

        return ret;
    }

    T& back() noexcept { return data.back(); }

    void emplace_back(T val) noexcept { data.emplace_back(val); }

    T& operator[](size_t index) noexcept { return data[index]; }

    bool empty() const noexcept { return data.empty(); }

    std::vector<T>::iterator       begin() noexcept { return data.begin(); }
    std::vector<T>::iterator       end() noexcept { return data.end(); }
    std::vector<T>::const_iterator cbegin() noexcept { return data.cbegin(); }
    std::vector<T>::const_iterator cend() noexcept { return data.cend(); }

    std::vector<T>::reverse_iterator       rbegin() noexcept { return data.rbegin(); }
    std::vector<T>::reverse_iterator       rend() noexcept { return data.rend(); }
    std::vector<T>::const_reverse_iterator crbegin() noexcept { return data.crbegin(); }
    std::vector<T>::const_reverse_iterator crend() noexcept { return data.crend(); }
};

#endif