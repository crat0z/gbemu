#include "gui.hpp"
#include <cmath>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

#include <thread>
#include <future>
#include <unordered_map>

#include <DroidSans.hpp>

#include "debugger.hpp"
#include "opcodes.hpp"

constexpr float X_PIXELS = 64.0;
constexpr float Y_PIXELS = 32.0;

std::unordered_map<int, uint8_t> keybinds = { { SDLK_1, 0x1 }, { SDLK_2, 0x2 }, { SDLK_3, 0x3 },
                                              { SDLK_4, 0xC }, { SDLK_q, 0x4 }, { SDLK_w, 0x5 },
                                              { SDLK_e, 0x6 }, { SDLK_r, 0xD }, { SDLK_a, 0x7 },
                                              { SDLK_s, 0x8 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
                                              { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB },
                                              { SDLK_v, 0xF } };

GUI::GUI(const std::string& file_name) : emu(file_name) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    SDL_WindowFlags flags =
            (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1580, 640,
                              flags);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    io.Fonts->AddFontFromMemoryCompressedBase85TTF(DroidSans_compressed_data_base85, 18);

    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);
}

GUI::~GUI() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GUI::prepare_imgui() {
    static bool show_demo_window = false;

    static Debugger debugger(emu);

    ImGuiWindowFlags main_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        ImGui::SetNextWindowSize({ 300, (float)size_x });

        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::Begin("chip8emu", nullptr,
                     main_flags); // Create a window called "Hello, world!" and append into it.
        if (ImGui::BeginTabBar("tabs")) {
            if (ImGui::BeginTabItem("main")) {
                ImGui::Checkbox("pause", &paused);

                ImGui::Text("%.3f fps", ImGui::GetIO().Framerate);
                ImGui::EndTabItem();
            }

            /* two lambdas for 1 and 2 parameter red colored text when variables change */
            auto colored_text = [&](bool val, const char* fmt, uint16_t v) {
                if (val) {
                    ImGui::TextColored({ 255, 0, 0, 255 }, fmt, v);
                }
                else {
                    ImGui::Text(fmt, v);
                }
            };

            auto colored_text2 = [&](bool val, const char* fmt, uint16_t v, uint16_t x) {
                if (val) {
                    ImGui::TextColored({ 255, 0, 0, 255 }, fmt, v, x);
                }
                else {
                    ImGui::Text(fmt, v, x);
                }
            };

            if (ImGui::BeginTabItem("debugger")) {
                ImGui::Text("Registers & Timers");
                // draw registers in table
                auto table_flags = ImGuiTableFlags_Borders;
                if (ImGui::BeginTable("registers", 4, table_flags)) {
                    // draw registers
                    for (auto row = 0; row < 4; ++row) {

                        ImGui::TableNextRow();

                        for (auto col = 0; col < 4; ++col) {
                            ImGui::TableSetColumnIndex(col);

                            auto index = 4 * row + col;

                            colored_text2(debugger.reg_changes[index], "V%02d 0x%02X", (index),
                                          emu.V[index]);
                        }
                    }

                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    colored_text(debugger.I_change, "I  0x%03X", emu.I);

                    ImGui::TableNextColumn();
                    colored_text(debugger.PC_change, "PC 0x%03X", emu.PC);

                    ImGui::TableNextColumn();
                    colored_text(debugger.dt_change, "D  0x%02X", emu.delay_timer);

                    ImGui::TableNextColumn();
                    colored_text(debugger.st_change, "S  0x%02X", emu.sound_timer);

                    ImGui::EndTable();
                }
                ImGui::Separator();

                if (paused) {
                    ImGui::Text("Next opcode: %04X", emu.opcode);

                    ImGui::Separator();

                    ImGui::Text("Next instruction: %s", opcode_instruction(emu.opcode).c_str());

                    ImGui::Separator();
                    // static bool description_header = false;
                    if (ImGui::CollapsingHeader("Description")) {
                        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 300.0f);
                        ImGui::Text("Description: %s", opcode_description(decode(emu.opcode)));
                    }
                }

                if (ImGui::Button("pause")) {
                    debugger.reset();
                    paused = true;
                }
                ImGui::SameLine();
                if (ImGui::Button("step")) {
                    debugger.single_step();
                }
                ImGui::SameLine();
                if (ImGui::Button("run")) {
                    debugger.reset();
                    paused = false;
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("settings")) {
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("test")) {
                ImGui::Checkbox("Demo Window", &show_demo_window);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
}

void GUI::prepare_game_gfx() {
    // game size
    float x_f = (float)size_x - 300.0f;
    float y_f = (float)size_y;

    float scale;
    float x_offset;
    float y_offset;

    /*
        calculate scale, and offset to start drawing from, this is all
        we need to calculate everything
    */
    if (x_f / y_f >= 2) {
        scale    = y_f / Y_PIXELS;
        x_offset = (x_f - scale * X_PIXELS) / 2.0;
        y_offset = 0.0f;
    }
    else {
        scale    = x_f / X_PIXELS;
        x_offset = 0;
        y_offset = (y_f - scale * Y_PIXELS) / 2.0;
    }

    x_offset += 300.0f;

    rects.clear();

    for (auto y = 0; y < 32; ++y) {
        for (auto x = 0; x < 64; ++x) {
            if (emu.framebuffer[y][x]) {
                rects.emplace_back(x_offset + x * scale, y_offset + y * scale, scale, scale);
            }
        }
    }
}

void GUI::draw() {

    // update our current screen size
    SDL_GetWindowSize(window, &size_x, &size_y);

    prepare_imgui();
    prepare_game_gfx();

    // actually do the drawing now
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRectsF(renderer, rects.data(), rects.size());
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

void GUI::handle_input() {
    // handle keys
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        // check to see if each event is one of our keys
        ImGui_ImplSDL2_ProcessEvent(&event);

        auto& io = ImGui::GetIO();

        if (event.type == SDL_QUIT) {
            done   = true;
            paused = true;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(window)) {
            done   = true;
            paused = true;
        }

        if (!io.WantCaptureKeyboard) {
            auto res = keybinds.find(event.key.keysym.sym);

            if (res != keybinds.end()) {
                if (event.type == SDL_KEYDOWN) {
                    emu.keys[res->second] = true;
                }
                else if (event.type == SDL_KEYUP) {
                    emu.keys[res->second] = false;
                }
            }
        }
    }
}

void GUI::run() {

    using namespace std::chrono_literals;

    std::promise<void> exit;

    std::future<void> signal = exit.get_future();

    auto gfx_thread = [&](std::future<void> exit) {
        while (exit.wait_for(std::chrono::nanoseconds(1)) == std::future_status::timeout) {
            draw();
            handle_input();
        }
    };

    std::thread thread(gfx_thread, std::move(signal));

    while (!done) {
        while (!paused) {
            emu.cycle();
        }
        std::this_thread::sleep_for(100ms);
        emu.timer.reset();
    }

    // set promise value, signal to gfx thread to end
    exit.set_value();

    thread.join();
}