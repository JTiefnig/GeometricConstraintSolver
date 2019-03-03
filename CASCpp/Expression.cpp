#include "Expression.h"
#include <sstream>
#include <math.h>
#include "Equation.h"

using namespace std;


Expression::Expression()
{
	A = new Constant(0);
}

Expression::Expression(double val)
{
	A = new Constant(val);
}

Expression::Expression(const std::string & id)
{
	A = new ModelParameter(id);
}
 

Expression::~Expression()
{
	if(A!=nullptr)
 		delete A;
}


Expression Expression::operator=(IExpression & expression)
{
	if (A)
	{
		delete A;
		A = nullptr;
	}
	this->A = expression.deepCopy();

	return *this;
}

Expression & Expression::operator=(const Expression & expr)
{
	if (this != &expr)
	{
		if (A)
		{
			delete A;
			A = nullptr;
		}
		this->A = expr.A->deepCopy();
		auto ret = Expression(A->deepCopy());
	}

	return *this;
}

double Expression::eval() const
{
	return A->eval();
}

std::string Expression::toString() const
{
	return A->toString();
}

IExpression * Expression::deepCopy() const
{
	return A->deepCopy();
}

Expression::operator IExpression&()
{
	return *A; // dont know if that leads to problems// TODO think about that
}

Expression::operator double() const
{
	return this->eval();
}

Expression Expression::partDif(ModelParameter & p)
{
	return A->partDif(p);
}

Expression Expression::simplify() const
{
	return A->simplify();
}


Plus::Plus(const IExpression & a, const IExpression & b)
{
	if (a.unWrap().type() == this->type())
	{
		auto pa = dynamic_cast<const Plus&>(a.unWrap());
		this->exps.insert(this->exps.end(), pa.exps.begin(), pa.exps.end());
	}
	else exps.push_back(a.unWrap());

	if (b.unWrap().type() == this->type())
	{
		auto pb = dynamic_cast<const Plus&>(b.unWrap());
		this->exps.insert(this->exps.end(), pb.exps.begin(), pb.exps.end());
	}
	else exps.push_back(b.unWrap());

}

Plus Plus::operator+=(const IExpression & a)
{
	this->exps.push_back(a);
	return *this;
}

Plus Plus::operator-=(const IExpression & a)
{
	this->exps.push_back(-a);
	return *this;
}

double Plus::eval() const
{
	double ret = 0;

	for (auto e : exps)
		ret += e.eval();

	return ret;
}

IExpression * Plus::deepCopy() const
{
	return new Plus(*this);
}

std::string Plus::toString() const
{
	stringstream ss;
	ss << "(";
	bool first = false;
	for (auto & e : exps)
	{
		if (first) ss << "+"; 
		ss << e.toString();
		first = true;
	}
	ss << ")";
	return ss.str();
}

Expression Plus::partDif(ModelParameter & p)
{
	Plus ret;

	for (auto & e : exps)
		ret += e.partDif(p);

	return ret;
}

Expression Plus::simplify() const
{
	// if there is only one Expr
	if (exps.size() == 1) 
		return exps.front();

	Plus ret;

	// cluster sums together
	for (auto & e : exps)
	{
		auto s = e.simplify();
		if (s.unWrap().type() == this->type())
		{
			auto pa = dynamic_cast<const Plus&>(s.unWrap());
			ret.exps.insert(ret.exps.end(), pa.exps.begin(), pa.exps.end());
		}
		else ret += s;
	}

	return ret;
}



double Multiply::eval() const
{
	return A->eval() * B->eval();
}

std::string Multiply::toString() const
{
	stringstream ss;
	ss << "(";
	ss << A->toString();
	ss << " * ";
	ss << B->toString();
	ss << ")";

	return ss.str();
}

IExpression * Multiply::deepCopy() const
{
	return new Multiply(*A, *B);
}



Expression Multiply::partDif(ModelParameter & p)
{
	return A->partDif(p) * (*B) + (*A) * B->partDif(p);
}

Expression Multiply::simplify() const
{
	return A->simplify() * B->simplify();
}

double Power::eval() const
{
	return pow(A->eval(), B->eval());
}

std::string Power::toString() const
{
	stringstream ss;
	
	ss << A->toString();
	ss << " ^ ";
	ss << B->toString();

	return ss.str();
}

IExpression * Power::deepCopy() const
{
	return new Power(*A, *B);
}

Expression Power::partDif(ModelParameter & p)
{
	return ln(*A)*B->partDif(p) + (*B / *A)*A->partDif(p);
}

Expression Power::simplify() const
{
	return Power(A->simplify(), B->simplify());
}

double Root::eval() const
{
	double db = 1 / B->eval();

	return pow(A->eval(), db);
}

std::string Root::toString() const
{
	stringstream ss;
	ss << "Root(";
	ss << A->toString();
	ss << " , ";
	ss << B->toString();
	ss << ")";

	return ss.str();
}

IExpression * Root::deepCopy() const
{
	return new Root(*A, *B);
}

Expression Root::simplify() const
{
	return Expression();
}

Expression Root::partDif(ModelParameter & p)
{
	return Power(*A, 1/ *B ).partDif(p); // TODO make this better
}

