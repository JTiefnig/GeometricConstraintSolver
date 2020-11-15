#include "stdafx.h"
#include "EquationProperty.h"
#include <sstream>



EquationProperty::EquationProperty()
{

}

EquationProperty::EquationProperty(Equation *eq)
{
	this->_Name = gcnew String("Ranodm Name");

	std::stringstream ss;
	eq->toLaTeX(ss);
	this->_LateX = gcnew String(ss.str().c_str());
}
