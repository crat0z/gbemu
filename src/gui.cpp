#include "gui.hpp"
#include <cmath>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

#include <iostream>
#include <thread>
#include <future>
#include <unordered_map>

#include <fmt/format.h>

#include "DroidSans.hpp"
#include "debugger.hpp"
#include "opcodes.hpp"
#include "disassembler.hpp"

constexpr float X_PIXELS = 64.0;
constexpr float Y_PIXELS = 32.0;

std::unordered_map<int, uint8_t> keybinds = { { SDLK_1, 0x1 }, { SDLK_2, 0x2 }, { SDLK_3, 0x3 },
                                              { SDLK_4, 0xC }, { SDLK_q, 0x4 }, { SDLK_w, 0x5 },
                                              { SDLK_e, 0x6 }, { SDLK_r, 0xD }, { SDLK_a, 0x7 },
                                              { SDLK_s, 0x8 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
                                              { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB },
                                              { SDLK_v, 0xF } };

ImVec4 ColorFromBytes(uint8_t r, uint8_t g, uint8_t b) {
    return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
}

void GUI::style() {

    // default sprite colours
    white_vec = ColorFromBytes(200, 200, 200);
    black_vec = ColorFromBytes(15, 15, 15);

    auto&   style  = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    const ImVec4 bgColor          = ColorFromBytes(37, 37, 38);
    const ImVec4 lightBgColor     = ColorFromBytes(82, 82, 85);
    const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

    const ImVec4 panelColor       = ColorFromBytes(51, 51, 55);
    const ImVec4 panelHoverColor  = ColorFromBytes(29, 151, 236);
    const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

    const ImVec4 textColor         = ColorFromBytes(255, 255, 255);
    const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
    const ImVec4 borderColor       = ColorFromBytes(78, 78, 78);

    colors[ImGuiCol_Text]                 = textColor;
    colors[ImGuiCol_TextDisabled]         = textDisabledColor;
    colors[ImGuiCol_TextSelectedBg]       = panelActiveColor;
    colors[ImGuiCol_WindowBg]             = bgColor;
    colors[ImGuiCol_ChildBg]              = bgColor;
    colors[ImGuiCol_PopupBg]              = bgColor;
    colors[ImGuiCol_Border]               = borderColor;
    colors[ImGuiCol_BorderShadow]         = borderColor;
    colors[ImGuiCol_FrameBg]              = panelColor;
    colors[ImGuiCol_FrameBgHovered]       = panelHoverColor;
    colors[ImGuiCol_FrameBgActive]        = panelActiveColor;
    colors[ImGuiCol_TitleBg]              = bgColor;
    colors[ImGuiCol_TitleBgActive]        = bgColor;
    colors[ImGuiCol_TitleBgCollapsed]     = bgColor;
    colors[ImGuiCol_MenuBarBg]            = panelColor;
    colors[ImGuiCol_ScrollbarBg]          = panelColor;
    colors[ImGuiCol_ScrollbarGrab]        = lightBgColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
    colors[ImGuiCol_ScrollbarGrabActive]  = veryLightBgColor;
    colors[ImGuiCol_CheckMark]            = panelActiveColor;
    colors[ImGuiCol_SliderGrab]           = panelHoverColor;
    colors[ImGuiCol_SliderGrabActive]     = panelActiveColor;
    colors[ImGuiCol_Button]               = panelColor;
    colors[ImGuiCol_ButtonHovered]        = panelHoverColor;
    colors[ImGuiCol_ButtonActive]         = panelHoverColor;
    colors[ImGuiCol_Header]               = panelColor;
    colors[ImGuiCol_HeaderHovered]        = panelHoverColor;
    colors[ImGuiCol_HeaderActive]         = panelActiveColor;
    colors[ImGuiCol_Separator]            = borderColor;
    colors[ImGuiCol_SeparatorHovered]     = borderColor;
    colors[ImGuiCol_SeparatorActive]      = borderColor;
    colors[ImGuiCol_ResizeGrip]           = bgColor;
    colors[ImGuiCol_ResizeGripHovered]    = panelColor;
    colors[ImGuiCol_ResizeGripActive]     = lightBgColor;
    colors[ImGuiCol_PlotLines]            = panelActiveColor;
    colors[ImGuiCol_PlotLinesHovered]     = panelHoverColor;
    colors[ImGuiCol_PlotHistogram]        = panelActiveColor;
    colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
    colors[ImGuiCol_DragDropTarget]       = bgColor;
    colors[ImGuiCol_NavHighlight]         = bgColor;
    colors[ImGuiCol_DockingPreview]       = panelActiveColor;
    colors[ImGuiCol_Tab]                  = bgColor;
    colors[ImGuiCol_TabActive]            = panelActiveColor;
    colors[ImGuiCol_TabUnfocused]         = bgColor;
    colors[ImGuiCol_TabUnfocusedActive]   = panelActiveColor;
    colors[ImGuiCol_TabHovered]           = panelHoverColor;

    style.WindowRounding    = 0.0f;
    style.ChildRounding     = 0.0f;
    style.FrameRounding     = 0.0f;
    style.GrabRounding      = 0.0f;
    style.PopupRounding     = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding       = 0.0f;
    style.WindowTitleAlign  = { 0.50f, 0.50f };
}

GUI::GUI() : emu(), debugger(emu), disassembler(emu) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI |
                                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 900,
                              flags);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();

    io.Fonts->AddFontFromMemoryCompressedBase85TTF(DroidSans_compressed_data_base85, 18);

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);

    style();
}

