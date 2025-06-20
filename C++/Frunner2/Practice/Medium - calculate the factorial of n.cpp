#include<bits/stdc++.h>
using namespace std;
vector<int> a(21);
void compile()
{
    a[0]=1;
    for(int i=1;i<21;i++)
        a[i]=a[i-1]*i;
}
int main()
{
    compile();
    int n;
    cin>>n;
    cout<<a[n];
}
