#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    while(n%2==0)
    {
        n/=2;
        cout<<2<<" ";
    }
    for(int i=3;i*i<n;i+=2)
    {
        while(n%i==0)
        {
            n/=i;
            cout<<i<<" ";
        }
    }
    cout<<n;
}
