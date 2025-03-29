#include<bits/stdc++.h>
using namespace std;

const int limit = 100000;
vector<bool> isPrime(limit + 1,true);
vector<long> Primes;

void progress()
{
    isPrime[0]=isPrime[1]=false;
    for(long long i=0;i<=limit;i++)
        if(isPrime[i])
        {
            for(long long j=i*2;j*j<=limit;j+=i)
                isPrime[j]=false;
            Primes.push_back(i);
        }
}
void giai(long long n)
{
    vector<pair<long long,int>> f;
    for(int i : Primes)
    {
        if(i*i>n) break;
        if(n%i==0)
        {
            int c=0;
            while(n%i==0)
            {
                c++;
                n/=i;
            }
            f.push_back({i,c});
        }
    }
    if(n>1)
        f.push_back({n,1});
    cout<<f.size()<<endl;
    for(const auto &i : f)
        cout<<i.first<<" "<<i.second<<endl;
}
int main()
{
    progress();
    long long n;
    cin>>n;
    giai(n);
}
