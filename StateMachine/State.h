#pragma once
#include <vector>
#include <unordered_map>
#include "Transition.h"
#include "String.h"
class Transition;
class State
{
protected:
	bool isFinal;
	std::unordered_map<char, std::vector<Transition>> transitions;
public:
	State(bool isFinal);
	State();
	State(const State*);
	void AddTransition(const Transition&);
	bool IsFinal() const;
	std::vector<Transition> Transist(String word) const;
	std::vector<Transition> GetAllTransitions() const;
	std::vector<std::vector<Transition>> GetAllTransitionsGrouped() const;
	void AddFunctionality(const State&, bool copyIsFinal);

};
