#pragma once
#include "State.h"
#include "Sorted.cpp"
class StateUnion :public State
{
	Sorted<int> statesIndexes;
public:
	bool operator==(const StateUnion& other) const;
	void AddFunctionality(const State&, bool copyIsFinal, int index);
};