#include<bits/stdc++.h>
using namespace std;
double sum(double n)
{
    if(n==1)
        return 1/(1+n);
    return 1/(1+sum(n-1));
}
int main()
{
    int T;
    cin>>T;
    double a[T],n;
    for(int i=0;i<T;i++)
    {
        cin>>n;
        a[i]=sum(n);
    }
    for(int i=0;i<T;i++)
        cout<<fixed<<setprecision(5)<<a[i]<<endl;
}
