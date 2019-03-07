#pragma once

// An Basic implementation of a Computer Algebra System (CAS)
// Author Johannes Tiefnig
// Feb 2019
// This is my 3rd attemped to write an CAS. Probably the best one so far
// Will think about an licenceing later


#define _USE_MATH_DEFINES
#include <string>
#include <vector>
#include <math.h>
#include <list>
#include "SymbolicMathElement.h"


// todo namespace algebra;

// goal no Pointers or dump delete
// outside the Expression struct


class Expression;
class ModelParameter;





class IExpression : public SymbolicMathElement
{

public:

	IExpression() {}

	virtual ~IExpression() {}

	virtual IExpression* deepCopy() const = 0; // hardcore overwriting from base class // dont think taht this is good

	IExpression(const Expression& temp) {}

	virtual double eval() const = 0; // TODO: replace double with some kind of nummeric Type class (Then Matrix / Vector / etc can also be evaluated)

	virtual Expression partDif(ModelParameter& p) = 0;

	virtual Expression simplify() const = 0; // returns a simplified expreeion 

	virtual IExpression & unWrap() { return *this; }
	virtual const IExpression & unWrap() const { return *this; }

	virtual IExpression * operator->()
	{
		return this;
	} // Todo for const..


	// think about hashing ... 00000 .. 
	virtual bool compare(const Expression&);

	virtual Expression substitute(const Expression&, const Expression&) =0;

	
	
};



// Kind of an Container AK Proxy// an expresion is not an Eaquation // difficulties Comparison
// an equation consists of two Expressions
class Expression : public IExpression // Check 
{
	IExpression * A;

	// No public access to the pointer 


	
public:

	// Constructors 

	Expression();

	Expression(const IExpression& a) // implicit conversion
	{
		A = a.unWrap().deepCopy();
	}

	Expression(const IExpression* a)	
	{
		A = a->deepCopy();
	}

	
	Expression(const Expression& a) // copyconsturctor // never just simply copy the pointer
	{
		this->A = a.A->deepCopy();
	}

	Expression(double val);

	Expression(const std::string&);

	// destructor

	virtual ~Expression();

	// Helpers
	virtual Expression operator = (IExpression &expression);

	Expression & operator = (const Expression &expr);

	virtual operator IExpression & ();

	virtual operator double() const;


	// Geerbt über IExpression
	virtual double eval() const override;

	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

	virtual Expression partDif(ModelParameter & p) override;

	virtual Expression simplify() const override;

	// Rethink this! 
	virtual IExpression & unWrap() override { return A->unWrap(); }
	virtual const IExpression & unWrap() const override { return A->unWrap(); }

	virtual IExpression * operator->()
	{
		return A;
	}


	// Geerbt über IExpression
	virtual void reHash() override;

	virtual hashid getHash() const override;

	virtual Expression substitute(const Expression &, const Expression &) override;

};







class Plus : virtual public IExpression
{

	// write a class Expression list that handles the dirty stuff
	std::list<Expression> exps;

public:

	Plus()
	{
		reHash();
	}
	
	Plus(const Plus & temp)
		:exps(temp.exps)
	{
		reHash();
	}

	
	Plus(const IExpression & a, const IExpression & b);


	Plus operator+=(const IExpression&);
	Plus operator-=(const IExpression&);


	// Geerbt über Expression
	virtual double eval() const override;

	virtual IExpression * deepCopy() const override;

	virtual std::string toString() const override;


	// Geerbt über IExpression
	virtual Expression partDif(ModelParameter & b) override;

	virtual Expression simplify() const override;

	virtual void reHash() override;

	virtual Expression substitute(const Expression &, const Expression &) override;
};




class Minus : public Plus 
{
public:
	Minus(const IExpression & a, const IExpression& b);
};



class Multiply : virtual public IExpression
{

	std::list<Expression> exps; // Maybe do that with IExpression Pointers later // for more speed and less memory usage

public:

	Multiply()
	{
		reHash();
	}

	Multiply(const Multiply & temp)
		:exps(temp.exps)
	{
		reHash();
	}

