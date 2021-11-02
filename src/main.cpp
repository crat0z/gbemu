#include <iostream>
#include "chip8.hpp"
#include "structopt.hpp"
#include <bitset>

struct Options {
    std::optional<size_t> cpu_freq = 741;
    std::optional<int>    x_res    = 1280;
    std::optional<int>    y_res    = 640;
    std::string           filename;
};

STRUCTOPT(Options, cpu_freq, x_res, y_res, filename);

int main(int argc, char** argv) {

    try {
        auto options = structopt::app("chip8emu").parse<Options>(argc, argv);

        Chip8 emu(options.filename, *options.cpu_freq);

        emu.run();
    }

    catch (structopt::exception& e) {
        std::cout << e.what() << "\n";
        std::cout << e.help();
    }
}