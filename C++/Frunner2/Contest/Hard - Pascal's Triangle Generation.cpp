#include<bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin>>n;
    int a[n][n];

    for(int i=0;i<n;i++)
        a[i][0]=1;

    for(int i=1;i<n;i++)
        for(int j=1;j<=i;j++)
        {
            if(j==i)
                a[i][j]=1;
            else
                a[i][j]=a[i-1][j]+a[i-1][j-1];
        }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<=i;j++)
            cout<<a[i][j]<<" ";
        cout<<endl;
    }
}
