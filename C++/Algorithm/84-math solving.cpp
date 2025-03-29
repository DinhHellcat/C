#include<bits/stdc++.h>
using namespace std;
const int mod = 998244353;
long long sum(long long n)
{
    return ((n*(n+1))/2)%mod;
}
int main()
{
    int T;
    cin>>T;
    long long x,y,z;
    vector<long long> n(T);
    for(int i=0;i<T;i++)
    {
        cin>>x>>y>>z;
        n[i]=(((sum(x)*sum(y))%mod)*sum(z))%mod;
    }
    for(int i=0;i<T;i++)
        cout<<n[i]<<endl;
}
