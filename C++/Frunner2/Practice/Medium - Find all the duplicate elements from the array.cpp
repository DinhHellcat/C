#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    int x=0;
    vector<int> t;
    vector<int> result;
    for(int i=0;i<n;i++)
    {
        if(find(t.begin(),t.end(),a[i])==t.end())
            t.push_back(a[i]);
        else
        {
            x++;
            result.push_back(a[i]);
        }
    }
    cout<<x<<endl;
    for(int i:result)
        cout<<i<<" ";
}
