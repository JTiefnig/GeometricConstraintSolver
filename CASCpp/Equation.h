#pragma once
#include "Expression.h"
#include "SymbolicMathElement.h"




class Equation : public SymbolicMathElement
{
	// left and riht side of the Eqation
	 Expression L, R;



public:
	Equation();
	// Not ideal
	
	// better 
	Equation(const Expression &left, const Expression &right);
	~Equation();


	

	// Geerbt über SymbolicMathElement
	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;


	// Geerbt über SymbolicMathElement
	virtual void reHash() override;

	virtual void toLaTeX(std::ostream &) const override;

	virtual bool contains(const SymbolicMathElement &) const override;

};



class Condition : public Expression
{
	// TODO
};




