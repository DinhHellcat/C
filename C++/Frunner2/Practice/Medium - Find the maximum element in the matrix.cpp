#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,m,x;
    cin>>n>>m;
    set<int> a;
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
        {
            cin>>x;
            a.insert(x);
        }
    cout<<*--a.end();
}
