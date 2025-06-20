#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,k;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    cin>>k;
    k=k%n;
    vector<int> b(n);
    for(int i=0;i<n;i++)
        b[(i+k)%n]=a[i];
    for(int i:b)
        cout<<i<<" ";
}
