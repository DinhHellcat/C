#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    int sum=0,t=INT_MIN;
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(a[j]>a[i])
                sum=a[j]-a[i];
            if(sum>t)
                t=sum;
            if(a[j]<a[i])
                break;
        }
    }
    cout<<t;
}
