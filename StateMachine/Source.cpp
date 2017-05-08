#include "StateMachine.h"
#include <iostream>
#include "Sorted.cpp"
int main()
{
	//StateMachine a = StateMachine("a*");
		//.Union(StateMachine("a")));
	String regex = "bor|bora|borut|borove|borovete|bob|boba|bobut|bobove|bobovete|rob|roba|robut|robi|robite";
	StateMachine a(regex);
	std::vector<String> tests = regex.Split('|');
	/*for (int i = 0; i < tests.size(); i++)
	{
		std::cout<<a.Recognize(tests[i]);
	}*/
	//a.Print();
	a.Minimize();
	a.Print();
	a.Minimize();
	a.Minimize();
	for (int i = 0; i < tests.size(); i++)
	{
		std::cout << a.Recognize(tests[i]);
	}

	return 0;
}