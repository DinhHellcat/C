#include<bits/stdc++.h>
using namespace std;
int main()
{
    int a,n=0;
    cin>>a;
    for(int i=1;i<=abs(a);i++)
        if(a%i==0)
            n++;
    cout<<n;
}
