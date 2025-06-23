#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];

    vector<int> b,c;
    for(int i=0;i<n;i++)
        if(a[i]>0)
        {
            b.push_back(a[i]);
            c.push_back(i);
        }
    sort(b.begin(),b.end());
    int m=b.size();
    for(int i=0;i<m;i++)
        a[c[i]]=b[i];
    for(int i:a)
        cout<<i<<" ";
}
