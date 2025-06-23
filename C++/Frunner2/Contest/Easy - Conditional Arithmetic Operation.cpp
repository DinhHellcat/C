#include<bits/stdc++.h>
using namespace std;
int main()
{
    long long a,b;
    cin>>a>>b;
    long long c = a+b;
    if(c>0)
    {
        float x=(sqrt(c));
        cout<<fixed<<setprecision(2)<<x;
    }
    else
    {
        long long x=c*c;
        cout<<x;
    }
}
