#pragma once

#include "Expression.h"
#include "Equation.h"
#include <vector>




class Model
{
private:
	std::vector<Equation> Equations;

public:
	Model();
	~Model();

	std::vector<Equation> getEquations() const
	{
		return this->Equations;
	}


	void AddEquation(const Equation& eq)
	{
		Equations.push_back(eq);
	}


};

