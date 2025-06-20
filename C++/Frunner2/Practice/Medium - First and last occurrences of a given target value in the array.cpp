#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,x;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    cin>>x;
    bool check = false;
    for(int i=0;i<n;i++)
        if(a[i]==x)
        {
            cout<<i<<" ";
            check=true;
            break;
        }
    for(int i=n-1;i>=0;i--)
        if(a[i]==x)
        {
            cout<<i;
            break;
        }
    if(check==false)
        cout<<-1<<" "<<-1;
}
