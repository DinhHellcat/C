#include<bits/stdc++.h>
using namespace std;
vector<int> a;
bool isPrime(int n)
{
    if(n==2 || n==3) return true;
    if(n%2==0 || n%3==0) return false;
    for(int i=5;i*i<=n;i+=6)
        if(n%i==0 || n%(i+2)==0)
            return false;
    return true;
}
void progress()
{
    for(int i=1;i*i<=1000000;i++)
        if(isPrime(i))
            a.push_back(i);
}
int main()
{
    progress();
    int n,s=0;
    cin>>n;
    for(int i:a)
        if(i<=n)
            s+=i;
        else
            break;
    cout<<s;
}
