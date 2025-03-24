#include<bits/stdc++.h>
using namespace std;
double sum(double x)
{
    if(x<=0) return 0;
    return 1/x+sum(x-1);
}
int main()
{
    long n;
    cin>>n;
    vector<double> a;
    double x;
    for(long i=0;i<n;i++)
    {
        cin>>x;
        a.push_back(sum(x));
    }
    for(double i : a)
        cout<<fixed<<setprecision(5)<<i<<endl;
}

