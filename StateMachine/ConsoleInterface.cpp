#pragma once
#include "ConsoleInterface.h"

void ConsoleInterface::Read(std::istream& is)
{
	String s;
	while (true)
	{
		is >> s;
		if (s == "")
		{
			continue;
		}
		std::vector<String> parameters = s.Split();
		String command = parameters[0];
		if (command == "Open")
		{

		}
		else if (command == "List")
		{

		}
		else if (command == "Print")
		{

		}
		else if (command == "Save")
		{

		}
		else if (command == "Empty")
		{
			if (parameters.size()!=2)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->IsEmpty(parameters[1]);
		}
		else if (command == "Deterministic")
		{
			if (parameters.size() != 2)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->IsDeterministic(parameters[1]);
		}
		else if (command == "Recognize")
		{
			if (parameters.size() > 3)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			else if (parameters.size() == 2)
			{
				parameters.push_back("");
			}
			this->Recognize(parameters[1], parameters[2]);
		}
		else if (command == "Union")
		{
			if (parameters.size() != 3)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Union(parameters[1], parameters[2]);
		}
		else if (command == "Concat")
		{
			if (parameters.size() != 3)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Concat(parameters[1], parameters[2]);
		}
		else if (command == "Un")
		{
			if (parameters.size() != 2)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Un(parameters[1]);
		}
		else if (command == "Reg")
		{
			if (parameters.size() != 2)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Reg(parameters[1]);
		}
		else if (command == "Set")
		{
			if (parameters.size() != 3)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			else if (parameters[1] != "destination")
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->SetDestination(parameters[2]);
		}
		else if (command == "Close")
		{
			this->os << "Closing the program!";
			return;
		}
	}
}

void ConsoleInterface::Reg(String regex)
{
	this->elements.push_back(StateMachine(regex));
	this->elementsCount++;
	this->os << "Succesfully created state machine with index " << this->elementsCount - 1 << "\n";
}

void ConsoleInterface::Recognize(int index, String word)
{
	if (index >= this->elementsCount || index < 0)
	{
		this->os << "Invalid state machine id!\n";
		return;
	}
	bool result = this->elements[index].Recognize(word);
	if (result)
	{
		this->os << "The word \"" << word << "\" is recognized by state machine with index " << index << "\n";
	}
	else
	{
		this->os << "The word \"" << word << "\" is NOT recognized by state machine with index " << index << "\n";
	}
}

void ConsoleInterface::SetDestination(String dest)
{
	this->destination = dest;
	os << "Succesfully changed the destination folder!\n";
}

ConsoleInterface::ConsoleInterface(std::istream& is, std::ostream& os) : is(is), os(os)
{
	this->elementsCount = 0;
}

void ConsoleInterface::Concat(int firstIndex, int secondIndex)
{
	int size = this->elements.size();
	if (firstIndex > size - 1 || secondIndex > size - 1 || firstIndex < 0 || secondIndex < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	this->AddElement(this->elements[firstIndex].Concatenate(this->elements[secondIndex]));
	this->os << "Succesfully created state machine with index " << this->elementsCount - 1 << "\n";
}

void ConsoleInterface::Union(int firstIndex, int secondIndex)
{
	int size = this->elements.size();
	if (firstIndex > size - 1 || secondIndex > size - 1 || firstIndex < 0 || secondIndex < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}
	this->AddElement(this->elements[firstIndex].Union(this->elements[secondIndex]));
	this->os << "Succesfully created state machine with index " << this->elementsCount - 1 << "\n";
}

void ConsoleInterface::Un(int index)
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	this->AddElement(this->elements[index].Iteration());
	this->os << "Succesfully created state machine with index " << this->elementsCount - 1 << "\n";
}

void ConsoleInterface::AddElement(const StateMachine& st)
{
	this->elements.push_back(st);
	this->elementsCount++;
}

void ConsoleInterface::IsEmpty(int index) const
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	bool result = this->elements[index].IsLanguageEmpty();
	if (result)
	{
		this->os << "The language of state machine with id " << index << " is empty!\n";
	}
	else
	{
		this->os << "The language of state machine with id " << index << " is NOT empty!\n";
	}
}

void ConsoleInterface::IsDeterministic(int index) const
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	bool result = this->elements[index].IsDeterministic();
	if (result)
	{
		this->os << "The language of state machine with id " << index << " is deterministic!\n";
	}
	else
	{
		this->os << "The language of state machine with id " << index << " is NOT deterministic!\n";
	}
}