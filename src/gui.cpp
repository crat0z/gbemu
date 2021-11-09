#include "gui.hpp"
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <iostream>
#include <thread>
#include <future>
#include <unordered_map>
#include <fmt/format.h>
#include <stack>
#include <SDL_image.h>

#include "roboto_medium.hpp"
#include "debugger.hpp"
#include "opcodes.hpp"
#include "disassembler.hpp"

// figure out what to do with this stuff, if anything at all. having auxillary functions
// defined like this actually makes browsing the stuff below much better
namespace {
    // enums
    enum icons
    {
        PAUSE,
        STEP_INTO,
        STEP_OVER,
        CONTINUE,
        REFRESH,
        STEP_OUT,
        BREAKPOINT,
        ARROW_LEFT,
        ARROW_RIGHT,
        ARROW_LEFT_INACTIVE,
        ARROW_RIGHT_INACTIVE,
        ARROW_RIGHT_PC
    };

    enum windows
    {
        METRICS_WINDOW,
        DEMO_WINDOW,
        LAUNCH_WINDOW,
        DISASSEMBLER_WINDOW,
        REGISTER_VIEW,
        EMULATOR_SETTINGS,
        MEMORY_VIEWER,
        STACK_VIEWER
    };

    // const data
    const std::array<std::string, 14> svgs = {
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path d='M4.5 3H6v10H4.5V3zm7 0v10H10V3h1.5z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M8 9.532h.542l3.905-3.905-1.061-1.06-2.637 2.61V1H7.251v6.177l-2.637-2.61-1.061 1.06 3.905 3.905H8zm1.956 3.481a2 2 0 1 1-4 0 2 2 0 0 1 4 0z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M14.25 5.75v-4h-1.5v2.542c-1.145-1.359-2.911-2.209-4.84-2.209-3.177 0-5.92 2.307-6.16 5.398l-.02.269h1.501l.022-.226c.212-2.195 2.202-3.94 4.656-3.94 1.736 0 3.244.875 4.05 2.166h-2.83v1.5h4.163l.962-.975V5.75h-.004zM8 14a2 2 0 1 0 0-4 2 2 0 0 0 0 4z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M2.5 2H4v12H2.5V2zm4.936.39L6.25 3v10l1.186.61 7-5V7.39l-7-5zM12.71 8l-4.96 3.543V4.457L12.71 8z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M4.681 3H2V2h3.5l.5.5V6H5V4a5 5 0 1 0 4.53-.761l.302-.954A6 6 0 1 1 4.681 3z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M8 1h-.542L3.553 4.905l1.061 1.06 2.637-2.61v6.177h1.498V3.355l2.637 2.61 1.061-1.06L8.542 1H8zm1.956 12.013a2 2 0 1 1-4 0 2 2 0 0 1 4 0z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='red'><path d='M8 4c.367 0 .721.048 1.063.145a3.943 3.943 0 0 1 1.762 1.031 3.944 3.944 0 0 1 1.03 1.762c.097.34.145.695.145 1.062 0 .367-.048.721-.145 1.063a3.94 3.94 0 0 1-1.03 1.765 4.017 4.017 0 0 1-1.762 1.031C8.72 11.953 8.367 12 8 12s-.721-.047-1.063-.14a4.056 4.056 0 0 1-1.765-1.032A4.055 4.055 0 0 1 4.14 9.062 3.992 3.992 0 0 1 4 8c0-.367.047-.721.14-1.063a4.02 4.02 0 0 1 .407-.953A4.089 4.089 0 0 1 5.98 4.546a3.94 3.94 0 0 1 .957-.401A3.89 3.89 0 0 1 8 4z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M7 3.093l-5 5V8.8l5 5 .707-.707-4.146-4.147H14v-1H3.56L7.708 3.8 7 3.093z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M9 13.887l5-5V8.18l-5-5-.707.707 4.146 4.147H2v1h10.44L8.292 13.18l.707.707z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='currentcolor'><path fill-rule='evenodd' clip-rule='evenodd' d='M7 3.093l-5 5V8.8l5 5 .707-.707-4.146-4.147H14v-1H3.56L7.708 3.8 7 3.093z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='currentcolor'><path fill-rule='evenodd' clip-rule='evenodd' d='M9 13.887l5-5V8.18l-5-5-.707.707 4.146 4.147H2v1h10.44L8.292 13.18l.707.707z'/></svg>",
        "<svg width='20' height='20' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='green'><path fill-rule='evenodd' clip-rule='evenodd' d='M9 13.887l5-5V8.18l-5-5-.707.707 4.146 4.147H2v1h10.44L8.292 13.18l.707.707z'/></svg>"

    };

