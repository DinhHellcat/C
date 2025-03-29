#include<bits/stdc++.h>
using namespace std;
int main()
{
    long n;
    cin>>n;
    if(n<2)
        cout<<"NOT FOUND";
    else
    {
        long a[n];
        for(int i=0;i<n;i++)
            cin>>a[i];
        sort(a,a+n);
        if(a[0]==a[n-1])
            cout<<"NOT FOUND";
        else
        {
            for(int i=n-2;i>=0;i--)
            {
                cout<<i<<" ";
                if(a[i]<a[n-1])
                {
                    cout<<a[i];
                    return 0;
                }
            }
            cout<<"NOT FOUND";
        }
    }
}
