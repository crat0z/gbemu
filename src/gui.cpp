#include <gui.hpp>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <chip8.hpp>

GUI::GUI() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);

    SDL_WindowFlags flags =
            (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                              flags);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

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

void GUI::run() {
    bool done = false;

    bool show_demo_window = false;

    ImGuiWindowFlags main_flags =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // run a game cycle
        SDL_GetWindowSize(window, &current_x, &current_y);

        // game size
        float x_f = (float)current_x - 300.0f;
        float y_f = (float)current_y;

        float scale;
        float x_offset;
        float y_offset;
        // calculate our scale of screen
        if (x_f / y_f >= 2) {
            scale    = y_f / 32.0;
            x_offset = (x_f - scale * 64.0) / 2.0;
            y_offset = 0.0f;
        }
        else {
            scale    = x_f / 64.0;
            x_offset = 0;
            y_offset = (y_f - scale * 32.0) / 2.0;
        }

        x_offset += 300.0f;

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {

            ImGui::SetNextWindowSize({ 300, (float)current_x });

            ImGui::SetNextWindowPos({ 0, 0 });
            ImGui::Begin("chip8emu", nullptr,
                         main_flags); // Create a window called "Hello, world!" and append into it.
            ImGui::Checkbox("Demo Window", &show_demo_window);

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
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }
}