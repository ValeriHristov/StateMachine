#include "State.h"

State::State()
{
	this->isFinal = false;
}

State::State(bool isEnd)
{
	this->isFinal = isEnd;
}

State::State(const State* state)
{
	this->isFinal = state->isFinal;
	this->transitions = state->transitions;
}

void State::AddFunctionality(const State& other, bool copyIsFinal)
{
	std::vector<Transition> temp = other.GetAllTransitions();
	for (int i = 0; i < temp.size(); i++)
	{
		this->AddTransition(temp[i]);
	}
	if (copyIsFinal)
	{
		this->isFinal = other.IsFinal();
	}
}

void State::AddTransition(const Transition& tr)
{
	this->transitions[tr.GetLetter()].push_back(tr);
}

bool State::IsFinal() const
{
	return this->isFinal;
}

std::vector<Transition> State::Transist(String word) const
{
	if (this->transitions.count(word[0]))
	{
		return this->transitions.at(word[0]);
	}
	std::vector<Transition> a;
	return a;
}

std::vector<Transition> State::GetAllTransitions() const
{
	std::vector<Transition> temp;
	for (const auto &myPair : this->transitions) {
		for (int i = 0; i < myPair.second.size(); i++)
		{
			temp.push_back(myPair.second[i]);
		}
	}
	return temp;
}
