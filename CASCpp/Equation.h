#pragma once
#include "Expression.h"
#include "SymbolicMathElement.h"




class Equation : public SymbolicMathElement
{
	// left and riht side of the Eqation
	IExpression *L, *R;



public:
	Equation();
	// Not ideal
	Equation(IExpression *left, IExpression *right);
	// better 
	Equation(IExpression &left, IExpression &right);
	~Equation();


	

	// Geerbt über SymbolicMathElement
	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

};



class Condition : public Expression
{
	// TODO
};




