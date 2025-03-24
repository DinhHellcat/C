#include<bits/stdc++.h>
using namespace std;
int main()
{
    long a[10001],n=0,x=0;
    do
    {
        cin>>a[n];
        n++;
    }
    while(a[n-1]!=0);
    for(int i=0;i<n;i++)
        if(a[i]<0)
        {
            x=1;
            cout<<a[i]<<" ";
        }
    if(x==0)
        cout<<"NOT FOUND";
}
