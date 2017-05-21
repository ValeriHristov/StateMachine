#include <vector>
#include <unordered_map>
#include <ctype.h>
#include <iostream>
#include <stack>
#include <queue>
#include "StateMachine.h"

int IndexOfStateUnion(std::vector<StateUnion*> vector, const StateUnion& state)
{
	for (int i = 0; i < vector.size(); i++)
	{
		if ((*(vector[i])) == state)
		{
			return i;
		}
	}
	return -1;
}

bool ValidateRegex(String regex)
{
	char t;
	int len = regex.Length();
	bool isPreviousSpecialSymbol=false;
	for (int i = 0; i < len; i++)
	{
		t = regex[i];
		if (t == '|' || t == '*')
		{
			if (isPreviousSpecialSymbol)
			{
				return false;
			}
			isPreviousSpecialSymbol = true;
			continue;
		}
		if (t == '(' || t == ')')
		{
			isPreviousSpecialSymbol = false;
			continue;
		}
		if (isalpha(t) || isdigit(t))
		{
			isPreviousSpecialSymbol = false;
			continue;
		}
		return false;
	}

	std::stack<char> stack;
	for (int i = 0; i < len; i++)
	{
		t = regex[i];
		if (t=='(')
		{
			stack.push('(');
			continue;
		}
		if (t==')')
		{
			if (stack.size()==0)
			{
				return false;
			}
			stack.pop();
		}
	}

	if (stack.size()!=0)
	{
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
	dest.starts = source.starts;
	dest.isDeterministic = source.isDeterministic;
	dest.isReversed = source.isReversed;
	int size = source.states.size();
	for (int i = 0; i < size; i++)
	{
		State* t = new State(source.states[i]);
		if (t->IsFinal())
		{
			dest.finalStates.push_back(t);
		}
		dest.states.push_back(t);
	}
	dest.RemoveUnreachableStates();
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
	size = this->starts.size();
	for (int i = 0; i < size; i++)
	{
		this->starts.pop_back();
	}
}

//TODO implement the proper recursive algorithm
std::vector<int> StateMachine::GetUnreachableStatesIndexes() const
{
	int size = this->states.size();
	bool* isStateReachable = new bool[size];
	for (int i = 0; i < size; i++)
	{
		isStateReachable[i] = false;
	}
	for (int i = 0; i < this->starts.size(); i++)
	{
		isStateReachable[this->starts[i]] = true;
	}
	std::vector<Transition> t;
	for (int i = 0; i < size; i++)
	{
		t = this->states[i]->GetAllTransitions();
		for (int j = 0; j < t.size(); j++)
		{
			isStateReachable[t[j].Transist()] = true;
		}
	}
	std::vector<int> UnreachableStatesIndexes;
	for (int i = size - 1; i >= 0; i--)
	{
		if (!isStateReachable[i])
		{
			UnreachableStatesIndexes.push_back(i);
		}
	}
	delete[] isStateReachable;
	return UnreachableStatesIndexes;
}

void StateMachine::RemoveUnreachableStates()
{
	std::vector<int> deletedStatesIndexes = this->GetUnreachableStatesIndexes();
	for (size_t i = 0; i < deletedStatesIndexes.size(); i++)
	{
		delete this->states[deletedStatesIndexes[i]];
		this->states.erase(this->states.begin() + deletedStatesIndexes[i]);
	}

	this->MapTransitionsToNewIndexes(deletedStatesIndexes);
}

void StateMachine::MapTransitionsToNewIndexes(std::vector<int> deletedStatesIndexes)
{
	State* temp;
	std::vector<Transition> transitions;
	for (int k = 0; k < deletedStatesIndexes.size(); k++)
	{
		for (int i = 0; i < this->starts.size(); i++)
		{
			if (deletedStatesIndexes[k] < this->starts[i])
			{
				this->starts[i]--;
			}
		}
		if (deletedStatesIndexes[k] < this->currentState)
		{
			this->currentState--;;
		}
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
				State modifiedState(temp->IsFinal());
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
}

StateMachine::StateMachine()
{
	this->regex = String();
	this->isReversed = false;
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

	this->starts.push_back(this->IndexOfState(startState));
	this->currentState = this->starts[0];
	startState->AddTransition(Transition(letter, IndexOfState(endState)));
	this->isDeterministic = true;
	this->isReversed = false;
}

StateMachine::StateMachine(String regex)
{
	if (!ValidateRegex(regex))
	{
		std::cerr << "Invalid regex. Creating an empty state machine\n";
		*this = StateMachine();
		return;
	}

	if (regex.Length() == 0)
	{
		*this = StateMachine(' ');
		return;
	}
	if (!(regex.ContainsAny(SpecialSymbols, strlen(SpecialSymbols))))
	{
		StateMachine newMachine(regex[0]);
		for (int i = 1; i < regex.Length(); i++)
		{
			newMachine = newMachine.Concatenate(StateMachine(regex[i]));
		}
		*this = newMachine;
		this->isReversed = false;
		return;
	}
	regex = this->RegexToRPN(regex);
	*this = this->Calculate(regex);
	this->isDeterministic = this->CheckIfDeterministic();
	this->isReversed = false;
}

StateMachine::StateMachine(std::ifstream& is)
{
	if (!is.is_open())
	{
		std::cout << "Invalid file name. Creating an empty state machine!\n";
		*this = StateMachine();
		return;
	}
	is >> this->regex;
	if (this->regex.ContainsAny("^R",2))
	{
		this->regex = this->regex.Substring(0, this->regex.Length() - 2);
		this->isReversed = true;
	}
	String temp;
	is >> temp;
	auto args = temp.Split();
	int maxStartStateIndex = args[args.size() - 1].ToInt();
	//add start states indexes
	for (int i = 0; i < args.size(); i++)
	{
		this->starts.push_back(args[i].ToInt());
	}

	is >> temp;
	args = temp.Split();
	if (args.size()==0)
	{
		std::cout << "Invalid input. Creating an empty state machine!\n";
		*this = StateMachine();
		return;
	}
	//add all states until the last final
	int finalStatesMaxIndex = args[args.size() - 1].ToInt();
	if (maxStartStateIndex<finalStatesMaxIndex)
	{
		maxStartStateIndex = finalStatesMaxIndex;
	}
	int counter = 0;
	int i = 0;
	while (i <= finalStatesMaxIndex)
	{
		if (i == args[counter].ToInt())
		{
			this->AddState(new State(true));
			counter++;
		}
		else
		{
			this->AddState(new State());
		}
		i++;
	}
	while (i<=maxStartStateIndex)
	{
		this->AddState(new State());
		i++;
	}

	//add transitions and states not added yet	
	char ch;
	while (is.get(ch))
	{
		is >> temp;
		temp.InsertAt(0, ch);
		
		args = temp.Split();
		if (args.size() != 3)
		{
			std::cout << "Invalid input. Creating an empty state machine!\n";
			*this = StateMachine();
			return;
		}
		char letter = args[1][0];
		int index = args[2].ToInt();
		this->states[args[0].ToInt()]->AddTransition(Transition(letter, index));
		while(index > this->states.size()-1)
		{
			this->AddState(new State());
		}
	}
	this->isDeterministic = CheckIfDeterministic();
}

String StateMachine::AddConcatenationOperator(String regex) const
{
	bool isPreviousLetter = false;
	bool isPreviousRightBracket = false;
	bool isPreviousIteration = false;
	bool isLetter = false;
	bool isCurrentLeftBracket = false;
	for (int i = 0; i < regex.Length(); i++)
	{
		isLetter = isalpha(regex[i]) || isdigit(regex[i]);
		isCurrentLeftBracket = regex[i] == '(';
		isPreviousLetter = isalpha(regex[i - 1]) || isdigit(regex[i - 1]);
		if (((isPreviousLetter || isPreviousRightBracket || isPreviousIteration) && isLetter) || (isCurrentLeftBracket && isPreviousLetter))
		{
			regex.InsertAt(i, '.');
		}
		isPreviousLetter = isalpha(regex[i]) || isdigit(regex[i]);
		isPreviousRightBracket = regex[i] == ')';
		isPreviousIteration = regex[i] == '*';
	}
	return regex;
}

struct Operator
{
	char ch;
	int priority;
	Operator(char ch)
	{
		this->ch = ch;
		if (ch == '|')
		{
			this->priority = 1;
		}
		else if (ch == '.')
		{
			this->priority = 2;
		}
	}
	bool operator>(Operator& other)
	{
		return this->priority > other.priority;
	}
};

//Shunting-yard algorithm
String StateMachine::RegexToRPN(String regex) const
{
	std::queue<char> output;
	std::stack<Operator> operations;
	String newRegex = this->AddConcatenationOperator(regex);
	char curr;
	for (int i = 0; i < newRegex.Length(); i++)
	{
		curr = newRegex[i];
		if (isalpha(curr) || isdigit(curr))
		{
			output.push(curr);
		}
		else if (curr == '(')
		{
			operations.push(curr);
		}
		else if (curr == ')')
		{
			while (operations.top().ch != '(')
			{
				output.push(operations.top().ch);
				operations.pop();
			}
			operations.pop();
		}
		else if (curr == '*')
		{
			output.push(curr);
		}
		else
		{
			Operator op(curr);
			if (operations.size() == 0 || op > operations.top())
			{
				operations.push(op);
			}
			else
			{
				while (!(operations.size() == 0 || operations.top().ch=='('|| op > operations.top()))
				{
					output.push(operations.top().ch);
					operations.pop();
				}
				operations.push(op);
			}
		}
	}
	while (operations.size() > 0)
	{
		output.push(operations.top().ch);
		operations.pop();
	}
	String result;

	while (output.size() > 0)
	{
		result.Append(output.front());
		output.pop();
	}
	return result;
}

//Creates a state machine using regular expression in reverse polish notation
StateMachine StateMachine::Calculate(String expr)
{
	std::stack<StateMachine> machines;
	for (int i = 0; i < expr.Length(); i++)
	{
		char t = expr[i];
		if (t == '|')
		{
			StateMachine a = machines.top();
			machines.pop();
			StateMachine b = machines.top();
			machines.pop();
			StateMachine un = b.Union(a);
			machines.push(un);
		}
		else if (t == '.')
		{
			StateMachine a = machines.top();
			machines.pop();
			StateMachine b = machines.top();
			machines.pop();
			StateMachine res = b.Concatenate(a);
			res.RemoveUnreachableStates();
			machines.push(res);
		}
		else if (t == '*')
		{
			StateMachine a = machines.top();
			machines.pop();
			machines.push(a.Iteration());
		}
		else
		{
			machines.push(StateMachine(t));
		}
	}
	while (machines.size() > 1)
	{
		StateMachine a = machines.top();
		machines.pop();
		StateMachine b = machines.top();
		machines.pop();
		StateMachine res = b.Concatenate(a);
		res.RemoveUnreachableStates();
		machines.push(res);
	}

	return machines.top();
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

bool StateMachine::RecognizeHelper(String word)
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
		recognized = recognized || this->RecognizeHelper(word.Substring(1));
		if (recognized)
		{
			return true;
		}
	}
	return false;
}

bool StateMachine::Recognize(String word)
{
	for (int i = 0; i < this->starts.size(); i++)
	{
		this->currentState = this->starts[i];
		if (this->RecognizeHelper(word))
		{
			return true;
		}
	}
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
	if (isReversed)
	{
		return this->regex + "^R";
	}
	return this->regex;
}

StateMachine StateMachine::Union(const StateMachine& other) const
{
	if (this == &other)
	{
		return *this;
	}
	StateMachine newMachine(*this);
	newMachine.regex.Append(String(String("|") + other.regex));

	// the new start is also an end if any of the old starts are end
	bool isFinal = false;
	for (int i = 0; i < this->starts.size() && !isFinal; i++)
	{
		isFinal = isFinal || this->states[this->starts[i]]->IsFinal();
	}
	for (int i = 0; i < other.starts.size() && !isFinal; i++)
	{
		isFinal = isFinal || other.states[other.starts[i]]->IsFinal();
	}
	State* newStart = new State(isFinal);
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
			newMachine.states[i + this->states.size() + 1]->
				AddTransition(Transition(temp[j].GetLetter(), oldStatesToNew[temp[j].Transist()]));
		}
	}

	//Make the new start do the work of the old start of this
	std::vector<Transition> temp;
	for (int k = 0; k < this->starts.size(); k++)
	{
		temp = this->states[this->starts[k]]->GetAllTransitions();
		for (int i = 0; i < temp.size(); i++)
		{
			Transition t = temp[i];
			Transition newT(t.GetLetter(), t.Transist());
			newStart->AddTransition(newT);
		}
	}

	//Make the new start do the work of the start of other
	for (int k = 0; k < other.starts.size(); k++)
	{
		temp = other.states[other.starts[k]]->GetAllTransitions();
		for (int i = 0; i < temp.size(); i++)
		{
			Transition t = temp[i];
			Transition newT(t.GetLetter(), oldStatesToNew[t.Transist()]);
			newStart->AddTransition(newT);
		}
	}

	newMachine.starts.clear();
	newMachine.starts.push_back(newMachine.IndexOfState(newStart));
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
	std::vector<Transition> temp;
	for (int i = 0; i < endStatesCount; i++)
	{
		for (int k = 0; k < other.starts.size(); k++)
		{
			temp = other.states[other.starts[k]]->GetAllTransitions();
			for (int j = 0; j < temp.size(); j++)
			{
				Transition t = temp[j];
				Transition newT(t.GetLetter(), oldStatesToNew[t.Transist()]);
				newMachine.finalStates[i]->AddTransition(newT);
			}
		}
	}

	//the final states remain final if the start of the second was final
	bool isFinal = false;
	for (int i = 0; i < other.starts.size() && !isFinal; i++)
	{
		isFinal = isFinal || other.states[other.starts[i]]->IsFinal();
	}
	if (!isFinal)
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
	
	if (this->regex.Length()==1)
	{
		newMachine.regex = this->regex+"*";
	}
	else
	{
		newMachine.regex = "(" + this->regex + ")*";
	}
	State* newState = new State(true);
	newMachine.AddState(newState);

	int endStatesCount = newMachine.finalStates.size();
	std::vector<Transition> t;
	for (int k = 0; k < this->starts.size(); k++)
	{
		t = this->states[this->starts[k]]->GetAllTransitions();
		for (int i = 0; i < endStatesCount; i++)
		{
			for (int j = 0; j < t.size(); j++)
			{
				if (!newMachine.finalStates[i]->HasTransition(t[j]))
				{
					newMachine.finalStates[i]->AddTransition(t[j]);
				}
			}
		}
	}

	newMachine.starts.clear();
	newMachine.starts.push_back(this->IndexOfState(newState));
	newMachine.currentState = newMachine.starts[0];
	newMachine.RemoveUnreachableStates();
	return newMachine;
}

