#include<bits/stdc++.h>
using namespace std;
long sum(long a)
{
    if(a==1) return 1;
    return pow(a,2) + sum(a-1);
}
int main()
{
    long a;
    cin>>a;
    cout<<sum(a);
}
