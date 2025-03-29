#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n,x,t=0;
    cin>>n>>x;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    for(int i=0;i<n;i++)
        if(a[i]==x)
        {
            t=1;
            break;
        }
    if(t==1) cout<<"YES";
    else cout<<"NO";
}