double Log::eval() const
{
	return log(B->eval())/log(A->eval());
}

std::string Log::toString() const
{
	stringstream ss;
	ss << "Log(";
	ss << A->toString()<< " , ";
	ss << B->toString() << ")";

	return ss.str();
}

IExpression * Log::deepCopy() const
{
	return new Log(*A, *B);
}

Expression Log::simplify() const
{
	return Log(A->simplify(), B->simplify());
}

Expression Log::partDif(ModelParameter & p)
{
	return (*A- B->partDif(p) - *B * *this * A->partDif(p)) / (*A * *B * ln(*A));
}

std::string Sqrt::toString() const
{
	stringstream ss;
	ss << "Sqrt(";
	ss << B->toString();
	ss << ")";

	return ss.str();
}

IExpression * Sqrt::deepCopy() const
{
	return new Sqrt(*A);
}

Expression Sqrt::partDif(ModelParameter & p)
{
	return 0.5/Sqrt(*A) * A->partDif(p);
}

std::string Constant::toString() const
{
	stringstream ss;
	ss << val;
	return ss.str();
}

IExpression * Constant::deepCopy() const
{
	return new Constant(this->val);
}

double Constant::eval() const 
{
	return val;
}

Expression Constant::partDif(ModelParameter & p)
{
	return Constant(0);
}

Expression Parameter::simplify() const
{
	return *this;
}

double Abs::eval() const
{
	return abs(A->eval());
}

std::string Abs::toString() const
{
	stringstream ss;

	ss << "Abs( " << A->toString() << ")";


	return ss.str();
}

IExpression * Abs::deepCopy() const
{
	return new Abs(*this->A);
}

Expression Abs::partDif(ModelParameter & p)
{

	throw NotImplementedOperationException();

	return Expression(); // todo sqrt(sqare().partdif()
}

Expression Abs::simplify() const
{
	return Abs(A->simplify());
}

double ModelParameter::eval() const 
{
	return val;
}

std::string ModelParameter::toString() const
{
	// maybe put more info in here // 
	return this->name;
}

IExpression * ModelParameter::deepCopy() const
{
	return new ModelParameter(this->name, this->val);
}

Expression ModelParameter::partDif(ModelParameter & p)
{
	if (p.name == this->name)
		return Constant(1);

	return Constant(0);
}

double Sin::eval() const
{
	return sin(A->eval());
}

Expression Sin::partDif(ModelParameter & p)
{
	return - Cos(*A) * (A->partDif(p));
}

std::string Sin::toString() const
{
	stringstream ss;
	ss << "sin(";
	ss << A->toString();
	ss << ")";

	return ss.str();
}

IExpression * Sin::deepCopy() const
{
	return new Sin(*A);
}

Expression Sin::simplify() const
{
	return Sin(A->simplify());
}

std::string Division::toString() const
{

	stringstream ss;
	ss << "(";
	ss << A->toString();
	ss << " / ";
	ss << B->toString();
	ss << ")";

	return ss.str();
}

IExpression * Division::deepCopy() const
{
	return new Division(*A, *B);
}

double Division::eval() const
{
	return A->eval()/ B->eval();
}

Expression Division::partDif(ModelParameter & p)
{
	
	return (A->partDif(p) * *B - B->partDif(p)) / (*B)^2;
}

Expression Division::simplify() const
{
	return Expression();
}

Expression operator+(const Expression & a, const Expression & b)
{
	return Plus(a, b);
}

Expression operator-(const Expression & a, const Expression & b)
{
	return Minus(a, b);
}

Expression operator*(const Expression & a, const Expression & b)
{
	return Multiply(a, b);
}

Expression operator/(const Expression & a, const Expression & b)
{
	return Division(a, b);
}

Expression operator-(const Expression & a)
{
	return Constant(-1) * a;
}


Expression operator+=(Expression & a, const Expression & b)
{
	return a = a + b;
}

Expression operator-=(Expression & a, const Expression & b)
{
	return a = a - b;
}

Expression operator*=(Expression & a, const Expression & b)
{
	return a = a * b;
}

Expression operator/=(Expression & a, const Expression & b)
{
	return a = a / b;
}

Expression operator^(const Expression & a, const Expression & b)
{
	return Power(a, b);
}

Expression ln(const Expression & a)
{
	auto e = SymbolicConstant::e;

	return Log(e, a);
}

Expression sin(const Expression & e)
{
	return Sin(e);
}

Expression cos(const Expression & e)
{
	return Cos(e);
}

Expression tan(const Expression & e)
{
	return Sin(e) / Cos(e);
}

double Cos::eval() const
{
	return cos(A->eval());
}

Expression Cos::partDif(ModelParameter & p)
{
	return Sin(*A) * A->partDif(p);
}

std::string Cos::toString() const
{
	stringstream ss;
	ss << "cos(";
	ss << A->toString();
	ss << ")";
	return ss.str();
}

IExpression * Cos::deepCopy() const
{
	return new Cos(*A);
}

Expression Cos::simplify() const
{
	return Expression(); // TODO
}

Minus::Minus(const IExpression & a, const IExpression & b)
	:Plus(a, -b)
{
}
