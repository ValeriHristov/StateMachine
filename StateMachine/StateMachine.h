#pragma once
#include <vector>
#include <unordered_map>
#include "State.h"
#include "Transition.h"

class StateMachine
{
private:
	char* regex;
	State* start;
	State* currentState;
	std::vector<State*> states;
	std::vector<State*> finalStates;
	void AddState(State*);
	void CopyStateMachine(StateMachine&, const StateMachine&);
	void DeleteStateMachine(StateMachine&);
	int IndexOfState(const State*) const;
public:
	StateMachine(char letter);
	StateMachine();
	StateMachine(const StateMachine&);
	~StateMachine();
	StateMachine& operator=(const StateMachine&);
	bool Recognize(char* word);
	StateMachine Union(const StateMachine& other)const;
	StateMachine Concatenate(const StateMachine& other) const;
	StateMachine Iteration() const;
	char* GetRegex() const;
	//TODO copyctor, =,==, destructor
	//TODO remove unreacheable states
	//TODO determination
	//TODO minimization
	//TODO use string instead of char*
};