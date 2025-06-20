#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,x=0;
    cin>>n;
    vector<vector<int>> a(n,vector<int> (n));
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            cin>>a[i][j];
            if(i==j)
                s+=a[i][j];
        }
    cout<<s;
}
