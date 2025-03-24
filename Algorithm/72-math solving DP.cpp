#include<bits/stdc++.h>
using namespace std;
int maxweight(int n, int c,const vector<int>& weights)
{
    vector<int> dp(c+1,0);
    for(int i=0;i<n;i++)
    {
        int weight=weights[i];
        for(int j=c;j>=weight;j--)
        {
            dp[j]=max(dp[j],dp[j-weight]+weight);
        }
    }
    return dp[c];
}
int main()
{
    int n,c;
    cin>>n>>c;
    vector<int> weights(n);
    for(int i=0;i<n;i++)
        cin>>weights[i];
    cout<<maxweight(n,c,weights);
}
