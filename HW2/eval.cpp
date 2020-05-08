#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool islowerPrecedence(char x, char y);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	int varCount = 0;
	int opCount = 0;

	if (infix.empty())
		return 1;

	for (int i = 0; i < infix.size(); i++)
	{
		if (islower(infix[i]))
			varCount++;
		if (infix[i] == '/' || infix[i] == '*' || infix[i] == '+' || infix[i] == '-')
			opCount++;
	}

	if ((opCount + 1 != varCount))
		return 1;

	int par1 = 0;
	int par2 = 0;
	for (int i = 0; i < infix.size(); i++)
	{
		if (infix[i] == '(')
			par1++;
		if (infix[i] == ')')
			par2++;
		if (infix[i] == '(' && (i + 1) == infix.size())
			return 1;
		if (infix[i] == '(' && infix[i + 1] == ')')
			return 1;
	}
	if (par1 != par2)
		return 1;
	if (par1 > 0 && varCount <= 0)
		return 1;
		

	postfix = "";
	stack<char> x; //operator stack

	for (int i = 0; i < infix.size(); i++)
	{
		if (islower(infix[i])) 
			postfix += infix[i];

		else 
		{

			switch (infix[i])
			{
			case '(':
				x.push(infix[i]);
				break;
			case ')':
				while (x.top() != '(')
				{
					postfix += x.top();
					x.pop();
				}
				x.pop();
				break;
			case '*':
			case '+':
			case '-':
			case '/':
				while (!x.empty() && x.top() != '(' && islowerPrecedence(infix[i], x.top()))
				{
					postfix += x.top();
					x.pop();
				}
				x.push(infix[i]);
				break;

			}
		}

	}

	while (!x.empty())
	{
		postfix += x.top();
		x.pop();
	}

	for (int i = 0; i < postfix.size(); i++)
	{
		if (islower(postfix[i]))
		{
			if (!(values.contains(postfix[i])))
				return 2;
		}

		if (!islower(postfix[i]) && !isspace(postfix[i]) && (postfix[i] != '/' && postfix[i] != '*' && postfix[i] != '+' && postfix[i] != '-'))
			return 1;
	}

	stack<int> y; //operand stack
	for (int i = 0; i < postfix.size(); i++)
	{
		
		if (isalpha(postfix[i]))
		{
			int f;
			values.get(postfix[i], f);
			y.push(f);
		}
		else
		{
			int operand2 = y.top();
			y.pop();
			int operand1 = y.top();
			y.pop();
			if (operand2 == 0 && postfix[i] == '/')
				return 3;
			if (postfix[i] == '/')
			{
				y.push((operand1 / operand2));
			}
			if (postfix[i] == '*')
			{
				y.push((operand1 * operand2));
			}
			if (postfix[i] == '-')
			{
				y.push((operand1 - operand2));
			}
			if (postfix[i] == '+')
			{
				y.push((operand1 + operand2));
			}
		}
	}
	result = y.top();
	return 0;
	
}

bool islowerPrecedence(char x, char y)
{
	if (x == '/' && (y == '+' || y == '-' || y == '*'))
		return false;
	if (x == '*' && (y == '+' || y == '-' || y == '/'))
		return false;
	if (x == ')' && (y == '/'))
		return false;
	if (x == ')' && (y == '*'))
		return false;
	return true;
}


int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+"  &&  answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*"  &&  answer == 999);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+"  &&  answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/"  &&  answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	cout << "Passed all tests" << endl;
}