	Multiply(const IExpression & a, const IExpression& b);

	Multiply operator*=(const IExpression&);

	// Geerbt über IExpression
	virtual double eval() const override;
	virtual std::string toString() const override;
	virtual IExpression * deepCopy() const override;

	virtual ~Multiply()
	{
	}

	// Geerbt über IExpression
	virtual Expression partDif(ModelParameter & p) override;

	virtual Expression simplify() const override;


	virtual void reHash() override;

	virtual Expression substitute(const Expression &, const Expression &) override;
};



class Division : public IExpression
{

	IExpression *A, *B;

public:

	Division(const IExpression & a, const IExpression& b)
	{
		A = a.deepCopy();
		B = b.deepCopy();
		reHash();
	}

	virtual ~Division()
	{
		delete A;
		delete B;
	}



	// Geerbt über IExpression
	virtual std::string toString() const override;
	virtual IExpression * deepCopy() const override;
	virtual double eval() const override;

	virtual Expression partDif(ModelParameter & p) override;
	virtual Expression simplify() const override;


	virtual void reHash() override;
	virtual Expression substitute(const Expression &, const Expression &) override;

};




class Parameter : public IExpression//, public Expression
{

public:
	// Dont know what to do in here at the thime // 
	// some kind of hashing maybe

	
	virtual ~Parameter()
	{
		// nothing to delete
	}

	virtual Expression simplify() const override;

	// Get name
	// Get value // 
};


// 
// Constant meant only constant for solver algorithm 
// TODO // Transform that into an nummeric type as template class
// 

class Constant : public Parameter
{
private:
	double val; // will be some kind of nummeric object // including matrices and komplex numbers

public:

	Constant(double value)
		:val(value)
	{
		reHash();
	}

	Constant()
		:val(0.0)
	{
		reHash();
	}

	// asignment operator 
	virtual double & operator = (const double &f) noexcept
	{
		val = f;
		return val;
	}

	// conversion operator
	virtual operator double const & () const
	{
		return this->val;// fuck about hashing!! // deshalb is er const!!
	}

	// Geerbt über Parameter
	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

	virtual double eval() const override;

	virtual Expression partDif(ModelParameter & p) override;

	virtual void reHash() override;
	virtual Expression substitute(const Expression &, const Expression &) override;
};


// need that for solver algorithm 
enum ModelParameterType
{
	Fix=0,
	Open,
	Bound
};


class ModelParameter : public Parameter
{
	const std::string name; // Name
	double val;

	ModelParameterType paramtype;

public:

	ModelParameter(const std::string & Name)
		:name(Name), val(0)
	{
		reHash();
	}

	ModelParameter(const std::string & Name, double value)
		:name(Name), val(value)
	{
		reHash();
	}

	ModelParameter(const std::string & Name, double value, ModelParameterType ptype)
		:name(Name), val(value), paramtype(ptype)
	{
		reHash();
	}

	

	// Geerbt über Parameter
	virtual double eval() const override;

	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

	virtual Expression partDif(ModelParameter & p) override;

	virtual void reHash() override;

	virtual Expression substitute(const Expression &, const Expression &) override;

};




// Function 
class Function : public IExpression
{
protected:
	IExpression *A;

public:
	// Enforce some additional Methods
	Function(const IExpression & a)
	{
		A = a.deepCopy();
		reHash();
	}
	


	virtual void reHash() override;
	
	~Function()
	{
		delete A;
	}

};



// this schould be linar .. somehow // but not lipschitz
class Abs : public Function
{
	

public: 
	Abs(const IExpression & a)
		:Function(a)
	{
	}

	// Geerbt über Function
	virtual double eval() const override;

	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;


	// Geerbt über Function
	virtual Expression partDif(ModelParameter & p) override;

	virtual Expression simplify() const override;
	
	virtual Expression substitute(const Expression &, const Expression &) override;
};




class Sin : public Function
{

public:

	Sin(const IExpression & a)
		:Function(a)
	{
	}

	// Geerbt über Function
	virtual double eval() const override;

	virtual Expression partDif(ModelParameter & p) override;

	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

