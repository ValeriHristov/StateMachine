#include "StateUnion.h"

bool StateUnion::operator==(const StateUnion& other) const
{
	return this->statesIndexes == other.statesIndexes;
}

StateUnion::StateUnion()
{}

StateUnion::StateUnion(bool isFinal)
{
	this->isFinal = isFinal;
}

void StateUnion::AddStateIndex(int index)
{
	this->statesIndexes.AddElement(index);
}