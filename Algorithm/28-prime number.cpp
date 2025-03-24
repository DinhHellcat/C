#include<bits/stdc++.h>
using namespace std;
bool ngto(long n)
{
    if(n<=1) return false;
    for(int i=2;i<=sqrt(n);i++)
        if(n%i==0)
            return false;
    return true;
}
int main()
{
    long n;
    cin>>n;
    if(ngto(n)==true)
        cout<<"YES";
    else
        cout<<"NO";
}
