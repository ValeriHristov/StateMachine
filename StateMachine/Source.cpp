#include "StateMachine.h"
#include <iostream>
int main()
{

	StateMachine b;
	
		StateMachine a('a');
		StateMachine c('c');
		StateMachine ac= a.Union(c);
		StateMachine acIteration = ac.Iteration();
	    std::cout << acIteration.Recognize("acaaac");


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


	//std::unordered_map<char, std::vector<Transition>> a;
	//a['a'].push_back(Transition());
	//std::unordered_map<char, std::vector<Transition>> b = a;

	//b['a'].pop_back();
	//std::cout << a['a'].capacity();
	////std::vector<Transition> t;
	////t.push_back(Transition());
	////std::vector<Transition> c = t;
	////c.pop_back();
	////std::cout << t.capacity();
	/*StateMachine();
	*/
	return 0;
}