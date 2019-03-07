#pragma once
#include "ISketchEntity.h"
#include <string>

class Entity :
	public ISketchEntity
{
public:
	Entity(std::string ID);
	~Entity();
};

