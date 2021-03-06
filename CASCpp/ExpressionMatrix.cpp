#include "ExpressionMatrix.h"
#include <string>
#include <sstream>

using namespace std;


ExpressionMatrix::ExpressionMatrix()
{
}


ExpressionMatrix::~ExpressionMatrix()
{
}

std::string ExpressionMatrix::toString() const
{
	stringstream ss;

	ss << "[";
	for (int i = 0; i < m; i++)
	{
		ss << "[";
		for (int j = 0; j < n; j++)
		{
			ss << v[i][j].toString();

			if (j < m - 1)
				ss << ",";
		}
		ss << "]";
	}
	ss << "]";


	return ss.str();
}

IExpression * ExpressionMatrix::deepCopy() const
{

	ExpressionMatrix *ret = new ExpressionMatrix(*this);

	return ret;
}

double ExpressionMatrix::eval() const
{
	return 0.0; // Rethink This!!
}

Expression ExpressionMatrix::partDif(ModelParameter & p)
{
	ExpressionMatrix ret(this->m, this->n);

	for(int i =0; i< this->m; i++)
		for (int j = 0; j < this->n; j++)
		{
			ret[i][j] = this->v[i][j].partDif(p); 
		}

	return ret;
}

Expression ExpressionMatrix::simplify() const
{
	ExpressionMatrix ret(this->m, this->n);

	for (int i = 0; i < this->m; i++)
		for (int j = 0; j < this->n; j++)
		{
			ret[i][j] = this->v[i][j].simplify();
		}

	return ret;
}

void ExpressionMatrix::reHash()
{
}

Expression ExpressionMatrix::substitute(const Expression &a, const Expression &b)
{
	if (compare(a))
		return b;

	ExpressionMatrix ret(*this);

	for (int i = 0; i < this->m; i++)
		for (int j = 0; j < this->n; j++)
		{
			ret[i][j] = this->substitute(a, b);
		}

	if (ret.compare(a))
		return b;

	return ret;
}

bool ExpressionMatrix::contains(const SymbolicMathElement &) const
{
	return false;
}

void ExpressionMatrix::toLaTeX(std::ostream &o) const
{
	o << "\\begin{matrix}";

	for (int i = 0; i < this->m; i++)
	{

		for (int j = 0; j < this->n; j++)
		{

			(*this)[i][j].toLaTeX(o);

			if (j < n - 1)
				o << "&";

		}
		o << "\\\\"; // next line
	}

	o << "\\end{matrix}";

}
