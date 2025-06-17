#include<bits/stdc++.h>
using namespace std;
int main()
{
    vector<int> a;
    int n,x;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        int j;
        cin>>j;
        a.push_back(j);
    }
    sort(a.begin(),a.end());
    cin>>x;
    auto vt = lower_bound(a.begin(),a.end(),x);
    int index = vt - a.begin();
    cout<<index;
}
