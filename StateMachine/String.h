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
	int GetLength() const;
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
	int Count(char) const;
	int IndexOf(char) const;
	int IndexOf(char ch, int startIndex)const ;
	int LastIndexOf(char) const;
	String Substring(int start, int len) const;
	String Substring(int start) const;
	std::vector<String> Split(char delimiter = ' ')const;
};
