#include<bits/stdc++.h>
using namespace std;
int check(long long n)
{
    if(n<2) return 0;
    if(n==2 || n==3) return 1;
    if(n%2==0 || n%3==0) return 0;
    for(long long i=5;i*i<=n;i+=6)
        if(n%i==0 || n%(i+2)==0)
            return 0;
    return 1;
}
int main()
{
    int n;
    cin>>n;
    cout<<check(n);
}
