#include <vector>
#include <unordered_map>
#include <ctype.h>
#include <iostream>
#include "StateMachine.h"

bool ValidateRegex(String regex)
{
	char t;
	for (int i = 0; i < regex.Length(); i++)
	{
		t = regex[i];
		if (isalpha(t) || isdigit(t) || t == '|' || t == '(' || t == ')' || t == '*')
		{
			continue;
		}
		return false;
	}
	return true;
}

int StateMachine::IndexOfState(const State* state) const
{
	return std::find(this->states.begin(), this->states.end(), state) - this->states.begin();
}

void StateMachine::CopyStateMachine(StateMachine& dest, const StateMachine& source)
{
	dest.regex = source.regex;

	int size = source.states.size();
	for (int i = 0; i < size; i++)
	{
		State* t = new State(source.states[i]);
		if (source.states[i] == source.states[source.start])
		{
			dest.start = i;
		}
		if (source.states[i] == source.states[source.currentState])
		{
			dest.currentState = i;
		}
		if (t->IsFinal())
		{
			dest.finalStates.push_back(t);
		}
		dest.states.push_back(t);
	}
}

void StateMachine::DeleteStateMachine(StateMachine& machine)
{
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

//TODO implement the proper recursive algorithm
void StateMachine::RemoveUnreachableStates()
{
	int size = this->states.size();
	bool* isStateReachable = new bool[size];
	for (int i = 0; i < size; i++)
	{
		isStateReachable[i] = false;
	}
	isStateReachable[start] = true;
	std::vector<Transition> t;
	for (int i = 0; i < size; i++)
	{
		t = this->states[i]->GetAllTransitions();
		for (int j = 0; j < t.size(); j++)
		{
			isStateReachable[t[j].Transist()] = true;
		}
	}

	std::vector<int> deletedStatesIndexes;
	for (int i = size - 1; i >= 0; i--)
	{
		if (!isStateReachable[i])
		{
			this->states.erase(this->states.begin() + i);
			deletedStatesIndexes.push_back(i);
		}
	}
	State* temp;
	std::vector<Transition> transitions;

	for (int k = 0; k < deletedStatesIndexes.size(); k++)
	{
		for (int i = 0; i < this->states.size(); i++)
		{
			temp = this->states[i];
			transitions = temp->GetAllTransitions();
			bool needsChange = false;
			for (int j = 0; j < transitions.size(); j++)
			{
				if (transitions[j].Transist()>deletedStatesIndexes[k])
				{
					needsChange = true;
					break;
				}
			}
			if (needsChange)
			{
				State modifiedState;
				for (int g = 0; g < transitions.size(); g++)
				{
					Transition trans = transitions[g];
					if (transitions[g].Transist()>deletedStatesIndexes[k])
					{
						modifiedState.AddTransition(Transition(trans.GetLetter(), trans.Transist() - 1));
					}
					else
					{
						modifiedState.AddTransition(trans);
					}
				}
				*temp = modifiedState;
			}
		}
	}
	delete[] isStateReachable;
}

StateMachine::StateMachine()
{
	this->regex = String();
	State* startState = new State(true);
	AddState(startState);
	this->start = 0;
	this->currentState = this->start;
}

StateMachine::StateMachine(const StateMachine& other)
{
	CopyStateMachine(*this, other);
}

StateMachine::StateMachine(char letter)
{
	this->regex = String(letter);

	State* startState = new State();
	State* endState = new State(true);

	AddState(startState);
	AddState(endState);

	this->start = this->IndexOfState(startState);
	this->currentState = this->start;
	startState->AddTransition(Transition(letter, IndexOfState(endState)));

}

StateMachine::StateMachine(String regex)
{
	if (!ValidateRegex(regex))
	{
		std::cerr << "Invalid regex. Creating an empty state machine";
		*this = StateMachine();
	}

	if (!(regex.ContainsAny(SpecialSymbols, strlen(SpecialSymbols))))
	{
		StateMachine newMachine(regex[0]);
		for (int i = 1; i < regex.Length(); i++)
		{
			newMachine = newMachine.Concatenate(StateMachine(regex[i]));
		}
		*this = newMachine;
		this->RemoveUnreachableStates();
		return;
	}

	int index = regex.FirstIndexOfAny(SpecialSymbols, strlen(SpecialSymbols));
	StateMachine temp(regex.Substring(0, index));

	if (regex[index] == '(')
	{
		String sub = regex.Substring(index + 1);
		StateMachine t(sub);
		temp = temp.Concatenate(temp);
	}
	else if (regex[index] == ')')
	{
		*this = temp;
		return;
	}
	else if (regex[index] == '|')
	{
		temp = temp.Union(StateMachine(regex.Substring(index + 1)));
	}
	*this = temp;
	this->regex = regex;
	this->RemoveUnreachableStates();
}

StateMachine::~StateMachine()
{
	DeleteStateMachine(*this);
}

StateMachine& StateMachine::operator=(const StateMachine& other)
{
	DeleteStateMachine(*this);
	CopyStateMachine(*this, other);
	return *this;
}

bool StateMachine::Recognize(String word)
{
	if (word.Length() == 0)
	{
		return this->states[currentState]->IsFinal();
	}

	std::vector<Transition> transitions = this->states[currentState]->Transist(word);
	bool recognized = false;
	for (int i = 0; i < transitions.size(); i++)
	{
		this->currentState = transitions[i].Transist();
		recognized = recognized || this->Recognize(word.Substring(1));
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
	if (state->IsFinal())
	{
		this->finalStates.push_back(state);
	}
	this->states.push_back(state);
}

String StateMachine::GetRegex() const
{
	return this->regex;
}

StateMachine StateMachine::Union(const StateMachine& other) const
{
	StateMachine newMachine(*this);
	newMachine.regex.Append(String(String("|") + other.regex));

	// the new start is also an end if any of the old starts are end
	State* newStart = new State(this->states[this->start]->IsFinal() || other.states[other.start]->IsFinal());
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
		std::vector<Transition> temp = other.states[i]->GetAllTransitions();
		for (int j = 0; j < temp.size(); j++)
		{
			//TEMPORAL
			/*	int index1 = i + this->states.size();
				State* tempState = newMachine.states[index1];
				int stateTransist = temp[j].Transist();
				char letter1 = temp[j].GetLetter();
				Transition trans(letter1, oldStatesToNew[stateTransist]);
				tempState->AddTransition(trans);*/
			//TEMPORAL
			newMachine.states[i + this->states.size()+1]->
				AddTransition(Transition(temp[j].GetLetter(), oldStatesToNew[temp[j].Transist()]));
		}
	}

	//Make the new start do the work of the old start of this
	std::vector<Transition> temp = this->states[this->start]->GetAllTransitions();
	for (int i = 0; i < temp.size(); i++)
	{
		Transition t = temp[i];
		Transition newT(t.GetLetter(), t.Transist());
		newStart->AddTransition(newT);
	}

	//Make the new start do the work of the start of other
	temp = other.states[other.start]->GetAllTransitions();
	for (int i = 0; i < temp.size(); i++)
	{
		Transition t = temp[i];
		Transition newT(t.GetLetter(), oldStatesToNew[t.Transist()]);
		newStart->AddTransition(newT);
	}

	newMachine.start = newMachine.IndexOfState(newStart);
	newMachine.currentState = newMachine.IndexOfState(newStart);
	newMachine.RemoveUnreachableStates();
	return newMachine;
}

StateMachine StateMachine::Concatenate(const StateMachine& other) const
{
	StateMachine newMachine(*this);
	newMachine.regex.Append(other.regex);

	//add all states from the other machine
	std::unordered_map<int, int> oldStatesToNew;
	std::vector<State*> tempStates = other.states;
	for (int i = 0; i < other.states.size(); i++)
	{
		oldStatesToNew[other.IndexOfState(tempStates[i])] = newMachine.states.size();
		State* t = new State(tempStates[i]->IsFinal());
		newMachine.AddState(t);
	}

	//add the functionality for the new states
	for (int i = 0; i < other.states.size(); i++)
	{
		std::vector<Transition> temp = other.states[i]->GetAllTransitions();
		for (int j = 0; j < temp.size(); j++)
		{
			newMachine.states[i + this->states.size()]->
				AddTransition(Transition(temp[j].GetLetter(), oldStatesToNew[temp[j].Transist()]));
		}
	}

	//make the final states of the first do the work of the start of the second
	int endStatesCount = this->finalStates.size();
	for (int i = 0; i < endStatesCount; i++)
	{
		std::vector<Transition> temp = other.states[other.start]->GetAllTransitions();
		for (int j = 0; j < temp.size(); j++)
		{
			Transition t = temp[j];
			Transition newT(t.GetLetter(), oldStatesToNew[t.Transist()]);
			newMachine.finalStates[i]->AddTransition(newT);
		}
	}

	//the final states remain final if the start of the second was final
	if (!(other.states[other.start]->IsFinal()))
	{
		for (int i = 0; i < this->finalStates.size(); i++)
		{
			newMachine.finalStates[i]->AddFunctionality(State(false), true);
		}
	}
	return newMachine;
}

StateMachine StateMachine::Iteration() const
{
	StateMachine newMachine(*this);
	State* newState = new State(true);
	newMachine.AddState(newState);

	int endStatesCount = newMachine.finalStates.size();
	std::vector<Transition> t = this->states[this->start]->GetAllTransitions();
	for (int i = 0; i < endStatesCount; i++)
	{
		for (int j = 0; j < t.size(); j++)
		{
			newMachine.finalStates[i]->AddTransition(t[j]);
		}
	}
	newMachine.start = this->IndexOfState(newState);
	newMachine.currentState = newMachine.start;
	return newMachine;
}