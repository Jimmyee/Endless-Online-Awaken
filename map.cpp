#include "map.hpp"

Map::Map()
{

}

void Map::Load(int id)
{
    this->emf = std::shared_ptr<EMF>(new EMF(id));
}
