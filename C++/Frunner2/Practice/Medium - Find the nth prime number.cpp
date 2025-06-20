#include<bits/stdc++.h>
using namespace std;
vector<int> n;
int check(long long n)
{
    if(n<2) return 0;
    if(n==2 || n==3) return 1;
    if(n%2==0 || n%3==0) return 0;
    for(long long i=5;i*i<=n;i+=6)
        if(n%i==0 || n%(i+2)==0)
            return 0;
    return 1;
}
void progress()
{
    for(int i=2;i<1000000;i++)
        if(check(i))
            n.push_back(i);
}
int main()
{
    progress();
    int x;
    cin>>x;
    cout<<n[x-1];
}
