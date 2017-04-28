#include "StateMachine.h"
#include <iostream>
int main()
{

	StateMachine b;
	StateMachine ac;
	if (true)
	{
		StateMachine a('a');
		StateMachine c('c');
		 ac = a.Union(c).Iteration();
	}

	std::cout << ac.Recognize("acaaacb");


	/*while (true)
	{
	StateMachine five;
	if (true)
	{
	StateMachine first('a');
	StateMachine second('b');
	StateMachine third = first.Union(second);
	StateMachine fourth = third.Union(StateMachine('g'));
	five = fourth;
	}


	std::cout << five.Recognize("à");
	}
	StateMachine first('a');
	StateMachine second('b');
	StateMachine third = first.Union(second);
	StateMachine fourth = third.Union(StateMachine('g'));
	std::cout << fourth.Recognize("t");*/

	return 0;
}