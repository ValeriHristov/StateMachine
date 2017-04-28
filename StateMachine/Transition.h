#pragma once
#include "State.h"
class State;
class Transition
{
private:
	char letter;
	//State* to;
	int indexOfNextState;
public:
	//Transition(char letter, State* to);
	Transition(char letter, int index);
	Transition();
	char GetLetter() const;
	int Transist() const;
};

