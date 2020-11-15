#include "Equation.h"
#include <sstream>

using namespace std;

Equation::Equation()
{
	reHash();
}




Equation::Equation( const Expression & left, const Expression & right)
	:L(left), R(right)
{
	reHash();
}

Equation::~Equation()
{

}

std::string Equation::toString() const
{
	stringstream ss;

	ss << L.toString() << " = " << R.toString();

	return ss.str();
}

IExpression * Equation::deepCopy() const
{
	return nullptr;
}

void Equation::reHash()
{
	this->hash = NULL; // Works for now // think this will be problem when i implement a database

	this->hash = (L.getHash() ^ R.getHash() & type());

}

void Equation::toLaTeX(std::ostream & o) const
{
	this->L.toLaTeX(o);
	o << "=";
	this->R.toLaTeX(o);

}

bool Equation::contains(const SymbolicMathElement &) const
{
	return false;
}
