#include<bits/stdc++.h>
using namespace std;

void print(vector<int>& v)
{
    for(int i:v)
        cout<<i<<" ";
    cout<<endl;
}
void bubblesort(vector<int>& v,int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-1;j++)
        {
            if(v[j]>v[j+1])
            {
                int t=v[j];
                v[j]=v[j+1];
                v[j+1]=t;
            }
        }
        print(v);
    }
}
int main()
{
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    bubblesort(a,n);
}
