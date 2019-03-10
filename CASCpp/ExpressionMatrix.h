#pragma once
#include "Matrix.h"
#include "Expression.h"



class ExpressionMatrix
	: public Matrix<Expression>
	, public IExpression
{

public:

	ExpressionMatrix(int M, int N)
		:Matrix<Expression>(N, M)
	{}

	ExpressionMatrix(int M, int N, const Expression & temp)
		:Matrix<Expression>(N, M, temp)
	{}

	ExpressionMatrix();
	~ExpressionMatrix();



	// Geerbt �ber IExpression
	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

	virtual double eval() const override; // double als r�ckgabewert // dump

	virtual Expression partDif(ModelParameter & p) override;

	virtual Expression simplify() const override;



	// Geerbt �ber IExpression
	virtual void reHash() override;

	virtual Expression substitute(const Expression &, const Expression &) override;


	// Geerbt �ber IExpression
	virtual bool contains(const SymbolicMathElement &) const override;

	virtual void toLaTeX(std::ostream &) const override;

};

