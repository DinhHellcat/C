#include<bits/stdc++.h>
using namespace std;
int x=0;
vector<int> a{1,2};
void compile(int n,int t)
{
    if(t>n)
        return;
    if(t==n)
    {
        x++;
        return;
    }
    for(int i=0;i<2;i++)
        compile(n,t+a[i]);
}
int main()
{
    int n;
    cin>>n;
    int t=0;
    compile(n,t);
    cout<<x;
}

/*
vector<int> fibo(1000);
void Fibo()
{
    fibo[1]=1;
    fibo[2]=2;
    for(int i=3;i<1000;i++)
        fibo[i]=fibo[i-1]+fibo[i-2];
}
int main()
{
    Fibo();
    int n;
    cin>>n;
    cout<<fibo[n];
}
*/
