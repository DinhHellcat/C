#include<bits/stdc++.h>
using namespace std;

int lcd(int a,int b)
{
    if(a==0||b==0)
        return a+b;
    while(a!=b)
    {
        if(a>b)
            a-=b;
        else
            b-=a;
    }
    return a;
}

int main() {
    int a,b;
    cin>>a>>b;
    cout<<lcd(a,b);
    return 0;
}

