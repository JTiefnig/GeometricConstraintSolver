#pragma once


#include "Equation.h"

using namespace System;




public ref class EquationProperty
{
	String^ _Name;
	String^ _LateX;


public:
	EquationProperty();
	EquationProperty(Equation* eq);

	property String^ Name
	{
		String^ get()
		{
			return _Name;
		}
	}


	property String^ LateX
	{
		String^ get()
		{
			return _LateX;
		}
	}
	
};

