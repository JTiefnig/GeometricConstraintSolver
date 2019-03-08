#pragma once
#include <xhash>
#include <exception>


typedef std::size_t hashid;


class IHashId
{
protected:

	hashid hash;

public:
	IHashId();
	IHashId(const IHashId&);

	~IHashId();

	virtual void reHash()=0;

	virtual hashid getHash() const;

	virtual bool compare(const IHashId&) const ;

};


class HashIdException : public std::exception
{
public:
	HashIdException(const char* msg)
		:exception(msg)
	{}
};