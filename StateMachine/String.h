#pragma once
#include <vector>
class String
{
private:
	int length;
	char* content;
public:
	String();
	String(const String&);
	String(const char*);
	String(char);
	~String();
	String& operator=(const String&);
	int Length() const;
	char operator[](int)const;
	String operator+(const String&) const;
	String operator+(const char*)const;
	String& operator+=(const String&);
	String& operator+=(const char*);
	bool operator==(const String&) const;
	void Append(const String&);
	void Append(const char*);
	String Concatenate(const String&) const;
	void Print()const;
	//Gets an array of chars and the chars count. Returns true if any of the chars is contained at least once in the string.
	bool ContainsAny(char*, int count) const;
	int Count(char) const;
	int IndexOf(char) const;
	int IndexOf(char ch, int startIndex)const;
	//Gets an array of chars and the chars count. Returns the first index a char is contained in the string.
	int FirstIndexOfAny(char* chars, int count) const;
	int LastIndexOf(char) const;
	String Substring(int start, int len) const;
	String Substring(int start) const;
	std::vector<String> Split(char delimiter = ' ')const;
};
