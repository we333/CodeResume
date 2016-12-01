#include <iostream>  
#include <cstring>  
#include <cassert>  
using namespace std;  
  
// 大整数类型  
const int MAXLENGTH = 1000;  
struct HP
{
	int len; 
	int s[MAXLENGTH];
};  
  
// 字符串转大整数  
void Str2HP(const char* s, HP& x)  
{  
    x.len = strlen(s);  
    for (int i=0; i<x.len; ++i)  
    {  
        assert(s[i]>='0' && s[i]<='9');  
        x.s[x.len-1-i] = s[i]-'0';  
    }  
    if (x.len == 0)  
    {  
        x.len = 1;  
        x.s[0] = 0;  
    }  
}  
  
void PrintHP(const HP& x)  
{  
    for (int i=x.len-1; i>=0; --i)  
        cout << x.s[i];  
}  
  
// 大整数的加法  
void PlusHP(const HP x, const HP y, HP &z)  
{  
    int i;  z.s[0] = 0;  
    // 大整数x,y的加法操作和输出大整数z的进位操作  
    for (i=0; i<x.len || i<y.len; ++i)  
    {  
        if (i<x.len) z.s[i] += x.s[i];  
        if (i<y.len) z.s[i] += y.s[i];  
        z.s[i+1] = z.s[i]/10;  
        z.s[i] %= 10;  
    }  
    // 第i位不会再进位，这里可省去  
    while (z.s[i])  
    {  
        z.s[i+1] = z.s[i]/10;  
        z.s[i] %= 10;  
        ++i;  
    }  
    z.len = i;  
}  
  
// 大整数的减法   
void SubtractHP(const HP x, const HP y, HP &z)  
{  
    int i, j;  
    // j表示是否要对高位进行借位，1表示借1位，0表示不借位  
    for (i=0, j=0; i<x.len; ++i)  
    {  
        z.s[i] = x.s[i] - j;  
        if (i<y.len) z.s[i] -= y.s[i];  
        if (z.s[i]<0)   
        {  
            // 向高位借位，该位补10  
            j=1;   
            z.s[i] += 10;  
        }  
        else  
            j=0;  
    }  
    do --i;  
    while (i>0 && !z.s[i]);  
    z.len = i+1;  
}  
  
// 大整数的比较  
int CompareHP(const HP &x, const HP &y)  
{  
    if (x.len > y.len) return 1;  
    if (x.len < y.len) return -1;  
    int i = x.len-1;  
    while (i>0 && x.s[i]==y.s[i]) --i;  
    return x.s[i]-y.s[i];  
}  
  
// 大整数的乘法  
void MultiHP(const HP x, const HP y, HP &z)  
{  
    int i, j;  
    // 对乘法结果大整数z初值化为0，以方便之后的+=运算  
    z.len = x.len + y.len;  
    for (i=0; i<z.len; ++i) z.s[i] = 0;  
    for (i=0; i<x.len; ++i)  
        for (j=0; j<y.len; ++j)  
            z.s[i+j] += x.s[i]*y.s[j];  
    // 大整数z进位  
    for (i=0; i<z.len-1; ++i) {z.s[i+1] += z.s[i]/10; z.s[i] %= 10;}  
    // 最高位继续进位，这步不会执行可省去  
    while (z.s[i]) {z.s[i+1] = z.s[i]/10; z.s[i] %= 10; i++;}  
    // 直到最高位不为0 ,以确定大整数的长度  
    while (i>0 && !z.s[i]) --i;  
    z.len = i+1;  
}  
  
  
void DivideHP(const HP x, const HP y, HP &u, HP &v)  
{  
    int i, j;  
    v.len = 1; v.s[0] = 0;  
    // u表示x的前i位除y的商  
    // v表示x的前i位除y的余数  
    for (i=x.len-1; i>=0; --i)  
    {  
        // 余数v先向左移一位，再加上x.s[i]  
        if (!(v.len==1 && v.s[0]==0))  
        {  
            for (j=v.len-1; j>=0; --j)  
                v.s[j+1] = v.s[j];  
            ++v.len;  
        }  
        v.s[0] = x.s[i];  
        // 每次循环都计算出商的第i位u.s[i]  
        u.s[i] = 0;  
        while ((j=CompareHP(v, y))>=0)  
        {  
            // 余数v大于等于除数y时，就进行减操作  
            SubtractHP(v, y, v);  
            ++u.s[i];  
            if (j==0) break;  
        }  
    }  
    i = x.len - 1;  
    while (i>0 && !u.s[i]) --i;  
    u.len = i+1;  
}  
  
// 大整数置0  
inline void ZeroHP(HP& x)  
{  
    x.len = 1; x.s[0] = 0;  
}  
  
// 大整数置1  
inline void OneHP(HP& x)  
{  
    x.len = 1; x.s[0] = 1;  
}  
  
// 二维数组（方阵）  
const int MAXSIDE = 2;  
struct Matrix
{
	int side; 
	HP a[MAXSIDE*MAXSIDE];
};  
  
// 方阵的乘法  
void MultiMatrix(const Matrix x, const Matrix y, Matrix &z)  
{  
    assert(x.side == y.side);  
    z.side = x.side;  
    HP tmp;  
    for (int i=0; i<z.side; ++i)  
        for (int j=0; j<z.side; ++j)  
        {  
            ZeroHP(z.a[i*z.side+j]);  
            for (int k=0; k<z.side; ++k)  
            {  
                MultiHP(x.a[i*x.side+k], y.a[k*y.side+j], tmp);  
                PlusHP(z.a[i*z.side+j], tmp, z.a[i*z.side+j]);  
            }  
        }  
}  
  
const HP& Fibonaci(int n)  
{  
    HP zero;  
    ZeroHP(zero);  
    if (n==0) return zero;  
    --n;  
    Matrix tmp; tmp.side = 2;  
    OneHP(tmp.a[0]);    OneHP(tmp.a[1]);  
    OneHP(tmp.a[2]);    ZeroHP(tmp.a[3]);  
    Matrix res; res.side = 2;  
    OneHP(res.a[0]);    ZeroHP(res.a[1]);  
    ZeroHP(res.a[2]);   OneHP(res.a[3]);  
    while (n)  
    {  
        if (n&1) MultiMatrix(res, tmp, res);  
        MultiMatrix(tmp, tmp, tmp);  
        n >>= 1;  
    }  
    return res.a[0];  
}  
  
int main()  
{  
    const HP& res = Fibonaci(92);  
    PrintHP(res);  
    cout << endl;  
}  