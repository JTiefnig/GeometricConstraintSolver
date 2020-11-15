#include "stdafx.h"

#include "CASCwrapper.h"
#include "Expression.h"

CASCwrapper::AlgebraSystem::AlgebraSystem()
{
	this->model = new Model();

	_Equations = gcnew ObservableCollection<EquationProperty^>();

}

void CASCwrapper::AlgebraSystem::TestSetup()
{
	



	auto ex1 = ModelParameter("a") + ModelParameter("bn");

	ex1 = Sqrt(ex1);


	auto x = ModelParameter("X");
	auto y = ModelParameter("Y");


	auto ex2 = Power(Cos(x), y);

	model->AddEquation(Equation(ex1, ex2));

	model->AddEquation(Equation(ex2, ex1));


	// toto seperate method
	auto modeqs = model->getEquations();
	for(int i = 0 ; i< modeqs.size(); i++)
	{
		auto neq = gcnew EquationProperty(&modeqs[i]);
		_Equations->Add(neq);
	}

	
}
