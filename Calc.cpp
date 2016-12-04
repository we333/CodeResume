#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

enum CalculateResult
{
	CalculateResult_NoError = 0,
	CalculateResult_DevideByZero = 1,
	CalculateResult_InvalidExpression = 2,
	CalculateResult_UnknownError = 1000
};

char oper[] = {"+-*/"};

bool isOperator(const char *p)
{
	for(int i = 0; i < sizeof(oper)/sizeof(oper[0]); i++)
		if(*p == oper[i])
			return true;
	return false;
}

bool isNum(const char *p)
{
	if(*p >= '0' && *p <= '9')
		return true;
	return false;
}

double BaseCalc(double s1, double s2, char op)
{
	double ret = 0.0;
	switch(op)
	{
		case '+':	ret = s1 + s2;	break;
		case '-':	ret = s1 - s2;	break;
		case '*':	ret = s1 * s2;	break;
		case '/':	ret = s1 / s2;	break;
		default:	break;
	}
	return ret;
}

double Calculate1( const char* str, CalculateResult& rc )
{
	double ret;
	stack<double> num;
	stack<char> op;

	char *p;
	string tmp;

	rc = CalculateResult_NoError;

	for(p = (char *)str; *p != '\0'; p++)
	{
		if(isOperator(p))
			op.push(*p);
		else if(isNum(p) || *p == '.')
		{
			if(tmp.size() == 0 && *p == '.')	// .出现在数字前面
			{
				rc = CalculateResult_InvalidExpression;
				return 0.0;
			}
			tmp += *p;
			char next = *(p+1);

			if(next == '\0' || isOperator(&next) || next == ' ') // 数字结束
			{
				double cur = atof(tmp.c_str());
				tmp.clear();
				if(op.empty() 
				||(op.top() != '*' && op.top() != '/'))
				{
					num.push(cur);
				}
				else	// 出栈,运算后再保存
				{
					double prev = num.top(); num.pop();
					char how = op.top(); op.pop();
					if(how == '/' && cur == 0)
					{
						rc = CalculateResult_DevideByZero;
						return 0.0;
					}
					num.push(BaseCalc(prev, cur, how));
				}
			}
		}
		else	// space
			;
	}

	while(!op.empty())
	{
		double s2 = num.top(); num.pop();
		double s1 = 0.0;
		if(!num.empty())
		{
			s1 = num.top(); num.pop();
		}
		char how = op.top(); op.pop();
		ret = BaseCalc(s1, s2, how);
	}

	cout<<"ret = "<<ret<<endl;

	return ret;
}

int main()
{
	CalculateResult rc;
	Calculate1("1000 + 2000.5", rc);
	return 0;
}

/*
"1+2"
"1*2+3"
"10+2*3"
"1*2-3*4"
"1.2+0.01"
"1 / 2 + 3 * 4"
"1000 + 2000.5"
"-100*4/2"
 "1+2/0"
*/
