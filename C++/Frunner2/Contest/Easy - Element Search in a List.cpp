#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    int x;
    bool check=false;
    cin>>x;
    for(int i=0;i<n;i++)
        if(a[i]==x)
            check=true;
    cout<<check;
}