void StateMachine::Determinate()
{
	if (this->isDeterministic)
	{
		return;
	}
	//Unite the start states
	StateUnion* newState = new StateUnion(false);
	int processedSoFar = 0;
	std::vector<StateUnion*> states;
	std::vector<std::vector<Transition>> t;
	std::vector<Transition > transitions;
	for (int i = 0; i < this->starts.size(); i++)
	{
		newState->AddStateIndex(this->starts[i]);
	}
	states.push_back(newState);
	bool isChanged = false;

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
				transitions = this->states[indexes[j]]->GetAllTransitions();

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

	//add the states to the machine with the correct transitions
	StateMachine newMachine;
	newMachine.regex = this->regex;
	for (int i = 0; i < states.size(); i++)
	{
		Sorted<int> indexes = states[i]->GetStatesIndexes();
		bool isFinal = false;
		for (int j = 0; j < indexes.Length(); j++)
		{
			if (this->states[indexes[j]]->IsFinal())
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
		newMachine.AddState(newState);
	}
	newMachine.starts.push_back(0);
	newMachine.regex = this->regex;
	newMachine.isReversed = this->isReversed;
	*this = newMachine;
	this->isDeterministic = true;
}

void StateMachine::Reverse()
{
	StateMachine newMachine;
	int startsCounter = 0;
	for (int i = 0; i < this->states.size(); i++)
	{
		State* state = new State();
		if (startsCounter < this->starts.size() && this->starts[startsCounter] == i)
		{
			startsCounter++;
			state->AddFunctionality(State(true), true);
		}
		if (this->states[i]->IsFinal())
		{
			newMachine.starts.push_back(i);
		}
		newMachine.AddState(state);

	}
	std::vector<Transition> t;
	int index;
	char letter;
	for (int i = 0; i < this->states.size(); i++)
	{
		t = this->states[i]->GetAllTransitions();
		for (int j = 0; j < t.size(); j++)
		{
			index = t[j].Transist();
			letter = t[j].GetLetter();
			newMachine.states[index]->AddTransition(Transition(letter, i));
		}
	}
	newMachine.regex = this->regex;
	newMachine.isReversed = this->isReversed;
	*this = newMachine;
	this->isReversed = !this->isReversed;
	this->isDeterministic = this->CheckIfDeterministic();
}

void StateMachine::Minimize()
{
	this->Determinate();
	this->Reverse();
	this->Determinate();
	this->Reverse();
	this->Determinate();
}

void StateMachine::Print() const
{
	this->ToString().Print();
}

bool StateMachine::IsDeterministic() const
{
	return this->isDeterministic;
}

bool StateMachine::CheckIfDeterministic() const
{
	for (int i = 0; i < this->states.size(); i++)
	{
		auto t = this->states[i]->GetAllTransitionsGrouped();
		for (int j = 0; j < t.size(); j++)
		{
			if (t[j].size()>1)
			{
				return false;
			}
		}
	}
	return true;
}

bool StateMachine::IsLanguageEmpty() const
{
	return this->regex == "";
}

String StateMachine::ToString() const
{
	String result;
	result.AppendLine(this->GetRegex());

	//add start states indexes
	for (int i = 0; i < this->starts.size(); i++)
	{
		char ch[15];
		_itoa_s(this->starts[i], ch, 10);
		String temp(ch);
		temp.Append(" ");
		result.Append(temp);
	}
	result.AppendLine("");

	//add final states
	for (int i = 0; i < this->states.size(); i++)
	{
		if (this->states[i]->IsFinal())
		{
			char ch[15];
			_itoa_s(i, ch, 10);
			String temp(ch);
			temp.Append(" ");
			result.Append(temp);
		}
	}
	result.AppendLine("");

	//add transitions
	result.Append(this->TransitionsToString());
	return result;
}

String StateMachine::TransitionsToString() const
{
	String result;
	for (int i = 0; i < this->states.size(); i++)
	{
		std::vector<Transition> t = this->states[i]->GetAllTransitions();
		for (int j = 0; j < t.size(); j++)
		{
			char ch[15];
			_itoa_s(i, ch, 10);
			String temp((ch));
			temp.Append(" ");
			temp.AppendLine(t[j].ToString());
			result.Append(temp);
		}
	}
	return result;
}

bool StateMachine::IsLanguageFinite() const
{
	if (this->regex.ContainsAny("*",1))
	{
		return false;
	}
	return true;
}