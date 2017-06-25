#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "Transition.h"
#include "String.h"
#include "StateUnion.h"

class StateMachine
{
private:
	String regex;
	std::vector<int> starts;
	int currentState;
	bool isDeterministic;
	bool isReversed;
	char* SpecialSymbols = "|()*.\0";
	std::vector<State*> states;
	std::vector<State*> finalStates;
	void AddState(State*);
	void CopyStateMachine(StateMachine&, const StateMachine&);
	void DeleteStateMachine(StateMachine&);
	void RemoveUnreachableStates();
	void TransferStates(const StateMachine& other, std::unordered_map<int, int>& oldStatesToNew);
	bool HasFinalStartState() const;
	void AddMappedTransitions(const StateMachine& other, std::unordered_map<int, int>& oldStatesToNew,int startIndex);
	int IndexOfState(const State*) const;
	String RegexToRPN(String regex) const;
	void MapTransitionsToNewIndexes(std::vector<int>);
	std::vector<int> GetUnreachableStatesIndexes() const;
	StateMachine Calculate(String regex);
	String AddConcatenationOperator(String regex) const;
	bool RecognizeHelper(String word);
	bool CheckIfDeterministic() const;
	
public:
	StateMachine(char letter);
	StateMachine();
	StateMachine(String);
	StateMachine(const StateMachine&);
	StateMachine(std::ifstream& is);
	~StateMachine();
	StateMachine& operator=(const StateMachine&);
	bool Recognize(String word);
	bool IsLanguageEmpty() const;
	bool IsDeterministic() const;
	bool IsLanguageFinite() const;
	StateMachine Union(const StateMachine& other)const;
	StateMachine Concatenate(const StateMachine& other) const;
	StateMachine Iteration() const;
	String GetRegex() const;
	void Determinate();
	void Reverse();
	void Minimize();
	void Print() const;
	String ToString() const;
	String TransitionsToString() const;
};