#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n,k;
    cin>>n>>k;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    int s=0;
    if(k==0)
        for(int i:a)
        {
            if(i%2==0)
                s+=i;
        }
    else
        for(int i:a)
        {
            if(i%2!=0)
                s+=i;
        }
    cout<<s;
}
