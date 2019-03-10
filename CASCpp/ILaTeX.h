#pragma once
#include <ostream>
#include <sstream>




// Will use that later - Maybe Baby
class LaTeXStream: public std::stringstream
{

public:
	bool ShowParameterValue = false;

	// some other definitions 
};



class ILaTeX
{
public:
	virtual ~ILaTeX() {}

	virtual void toLaTeX(std::ostream &) const =0;


};

