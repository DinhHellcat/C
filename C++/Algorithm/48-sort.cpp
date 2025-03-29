#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n;
    cin>>n;
    long a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    sort(a,a+n);
    for(int i=n-1;i>=0;i--)
        cout<<a[i]<<" ";
}
