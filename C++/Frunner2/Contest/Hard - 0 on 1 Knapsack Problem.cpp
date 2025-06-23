#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,W;
    cin>>n>>W;
    vector<int> value(n),weight(n);
    for(int i=0;i<n;i++)
        cin>>value[i]>>weight[i];

    vector<int> dp(W+1,0);
    for(int i=0;i<n;i++)
        for(int j=W;j>=weight[i];j--)
            dp[j]=max(dp[j],value[i]+dp[j-weight[i]]);
    cout<<dp[W];
}
