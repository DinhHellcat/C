#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    sort(a.begin(),a.end());

    if(a[0]!=0)
        cout<<0;
    else
        for(int i=0;i<n;i++)
            if(a[i]+1!=a[i+1])
            {
                cout<<a[i]+1;
                break;
            }
}
