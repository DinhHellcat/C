#include<bits/stdc++.h>
using namespace std;
int limit = 1000001;
vector<double> a(limit);
void progress()
{
    a[0]=0;
    for(double i=1;i<limit;i++)
        a[i]=1/(2*i-1) + a[i-1];
}
int main()
{
    progress();
    int n,x;
    cin>>n;
    vector<double> b(n);
    for(int i=0;i<n;i++)
    {
        cin>>x;
        b[i]=a[x];
    }
    for(double i : b)
        cout<<fixed<<setprecision(5)<<i<<endl;
}
