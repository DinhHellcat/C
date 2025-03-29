#include<bits/stdc++.h>
using namespace std;
double sum(int n)
{
    double s=0;
    for(int i=n;i>=1;i--)
        s=sqrt(i+s);
    return s;
}
int main()
{
    int T;
    cin>>T;
    double a[T];
    for(int i=0;i<T;i++)
        cin>>a[i];
    for(int i=0;i<T;i++)
        cout<<fixed<<setprecision(5)<<sum(a[i])<<endl;
}
