#include "StateUnion.h"

bool StateUnion::operator==(const StateUnion& other) const
{
	return this->statesIndexes == other.statesIndexes;
}

StateUnion::StateUnion(bool isFinal) : State(isFinal)
{}

void StateUnion::AddStateIndex(int index)
{
	this->statesIndexes.AddElement(index);
}