#include <iostream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

/*
	input: a = 20171127
	output: a^6的每一位数相加的和

	bd: string of birthday
*/
#define SIZE(a) {sizeof(a)/sizeof(a[0])}

void print_ret(int a[], int n)
{
	cout<<"calc result : ";
	for(int i = n-1; i >= 0; i--)
		cout<<a[i]<<"";
	cout<<endl;
}

void multi(char a[], char b[])
{
	int len_a = strlen(a);
	int len_b = strlen(b);
	int big_num[100] = {0};

	for(int i = len_a-1; i >= 0; i--)	//乘法上面的数
	{
		// x:第一层循环是个位乘法，第二层从十位开始
		int x = len_a-1-i, carry = 0, ret = 0, tmp = 0;
		for(int j = len_b-1; j >= 0; j--)	//乘法下面的数
		{
			int n1 = a[i] - '0', n2 = b[j] - '0';
			tmp = n1*n2+carry+big_num[x];
			ret = tmp%10;
			carry = tmp/10;
			big_num[x] = ret;

// cout<<"tmp = "<<tmp<<" ,ret = "<<ret<<" ,carry = "<<carry<<", big["<<x<<"] = "<<big_num[x]<<endl;
			x++;
		}
		big_num[x] += carry;
// cout<<"big["<<x<<"] = "<<big_num[x]<<endl;
		carry = 0;
		print_ret(big_num, 20);
	}
}

void multi2(int a[], int b[])
{
	int len_a = SIZE(a);
	int len_b = SIZE(b);

	for(int i = len_a-1; i >= 0; i--)	//乘法上面的数
	{
		// x:第一层循环是个位乘法，第二层从十位开始
		int x = len_a-1-i, carry = 0, ret = 0, tmp = 0;
		for(int j = len_b-1; j >= 0; j--)	//乘法下面的数
		{
			tmp = a[i]*b[j]+carry+big_num[x];
			ret = tmp%10;
			carry = tmp/10;
			big_num[x] = ret;

 cout<<"tmp = "<<tmp<<" ,ret = "<<ret<<" ,carry = "<<carry<<", big["<<x<<"] = "<<big_num[x]<<endl;
			x++;
		}
		big_num[x] += carry;
cout<<"big["<<x<<"] = "<<big_num[x]<<endl;
		carry = 0;
		print_ret(big_num, 20);
	}
}

int main()
{
	multi((char *)"20181127", (char *)"20181127");
	return 0;
}