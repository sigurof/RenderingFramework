// Tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class A
{
public:
	A(int a, const std::string& str) : a(a), str(str) {}
	int getA() const { return a; }
	const std::string& getStr() const { return str; }

private:
	int a;
	std::string str;

};

bool operator<(const A& lhs, const A& rhs)
{
	if (lhs.getA() == rhs.getA())
	{
		return lhs.getStr() < rhs.getStr();
	}
	return lhs.getA() < rhs.getA();
}

class B
{
public:
	B() : valid(false) {}
	B(int b) : b(b), valid(true) {}
	int getB() const { return b; }

	explicit operator bool() const
	{
		return this->valid;
	}

private:
	int b;
	bool valid;
};

int main()
{
	std::map<A, B> u;
	for (int i = 0; i < 10; i++)
	{
		A a(i, "a"); B b(i*i);
		u[a] = b;
	}
	std::cout << "!u[A(2,\"a\")] = " << u[A(2,"a")].operator bool();
}

