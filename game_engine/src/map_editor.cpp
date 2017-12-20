// Endless Online Awaken

#include "map_editor.hpp"

#include "input_handler.hpp"
#include "gfx_loader.hpp"
#include "gui.hpp"
#include "map.hpp"

#include <imgui.h>
#include <iostream>

bool MapEditor::initialized_ = false;
int MapEditor::render_xoff;
int MapEditor::render_yoff;

MapEditor::MapEditor()
{
    if(!this->initialized_)
    {
        this->render_xoff = 0;
        this->render_yoff = 0;

        this->initialized_ = true;
    }
}

void MapEditor::MakeGUI()
{
    std::string menu_action = "";
    GFXLoader gfx_loader;
    Map map;

    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("New")) menu_action = "New";
            if(ImGui::MenuItem("Load...")) menu_action = "Load";
            if(ImGui::MenuItem("Save...")) menu_action = "Save";
            if(ImGui::MenuItem("Save As...")) menu_action = "SaveAs";
            if(ImGui::MenuItem("Exit")) menu_action = "Exit";

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if(menu_action == "New") ImGui::OpenPopup("New");
    if(menu_action == "Load") ImGui::OpenPopup("Load");
    if(menu_action == "Save")
    {
        if(map.id == 0) ImGui::OpenPopup("Save As");
        else if(map.id > 0) map.Save();
    }
    if(menu_action == "SaveAs") ImGui::OpenPopup("Save As");
    if(menu_action == "Exit")
    {
        map.Reset();
        GUI().SetState(GUI::State::MainMenu);
    }

    if (ImGui::BeginPopup("New"))
    {
        static int i_id = 1;
        static int i_w = 1;
        static int i_h = 1;
        static int i_fill = 0;
        static char str_name[128] = "";

        ImGui::Text("Create new map.");

        ImGui::InputInt("ID", &i_id);
        ImGui::InputText("Name", str_name, IM_ARRAYSIZE(str_name));
        ImGui::InputInt("Width", &i_w);
        ImGui::InputInt("Height", &i_h);

        ALLEGRO_BITMAP *fill_tile = gfx_loader.GetBitmap(3, i_fill);
        if(fill_tile != NULL)
        {
            int w = al_get_bitmap_width(fill_tile);
            int h = al_get_bitmap_height(fill_tile);
            ImGui::Image((void*)fill_tile, ImVec2(w, h), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
            ImGui::SameLine();
        }
        ImGui::InputInt("Fill tile", &i_fill);

        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("Sub", ImVec2(340, 256), true);
        ImGui::Columns(4, "", false);
        int tile_id = 1;
        ALLEGRO_BITMAP *tile = NULL;
        do
        {
            tile = gfx_loader.GetBitmap(3, tile_id);
            if(tile != NULL)
            {
                int w = al_get_bitmap_width(tile);
                int h = al_get_bitmap_height(tile);

                if(ImGui::ImageButton((void*)tile, ImVec2(64, 32), ImVec2(0,0), ImVec2(64.0f/w, 32/h), 1, ImColor(0,0,0,255)))
                {
                    i_fill = tile_id;
                }
            }
            tile_id++;
            ImGui::NextColumn();
        } while(tile != NULL);
        ImGui::EndChild();
        ImGui::PopStyleVar();

        if(ImGui::Button("Create"))
        {
            map.Reset();
            map.id = i_id;
            map.name = str_name;
            map.width = i_w;
            map.height = i_h;
            map.fill_tile = i_fill;

            this->render_xoff = 0;
            this->render_yoff = 0;

            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
    if(ImGui::BeginPopup("Load"))
    {
        static int i_id = 1;

        ImGui::Text("Load map.");
        ImGui::InputInt("ID", &i_id);
        if(ImGui::Button("Load"))
        {
            map.Load(i_id);
            if(!map.Exists())
            {
                map.Reset();
                menu_action = "ErrorLoad";
            }
            else
            {
                this->render_xoff = 0;
                this->render_yoff = 0;
                ImGui::CloseCurrentPopup();
            }
        }

        if(menu_action == "ErrorLoad") ImGui::OpenPopup("Error loading map");

        if(ImGui::BeginPopupModal("Error loading map", NULL, ImGuiWindowFlags_NoResize))
        {
            ImGui::Text("Could not load map file of given ID.");
            if(ImGui::Button("OK")) ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
        if(ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("Save As"))
    {
        static int i_id =  map.id;

        ImGui::InputInt("ID", &i_id);
        if(ImGui::Button("Save"))
        {
             map.id = i_id;
             map.Save();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void MapEditor::ProcessInput()
{
    InputHandler input_handler;
    int xoff = 0;
    int yoff = 0;

    if(input_handler.keys[ALLEGRO_KEY_UP]) yoff++;
    if(input_handler.keys[ALLEGRO_KEY_RIGHT]) xoff--;
    if(input_handler.keys[ALLEGRO_KEY_DOWN]) yoff--;
    if(input_handler.keys[ALLEGRO_KEY_LEFT]) xoff++;

    if(xoff != 0) this->render_xoff += xoff * 7;
    if(yoff != 0) this->render_yoff += yoff * 7;
}
