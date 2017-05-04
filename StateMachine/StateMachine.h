#pragma once
#include <vector>
#include <unordered_map>
#include "Transition.h"
#include "String.h"
#include "StateUnion.h"

class StateMachine
{
private:
	String regex;
	std::vector<int> starts;
	int currentState;
	char* SpecialSymbols = "|()*.\0";
	std::vector<State*> states;
	std::vector<State*> finalStates;
	void AddState(State*);
	void CopyStateMachine(StateMachine&, const StateMachine&);
	void DeleteStateMachine(StateMachine&);
	void RemoveUnreachableStates();
	int IndexOfState(const State*) const;	
	String RegexToRPN(String regex) const;
	void MapTransitionsToNewIndexes(std::vector<int>);
	std::vector<int> GetUnreachableStatesIndexes() const;
	StateMachine Calculate(String regex);
	String AddConcatenationOperator(String regex) const;
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
	//TODO determination
	//TODO minimization
};