#include "SymbolicMathElement.h"
#include <typeinfo>



SymbolicMathElement::SymbolicMathElement()
{
}


SymbolicMathElement::~SymbolicMathElement()
{
}

std::size_t SymbolicMathElement::type() const
{
	return typeid(*this).hash_code();
}
