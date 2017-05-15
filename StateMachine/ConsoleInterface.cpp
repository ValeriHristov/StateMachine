#pragma once
#include "ConsoleInterface.h"
#include <fstream>

bool ConsoleInterface::ValidateInput(String stringToInt, int parametersNeeded, std::vector<String> str) const
{
	if (stringToInt.TryParseToInt())
	{
		return str.size() == parametersNeeded;
	}
	return false;
}

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
			if (parameters.size()!= 2)
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Open(parameters[1]);
		}
		else if (command == "List")
		{
			this->os << "Not implemented yet!\n";
		}
		else if (command == "Print")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Print(parameters[1]);
		}
		else if (command == "Save")
		{
			if (!ValidateInput(parameters[1], 3, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Save(parameters[1], parameters[2]);
		}
		else if (command == "Empty")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->IsEmpty(parameters[1]);
		}
		else if (command == "Deterministic")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->IsDeterministic(parameters[1]);
		}
		else if (command == "Recognize")
		{
			if (!ValidateInput(parameters[1], 3, parameters))
			{
				if (ValidateInput(parameters[1], 2, parameters))
				{
					parameters.push_back(" ");
				}
				else
				{
					this->os << "Invalid parameters!\n";
					continue;
				}
			}
			this->Recognize(parameters[1], parameters[2]);
		}
		else if (command == "Union")
		{
			if (!(ValidateInput(parameters[1], 3, parameters) && parameters[2].TryParseToInt()))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Union(parameters[1], parameters[2]);
		}
		else if (command == "Concat")
		{
			if (!(ValidateInput(parameters[1], 3, parameters) && parameters[2].TryParseToInt()))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Concat(parameters[1], parameters[2]);
		}
		else if (command == "Un")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Un(parameters[1]);
		}
		else if (command == "Reg")
		{
			if (parameters.size()==1)
			{
				parameters.push_back("");
			}
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
		else if (command == "Minimize")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Minimize(parameters[1]);
		}
		else if (command == "Determinate")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Determinate(parameters[1]);
		}
		else if (command == "Reverse")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->Reverse(parameters[1]);
		}
		else if (command == "Language")
		{
			if (!ValidateInput(parameters[1], 2, parameters))
			{
				this->os << "Invalid parameters!\n";
				continue;
			}
			this->PrintLanguage(parameters[1]);
		}
		else if (command == "Close")
		{
			this->os << "Closing the program!\n";
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
		this->os << "The state machine with id " << index << " is deterministic!\n";
	}
	else
	{
		this->os << "The state machine with id " << index << " is NOT deterministic!\n";
	}
}

void ConsoleInterface::Minimize(int index)
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	this->elements[index].Minimize();
	this->os << "Succesfully minimized the state machine with id " << index << "\n";
}

void ConsoleInterface::Determinate(int index)
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	this->elements[index].Determinate();
	this->os << "Succesfully determinated the state machine with id " << index << "\n";
}

void ConsoleInterface::Reverse(int index)
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}
	this->elements[index].Reverse();
	this->os << "Succesfully reversed the state machine with id " << index << "\n";
}

void ConsoleInterface::PrintLanguage(int index) const
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	char* output = this->elements[index].GetRegex().ToCharArray();
	this->os << output  << '\n';
	delete[] output;
}

void ConsoleInterface::Save(int index, String fileName) const
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	char* fileNameTemp = fileName.ToCharArray();
	std::ofstream ofs(fileNameTemp);
	char* output = this->elements[index].ToString().ToCharArray();
	ofs << output;
	ofs.close();
	delete[] fileNameTemp;
	delete[] output;
}

void ConsoleInterface::Print(int index) const
{
	if (index > this->elements.size() - 1 || index < 0)
	{
		this->os << "Invalid state machine ids!\n";
		return;
	}

	this->elements[index].TransitionsToString().Print();
}

void ConsoleInterface::Open(String fileName)
{
	char* temp = fileName.ToCharArray();
	std::ifstream ifs(temp);
	StateMachine a(ifs);
	this->AddElement(a);
	this->os << "Opened state machine with index " << this->elementsCount - 1 << "\n";
	delete[] temp;
}