	virtual Expression simplify() const override;

	virtual Expression substitute(const Expression &, const Expression &) override;
};



class Cos : public Function
{
public:

	Cos(const IExpression & a)
		:Function(a)
	{}


	// Geerbt über Function
	virtual double eval() const override;

	virtual Expression partDif(ModelParameter & p) override;

	virtual std::string toString() const override;

	virtual IExpression * deepCopy() const override;

	virtual Expression simplify() const override;

	virtual Expression substitute(const Expression &, const Expression &) override;

};


class TriangleOfPower : public IExpression
{
protected:

	IExpression *A, *B;

	TriangleOfPower(IExpression* a, IExpression* b)
		:A(a), B(b)
	{
		reHash();
	}


	virtual ~TriangleOfPower()
	{
		delete A;
		delete B;
	}

	virtual Expression partDif(ModelParameter & p) override
	{
		throw "not implemented";
	}

	virtual void reHash() override;



};

class Power : public TriangleOfPower
{
public:

	Power(const IExpression & a, const IExpression & b)
		:TriangleOfPower(a.deepCopy(), b.deepCopy())
	{

	}


	// Geerbt über TriangleOfPower
	virtual double eval() const override;
	virtual std::string toString() const override;
	virtual IExpression * deepCopy() const override;
	virtual Expression partDif(ModelParameter & p) override;
	virtual Expression simplify() const override;

	// Geerbt über TriangleOfPower
	virtual Expression substitute(const Expression &, const Expression &) override;
};

class Log : public TriangleOfPower
{
public:

	Log(const IExpression & a, const IExpression & b)
		:TriangleOfPower(a.deepCopy(), b.deepCopy())
	{

	}



	// Geerbt über TriangleOfPower
	virtual double eval() const override;
	virtual std::string toString() const override;
	virtual IExpression * deepCopy() const override;
	virtual Expression simplify() const override;
	virtual Expression partDif(ModelParameter & p) override;

	// Geerbt über TriangleOfPower
	virtual Expression substitute(const Expression &, const Expression &) override;
};



class Root : public TriangleOfPower
{
public:
	Root(const IExpression & a, const IExpression & b)
		:TriangleOfPower(a.deepCopy(), b.deepCopy())
	{

	}

	// Geerbt über TriangleOfPower
	virtual double eval() const override;
	virtual std::string toString() const override;
	virtual IExpression * deepCopy() const override;
	virtual Expression simplify() const override;
	virtual Expression partDif(ModelParameter & p) override;

	// Geerbt über TriangleOfPower
	virtual Expression substitute(const Expression &, const Expression &) override;
};



class Sqrt : public Root
{
public:
	Sqrt(const IExpression & a)
		:Root(a, Constant(2)) // ziemlicher pfusch // glaub ned das das geht //dynamic_cast<IExpression*>
	{}
	

	// Geerbt über Root
	virtual std::string toString() const override;
	virtual IExpression * deepCopy() const override;
	virtual Expression partDif(ModelParameter & p) override;
};




Expression operator+ (const Expression& a, const  Expression& b);
Expression operator- (const Expression& a, const  Expression& b);
Expression operator* (const Expression& a, const  Expression& b);
Expression operator/ (const Expression& a, const  Expression& b);

Expression operator-(const Expression& a);


Expression operator+= (Expression& a, const  Expression& b);
Expression operator-= (Expression& a, const  Expression& b);
Expression operator*= (Expression& a, const  Expression& b);
Expression operator/= (Expression& a, const  Expression& b);




Expression operator^ (const Expression& a, const  Expression& b);

Expression ln(const Expression& e);

Expression sin(const Expression  & e);
Expression cos(const Expression  & e);
Expression tan(const Expression  & e);



namespace SymbolicConstant
{
	// Maybe write another type like "NamedConstants"

	static const ModelParameter pi("pi", M_PI, ModelParameterType::Fix);
	static const ModelParameter e("e", M_E, ModelParameterType::Fix);

	static const ModelParameter i("i"); // TODO Rethink complex numbers!!

}


// Mixed - Type Operators