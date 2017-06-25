#include "StateMachineDeterminizator.h"

int IndexOfStateUnion(std::vector<StateUnion*> vector, const StateUnion& state);

void StateMachineDeterminizator::DeterminationHelperCreateUnionStates(std::vector<StateUnion*>& states, StateMachine* object)
{
	std::vector<Transition > transitions;
	std::vector<std::vector<Transition>> t;
	bool isChanged;
	int processedSoFar = 0;
	//Process until no new states are created
	do
	{
		isChanged = false;
		//add transitions to the new unions
		for (int i = processedSoFar; i < states.size(); i++)
		{
			Sorted<int> indexes = states[i]->GetStatesIndexes();
			for (int j = 0; j < indexes.Length(); j++)
			{
				transitions = object->states[indexes[j]]->GetAllTransitions();

				for (int k = 0; k < transitions.size(); k++)
				{
					states[i]->AddTransition(transitions[k]);
					isChanged = true;
				}
			}
		}

		//create the new unions
		int size = states.size();
		for (int g = processedSoFar; g < size; g++)
		{
			t = states[g]->GetAllTransitionsGrouped();
			for (int i = 0; i < t.size(); i++)
			{
				StateUnion temp;
				for (int j = 0; j < t[i].size(); j++)
				{
					Transition tempo = t[i][j];
					temp.AddStateIndex(t[i][j].Transist());
				}
				if (IndexOfStateUnion(states, temp) == -1)
				{
					states.push_back(new StateUnion(temp));
				}
			}
			processedSoFar++;
		}
	} while (processedSoFar != states.size() && isChanged);
}

void StateMachineDeterminizator::DeterminationHelperAddStates(const std::vector<StateUnion*> states, const StateMachine& source,StateMachine* object)
{
	std::vector<std::vector<Transition>> t;
	for (int i = 0; i < states.size(); i++)
	{
		Sorted<int> indexes = states[i]->GetStatesIndexes();
		bool isFinal = false;
		for (int j = 0; j < indexes.Length(); j++)
		{
			if (source.states[indexes[j]]->IsFinal())
			{
				isFinal = true;
				break;
			}
		}
		State* newState = new State(isFinal);
		t = states[i]->GetAllTransitionsGrouped();
		for (int j = 0; j < t.size(); j++)
		{
			StateUnion temp;
			for (int k = 0; k < t[j].size(); k++)
			{
				temp.AddStateIndex(t[j][k].Transist());
			}
			newState->AddTransition(Transition(t[j][0].GetLetter(), IndexOfStateUnion(states, temp)));
		}
		object->AddState(newState);
	}
}

void StateMachineDeterminizator::Determinate(StateMachine* object)
{
	//Unite the start states
	StateUnion* newState = new StateUnion();
	int processedSoFar = 0;
	std::vector<StateUnion*> states;
	for (int i = 0; i < object->starts.size(); i++)
	{
		newState->AddStateIndex(object->starts[i]);
	}
	states.push_back(newState);

	this->DeterminationHelperCreateUnionStates(states, object);

	//add the states to the machine with the correct transitions
	StateMachine* newMachine = new StateMachine();
	this->DeterminationHelperAddStates(states, *object, newMachine);

	newMachine->starts.push_back(0);
	newMachine->regex = object->regex;
	newMachine->isReversed = object->isReversed;
	*object = *newMachine;
	object->isDeterministic = true;
	delete newMachine;
}