#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

#define LEN (1000)

void f(int a[], int b[], int c[])
{
	for(int i=LEN-1; i>=1; i--)
    {
        c[i-1] = (a[i] + b[i] + c[i]) / 10;
        c[i] = (a[i] + b[i] + c[i]) % 10;
    }
}

string Fibonacci( unsigned int i )
{
	string ret;
	int fib[LEN][LEN];
	bzero(fib, sizeof(fib));
	fib[1][LEN-1] = fib[2][LEN-1] = 1;

	for(int idx = 3; idx < LEN; idx++)
	{
		f(fib[idx-2],fib[idx-1],fib[idx]);
	}

	for(int j = 0; j < LEN; j++)
		if(fib[i][j] != 0)
		{
			for(; j < LEN; j++)
			{
			//	cout<<fib[idx][j];
				char tmp[1];
				sprintf(tmp, "%d", fib[i][j]);
				ret += tmp;
			}
			break;
		}


	return ret;
}

void TestFibonacci()
{
	// 何番目のフィボナッチ数を計算したいか、ここで指定しよう。
	unsigned int i = 256;
	
	cout << "Fibonacci( " << i << " ) = " << Fibonacci( i ) << endl;
}



int main()
{
	TestFibonacci();

	unsigned long long ret = 7540113804746346429;

	return 0;
}