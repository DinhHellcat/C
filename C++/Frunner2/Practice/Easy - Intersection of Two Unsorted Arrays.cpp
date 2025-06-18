#include<bits/stdc++.h>
using namespace std;

int main()
{
    /*
    string t;
    set<int> a,b,c;

    if(getline(cin,t))
    {
        istringstream iss(t);
        int x;
        while(iss>>x) a.insert(x);
    }

    if(getline(cin,t))
    {
        istringstream iss(t);
        int x;
        while(iss>>x) b.insert(x);
    }*/
    int n,m,x;
    cin>>n>>m;
    set<int> a,b,c;
    for(int i=0;i<n;i++)
    {
        cin>>x;
        a.insert(x);
    }
    for(int i=0;i<m;i++)
    {
        cin>>x;
        b.insert(x);
    }

    for(int i:a)
        if(b.count(i))
            c.insert(i);
    for(int i:c)
        cout<<i<<" ";
}
