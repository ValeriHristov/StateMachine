#include "StateMachine.h"
#include <iostream>
#include <fstream>
#include "Sorted.cpp"
#include "ConsoleInterface.h"
#include <cassert>
int main()
{
	//StateMachine c("a|a|a|a|a*");
	//c.Minimize();
	//std::ifstream is;
	//is.open("test.txt");

	//StateMachine test(is);

	//is.close();
	//std::ofstream os("testReading.txt");
	//os << test.ToString().ToCharArray();
	//os.close();

	//is.open("testReading.txt");
	//StateMachine read(is);
	//StateMachine fund("123123kksadlkjaslkfjdalksdjalskd");
	/*ConsoleInterface a;
	a.Read();*/
	StateMachine qwe("bor|bora|borut|borove|borovete|bob|boba|bobut|bobove|bobovete|rob|roba|robut|robi|robite");
	//StateMachine c("bor|bora|borut|borove|borovete");
	//StateMachine x("bob|boba|bobut|bobove|bobovete");
	//StateMachine w("rob|roba|robut|robi|robite");
	//x.Minimize();
	//c.Minimize();
	//w.Minimize();
	////x.Print();
	//
	//StateMachine p = x.Union(c);
	//StateMachine q = c.Union(x);
	//
	//p.Minimize();
	//q.Minimize();
	//q = q.Union(w);
	//p = p.Union(w);
	//q.Minimize();
	//p.Minimize();
	//q.Print();
	//p.Print();
	qwe.Minimize();
	qwe.Print();

	//c.Minimize();
	//StateMachine u = c.Union(x);
	//u.Minimize();
//	u.Print();
//	q.Print();
	//x.Print();
	return 0;
}