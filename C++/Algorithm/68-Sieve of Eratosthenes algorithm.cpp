#include<bits/stdc++.h>
using namespace std;
vector<int> sangngto(int n)
{
    vector<bool> x(n+1,true);
    vector<int> t;
    x[0] = x[1] = false;
    for(int i=2;i*i<=n;i++)
        if(x[i])
            for(int j=i*i;j<=n;j+=i)
                x[j]=false;
    for(int i=0;i<=n;i++)
        if(x[i])
            t.push_back(i);
    return t;
}
int main()
{
    int n;
    cin>>n;
    vector<int> a = sangngto(n);
    for(int i : a)
        cout<<i<<" ";
}
