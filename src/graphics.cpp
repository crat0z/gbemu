#include "graphics.hpp"
#include <cmath>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

constexpr float X_PIXELS = 64.0;
constexpr float Y_PIXELS = 32.0;

Graphics::Graphics() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    SDL_WindowFlags flags =
            (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1580, 640,
                              flags);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer);
}

Graphics::~Graphics() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Graphics::prepare_imgui() {
    static bool show_demo_window = false;

    static int val = 30;

    ImGuiWindowFlags main_flags =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

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
        ImGui::Checkbox("Demo Window", &show_demo_window);

        ImGui::SliderInt("CPU Frequency", &val, 0, 20, "format %d");
        /* ImGui::SliderFloat("float", &f, 0.0f,
                               1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color",
                              (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button(
                        "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            */
        ImGui::Text("%.3f fps", ImGui::GetIO().Framerate);

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
}

void Graphics::prepare_game_gfx() {
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
            if (pixels[y][x]) {
                rects.emplace_back(x_offset + x * scale, y_offset + y * scale, scale, scale);
            }
        }
    }
}

void Graphics::draw() {

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

void Graphics::clear() {
    for (auto& line : pixels) {
        for (auto& pixel : line) {
            pixel = false;
        }
    }
}
