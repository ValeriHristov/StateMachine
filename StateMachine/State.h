#pragma once
#include <vector>
#include <unordered_map>
#include "Transition.h"

class Transition;
class State
{
	bool isFinal;
	std::unordered_map<char, std::vector<Transition>> transitions;
public:
	State(bool isFinal);
	State();
	State(const State*);
	bool operator==(const State&) const;
	void AddTransition(const Transition&);
	bool IsFinal() const;
	std::vector<Transition> Transist(char* word) const;
	std::vector<Transition> GetAllTransitions() const;
	void AddFunctionality(const State&, bool copyIsFinal);

};
