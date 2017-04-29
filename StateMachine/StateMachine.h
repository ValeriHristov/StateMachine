#pragma once
#include <vector>
#include <unordered_map>
#include "State.h"
#include "Transition.h"
#include "String.h"

class StateMachine
{
private:
	String regex;
	int start;
	int currentState;
	std::vector<State*> states;
	std::vector<State*> finalStates;
	void AddState(State*);
	void CopyStateMachine(StateMachine&, const StateMachine&);
	void DeleteStateMachine(StateMachine&);
	void RemoveUnreachableStates();
	int IndexOfState(const State*) const;
	char* SpecialSymbols = "|()*\0";
public:
	StateMachine(char letter);
	StateMachine();
	StateMachine(String);
	StateMachine(const StateMachine&);
	~StateMachine();
	StateMachine& operator=(const StateMachine&);
	bool Recognize(String word);
	StateMachine Union(const StateMachine& other)const;
	StateMachine Concatenate(const StateMachine& other) const;
	StateMachine Iteration() const;
	String GetRegex() const;
	//TODO remove unreacheable states
	//TODO determination
	//TODO minimization
	//TODO use string instead of char*
};