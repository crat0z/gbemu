#include <chrono>

// constexpr timer
template<int64_t f>
class CETimer {
    using clock      = std::chrono::steady_clock;
    using time_point = clock::time_point;
    using duration   = std::chrono::duration<int64_t, std::ratio<1, f>>;

    /* time_point start;
    time_point last_tick;

    duration tick{ 1 }; */

public:
    CETimer() = default;

    bool update(auto& tp) {
        /* static bool once = true;
        if (once) {
            start     = clock::now();
            last_tick = start + tp;
        }

        if (tp > last_tick) {
            last_tick += tick;
            return true;
        }
        return false; */

        static auto start = clock::now();

        static std::chrono::duration<int64_t, std::ratio<1, f>> duration{ 1 };

        static auto next_tick = start + duration;

        if (tp > next_tick) {
            next_tick += duration;
            return true;
        }
        return false;
    }

    // just run the timer until it stops returning true, for now
    void reset() {
        //last_tick = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        while (update(now))
            ;
    }
};

/* class Timer {
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
}; */