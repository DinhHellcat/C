#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n,k;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    cin>>k;

    int t=0;
    for(int i=0;i<n;i++)
    {
        int s=0;
        for(int j=i;j<n;j++)
        {
            s+=a[j];
            if(s<k)
                t++;
        }
    }
    cout<<t;
}
