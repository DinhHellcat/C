#include<bits/stdc++.h>
using namespace std;
int main()
{
    long long a;
    cin>>a;
    if(pow(round(sqrt(a)),2) == a)
        cout<<"YES";
    else
        cout<<"NO";
}
