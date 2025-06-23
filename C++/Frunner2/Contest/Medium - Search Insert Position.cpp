#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    int k;
    cin>>k;

    auto x=lower_bound(a.begin(),a.end(),k);
    int d=distance(a.begin(),x);
    cout<<d;
}
