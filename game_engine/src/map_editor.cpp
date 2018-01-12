// Endless Online Awaken

#include "map_editor.hpp"

#include "input_handler.hpp"
#include "gfx_loader.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "client.hpp"
#include "map_cursor.hpp"

#include <imgui.h>
#include <iostream>

bool MapEditor::initialized_ = false;
unsigned int MapEditor::graphic_id;
Map::Layer::Type MapEditor::type;
int MapEditor::render_xoff;
int MapEditor::render_yoff;

MapEditor::MapEditor()
{
    if(!this->initialized_)
    {
        this->graphic_id = 0;
        this->type = Map::Layer::Type::Ground;
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

            this->Reset();

            Character *character = new Character("Artist", i_id, 1, 1);
            character->speed = 18;

            Map().characters.push_back(std::shared_ptr<Character>(character));
            Client().character = character;

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
                this->Reset();

                Character *character = new Character("Artist", i_id, 1, 1);
                character->speed = 18;

                Map().characters.push_back(std::shared_ptr<Character>(character));
                Client().character = character;

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

    int gid_offset[2] = { 3, 4 };

    ALLEGRO_BITMAP *tile = NULL;
    int tile_id = 1;
    int max_w = 0;

    do
    {
        tile = gfx_loader.GetBitmap(gid_offset[(int)this->type], tile_id);
        if(tile != NULL)
        {
            int w = al_get_bitmap_width(tile);

            if(w > max_w) max_w = w;
        }
        tile_id++;
    } while(tile != NULL);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowPos(ImVec2(10, 308));
    ImGui::SetNextWindowSize(ImVec2(620, 162));
    ImGui::Begin("Tile sets", 0, flags);

    const char* layer_names[] = { "Ground", "Object" };
    static int item2 = 0;
    ImGui::Combo("Layers", &item2, layer_names, IM_ARRAYSIZE(layer_names));

    if(item2 != (int)this->type)
    {
        this->type = (Map::Layer::Type)item2;
        this->graphic_id = 0;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
    ImGui::BeginChild("Sub", ImVec2(590, 100), true);
    ImGui::Columns(600 / max_w, "", false);

    tile_id = 1;

    do
    {
        tile = gfx_loader.GetBitmap(gid_offset[(std::size_t)this->type], tile_id);
        if(tile != NULL)
        {
            int w = al_get_bitmap_width(tile);
            int h = al_get_bitmap_height(tile);

            if(ImGui::ImageButton((void*)tile, ImVec2(w, h), ImVec2(0,0), ImVec2(w/w, h/h), 1, ImColor(0,0,0,255)))
            {
                this->graphic_id = tile_id;
            }
        }
        tile_id++;
        ImGui::NextColumn();
    } while(tile != NULL);

    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::End();
}

void MapEditor::ProcessInput()
{
    InputHandler input_handler;
    MapCursor map_cursor;

    if(input_handler.mouse[1] && this->graphic_id != 0)
    {
        if(!map_cursor.MouseOnMap()) return;

        if(this->graphic_id == 0) return;

        Map map;
        std::shared_ptr<Map::Tile> tile = map.layers[this->type].tiles[map_cursor.x][map_cursor.y];

        if(tile == 0)
        {
            tile = std::shared_ptr<Map::Tile>(new Map::Tile());

            tile->graphic_id = this->graphic_id;
            tile->x = map_cursor.x;
            tile->y = map_cursor.y;

            map.layers[this->type].tiles[tile->x][tile->y] = tile;
        }
        else
        {
            if(tile->graphic_id != this->graphic_id && this->graphic_id != map.fill_tile)
            {
                tile->graphic_id = this->graphic_id;
            }
        }
    }
    if(input_handler.mouse[2])
    {
        MapCursor map_cursor;

        if(!map_cursor.MouseOnMap()) return;

        Map map;

        if(map.layers[this->type].tiles[map_cursor.x][map_cursor.y] != 0)
            map.layers[this->type].tiles[map_cursor.x][map_cursor.y].reset();

        //map.layers[this->type].tiles[map_cursor.x][map_cursor.y] = std::shared_ptr<Map::Tile>(0);
    }

    if(input_handler.keys[ALLEGRO_KEY_W])
    {
        Map::Attribute *att = new Map::Attribute();
        att->type = Map::Attribute::Type::Wall;
        att->x = map_cursor.x;
        att->y = map_cursor.y;

        Map map;

        if(map.atts[map_cursor.x][map_cursor.y] != 0) map.atts[map_cursor.x][map_cursor.y].reset();

        map.atts[map_cursor.x][map_cursor.y] = std::shared_ptr<Map::Attribute>(att);
    }
    if(input_handler.keys[ALLEGRO_KEY_E])
    {
        Map map;

        if(map.atts[map_cursor.x][map_cursor.y] != 0) map.atts[map_cursor.x][map_cursor.y].reset();
    }
}

void MapEditor::Reset()
{
    this->graphic_id = 0;
    this->type = Map::Layer::Type::Ground;
    this->render_xoff = 0;
    this->render_yoff = 0;
}
