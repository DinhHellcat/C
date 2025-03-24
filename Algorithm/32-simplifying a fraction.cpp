#include<bits/stdc++.h>
using namespace std;
int gcd(int a,int b)
{
    if(a==0 || b==0)
        return a + b;
    while (a != b)
    {
        if (a > b)
            a-=b;
        else
            b-=a;
    }
    return a;
}
int main()
{
    int a,b;
    cin>>a>>b;
    if(b==0)
        cout<<"INVALID";
    else if(b==1)
        cout<<a;
    else
    {
        int a2,b2,c;
        a2=abs(a);
        b2=abs(b);
        c=gcd(a2,b2);
        a/=c;
        b/=c;
        if(b==1)
            cout<<a;
        else if(b<=0)
            cout<<0-a<<" "<<0-b;
        else
            cout<<a<<" "<<b;
    }
}
