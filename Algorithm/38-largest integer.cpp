#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,x,sum=0;
    cin>>n;
    for(int i=1;i;i++)
    {
        sum+=i;
        if(sum>n)
        {
            x=i-1;
            break;
        }
    }
    cout<<x;
}
