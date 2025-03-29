#include<bits/stdc++.h>
using namespace std;
const long long mod = 1000000007;

long long cal(long long a, long long n)
{
    long long result=1;
    a%=mod;
    while(n>0)
    {
        if(n%2==1)
            result=(result*a)%mod;
        a=(a*a)%mod;
        n/=2;
    }
    return result;
}
int main()
{
    long long a,n;
    cin>>a>>n;
    cout<<cal(a,n);
}
//nhanando