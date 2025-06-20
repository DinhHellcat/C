#include<bits/stdc++.h>
using namespace std;

void print(vector<string>& v)
{
    for(string i:v)
        cout<<i<<" ";
    cout<<endl;
}
void selectionsort(vector<string>& v,int n)
{
    int t=0;
    string x;
    for(int i=0;i<n-1;i++)
    {
        for(int j=i;j<n;j++)
            if(v[j]<v[i])
                t=j;
        x=v[t];
        v[t]=v[i];
        v[i]=x;
        print(v);
    }
}
int main()
{
    int n;
    cin>>n;
    vector<string> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    selectionsort(a,n);
}
