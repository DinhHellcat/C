#include<bits/stdc++.h>
using namespace std;

void print(vector<int> temp)
{
    for(int i:temp)
        cout<<i<<" ";
    cout<<endl;
}
void dequi(vector<int>& input,int x,vector<int>& temp,int index)
{
    if(x<0) return;
    if(x==0) {print(temp);return;}

    for(int i=index;i<input.size();i++)
    {
        temp.push_back(input[i]);
        dequi(input,x-input[i],temp,i);
        temp.pop_back();
    }
}
int main()
{
    int n,x;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)
        cin>>a[i];
    cin>>x;

    sort(a.begin(),a.end());
    vector<int> t;
    dequi(a,x,t,0);
}
