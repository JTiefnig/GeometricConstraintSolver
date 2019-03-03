#pragma once
#include "ISketchEntity.h"
#include "Expression.h"




class Constraint :
	public ISketchEntity
{
public:

	virtual ~Constraint();


	Constraint(std::string ID);

	~Constraint();

	// fix or open ?
	virtual ModelParameterType getType()
	{
		return ModelParameterType::Fix;
	}

	virtual void ModifyToSatisfy() {};
};




class Distance : public Constraint
{
protected:

	Parameter* val;

public:

	Distance(std::string ID, BaseParam *p)
		:Constraint(ID), val(p)
	{}

	virtual double& getVal() const
	{
		return val->Val();
	}

	virtual std::string getParamId() const
	{
		return val->getName();
	}

	virtual void ModifyToSatisfy() override;

	virtual Vector2d GetP1() const = 0;
	virtual Vector2d GetP2() const = 0;

	virtual ParamType getType()
	{
		return val->getType();
	}

};