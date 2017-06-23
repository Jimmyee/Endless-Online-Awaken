// Endless Online Awaken v0.0.1

#include "map.hpp"
#include "singleton.hpp"

Map::Map(int id)
{
    this->Load(id);
}

Map::Map(shared_ptr<EMF> emf)
{
    this->emf = emf;
}

void Map::Load(int id)
{
    S::GetInstance().emf = std::shared_ptr<EMF>(new EMF(id));
}

shared_ptr<Character> Map::GetCharacter(short gameworld_id)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i]->gameworld_id == gameworld_id)
        {
            return this->characters[i];
        }
    }

    return shared_ptr<Character>(0);
}

shared_ptr<Character> Map::GetCharacter(std::string name)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i]->name == name)
        {
            return this->characters[i];
        }
    }

    return shared_ptr<Character>(0);
}

void Map::RemoveCharacter(short gameworld_id)
{
    shared_ptr<Character> character = this->GetCharacter(gameworld_id);

    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i]->gameworld_id == gameworld_id)
        {
            this->characters.erase(this->characters.begin() + i);
            return;
        }
    }
}
