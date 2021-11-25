#include "gui/gui.hpp"
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <chrono>
#include <thread>
#include <future>
#include "global.hpp"
#include "roboto_medium.hpp"
#include "gui/icons.hpp"
#include "gui/imgui_helpers.hpp"
#include "gui/launcher.hpp"
#include "gui/settings.hpp"
#include "gui/debugger/disassembly_view.hpp"
#include "gui/debugger/memory_view.hpp"
#include "gui/debugger/register_view.hpp"
#include "gui/debugger/stack_view.hpp"

namespace GUI {

    Main::Main() : game_window(emu) {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

        SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI |
                                                  SDL_WINDOW_RESIZABLE);

        window = SDL_CreateWindow("gbemu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500,
                                  900, flags);

        renderer = SDL_CreateRenderer(window, -1,
                                      SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

        ImGui::CreateContext();
        auto& io = ImGui::GetIO();

        // fix for modularity

        io.Fonts->AddFontFromMemoryCompressedBase85TTF(roboto_medium_compressed_data_base85,
                                                       font_size);

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplSDL2_InitForSDLRenderer(window);
        ImGui_ImplSDLRenderer_Init(renderer);

        // load icon textures, maybe put this in a method later
        global::icon_textures() = generate_icons(font_size, renderer);

        style();
    }

    Main::~Main() {
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void Main::style() {

        // default sprite colours
        global::white_vec() = helpers::color_from_bytes(200, 200, 200);
        global::black_vec() = helpers::color_from_bytes(15, 15, 15);

        auto&   style  = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        const ImVec4 bgColor          = helpers::color_from_bytes(37, 37, 38);
        const ImVec4 lightBgColor     = helpers::color_from_bytes(82, 82, 85);
        const ImVec4 veryLightBgColor = helpers::color_from_bytes(90, 90, 95);

        const ImVec4 panelColor       = helpers::color_from_bytes(51, 51, 55);
        const ImVec4 panelHoverColor  = helpers::color_from_bytes(29, 151, 236);
        const ImVec4 panelActiveColor = helpers::color_from_bytes(0, 119, 200);

        const ImVec4 textColor         = helpers::color_from_bytes(255, 255, 255);
        const ImVec4 textDisabledColor = helpers::color_from_bytes(151, 151, 151);
        const ImVec4 borderColor       = helpers::color_from_bytes(78, 78, 78);

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
        style.ButtonTextAlign   = { 0.50f, 0.50f };
        //style.SelectableTextAlign = { 0.50f, 0.0f };
    }

    void Main::prepare_imgui() {

        static bool demo_window    = false;
        static bool metrics_window = false;

        global::dock_id() = ImGui::DockSpaceOverViewport(
                ImGui::GetMainViewport(),
                ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);

        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("Emulator")) {
                if (ImGui::MenuItem("Launcher")) {
                    windows.emplace_back(std::make_unique<Launcher>(font_size, emu));
                }
                if (ImGui::MenuItem("Settings")) {
                    windows.emplace_back(std::make_unique<Settings>(font_size));
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debugger")) {

                if (ImGui::MenuItem("All windows")) {
                    windows.emplace_back(std::make_unique<RegisterView>(font_size, emu));
                    windows.emplace_back(std::make_unique<DisassemblyView>(font_size, emu));
                    windows.emplace_back(std::make_unique<StackView>(font_size, emu));
                    windows.emplace_back(std::make_unique<MemoryView>(font_size, emu));
                }

                if (ImGui::MenuItem("Register view")) {
                    windows.emplace_back(std::make_unique<RegisterView>(font_size, emu));
                }
                if (ImGui::MenuItem("Disassembly view")) {
                    windows.emplace_back(std::make_unique<DisassemblyView>(font_size, emu));
                }
                if (ImGui::MenuItem("Stack view")) {
                    windows.emplace_back(std::make_unique<StackView>(font_size, emu));
                }
                if (ImGui::MenuItem("Memory view")) {
                    windows.emplace_back(std::make_unique<MemoryView>(font_size, emu));
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Test")) {
                if (ImGui::MenuItem("Show demo window", nullptr, &demo_window)) {}
                if (ImGui::MenuItem("Show metrics window", nullptr, &metrics_window)) {}

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        game_window.draw_window();

        // draw our windows
        for (auto it = windows.begin(); it != windows.end();) {
            if ((*it)->is_enabled()) {
                (*it)->draw_window();
                it++;
            }
            else {
                it = windows.erase(it);
            }
        }

        auto send_to_type = [&](const std::type_info& type, const GUIMessage& msg) {
            for (auto& w : windows) {
                auto ref = w.get();
                if (typeid(*ref) == type) {
                    w->process_message(msg);
                }
            }
        };

        auto forward_message = [&](const std::optional<GUIMessage>& g) {
            if (g.has_value()) {
                const auto& msg = g.value();
                switch (msg.target) {
                case gui_component::all: {
                    for (auto& w : windows) {
                        w->process_message(msg);
                    }
                    break;
                }
                case gui_component::disassembly_view: {
                    send_to_type(typeid(DisassemblyView), msg);
                    break;
                }
                case gui_component::register_view: {
                    send_to_type(typeid(RegisterView), msg);
                    break;
                }
                case gui_component::memory_view: {
                    send_to_type(typeid(MemoryView), msg);
                    break;
                }
                case gui_component::stack_view: {
                    send_to_type(typeid(StackView), msg);
                    break;
                }
                case gui_component::launcher: {
                    send_to_type(typeid(Launcher), msg);
                    break;
                }
                case gui_component::settings: {
                    send_to_type(typeid(Settings), msg);
                    break;
                }
                }
            }
        };

        // check for any messages
        for (auto& w : windows) {
            forward_message(w->get_message());
        }

        if (demo_window) {
            ImGui::ShowDemoWindow(&demo_window);
        }

        if (metrics_window) {
            ImGui::ShowMetricsWindow(&metrics_window);
        }
    }

    void Main::draw() {

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

    void Main::handle_input() {
        // handle keys
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {
            // check to see if each event is one of our keys
            ImGui_ImplSDL2_ProcessEvent(&event);

            auto& io = ImGui::GetIO();

            if (event.type == SDL_QUIT) {
                exit(0);
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window)) {
                exit(0);
            }

            /* if (!io.WantCaptureKeyboard) {
                auto input_key = event.key.keysym.sym;

                auto mapping = global::keymap().translate_key(input_key);

                if (mapping.has_value()) {

                    if (event.type == SDL_KEYDOWN) {
                        emu.get_keys()[*mapping] = true;
                    }
                    else if (event.type == SDL_KEYUP) {
                        emu.get_keys()[*mapping] = false;
                    }
                }
            } */
        }
    }

    void Main::run() {

        using namespace std::chrono_literals;

        std::promise<void> exit;

        std::future<void> signal = exit.get_future();

        auto gfx_thread = [&](std::future<void> exit) {
            while (exit.wait_for(std::chrono::nanoseconds(1)) == std::future_status::timeout) {
                while (emu.is_ready()) {
                    emu.cycle();
                }
                std::this_thread::sleep_for(100ms);
            }
        };

        std::thread thread(gfx_thread, std::move(signal));

        while (!done) {
            draw();
            handle_input();
        }

        // set promise value, signal to gfx thread to end
        exit.set_value();

        thread.join();
    }
} // namespace GUI
