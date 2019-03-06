#pragma once






// absract?
class INumericTypes
{
public:
	INumericTypes();
	~INumericTypes();

	virtual int getM()=0;
	virtual int getN()=0;

};




template <class T>
class Numeric
{
	T value;

public:

	// rethink this!!

	virtual operator T& ()
	{
		return this->value;
	}
};

