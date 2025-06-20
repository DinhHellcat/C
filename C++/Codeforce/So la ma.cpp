#include<bits/stdc++.h>
using namespace std;

int main()
{
    string a;
    cin>>a;
    int n=0;
    for(int i=0;i<a.length();i++)
    {
        switch(a[i])
        {
            case 'M': n+=1000;break;
            case 'D': n+=500;break;
            case 'C': n+=100;break;
            case 'L': n+=50;break;
            case 'X': n+=10;break;
            case 'V': n+=5;break;
            case 'I': n+=1;break;
        }
        if(a[i]=='M' || a[i]=='D')
            if(a[i-1]=='C')
                n-=200;
        if(a[i]=='C' || a[i]=='L')
            if(a[i-1]=='X')
                n-=20;
        if(a[i]=='X' || a[i]=='V')
            if(a[i-1]=='I')
                n-=2;
    }
    cout<<n;
}
