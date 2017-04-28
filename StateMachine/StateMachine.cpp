#include <vector>
#include <unordered_map>
#include "StateMachine.h"

int StateMachine::IndexOfState(const State* state) const
{
	return std::find(this->states.begin(), this->states.end(), state) - this->states.begin();
}

void StateMachine::CopyStateMachine(StateMachine& dest, const StateMachine& source)
{
	char* temp = source.GetRegex();
	dest.regex = new char[strlen(temp) + 1];
	strcpy(dest.regex, temp);
	delete[] temp;

	int size = source.states.size();
	for (int i = 0; i < size; i++)
	{
		State* t = new State(source.states[i]);
		if (source.states[i] == source.start)
		{
			dest.start = t;
		}
		if (source.states[i] == source.currentState)
		{
			dest.currentState = t;
		}
		if ((*t).IsFinal())
		{
			dest.finalStates.push_back(t);
		}
		dest.states.push_back(t);
	}
}

void StateMachine::DeleteStateMachine(StateMachine& machine)
{
	delete[] machine.regex;
	for (int i = 0; i < machine.states.size(); i++)
	{
		delete machine.states[i];
	}
	int size = this->states.size();
	for (int i = 0; i < size; i++)
	{
		this->states.pop_back();
	}
	size = this->finalStates.size();
	for (int i = 0; i < size; i++)
	{
		this->finalStates.pop_back();
	}
}

StateMachine::StateMachine()
{
	this->regex = new char[1];
	State* startState = new State(true);
	this->start = startState;
	this->currentState = this->start;
	AddState(startState);
}

StateMachine::StateMachine(const StateMachine& other)
{
	CopyStateMachine(*this, other);
}

StateMachine::~StateMachine()
{
	DeleteStateMachine(*this);
}

StateMachine::StateMachine(char letter)
{
	this->regex = new char(letter);

	State* startState = new State();
	State* endState = new State(true);

	this->start = startState;
	AddState(startState);
	AddState(endState);

	(*startState).AddTransition(Transition(letter, IndexOfState(endState)));

	this->currentState = this->start;
}

StateMachine& StateMachine::operator=(const StateMachine& other)
{
	DeleteStateMachine(*this);
	CopyStateMachine(*this, other);
	return *this;
}

bool StateMachine::Recognize(char* word)
{
	if (strlen(word) == 0)
	{
		return (*this->currentState).IsFinal();
	}

	std::vector<Transition> transitions = (*this->currentState).Transist(word);
	bool recognized = false;
	for (int i = 0; i < transitions.size(); i++)
	{
		this->currentState = this->states[transitions[i].Transist()];
		recognized = recognized || this->Recognize(word + 1);
		if (recognized)
		{
			this->currentState = this->start;
			return true;
		}
	}
	this->currentState = this->start;
	return false;
}

void StateMachine::AddState(State* state)
{
	if ((*state).IsFinal())
	{
		this->finalStates.push_back(state);
	}
	this->states.push_back(state);
}

char* StateMachine::GetRegex() const
{
	return new char(*this->regex);
}

StateMachine StateMachine::Union(const StateMachine& other) const
{
	StateMachine newMachine(*this);
	//TODO write correct regex for the union

	// the new start is also an end if any of the old starts are end
	State* newStart = new State((*this->start).IsFinal() || (*other.start).IsFinal());
	newMachine.AddState(newStart);

	//add all states from the other machine
	std::vector<State*> tempState = other.states;
	std::unordered_map<int, int> oldStatesToNew;
	for (int i = 0; i < other.states.size(); i++)
	{
		State* t = new State((*tempState[i]).IsFinal());
		newMachine.AddState(t);
		oldStatesToNew[other.IndexOfState(tempState[i])] = newMachine.states.size() - 1;
	}

	//add the functionality for the new states
	for (int i = 0; i < other.states.size(); i++)
	{
		std::vector<Transition> temp = (*other.states[i]).GetAllTransitions();
		for (int j = 0; j < temp.size(); j++)
		{
			(*newMachine.states[i + this->states.size()]).AddTransition(Transition(temp[j].GetLetter(), oldStatesToNew[temp[i].Transist()]));
		}
	}

	//Make the new start do the work of the old start of this
	std::vector<Transition> temp = (*this->start).GetAllTransitions();
	(*newStart).AddFunctionality(this->start, false);

	//Make the new start do the work of the start of other
	temp = (*other.start).GetAllTransitions();
	for (int i = 0; i < temp.size(); i++)
	{
		Transition t = temp[i];
		Transition newT(t.GetLetter(), oldStatesToNew[t.Transist()]);
		(*newStart).AddTransition(newT);
	}

	newMachine.start = newStart;
	newMachine.currentState = newStart;
	return newMachine;
}

StateMachine StateMachine::Concatenate(const StateMachine& other) const
{
	StateMachine newMachine(*this);

	//add all states from the other machine
	std::vector<State*> tempState = other.states;
	std::unordered_map<int, int> oldStatesToNew;
	for (int i = 0; i < other.states.size(); i++)
	{
		State* t = new State(tempState[i]);
		newMachine.AddState(t);
		oldStatesToNew[other.IndexOfState(tempState[i])] = newMachine.states.size() - 1;
	}

	//add the functionality for the new states
	for (int i = 0; i < other.states.size(); i++)
	{
		std::vector<Transition> temp = (*other.states[i]).GetAllTransitions();
		for (int j = 0; j < temp.size(); j++)
		{
			(*newMachine.states[i + this->states.size()])
				.AddTransition(Transition(temp[j].GetLetter(),oldStatesToNew[temp[j].Transist()]));
		}
	}

	//make the final states of the first do the work of the start of the second
	int endStatesCount = this->finalStates.size();
	for (int i = 0; i < endStatesCount; i++)
	{
		std::vector<Transition> temp = (*other.start).GetAllTransitions();
		for (int j = 0; j < temp.size(); j++)
		{
			Transition t = temp[j];
			Transition newT(t.GetLetter(), oldStatesToNew[t.Transist()]);
			(*newMachine.finalStates[i]).AddTransition(newT);
		}
	}

	//the final states remain final if the start of the second was final
	if (!(*(other.start)).IsFinal())
	{
		for (int i = 0; i < this->finalStates.size(); i++)
		{
			(*(newMachine.finalStates[i])).AddFunctionality(State(false), true);
		}
	}
	return newMachine;
}

