#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n,a[10],t=0;
    for(int i=0;i<10;i++)
        cin>>a[i];
    cin>>n;
    for(int i=0;i<10;i++)
        if(a[i]==n)
        {
            t=1;
            cout<<i+1<<" ";
        }
    if(t==0)
        cout<<-1;
}
