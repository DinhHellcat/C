#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    bool check = false;
    for(int i=0;i<n;i++)
        if(a[i]==i)
        {
            cout<<i;
            check=true;
        }
    if(!check)
        cout<<-1;
}
