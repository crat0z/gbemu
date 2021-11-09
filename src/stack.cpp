#include "stack.hpp"

Stack::Stack(size_t res) { data.reserve(res); }

size_t Stack::size() const noexcept { return data.size(); }

void Stack::push_back(uint16_t val) noexcept { data.push_back(val); }

uint16_t Stack::pop_back() noexcept {
    auto ret = data.back();
    data.pop_back();

    return ret;
}

uint16_t& Stack::back() noexcept { return data.back(); }

void Stack::emplace_back(uint16_t val) noexcept { data.emplace_back(val); }

uint16_t& Stack::operator[](size_t index) noexcept { return data[index]; }

bool Stack::empty() const noexcept { return data.empty(); }

std::vector<uint16_t>::iterator       Stack::begin() noexcept { return data.begin(); }
std::vector<uint16_t>::iterator       Stack::end() noexcept { return data.end(); }
std::vector<uint16_t>::const_iterator Stack::cbegin() noexcept { return data.cbegin(); }
std::vector<uint16_t>::const_iterator Stack::cend() noexcept { return data.cend(); }

std::vector<uint16_t>::reverse_iterator       Stack::rbegin() noexcept { return data.rbegin(); }
std::vector<uint16_t>::reverse_iterator       Stack::rend() noexcept { return data.rend(); }
std::vector<uint16_t>::const_reverse_iterator Stack::crbegin() noexcept { return data.crbegin(); }
std::vector<uint16_t>::const_reverse_iterator Stack::crend() noexcept { return data.crend(); }