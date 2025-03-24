#include<bits/stdc++.h>
using namespace std;
int main()
{
    int a,b,i=0;
    cin>>a>>b;
    b--;
    if(a<=b)
    {
        while(a!=b)
        {
            if(b%3==0)
            {
                cout<<b<<" ";
                i=1;
            }
            b--;
        }
        if(i==0)
            cout<<"NOT FOUND";
    }
    else
        cout<<"NOT FOUND";
}
