#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<vector<int>> a(n,vector<int>(2));
    for(int i=0;i<n;i++)
        cin>>a[i][0]>>a[i][1];
    vector<vector<int>> b;
    b.push_back(a[0]);
    for(int i=1;i<n;i++)
    {
        vector<int>& x=b.back();
        vector<int>& y=a[i];
        if(y[0]<=x[1])
            x[1]=max(x[1],y[1]);
        else
            b.push_back(y);
    }
    for(auto i:b)
        cout<<i[0]<<" "<<i[1]<<endl;
}
