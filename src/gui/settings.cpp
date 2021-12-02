#include "gui/settings.hpp"
#include "gui/imgui_helpers.hpp"
#include <SDL.h>
#include <fmt/format.h>
#include "global.hpp"

namespace GUI {

    Settings::Settings(float fs) : GUIComponent(fs) {}

    void Settings::draw_window() {
        ImGui::Begin("Emulator settings", &window_state, ImGuiWindowFlags_AlwaysAutoResize);

        helpers::center_text("Colour settings");

        ImGui::ColorEdit4("white colour", &global::white_vec().x);
        ImGui::ColorEdit4("black colour", &global::black_vec().x);

        ImGui::Separator();

        helpers::center_text("Controls");

        // table kind of overkill, since we have 1 row
        if (ImGui::BeginTable("all keybinds", 4, ImGuiTableFlags_SizingStretchSame)) {
            ImGui::TableNextRow();

            for (int i = 0; i < 4; ++i) {
                ImGui::TableNextColumn();

                if (ImGui::BeginTable("inner", 2, ImGuiTableFlags_SizingStretchSame)) {
                    for (int j = i; j < 16; j += 4) {

                        auto& k = global::keymap()[static_cast<size_t>(j)];
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
                                                   ImGuiWindowFlags_AlwaysAutoResize |
                                                           ImGuiWindowFlags_NoSavedSettings)) {
                            helpers::center_text(
                                    fmt::format("Changing keybind for {}", k.second).c_str());
                            helpers::center_text("Press a key. Press ESC to cancel.");
                            ImGui::CaptureKeyboardFromApp(true);

                            auto& io = ImGui::GetIO();
                            // check every key
                            for (int key = 0; key < static_cast<int>(sizeof(io.KeysDown)); ++key) {
                                SDL_Scancode scancode = static_cast<SDL_Scancode>(key);

                                if (ImGui::IsKeyDown(key)) {
                                    // ImGui keys are scancodes
                                    if (scancode == SDL_SCANCODE_ESCAPE) {
                                        ImGui::CloseCurrentPopup();
                                    }
                                    else {
                                        auto keycode = SDL_GetKeyFromScancode(scancode);

                                        // key already bound
                                        if (global::keymap().contains(keycode)) {
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
                                                       ImGuiWindowFlags_AlwaysAutoResize |
                                                               ImGuiWindowFlags_NoSavedSettings)) {
                                ImGui::Text("That key is already in use.");

                                if (helpers::center_button("OK")) {
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                            // wrong key

                            if (ImGui::BeginPopupModal("wrong key", nullptr,
                                                       ImGuiWindowFlags_AlwaysAutoResize |
                                                               ImGuiWindowFlags_NoSavedSettings)) {

                                ImGui::Text("Only alphanumeric keys allowed.");
                                helpers::center_text("Please try again.");

                                if (helpers::center_button("OK")) {
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
            global::keymap().reset();
        }

        ImGui::End();
    }

} // namespace GUI