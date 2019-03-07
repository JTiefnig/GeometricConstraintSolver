#pragma once
#include <string>
#include <list>
#include "Equation.h"
#include "Expression.h"



class ISketchEntity
{
private:
	const std::string id;

public:
	ISketchEntity(std::string ID);

	virtual ~ISketchEntity();

	std::string getId();

	// Todo: Rethink this
	virtual void GenerateEq(std::list<Expression> &l) const = 0;
};

