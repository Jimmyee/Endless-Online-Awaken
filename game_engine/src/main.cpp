// Endless Online Awaken

#include "const/packet.hpp"
#include "util.hpp"
#include "config.hpp"
#include "game_state.hpp"
#include "client.hpp"
#include "input_handler.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "map_cursor.hpp"
#include "map_editor.hpp"
#include "font_handler.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <imgui.h>
#include "imgui_impl_a5.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

int main(int, char**)
{
    const float FPS = 60.0f;

    try
    {
        ALLEGRO_DISPLAY* display = NULL;
        ALLEGRO_EVENT_QUEUE* event_queue = NULL;
        ALLEGRO_TIMER *fps_timer = NULL;
        ALLEGRO_TIMER *input_timer = NULL;
        ALLEGRO_TIMER *update_timer = NULL;

        Config config("./data/config.ini");

        std::vector<std::string> args = GetArgs(config.GetValue("Resolution"));
        int screen_width = std::atoi(args[0].c_str());
        int screen_height = std::atoi(args[1].c_str());

        int flags = config.GetValue("Fullscreen") == "yes"? ALLEGRO_FULLSCREEN : ALLEGRO_WINDOWED;

        al_init();
        al_install_keyboard();
        al_install_mouse();
        al_init_primitives_addon();
        al_init_image_addon();
        al_init_font_addon();
        al_init_ttf_addon();

        al_set_new_display_flags(flags);
        display = al_create_display(screen_width, screen_height);
        if(!display)
        {
            throw std::runtime_error("Could not create display!");
        }
        al_set_window_title(display, "game engine");

        event_queue = al_create_event_queue();
        if(!event_queue)
        {
            throw std::runtime_error("Could not create event queue!");
        }

        fps_timer = al_create_timer(1.0 / FPS);
        input_timer = al_create_timer(0.01);
        update_timer = al_create_timer(0.006);
        if(!fps_timer)
        {
            throw std::runtime_error("Could not create timers!");
        }
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        al_register_event_source(event_queue, al_get_mouse_event_source());
        al_register_event_source(event_queue, al_get_timer_event_source(fps_timer));
        al_register_event_source(event_queue, al_get_timer_event_source(input_timer));
        al_register_event_source(event_queue, al_get_timer_event_source(update_timer));
        al_start_timer(fps_timer);
        al_start_timer(input_timer);
        al_start_timer(update_timer);

        float sx = screen_width / (float)640;
        float sy = screen_height / (float)480;

        ALLEGRO_TRANSFORM trans;
        al_identity_transform(&trans);
        al_scale_transform(&trans, sx, sy);
        al_use_transform(&trans);

        ALLEGRO_COLOR clear_color = al_map_rgb(0, 0, 0);

        ImGui_ImplA5_Init(display);

        GameState game_state;
        InputHandler input_handler;
        Client client;
        GFXLoader gfx_loader("./data/gfx/");
        GUI gui;
        Map map;
        MapCursor map_cursor;
        MapEditor map_editor;
        FontHandler font_handler;

        std::array<int, 3> version = client.version;

        std::cout << "Endless Online Awaken Client v" << version[0] << "." << version[1] << "." << version[2] <<
        " pre-alpha." << std::endl << std::endl;

        while(game_state.Get() != GameState::State::Exit)
        {
            ALLEGRO_EVENT event;
            ALLEGRO_TIMEOUT timeout;
            al_init_timeout(&timeout, 0.06);
            bool redraw = false;

            client.Tick();

            bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);
            if(get_event)
            {
                ImGui_ImplA5_ProcessEvent(&event);

                if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) game_state.Set(GameState::State::Exit);
                if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
                {
                    ImGui_ImplA5_InvalidateDeviceObjects();
                    al_acknowledge_resize(display);
                    Imgui_ImplA5_CreateDeviceObjects();
                }

                if(event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP ||
                   event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    input_handler.ProcessEvent(event);

                    if(input_handler.keys[ALLEGRO_KEY_ESCAPE])
                    {
                        if(gui.GetState() != GUI::State::MainMenu)
                        {
                            gui.SetState(GUI::State::MainMenu);
                            if(client.Connected()) client.Disconnect();
                            game_state.Set(GameState::State::MainMenu);
                        }
                        else
                        {
                            game_state.Set(GameState::State::Exit);
                        }
                    }
                }

                if(event.type == ALLEGRO_EVENT_TIMER)
                {
                    if(event.timer.source == fps_timer)
                    {
                        redraw = true;
                    }
                    if(event.timer.source == input_timer)
                    {
                        input_handler.Process();

                        if(input_handler.keys[ALLEGRO_KEY_F9] && gui.GetState() == GUI::State::MainMenu)
                        {
                            gui.SetState(GUI::State::Editor);
                        }

                        if(gui.GetState() == GUI::State::Editor) map_editor.ProcessInput();
                    }
                    if(event.timer.source == update_timer)
                    {
                        map.Tick();
                    }
                }
            }

            if(redraw && al_is_event_queue_empty(event_queue))
            {
                ImGui_ImplA5_NewFrame();

                al_clear_to_color(clear_color);

                if(map.id != 0)
                {
                    int screen_x = al_get_display_width(display) / 2 - 32;;
                    int screen_y = (320 / 2) - 16; // 320 = render screen height + 10

                    if(client.character != 0)
                    {
                        screen_x -= client.character->x * 64 - client.character->x * 32 - client.character->y * 32;
                        screen_y -= client.character->y * 16 + client.character->x * 16;

                        if(client.character->anim_state == Character::AnimState::Walk)
                        {
                            int position_off_x[4] = { -1, -1, 1, 1 };
                            int position_off_y[4] = { 1, -1, -1, 1 };

                            int dir_off_x = position_off_x[(int)client.character->direction];
                            int dir_off_y = position_off_y[(int)client.character->direction];

                            screen_x -= ((8 * (4 - (int)client.character->animation.current_frame)) * dir_off_x);
                            screen_y -= ((4 * (4 - (int)client.character->animation.current_frame)) * dir_off_y);
                        }
                    }

                    if(gui.GetState() == GUI::State::Editor)
                    {
                        screen_x += map_editor.render_xoff;
                        screen_y += map_editor.render_yoff;
                    }

                    map.Render(screen_x, screen_y);
                }

                if(!client.Connected() && client.state != Client::State::Uninitialized)
                {
                    client.state = Client::State::Uninitialized;
                    ImGui::OpenPopup("Disconnected");
                }

                gui.Process();
                gui.Render();

                al_flip_display();
            }

            al_rest(0.0000005);
        }

        ImGui_ImplA5_Shutdown();
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
    }
    catch(std::runtime_error rt_err)
    {
        std::cout << "Runtime error: " << rt_err.what() << std::endl;
        std::ofstream file("errorlog.txt", std::ios::trunc);
        if(file.is_open() && file.good())
        {
            file << rt_err.what() << std::endl;
            file.close();
        }
    }

    return 0;
}
