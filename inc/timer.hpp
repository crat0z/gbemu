#include <chrono>

template<size_t N>
class Timer {
    typedef std::chrono::steady_clock                       clock;
    typedef std::chrono::duration<double, std::ratio<1, N>> duration;

    clock::time_point start;

    double count;

public:
    Timer() : start{ clock::now() }, count{ 0 } {}

    bool update() {
        // how many ticks have gone by since starting?
        duration elapsed = clock::now() - start;

        // compare to our own counter, increment ours if the real time
        // is greater than our counter, return true if so
        if (elapsed.count() > count) {
            count += 1.0;
            return true;
        }
        else {
            return false;
        }
    }
};