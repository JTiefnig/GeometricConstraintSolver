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



	// Geerbt über IExpression
	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

	virtual double eval() const override; // double als rückgabewert // dump

	virtual Expression partDif(ModelParameter & p) override;

	virtual Expression simplify() const override;


};