    constexpr float X_PIXELS  = 64.0;
    constexpr float Y_PIXELS  = 32.0;
    constexpr float font_size = 20.0f;

    std::vector<std::pair<SDL_Keycode, uint8_t>> default_keymap = {
        { SDLK_1, 0x1 }, { SDLK_2, 0x2 }, { SDLK_3, 0x3 }, { SDLK_4, 0xC },
        { SDLK_q, 0x4 }, { SDLK_w, 0x5 }, { SDLK_e, 0x6 }, { SDLK_r, 0xD },
        { SDLK_a, 0x7 }, { SDLK_s, 0x8 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
        { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB }, { SDLK_v, 0xF }
    };

    class Keys {
        std::vector<std::pair<SDL_Keycode, uint8_t>> data;

    public:
        Keys() { data = default_keymap; };

        ~Keys() = default;

        // set a key from old value to a new one, returns true
        // if successful, returns false otherwise
        bool change_key(SDL_Keycode from, SDL_Keycode to) {
            for (auto& v : data) {
                if (v.first == from) {
                    v.first = to;
                    return true;
                }
            }
            return false;
        }

        std::pair<SDL_Keycode, uint8_t>& operator[](size_t v) noexcept { return data[v]; }

        std::optional<uint8_t> translate_key(SDL_Keycode k) const noexcept {
            for (auto& p : data) {
                if (p.first == k) {
                    return p.second;
                }
            }
            return std::nullopt;
        }

        bool contains(SDL_Keycode k) const noexcept {
            for (auto& v : data) {
                if (k == v.first) {
                    return true;
                }
            }
            return false;
        }

        bool contains(uint8_t k) const noexcept {
            for (auto& v : data) {
                if (k == v.second) {
                    return true;
                }
            }
            return false;
        }

        // this is actually safe, since graphics and keyboard events are processed
        // in same thread
        void reset() noexcept {
            data = { { SDLK_1, 0x1 }, { SDLK_2, 0x2 }, { SDLK_3, 0x3 }, { SDLK_4, 0xC },
                     { SDLK_q, 0x4 }, { SDLK_w, 0x5 }, { SDLK_e, 0x6 }, { SDLK_r, 0xD },
                     { SDLK_a, 0x7 }, { SDLK_s, 0x8 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
                     { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB }, { SDLK_v, 0xF } };
        }

        size_t size() const noexcept { return data.size(); }
    };

    float last_scroll_val;

    struct d_scroll_handler {
        std::stack<float> bw_history;
        std::stack<float> fw_history;

        ImGuiListClipper clipper;

        uint16_t target_addr;
        float    target_scroll;

        bool show_left() { return !bw_history.empty(); }
        bool show_right() { return !fw_history.empty(); }

        float fix_float(float v) {
            if (v == 0.0f) {
                return v += FLT_MIN;
            }
            else {
                return v;
            }
        }
        uint16_t fix_u16(uint16_t v) {
            if (v == 0) {
                return v + 1;
            }
            else {
                return v;
            }
        }
        // called to clear the forward stack, i.e., we've gone back, and decided
        // to go down another path of history
        void push() {

            while (!fw_history.empty())
                fw_history.pop();
            bw_history.push(fix_float(last_scroll_val));
        }

        // queue up a new scroll target. optionally destroys forward history we've kept
        void queue_scroll(uint16_t addr, bool save_to_history = false) {
            if (save_to_history) {
                push();
            }
            set_value(addr);
        }

        void set_value(float v) { target_scroll = fix_float(v); }

        void set_value(uint16_t v) { target_addr = fix_u16(v); }

        // actually do the scrolling, call this at particular spot
        void scroll_to_target() {
            if (target_addr) {
                float item_pos_y = clipper.StartPosY + clipper.ItemsHeight * (target_addr * 0.5f);
                ImGui::SetScrollFromPosY(item_pos_y - ImGui::GetWindowPos().y);
                target_addr = 0;
            }
            else if (target_scroll) {
                ImGui::SetScrollY(target_scroll);
                target_scroll = 0.0f;
            }
            //reset clipper now
            clipper = ImGuiListClipper();
        }

        void go_back() {
            // get our last back point
            float ret = bw_history.top();
            bw_history.pop();

            // push our current one
            fw_history.push(fix_float(last_scroll_val));

            // make sure its not zero
            set_value(ret);
        }

        void go_forward() {
            float ret = fw_history.top();
            fw_history.pop();
            bw_history.push(fix_float(last_scroll_val));

            set_value(ret);
        }
    };

    d_scroll_handler ds_handler;

    Keys keymap;

    ImVec4 color_from_bytes(uint8_t r, uint8_t g, uint8_t b) {
        return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
    }

    // centers cursor horizontally, given it knows an item size
    void center_cursor(float item_size) {
        auto width = ImGui::GetContentRegionAvail().x;
        auto x     = ImGui::GetCursorPosX();

        ImGui::SetCursorPosX(x + ((width - item_size) / 2.0f));
    }

    // centers text horizontally, if it knows the size.
    void center_text_known(const char* format, float size, ...) {

        center_cursor(size);

        va_list args;
        va_start(args, size);
        ImGui::TextV(format, args);
        va_end(args);
    }

    // centers a string
    void center_text(const char* format) {
        center_cursor(ImGui::CalcTextSize(format).x);

        ImGui::Text(format);
    }

    void colored_centered_text_known(const ImVec4& colour, bool cond, float size,
                                     const char* format, ...) {
        va_list args;
        va_start(args, format);

        center_cursor(size);

        if (cond) {
            ImGui::TextColoredV(colour, format, args);
        }
        else {
            ImGui::TextV(format, args);
        }
        va_end(args);
    }

    void colored_text(const ImVec4& colour, bool cond, const char* format, ...) {

        va_list args;
        va_start(args, format);
        if (cond) {
            ImGui::TextColoredV(colour, format, args);
        }
        else {
            ImGui::TextV(format, args);
        }
        va_end(args);
    }

} // namespace

GUI::GUI() : emu(), debugger(emu), disassembler(debugger) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 900,
                              flags);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();

