#include "ISketchEntity.h"






ISketchEntity::ISketchEntity(std::string ID)
	:id(ID)
{

}

ISketchEntity::~ISketchEntity()
{
}

std::string ISketchEntity::getId()
{
	return this->id;
}
