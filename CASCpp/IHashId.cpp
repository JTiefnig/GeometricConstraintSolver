#include "IHashId.h"



IHashId::IHashId()
	:hash(NULL)
{

}

IHashId::IHashId(const IHashId &temp )
	:hash(temp.getHash()) // you can only copy if there is an hash// will see if thats smart
{
}


IHashId::~IHashId()
{
}

hashid IHashId::getHash() const
{
	if (hash == NULL)
		throw HashIdException("Hash is NULL!");

	return this->hash;
}

bool IHashId::compare(const IHashId &a) const
{
	return this->getHash() == a.getHash();
}

