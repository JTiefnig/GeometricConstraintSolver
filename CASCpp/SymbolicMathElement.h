#pragma once
#include <string>
#include <typeinfo>
#include <iostream>
#include <exception>

class IExpression;


class SymbolicMathElement
{
public:
	SymbolicMathElement();
	~SymbolicMathElement();

	virtual std::string toString() const = 0;

	std::size_t type() const;
 
	// TODO:
	//virtual void toLaTex(ostream&) const = 0; // Wirtes the equation in Latex format // very important for visualization later
	virtual SymbolicMathElement* deepCopy() const = 0; // enforce deepcopy on this level;


};



class NotImplementedOperationException : public std::exception
{
public: 
	
	NotImplementedOperationException()
		:exception("Operation Not Implemented jet!")
	{

	}



	NotImplementedOperationException(const char* msg)
		:exception(msg)
	{

	}
};