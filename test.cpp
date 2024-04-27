#include <iostream>
#include <sstream>
using namespace std;

struct poop {
	int x;
};

istream& operator>>(istream& is, poop& p)
{
	return is >> p.x;
}

int main()
{
	poop p;
	stringstream ss("12345");
	ss >> p;
	cout << p.x << endl;
	return 0;
}
