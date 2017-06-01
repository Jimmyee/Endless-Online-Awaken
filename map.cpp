#include "map.hpp"
#include "singleton.hpp"

Map::Map(int id)
{
    this->Load(id);
}

void Map::Load(int id)
{
    S::GetInstance().emf = std::shared_ptr<EMF>(new EMF(id));
}