GUI::~GUI() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GUI::game_window() {
    ImU32 white = ImColor(white_vec);
    ImU32 black = ImColor(black_vec);
    /*
        styles for the game window.
    */

    // vars
    ImGui::PushStyleColor(ImGuiCol_ChildBg, black_vec);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiWindowFlags win_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav |
                                 ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav |
                                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;

    ImGui::Begin("game window", nullptr, win_flags);

    /* 
            menu bar is where most functionality will be
        */

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    ImVec2 size = { vMax.x - vMin.x, vMax.y - vMin.y };

    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;

    float scale;
    float x_offset;
    float y_offset;

    if (size.x / size.y >= 2.0f) {
        scale    = size.y / Y_PIXELS;
        x_offset = (size.x - scale * X_PIXELS) / 2.0f;
        y_offset = 0.0f;
    }
    else {
        scale    = size.x / X_PIXELS;
        x_offset = 0.0f;
        y_offset = (size.y - scale * Y_PIXELS) / 2.0f;
    }

    vMin.x += x_offset;
    vMin.y += y_offset;

    for (auto i = 0; i < 32; ++i) {
        auto y = vMin.y + i * scale;
        for (auto j = 0; j < 64; ++j) {
            auto x = vMin.x + j * scale;
            if (emu.framebuffer[i][j]) {
                draw_list->AddRectFilled(ImVec2{ x, y }, ImVec2{ x + scale, y + scale }, white,
                                         0.0f, ImDrawFlags_RoundCornersNone);
            }
            else {
                draw_list->AddRectFilled(ImVec2{ x, y }, ImVec2{ x + scale, y + scale }, black,
                                         0.0f, ImDrawFlags_RoundCornersNone);
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

bool GUI::emulator_settings() {
    bool open = true;
    ImGui::Begin("Emulator settings", &open);

    ImGui::ColorEdit4("white colour", &white_vec.x);
    ImGui::ColorEdit4("black colour", &black_vec.x);

    ImGui::End();
    return open;
}

bool GUI::launch_settings() {

    bool open = true;

    static uint16_t entry_setting = 0x200;
    static uint16_t base_address  = 0x200;

    static bool launch_paused = false;

    static ImGuiWindowFlags launch_window_settings =
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;

    static std::string last_file_name = "";

    filebrowser.Display();

    if (filebrowser.HasSelected()) {
        last_file_name = std::move(filebrowser.GetSelected());
        filebrowser.ClearSelected();
    }

    ImGui::Begin("Launch settings", &open, launch_window_settings);

    if (ImGui::Button("Choose file")) {
        filebrowser.Open();
    }

    ImGui::TextWrapped("file chosen: %s", last_file_name.c_str());

    ImGui::Separator();

    ImGui::InputScalar("entry point (hexadecimal)", ImGuiDataType_U16, &entry_setting, nullptr,
                       nullptr, "%03x",
                       ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_AutoSelectAll);

    ImGui::InputScalar("base address (hexadecimal)", ImGuiDataType_U16, &base_address, nullptr,
                       nullptr, "%03x",
                       ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_AutoSelectAll);

    ImGui::Separator();

    ImGui::Checkbox("launch paused", &launch_paused);

    if (ImGui::Button("OK")) {
        // new game
        emu.new_game(last_file_name, entry_setting, base_address, launch_paused);

        if (launch_paused) {
            debugger.pause();
        }

        open = false;
    }

    ImGui::End();

    return open;
}

bool GUI::debugger_window() {

    // two lambdas for 1 and 2 parameter red colored text when variables change
    static auto colored_text = [&](bool val, const char* fmt, uint16_t v) {
        if (val) {
            ImGui::TextColored({ 255, 0, 0, 255 }, fmt, v);
        }
        else {
            ImGui::Text(fmt, v);
        }
    };

    static auto colored_text2 = [&](bool val, const char* fmt, uint16_t v, uint16_t x) {
        if (val) {
            ImGui::TextColored({ 255, 0, 0, 255 }, fmt, v, x);
        }
        else {
            ImGui::Text(fmt, v, x);
        }
    };

    bool open = true;

    ImGui::Begin("Registers");
    {
        ImGui::Text("Registers & Timers");
        // draw registers in table
        auto table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedSame;
        if (ImGui::BeginTable("registers", 4, table_flags)) {
            // draw registers
            for (auto row = 0; row < 4; ++row) {

                ImGui::TableNextRow();

                for (auto col = 0; col < 4; ++col) {
                    ImGui::TableSetColumnIndex(col);

                    auto index = row + 4 * col;

                    colored_text2(debugger.reg_changes[index], "V%01x 0x%02X", (index),
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

        if (debugger.is_paused()) {
            ImGui::Text("Next opcode: %04X", debugger.get_opcode());

            ImGui::Separator();

            ImGui::Text("Next instruction: %s", debugger.get_instruction());

            ImGui::Separator();
            // static bool description_header = false;
            if (ImGui::CollapsingHeader("Description")) {
                ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 300.0f);
                ImGui::Text("Description: %s", debugger.get_description());
            }
        }

        if (ImGui::Button("pause")) {
            debugger.pause();
        }
        ImGui::SameLine();
        if (ImGui::Button("step")) {
            debugger.single_step();
        }
        ImGui::SameLine();
        if (ImGui::Button("run")) {
            debugger.run();
        }
    }
    ImGui::End();

    ImGui::Begin("Disassembler", &open);

    // disassembly view
    {

        if (ImGui::Button("Refresh")) {
            disassembler.analyze();
        }

        for (uint16_t i = 0; i < 2048; ++i) {

            auto& ins1 = disassembler.found_instructions[2 * i];

            if (ins1.operation != op::UNKNOWN) {
                ImGui::Selectable(
                        fmt::format("\t0x{0:03x}\t{1}", ins1.address, ins1.mnemonic).c_str());
            }
            else {
                ImGui::Selectable(fmt::format("\t0x{0:03x}\t???????", ins1.address).c_str());
            }
        }
    }

    ImGui::End();

    return open;
}

void GUI::prepare_imgui() {
    static bool show_demo_window   = false;
    static bool show_launch_window = false;
    static bool show_emu_settings  = false;
    static bool show_debugger      = false;

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar);

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Emulator")) {
            if (ImGui::MenuItem("Open new ROM")) {
                show_launch_window = true;
            }
            if (ImGui::MenuItem("Settings")) {
                show_emu_settings = true;
            }

            if (ImGui::MenuItem("Debugger")) {
                show_debugger = true;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Test")) {
            ImGui::MenuItem("Show demo window", nullptr, &show_demo_window);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (show_emu_settings) {
        show_emu_settings = emulator_settings();
    }

    if (show_debugger) {
        show_debugger = debugger_window();
    }

    if (show_launch_window) {
        show_launch_window = launch_settings();
    }

    game_window();

    /* // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {

        ImGui::Begin("chip8emu"); // Create a window called "Hello, world!" and append into it.
        if (ImGui::BeginTabBar("tabs")) {
            if (ImGui::BeginTabItem("main")) {
                ImGui::Checkbox("pause", &paused);
                if (ImGui::Button("analyze")) {
                    debugger.analyze();
                }
                ImGui::Text("%.3f fps", ImGui::GetIO().Framerate);
                if (ImGui::Button("select rom")) {
                    filebrowser.Open();
                }
                ImGui::EndTabItem();
            }

            // two lambdas for 1 and 2 parameter red colored text when variables change
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
                ImGui::ColorEdit4("white colour", &white_vec.x);
                ImGui::ColorEdit4("black colour", &black_vec.x);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("test")) {
                ImGui::Checkbox("Demo Window", &show_demo_window);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();

        
    } */
}

void GUI::draw() {

    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    prepare_imgui();

    // Rendering
    ImGui::Render();

    SDL_RenderClear(renderer);

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
            done = true;
        }
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(window)) {
            done = true;
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
        while (!debugger.is_paused() && emu.is_ready) {
            emu.cycle();
        }
        std::this_thread::sleep_for(100ms);
        emu.timer.reset();
    }

    // set promise value, signal to gfx thread to end
    exit.set_value();

    thread.join();
}