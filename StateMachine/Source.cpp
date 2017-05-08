#include "StateMachine.h"
#include <iostream>
#include "Sorted.cpp"
int main()
{
	StateMachine a = StateMachine("a")
		.Union((StateMachine("a"))
		.Union(StateMachine("a")));
		//.Union(StateMachine("a")));
	//StateMachine a("a|ab|ac");
	a.Determinate();
	//std::cout << a.Recognize("abv") << a.Recognize("abva") << a.Recognize("aed") << a.Recognize("aeda") << a.Recognize("a")
	//	<< a.Recognize("git") << a.Recognize("gitgit") << a.Recognize("abvagit");

	return 0;
}