#include "Equation.h"
#include <sstream>

using namespace std;

Equation::Equation()
{
}

Equation::Equation(IExpression * left, IExpression * right)
{
}


Equation::Equation(IExpression & left, IExpression & right)
{
}

Equation::~Equation()
{
}

std::string Equation::toString() const
{
	stringstream ss;

	ss << L->toString() << " = " << R->toString();

	return ss.str();
}

IExpression * Equation::deepCopy() const
{
	return nullptr;
}
