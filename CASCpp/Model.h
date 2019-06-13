#pragma once

#include "Expression.h"
#include "Equation.h"
#include <vector>




class Model
{
private:
	std::vector<Equation> Equation;




public:
	Model();
	~Model();
};

