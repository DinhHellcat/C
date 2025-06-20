#include<bits/stdc++.h>
using namespace std;
int main()
{
    long long n;
    cin>>n;
    if(pow(round(sqrt(n)),2)==n)
        cout<<1;
    else cout<<0;
}
