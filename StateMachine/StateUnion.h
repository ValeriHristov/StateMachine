#pragma once
#include "State.h"
#include "Sorted.cpp"
class StateUnion :public State
{
	Sorted<int> statesIndexes;
public:
	StateUnion();
	StateUnion(bool isFinal);
	bool operator==(const StateUnion& other) const;
	void AddStateIndex(int index);
	Sorted<int> GetStatesIndexes()const
	{
		return this->statesIndexes;
	}
};