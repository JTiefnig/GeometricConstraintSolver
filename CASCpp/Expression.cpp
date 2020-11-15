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
 

Expression::Expression(Expression&& temp)
{
	this->A = temp.A;
	temp.A = NULL;
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

void Expression::reHash()
{
	A -> reHash();
	this->hash = A->getHash();
}

hashid Expression::getHash() const
{
	return A->getHash();
}


Expression Expression::substitute(const Expression &a, const Expression &b)
{
	auto ret = A->substitute(a, b);

	if (ret->compare(a))
		return b;

	return ret;
}

bool Expression::contains(const SymbolicMathElement &a) const
{
	return A->contains(a);
}

void Expression::toLaTeX(std::ostream &o) const
{
	A->toLaTeX(o);
}


Plus::Plus(const IExpression & a, const IExpression & b)
{
	
	if (a.unWrap().type() == this->type())
	{
		auto pa = dynamic_cast<const Plus&>(a.unWrap());
		this->exps.insert(this->exps.end(), pa.exps.begin(), pa.exps.end());
	}
	else exps.push_back(a);

	if (b.unWrap().type() == this->type())
	{
		auto pb = dynamic_cast<const Plus&>(b.unWrap());
		this->exps.insert(this->exps.end(), pb.exps.begin(), pb.exps.end());
	}
	else exps.push_back(b);


	this->reHash();
}

Plus Plus::operator+=(const IExpression & a)
{
	this->exps.push_back(a);
	this->reHash();
	return *this;
}

Plus Plus::operator-=(const IExpression & a)
{
	this->exps.push_back(-1*a);
	this->reHash();
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
		ret += e->partDif(p);

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

void Plus::reHash()
{
	this->hash =NULL; // Wors for now // think this will be problem when i implement a database

	for (auto & exp : exps)
	{
		this->hash ^= (exp.getHash() & type());
	}

}

Expression Plus::substitute(const Expression &a, const Expression &b)
{
	if (this->compare(a))
		return b;


	Plus ret;
	// try subst components
	for (auto & exp : exps)
	{
		ret += exp.substitute(a, b);
	}

	if (ret.compare(a))
		return b;

	return ret;
}

bool Plus::contains(const SymbolicMathElement &a) const
{
	if(this->compare(a))
		return true;

	for (auto & exp : exps)
	{
		if (exp.contains(a))
			return true;
	}

	return false;
}

void Plus::toLaTeX(std::ostream &o) const
{
	for (list<Expression>::const_iterator exp = exps.begin(); exp != exps.end(); exp++)
	{
		if (exp != exps.begin())
			o << "+";
		exp->toLaTeX(o);
	}
}



Multiply::Multiply(const IExpression & a, const IExpression & b)
{

	if (a.unWrap().type() == this->type())
	{
		auto pa = dynamic_cast<const Multiply&>(a.unWrap());
		this->exps.insert(this->exps.end(), pa.exps.begin(), pa.exps.end());
	}
	else exps.push_back(a.unWrap());

	if (b.unWrap().type() == this->type())
	{
		auto pb = dynamic_cast<const Multiply&>(b.unWrap());
		this->exps.insert(this->exps.end(), pb.exps.begin(), pb.exps.end());
	}
	else exps.push_back(b.unWrap());


	this->reHash();
}

Multiply Multiply::operator*=(const IExpression &a)
{
	this->exps.push_back(a);
	this->reHash();
	return *this;
}

double Multiply::eval() const
{
	double ret = 1; 

	for (auto &exp : exps)
		ret *= exp.eval();

	return ret;
}

std::string Multiply::toString() const
{
	stringstream ss;

	for(list<Expression>::const_iterator exp = exps.begin(); exp!=exps.end() ; exp++)
	{
		if(exp != exps.begin())
			ss << " * ";

		ss << exp->toString();
	}
	return ss.str();
}

IExpression * Multiply::deepCopy() const
{
	return new Multiply(*this);
}



Expression Multiply::partDif(ModelParameter & p)
{
	Plus ret;


	for (list<Expression>::iterator exp = exps.begin(); exp != exps.end(); exp++)
	{
		Expression temp = *exp;
		*exp = exp->partDif(p);
		ret += *this;
		*exp = temp;
	}

	return ret;
}

Expression Multiply::simplify() const
{
	Multiply ret;

	for (auto & exp : exps)
	{
		if (exp.unWrap().type() == this->type())
		{
			auto mult = dynamic_cast<const Multiply*>(&(exp.unWrap()));
			for (auto & m : mult->exps)
				ret.exps.push_back(m);
		}
		else
		{
			ret *= exp.simplify();
		}
	}

	return ret;
}

void Multiply::reHash()
{
	this->hash = NULL; // Works for now // think this will be problem when i implement a database

	for (auto & exp : exps)
	{
		this->hash ^= (exp.getHash() & type());
	}

}

Expression Multiply::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Multiply ret;

	for (auto &exp : exps)
		ret *= exp.substitute(a, b);
	
	if (ret.compare(a))
		return b;

	return ret;
}

bool Multiply::contains(const SymbolicMathElement &a) const
{
	for (auto & exp : exps)
	{
		if (exp.contains(a))
			return true;
	}

	return false;
}

void Multiply::toLaTeX(std::ostream &o) const
{
	for (list<Expression>::const_iterator exp = exps.begin(); exp != exps.end(); exp++)
	{
		if (exp != exps.begin())
			o << "*"; // ge nur wenn exp == const && prev exp == const
		if (exp->type() == typeid(Plus).hash_code())
		{
			o << "\\left( ";
			exp->toLaTeX(o);
			o << "\\right) ";
		}
		else
			exp->toLaTeX(o);
	}
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

Expression Power::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Power ret(A->substitute(a, b), B->substitute(a, b));

	if (ret.compare(a))
		return b;

	return ret;
}

void Power::toLaTeX(std::ostream &o) const
{
	o << "{";
	A->toLaTeX(o);
	o << "}^{";
	B->toLaTeX(o);
	o << "}";
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

Expression Root::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Root ret(A->substitute(a, b), B->substitute(a, b));

	if (ret.compare(a))
		return b;

	return ret;
}

void Root::toLaTeX(std::ostream &o) const
{
	o << "\\sqrt[";
	B->toLaTeX(o);
	o << "]{";
	A->toLaTeX(o);
	o << "}";
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

Expression Log::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Log ret(A->substitute(a, b), B->substitute(a, b));

	if (ret.compare(a))
		return b;

	return ret;
}

void Log::toLaTeX(std::ostream &o) const
{
	o << "\\log_{";
	A->toLaTeX(o);
	o << "} \\left(";
	B->toLaTeX(o);
	o << "\\right)";
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

void Sqrt::toLaTeX(std::ostream &o) const
{
	o << "\\sqrt {";
	A->toLaTeX(o);
	o << "}";
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

void Constant::reHash()
{
	std::hash<double> hf;
	this->hash = hf(val);
}

Expression Constant::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	return *this;
}

void Constant::toLaTeX(std::ostream &o) const
{
	o << val;
}



Expression Parameter::simplify() const
{
	return *this;
}

bool Parameter::contains(const SymbolicMathElement &a) const
{
	return compare(a);
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


Expression Abs::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Abs ret(A->substitute(a, b));

	if (ret.compare(a))
		return b;

	return ret;
}

void Abs::toLaTeX(std::ostream &o) const
{
	o << "";


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

void ModelParameter::reHash()
{
	std::hash<std::string> hf;
	this->hash = hf(name);
}

Expression ModelParameter::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	return *this;
}

void ModelParameter::toLaTeX(std::ostream &o) const
{
	o << this->name;

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


Expression Sin::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Sin ret(A->substitute(a, b));

	if (ret.compare(a))
		return b;

	return ret;
}

void Sin::toLaTeX(std::ostream &o) const
{
	o << "\\sin\\left(";
	A->toLaTeX(o);
	o << "\\right)";
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
	return Division(A->simplify(), B->simplify());
}

void Division::reHash()
{
	this->hash = A->getHash() & type() ^ B->getHash();
}

Expression Division::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Division ret(A->substitute(a, b), B->substitute(a, b));

	if (ret->compare(a))
		return b;


	return ret;
}

bool Division::contains(const SymbolicMathElement &a) const
{
	if (compare(a))
		return true;

	return false;
}

void Division::toLaTeX(std::ostream &o) const
{
	o << "//frac{";
	A->toLaTeX(o);
	o << "}{";
	B->toLaTeX(o);
	o << "}";
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

Expression Cos::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	Cos ret(A->substitute(a, b));

	if (ret.compare(a))
		return b;

	return ret;
}

void Cos::toLaTeX(std::ostream &o) const
{
	o << "\\cos\\left(";
	A->toLaTeX(o);
	o << "\\right)";
}

Minus::Minus(const IExpression & a, const IExpression & b)
	:Plus(a, -b)
{
}

void Function::reHash()
{
	this->hash = ( type() & ~A->getHash());
}


bool Function::contains(const SymbolicMathElement &a) const
{
	return compare(a) | A->contains(a);
}



void TriangleOfPower::reHash()
{
	this->hash = ~(A->getHash() & type()) ^ (B->getHash() | type());
}

bool TriangleOfPower::contains(const SymbolicMathElement &a) const
{
	return compare(a) | A->contains(a) | B->contains(a);
}
