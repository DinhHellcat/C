#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<vector<int>> a(n,vector<int>(n));
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            cin>>a[i][j];
    int s=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(i+j+1==n)
                s+=a[i][j];
    cout<<s;
}
