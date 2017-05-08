#include "StateUnion.h"

bool StateUnion::operator==(const StateUnion& other) const
{
	return this->statesIndexes == other.statesIndexes;
}

void StateUnion::AddFunctionality(const State& other, bool copyIsFinal,int index)
{
	State::AddFunctionality(other, copyIsFinal);
	this->statesIndexes.AddElement(index);
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