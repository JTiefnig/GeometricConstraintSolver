#pragma once






// absract?
class INumericTypes
{
public:
	INumericTypes();
	~INumericTypes();
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

