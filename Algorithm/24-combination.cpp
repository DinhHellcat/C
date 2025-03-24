#include<bits/stdc++.h>
using namespace std;
long long giaithua(long long x)
{
    if(x==0) return 1;
    return x*giaithua(x-1);
}
int main()
{
    long long n,k;
    cin>>n>>k;
    cout<<giaithua(n)/(giaithua(k)*giaithua(n-k));
}
