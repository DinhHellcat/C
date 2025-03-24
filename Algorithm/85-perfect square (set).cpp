#include<bits/stdc++.h>
using namespace std;
bool chinhphuong(int n)
{
    if(n<=0) return false;
    if(n==pow(round(sqrt(n)),2))
        return true;
    return false;
}
int main()
{
    int m,n;
    cin>>m>>n;
    set<int> t;
    vector<vector<int>> a(m,vector<int>(n));
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
        {
            cin>>a[i][j];
            if(chinhphuong(a[i][j]))
                t.insert(a[i][j]);
        }
    if(!t.empty())
        for(int i : t)
            cout<<i<<" ";
    else
        cout<<"NOT FOUND";
}
