#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,S;
    cin>>n>>S;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    vector<int> dp(S+1,0);
    dp[0]=1;
    for(int i=0;i<n;i++)
    //{
        for(int j=S;j>=a[i];j--)
            if(dp[j-a[i]]==1)
                dp[j]=1;
        /*for(int i : dp)
            cout<<i<<" ";
        cout<<endl;
    }
    for(int i : dp)
        cout<<i<<" ";*/
    cout<<dp[S];
}
