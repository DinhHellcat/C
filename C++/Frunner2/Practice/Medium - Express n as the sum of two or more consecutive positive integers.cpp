#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,t=0;
    cin>>n;
    for(int i=1;2*i<=n+1;i++)
    {
        int s=0;
        for(int j=i;2*j<=n+1;j++)
        {
            s+=j;
            if(s==n)
            {
                t++;
                break;
            }
            if(s>n)
                break;
        }
    }
    cout<<t;
}
