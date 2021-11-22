#include <chrono>

// constexpr timer
template<int64_t f>
class CETimer {
    using clock = std::chrono::steady_clock;

public:
    CETimer() {}

    bool update() {

        static auto start = clock::now();

        static std::chrono::duration<int64_t, std::ratio<1, f>> duration{ 1 };

        static auto next_tick = start + duration;

        if (clock::now() > next_tick) {
            next_tick += duration;
            return true;
        }
        return false;
    }

    // just run the timer until it stops returning true, for now
    void reset() {
        while (update())
            ;
    }
};

class Timer {
    using clock = std::chrono::steady_clock;

    clock::time_point start;

    double count;

    double period_ns;

public:
    Timer(size_t freq) : start{ clock::now() }, count{ 0 }, period_ns(1000000000.0 / freq) {}

    bool update() {
        // how many nanosecond ticks have gone by since starting?
        std::chrono::duration<double, std::nano> elapsed = clock::now() - start;

        if (elapsed.count() > count) {
            count = elapsed.count() + period_ns;
            return true;
        }
        return false;
    }
};