#include<bits/stdc++.h>
using namespace std;
vector<long long> a(46341);
void compile()
{
    a[1]=1;
    for(int i=2;i<46341;i++)
        a[i]=a[i-1]*2;
}
int main()
{
    compile();
    int n;
    cin>>n;
    if(find(a.begin(),a.end(),n)!=a.end())
        cout<<1;
    else
        cout<<0;
}
