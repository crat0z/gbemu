#include <chrono>

class Timer {
    typedef std::chrono::steady_clock clock;

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