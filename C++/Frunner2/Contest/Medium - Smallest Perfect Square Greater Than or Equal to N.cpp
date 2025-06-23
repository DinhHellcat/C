#include<bits/stdc++.h>
using namespace std;
set<int> a;
void compile()
{
    for(int i=1;i*i<pow(10,9);i++)
        a.insert(i*i);
}
int main()
{
    compile();
    int x;
    cin>>x;
    for(int i:a)
        if(i>=x)
        {
            cout<<i;
            break;
        }
}
