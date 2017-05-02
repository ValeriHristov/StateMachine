#include "StateMachine.h"
#include <iostream>
int main()
{
	StateMachine a("(abv|aed)a*");
	std::cout << a.Recognize("abv") << a.Recognize("abva") << a.Recognize("aed") << a.Recognize("aeda") << a.Recognize("a");

	return 0;
}