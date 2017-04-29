#include "Transition.h"

//Transition::Transition(char letter, State* to)
//{
//	this->letter = letter;
//	this->to = to;
//}

Transition::Transition(char letter, int index)
{
	this->letter = letter;
	this->indexOfNextState = index;
}

char Transition::GetLetter()const
{
	return this->letter;
}

int Transition::Transist() const
{
	return this->indexOfNextState;
}

bool Transition::operator==(const Transition& other) const
{
	return this->letter == other.letter && this->indexOfNextState == other.indexOfNextState;
}