    io.Fonts->AddFontFromMemoryCompressedBase85TTF(roboto_medium_compressed_data_base85, font_size);

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);

    // load icon textures, maybe put this in a method later

    int i = 0;

    for (const auto& icon_svg : svgs) {
        auto tmp         = SDL_RWFromConstMem(icon_svg.c_str(), icon_svg.size());
        auto tmp2        = IMG_Load_RW(tmp, 1);
        icon_textures[i] = SDL_CreateTextureFromSurface(renderer, tmp2);
        i++;
    }

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

void GUI::style() {

    // default sprite colours
    white_vec = color_from_bytes(200, 200, 200);
    black_vec = color_from_bytes(15, 15, 15);

    auto&   style  = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    const ImVec4 bgColor          = color_from_bytes(37, 37, 38);
    const ImVec4 lightBgColor     = color_from_bytes(82, 82, 85);
    const ImVec4 veryLightBgColor = color_from_bytes(90, 90, 95);

    const ImVec4 panelColor       = color_from_bytes(51, 51, 55);
    const ImVec4 panelHoverColor  = color_from_bytes(29, 151, 236);
    const ImVec4 panelActiveColor = color_from_bytes(0, 119, 200);

    const ImVec4 textColor         = color_from_bytes(255, 255, 255);
    const ImVec4 textDisabledColor = color_from_bytes(151, 151, 151);
    const ImVec4 borderColor       = color_from_bytes(78, 78, 78);

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
                                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration |
                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBringToFrontOnFocus;

    auto vp = ImGui::GetMainViewport();

    ImGui::SetNextWindowDockID(dock_id);

    ImGui::SetNextWindowPos(vp->WorkPos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(vp->WorkSize, ImGuiCond_Once);

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

void GUI::emulator_settings() {

    ImGui::Begin("Emulator settings", &window_state[EMULATOR_SETTINGS],
                 ImGuiWindowFlags_AlwaysAutoResize);

    center_text("Colour settings");

    ImGui::ColorEdit4("white colour", &white_vec.x);
    ImGui::ColorEdit4("black colour", &black_vec.x);

    ImGui::Separator();

    center_text("Controls");

    // table kind of overkill, since we have 1 row
    if (ImGui::BeginTable("all keybinds", 4, ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableNextRow();

        for (auto i = 0; i < 4; ++i) {
            ImGui::TableNextColumn();

            if (ImGui::BeginTable("inner", 2, ImGuiTableFlags_SizingStretchSame)) {
                for (auto j = i; j < 16; j += 4) {

                    auto& k = keymap[j];
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();

                    ImGui::PushID(j);
                    ImGui::PushID(i);
                    if (ImGui::Button(SDL_GetKeyName(k.first), ImVec2(-FLT_MIN, 0.0f))) {
                        ImGui::OpenPopup("keybind");
                    }

                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal("keybind", nullptr,
                                               ImGuiWindowFlags_AlwaysAutoResize)) {
                        ImGui::Text("Changing keybind for %01X", k.second);
                        ImGui::Text("Press a key. Press ESC to cancel.");
                        ImGui::CaptureKeyboardFromApp(true);

                        auto& io = ImGui::GetIO();
                        // check every key
                        for (int i = 0; i < sizeof(io.KeysDown); ++i) {
                            SDL_Scancode scancode = static_cast<SDL_Scancode>(i);

                            if (ImGui::IsKeyDown(i)) {
                                // ImGui keys are scancodes
                                if (scancode == SDL_SCANCODE_ESCAPE) {
                                    ImGui::CloseCurrentPopup();
                                }
                                else {
                                    auto keycode = SDL_GetKeyFromScancode(scancode);

                                    // key already bound
                                    if (keymap.contains(keycode)) {
                                        ImGui::OpenPopup("already bound");
                                        break;
                                    }
                                    // only allow 0-9a-z
                                    // https://wiki.libsdl.org/SDLKeycodeLookup
                                    else if ((keycode > 0x29 && keycode < 0x3a) ||
                                             (keycode > 0x60 && keycode < 0x7b)) {
                                        k.first = keycode;
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else {
                                        ImGui::OpenPopup("wrong key");
                                        break;
                                    }
                                }
                            }
                        }

                        // key bound
                        if (ImGui::BeginPopupModal("already bound", nullptr,
                                                   ImGuiWindowFlags_AlwaysAutoResize)) {
                            ImGui::Text("That key is already in use.");

                            if (ImGui::Button("OK")) {
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                        // wrong key

                        if (ImGui::BeginPopupModal("wrong key", nullptr,
                                                   ImGuiWindowFlags_AlwaysAutoResize)) {
                            const char* line1 = "Only alphanumeric keys allowed.";
                            const char* line2 = "Please try again.";

                            ImGui::Text(line1);
                            // center second line
                            ImGui::SetCursorPosX((ImGui::GetContentRegionAvailWidth() -
                                                  ImGui::CalcTextSize(line2).x) *
                                                 0.5f);
                            ImGui::Text(line2);

                            if (ImGui::Button("OK")) {
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }

                        ImGui::EndPopup();
                    }

                    ImGui::PopID();
                    ImGui::PopID();

                    ImGui::TableNextColumn();
                    // center the text here
                    auto text_size = ImGui::CalcTextSize("f");
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                                         (ImGui::GetContentRegionAvailWidth() - text_size.x) *
                                                 0.5f);
                    ImGui::Text("%01X", k.second);
                }
                ImGui::EndTable();
            }
        }
        ImGui::EndTable();
    }

    if (ImGui::Button("Reset to default")) {
        keymap.reset();
    }

    ImGui::End();
}

void GUI::launch_settings() {

    static uint16_t entry_setting = 0x200;
    static uint16_t base_address  = 0x200;

    static bool launch_paused = false;

    static ImGuiWindowFlags launch_window_settings =
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;

    static std::string last_file_name = "";

    filebrowser.Display();

    if (filebrowser.HasSelected()) {
        last_file_name = std::move(filebrowser.GetSelected().string());
        filebrowser.ClearSelected();
    }

    ImGui::Begin("Launch settings", &window_state[LAUNCH_WINDOW], launch_window_settings);

    if (ImGui::Button("Choose file")) {
        filebrowser.Open();
    }

    ImGui::TextWrapped("file chosen: %s", last_file_name.c_str());

    ImGui::Separator();
    ImGui::SetNextItemWidth(150.0f);

    ImGui::InputScalar("entry point (hex)", ImGuiDataType_U16, &entry_setting, nullptr, nullptr,
                       "%03x",
                       ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_AutoSelectAll);
    ImGui::SetNextItemWidth(150.0f);

    ImGui::InputScalar("base address (hex)", ImGuiDataType_U16, &base_address, nullptr, nullptr,
                       "%03x",
                       ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_AutoSelectAll);

    ImGui::Separator();

    ImGui::Checkbox("launch paused", &launch_paused);

    if (ImGui::Button("OK")) {
        // new game
        emu.new_game(last_file_name, entry_setting, base_address, launch_paused);

        if (launch_paused) {
            debugger.pause();
        }

        window_state[LAUNCH_WINDOW] = false;
    }

    ImGui::End();
}

void GUI::stack_viewer() {
    ImGui::Begin("Stack view", &window_state[STACK_VIEWER]);
    {
        auto& stack = debugger.get_stack();
        for (auto v : stack) {
            ImGui::Selectable(fmt::format("{0:03X}", v).c_str());
        }
    }
    ImGui::End();
}

void GUI::register_viewer() {

    static const float onesize    = ImGui::CalcTextSize("A").x;
    static const float lhs_width  = ImGui::CalcTextSize("V0").x;
    static const float rhs_width  = ImGui::CalcTextSize("FF").x;
    static const float threesize  = ImGui::CalcTextSize("FFF").x;
    static const float pair_width = lhs_width + rhs_width + ((lhs_width + rhs_width) / 2.0f);

    auto draw_I = [&]() {
        ImGui::TableNextColumn();
        center_text("I");
        ImGui::TableNextColumn();
        colored_centered_text_known({ 255, 0, 0, 255 }, debugger.I_change, threesize, "%03X",
                                    debugger.get_I());
    };

    auto draw_PC = [&]() {
        ImGui::TableNextColumn();
        center_text("PC");
        ImGui::TableNextColumn();
        colored_centered_text_known({ 255, 0, 0, 255 }, debugger.PC_change, threesize, "%03X",
                                    debugger.get_PC());
    };

    auto draw_D = [&]() {
        ImGui::TableNextColumn();
        center_text("D");
        ImGui::TableNextColumn();
        colored_centered_text_known({ 255, 0, 0, 255 }, debugger.dt_change, rhs_width, "%02X",
                                    debugger.get_DT());
    };

    auto draw_S = [&]() {
        ImGui::TableNextColumn();
        center_text("S");
        ImGui::TableNextColumn();
        colored_centered_text_known({ 255, 0, 0, 255 }, debugger.st_change, rhs_width, "%02X",
                                    debugger.get_ST());
    };

    // we check one time to calculate how much space we will need for our pair of register/values
    ImGui::Begin("Registers", &window_state[REGISTER_VIEW]);

    center_text("Registers");
    // calculate how many pairs we can draw horizontally
    auto avail = ImGui::GetContentRegionAvail();
    int  count = static_cast<int>(std::floor(avail.x / pair_width));

    if (count == 0) {
        count = 1;
    }

    // draw outer table
    if (ImGui::BeginTable("allregisters", count,
                          ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame |
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
        for (auto index = 0; index < 16; ++index) {
            ImGui::TableNextColumn();

            center_cursor(pair_width);
            // draw each pair
            if (ImGui::BeginTable("pair", 2,
                                  ImGuiTableFlags_SizingStretchSame |
                                          ImGuiTableFlags_NoHostExtendX |
                                          ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX,
                                  { pair_width, 0.0f })) {
                ImGui::TableNextColumn();
                center_text_known("V%01x", lhs_width, index);
                ImGui::TableNextColumn();

                if (debugger.is_readable()) {
                    colored_centered_text_known({ 255, 0, 0, 255 }, debugger.reg_changes[index],
                                                rhs_width, "%02X", debugger.get_V(index));
                }
                else {
                    ImGui::Text("??");
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndTable();
    }
    // three cases, we draw vertically, horizontally, or in a table if we're square
    // vertical case
    if (count == 1) {
        if (ImGui::BeginTable("special registers", 2,
                              ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame |
                                      ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
            draw_I();
            draw_PC();
            draw_D();
            draw_S();
            ImGui::EndTable();
        }
    }
    // horizontal case
    else if (count > 7) {
        if (ImGui::BeginTable("special registers", 8,
                              ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame |
                                      ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
            draw_I();
            draw_PC();
            draw_D();
            draw_S();
            ImGui::EndTable();
        }
    }
    // square
    else {
        if (ImGui::BeginTable("special registers", 2,
                              ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame |
                                      ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
            ImGui::TableNextColumn();
            center_text("Special");
            ImGui::TableNextColumn();
            center_text("Timers");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            if (ImGui::BeginTable(
                        "Special", 2,
                        ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX |
                                ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX)) {

                draw_I();

                draw_PC();

                ImGui::EndTable();
            }

            ImGui::TableNextColumn();

            if (ImGui::BeginTable(
                        "Timers", 2,
                        ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX |
                                ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX)) {

                draw_D();
                draw_S();

                ImGui::EndTable();
            }

            ImGui::EndTable();
        }
    }

    /* 
        if (ImGui::BeginTable("special registers", 2, table_flags)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            colored_text(debugger.dt_change, "D 0x%02X", debugger.get_DT());
            ImGui::TableNextColumn();
            colored_text(debugger.I_change, "I  0x%03X", debugger.get_I());
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            colored_text(debugger.st_change, "S 0x%02X", debugger.get_ST());
            ImGui::TableNextColumn();
            colored_text(debugger.PC_change, "PC 0x%03X", debugger.get_PC());
            ImGui::EndTable();
        }
    }*/
    ImGui::End();
}

void GUI::memory_viewer() {
    ImGui::Begin("Memory viewer");
    if (ImGui::BeginTable("memory", 8)) {};
}

void GUI::disassembly() {

    static uint16_t jump;

    // widths for text in the table for centering
    float t3_width = ImGui::CalcTextSize("fff").x;
    float t4_width = ImGui::CalcTextSize("ffff").x;

    ImGui::Begin("Disassembler", &window_state[DISASSEMBLER_WINDOW],
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

    // disassembly view
    {

        ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
                                ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                                ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp |
                                ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;

        auto test = ImGui::GetContentRegionMax();

        ImVec2 outer = ImVec2(0.0f, test.y - 4.5 * font_size);

        if (ImGui::BeginTable("text_table", 5, flags, outer)) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn(
                    "PC",
                    ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderWidth |
                            ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_NoReorder |
                            ImGuiTableColumnFlags_NoResize,
                    font_size);
            ImGui::TableSetupColumn(
                    "BP",
                    ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderWidth |
                            ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_NoReorder |
                            ImGuiTableColumnFlags_NoResize,
                    font_size);
            ImGui::TableSetupColumn("Address");
            ImGui::TableSetupColumn("Value");
            ImGui::TableSetupColumn("Instruction");

            ImGui::TableHeadersRow();

            ds_handler.clipper.Begin(2048);

            ImGui::PushStyleColor(ImGuiCol_Header, color_from_bytes(80, 80, 80));

            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, { 0.5f, 0.0f });

            while (ds_handler.clipper.Step()) {
                for (auto i = ds_handler.clipper.DisplayStart; i < ds_handler.clipper.DisplayEnd;
                     i++) {
                    ImGui::TableNextRow();

                    auto& ins1 = disassembler.found_instructions[2 * i];

                    // PC icon
                    ImGui::TableNextColumn();
                    if (debugger.get_PC() == ins1.address) {
                        ImGui::Image(icon_textures[ARROW_RIGHT_PC], ImVec2(font_size, font_size));
                    }

                    // breakpoint icon
                    ImGui::TableNextColumn();

                    if (debugger.is_breakpoint_set(ins1.address)) {
                        ImGui::Image(icon_textures[BREAKPOINT], ImVec2(font_size, font_size));
                    }

                    ImGui::TableNextColumn();
                    // show address
                    center_text_known("%03X", t3_width, ins1.address);

                    // show value at this address
                    ImGui::TableNextColumn();
                    center_text_known("%04X", t4_width, ins1.opcode);

                    // show instruction
                    ImGui::TableNextColumn();
                    if (ins1.operation != op::UNKNOWN) {

                        ImGui::PushID(ins1.address);
                        ImGui::Selectable(fmt::format("{0}", ins1.mnemonic).c_str(), false,
                                          ImGuiSelectableFlags_SpanAllColumns);

                        // menu for right clicks
                        if (ImGui::BeginPopupContextItem()) {
                            // display different text depending on if bp is set
                            if (debugger.is_breakpoint_set(ins1.address)) {
                                if (ImGui::Selectable(
                                            fmt::format("Remove breakpoint at address 0x{0:03x}",
                                                        ins1.address)
                                                    .c_str())) {
                                    debugger.remove_breakpoint(ins1.address);
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                            else {
                                if (ImGui::Selectable(
                                            fmt::format("Add breakpoint at address 0x{0:03x}",
                                                        ins1.address)
                                                    .c_str())) {
                                    debugger.set_breakpoint(ins1.address);
                                    ImGui::CloseCurrentPopup();
                                }
                            }
                            // follow to jump target in disassembly
                            if (is_followable(ins1.operation)) {
                                uint16_t imm12 = ins1.opcode & 0xFFF;
                                if (ImGui::Selectable(
                                            fmt::format("Follow to address 0x{0:03x}", imm12)
                                                    .c_str())) {
                                    ds_handler.queue_scroll(imm12, true);
                                    ImGui::CloseCurrentPopup();
                                }
                            }

                            ImGui::EndPopup();
                        }

                        ImGui::PopID();
                    }
                    else {
                        ImGui::Selectable(fmt::format("???????", ins1.address).c_str(), false,
                                          ImGuiSelectableFlags_SpanAllColumns |
                                                  ImGuiSelectableFlags_Disabled);
                    }
                }
            }

            ImGui::PopStyleVar();

            ImGui::PopStyleColor();

            // debugger tells us when we've reached a PC we should scroll to
            if (debugger.reached_destination()) {
                debugger.recv_destination();
                ds_handler.queue_scroll(debugger.get_PC());
            }
            // save last_scroll_val in case we scroll next frame
            last_scroll_val = ImGui::GetScrollY();
            // scroll now to any targets we might have
            ds_handler.scroll_to_target();

            ImGui::EndTable();

            if (ImGui::ImageButton(icon_textures[REFRESH], ImVec2(font_size, font_size))) {
                disassembler.analyze();
                ds_handler.queue_scroll(debugger.get_entry(), true);
            }

            ImGui::SameLine();

            if (ImGui::ImageButton(icon_textures[PAUSE], ImVec2(font_size, font_size))) {
                debugger.pause();
                ds_handler.queue_scroll(debugger.get_PC(), true);
            }
            ImGui::SameLine();

            if (ImGui::ImageButton(icon_textures[STEP_OVER], ImVec2(font_size, font_size))) {
                debugger.step_over();
            }

            ImGui::SameLine();
            if (ImGui::ImageButton(icon_textures[STEP_INTO], ImVec2(font_size, font_size))) {
                debugger.single_step();
            }

            ImGui::SameLine();
            if (ImGui::ImageButton(icon_textures[STEP_OUT], ImVec2(font_size, font_size))) {
                debugger.step_out();
            }

            ImGui::SameLine();
            if (ImGui::ImageButton(icon_textures[CONTINUE], ImVec2(font_size, font_size))) {
                debugger.continue_emu();
            }

            ImGui::SameLine();

            ImGui::SetNextItemWidth(3 * font_size);
            if (ImGui::InputScalar("", ImGuiDataType_U16, &jump, nullptr, nullptr, "%03x",
                                   ImGuiInputTextFlags_EnterReturnsTrue)) {
                ds_handler.queue_scroll(jump);
                jump = 0;
            }

            if (ds_handler.show_left()) {
                if (ImGui::ImageButton(icon_textures[ARROW_LEFT], ImVec2(font_size, font_size))) {
                    ds_handler.go_back();
                }
            }
            else {
                ImGui::ImageButton(icon_textures[ARROW_LEFT_INACTIVE],
                                   ImVec2(font_size, font_size));
            }
            ImGui::SameLine();
            if (ds_handler.show_right()) {
                if (ImGui::ImageButton(icon_textures[ARROW_RIGHT], ImVec2(font_size, font_size))) {
                    ds_handler.go_forward();
                }
            }
            else {
                ImGui::ImageButton(icon_textures[ARROW_RIGHT_INACTIVE],
                                   ImVec2(font_size, font_size));
            }
        }
    }

    ImGui::End();
}

void GUI::prepare_imgui() {

    auto toggle = [](bool& r) { r ^= true; };

    dock_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),
                                           ImGuiDockNodeFlags_AutoHideTabBar |
                                                   ImGuiDockNodeFlags_PassthruCentralNode);

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Emulator")) {
            if (ImGui::MenuItem("Open new ROM")) {
                toggle(window_state[LAUNCH_WINDOW]);
            }
            if (ImGui::MenuItem("Settings", nullptr, window_state[EMULATOR_SETTINGS])) {
                toggle(window_state[EMULATOR_SETTINGS]);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debugger")) {
            if (ImGui::MenuItem("All windows")) {
                window_state[REGISTER_VIEW]       = true;
                window_state[DISASSEMBLER_WINDOW] = true;
                window_state[STACK_VIEWER]        = true;
            }
            if (ImGui::MenuItem("Register view", nullptr, window_state[REGISTER_VIEW])) {
                toggle(window_state[REGISTER_VIEW]);
            }
            if (ImGui::MenuItem("Disassembly", nullptr, window_state[DISASSEMBLER_WINDOW])) {
                toggle(window_state[DISASSEMBLER_WINDOW]);
            }
            if (ImGui::MenuItem("Stack view", nullptr, window_state[STACK_VIEWER])) {
                toggle(window_state[STACK_VIEWER]);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Test")) {
            if (ImGui::MenuItem("Show demo window", nullptr, window_state[DEMO_WINDOW])) {
                toggle(window_state[DEMO_WINDOW]);
            }
            if (ImGui::MenuItem("Show metrics window", nullptr, window_state[METRICS_WINDOW])) {
                toggle(window_state[METRICS_WINDOW]);
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    game_window();

    if (window_state[DEMO_WINDOW]) {
        ImGui::ShowDemoWindow(&window_state[DEMO_WINDOW]);
    }

    if (window_state[METRICS_WINDOW]) {
        ImGui::ShowMetricsWindow(&window_state[METRICS_WINDOW]);
    }

    if (window_state[EMULATOR_SETTINGS]) {
        emulator_settings();
    }

    if (window_state[REGISTER_VIEW]) {
        register_viewer();
    }

    if (window_state[DISASSEMBLER_WINDOW]) {
        disassembly();
    }

    if (window_state[STACK_VIEWER]) {
        stack_viewer();
    }

    if (window_state[LAUNCH_WINDOW]) {
        launch_settings();
    }
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
            auto input_key = event.key.keysym.sym;

            auto mapping = keymap.translate_key(input_key);

            if (mapping.has_value()) {

                if (event.type == SDL_KEYDOWN) {
                    emu.keys[*mapping] = true;
                }
                else if (event.type == SDL_KEYUP) {
                    emu.keys[*mapping] = false;
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
        while (!debugger.is_paused() && debugger.is_ready()) {
            debugger.cycle();
        }
        std::this_thread::sleep_for(100ms);
        debugger.reset_timer();
    }

    // set promise value, signal to gfx thread to end
    exit.set_value();

    thread.join();
}