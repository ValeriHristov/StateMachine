#include "String.h"
#include <cstring>
#include <iostream>
String::String()
{
	this->content = nullptr;
	this->length = 0;
}
String::String(const String& str)
{
	this->length = str.GetLength();
	this->content = new char[this->length];
	for (int i = 0; i < this->length; i++)
	{
		this->content[i] = str[i];
	}
}
String::String(const char* str)
{
	this->length = strlen(str);
	this->content = new char[this->length];
	for (int i = 0; i < this->length; i++)
	{
		this->content[i] = str[i];
	}
}
String::String(char ch)
{
	this->length = 1;
	this->content = new char[1];
	this->content[0] = ch;
}
String::~String()
{
	delete this->content;
}
String String::operator+(const String& str) const
{
	return this->Concatenate(str);
}
String String::operator+(const char* str) const
{
	return *this + String(str);
}
String& String::operator=(const String& str)
{
	if (this== &str)
	{
		return *this;
	}
	delete this->content;
	this->length = str.GetLength();
	this->content = new char[this->length];
	for (int i = 0; i < this->length; i++)
	{
		this->content[i] = str[i];
	}
	return *this;
}
String& String::operator+=(const String& str)
{
	this->Append(str);
	return *this;
}
String& String::operator+=(const char* str)
{
	this->Append(String(str));
	return *this;
}
char String::operator[](int index)const
{
	if (index >= this->length || index < 0)
	{
		return '\0';
	}
	return this->content[index];
}
bool String::operator==(const String& other) const
{
	if (this->length != other.length)
	{
		return false;
	}
	for (int i = 0; i < this->length; i++)
	{
		if (this->content[i] != other.content[i])
		{
			return false;
		}
	}
	return true;
}
int String::GetLength() const
{
	return this->length;
}
String String::Concatenate(const String& str) const
{
	String temp = *this;
	temp.Append(str);
	return temp;
}
void String::Append(const String& str)
{
	char* temp = this->content;
	this->length += str.GetLength();
	this->content = new char[this->length];
	for (int i = 0; i < this->length - str.GetLength(); i++)
	{
		this->content[i] = temp[i];
	}
	for (int i = this->length - str.GetLength(), j = 0; i < this->length; i++, j++)
	{
		this->content[i] = str[j];
	}
	delete temp;
}
void String::Append(const char* ch)
{
	this->Append(String(ch));
}
void String::Print() const
{
	for (int i = 0; i < this->length; i++)
	{
		std::cout << (*this)[i];
	}
}
int String::IndexOf(char ch) const
{
	return this->IndexOf(ch, 0);
}
int String::IndexOf(char ch, int startIndex) const
{
	for (int i = startIndex; i < this->length; i++)
	{
		if (this->content[i] == ch)
		{
			return i;
		}
	}
	return -1;
}
int String::Count(char ch) const
{
	int counter = 0;
	for (int i = 0; i < this->length; i++)
	{
		if (this->content[i]==ch)
		{
			counter++;
		}
	}
	return counter;
}
int String::LastIndexOf(char ch) const
{
	for (int i = this->length - 1; i >= 0; i--)
	{
		if (this->content[i] == ch)
		{
			return i;
		}
	}
	return -1;
}
String String::Substring(int start, int len) const
{
	if (start < 0)
	{
		return String();
	}
	String t;
	if (start+len> this->length)
	{
		len = this->length - start;
	}
	for (int i = 0; i < len; i++)
	{
		if (i >= this->length)
		{
			return t;
		}
		t += this->content[i + start];
	}
	return t;
}
String String::Substring(int start) const
{
	return this->Substring(start, this->length);
}
std::vector<String> String::Split(char delimiter) const
{
	std::vector<String> result;
	int count = this->Count(delimiter);
	int currIndex = 0;
	int indexOfDelimiter = this->IndexOf(delimiter);
	for (int i = 0; i < count; i++)
	{		
		result.push_back(String(this->Substring(currIndex, indexOfDelimiter-currIndex)));
		currIndex = indexOfDelimiter+1;
		indexOfDelimiter = this->IndexOf(delimiter, currIndex);
	}
	result.push_back(String(this->Substring(currIndex)));
	
	//Remove empty entries
	for (int i = result.size()-1; i >= 0; i--)
	{
		if (result[i] == String())
		{
			result.erase(result.begin()+i);
		}
	}

	return result;
}

