#include<bits/stdc++.h>
using namespace std;

int main()
{
    int m,n,r,c,k=0;
    cin>>m>>n;
    vector<int> a(m*n);
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
        {
            cin>>a[k];
            k++;
        }
    cin>>r>>c;
    k=0;
    if(m*n!=r*c)
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<n;j++)
            {
                cout<<a[k]<<" ";
                k++;
            }
            cout<<endl;
        }
    else
        for(int i=0;i<r;i++)
        {
            for(int j=0;j<c;j++)
            {
                cout<<a[k]<<" ";
                k++;
            }
            cout<<endl;
        }
}
