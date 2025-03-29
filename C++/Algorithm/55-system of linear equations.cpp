#include<bits/stdc++.h>
using namespace std;
int main()
{
    double a1,b1,c1,a2,b2,c2,x,y,D,Dx,Dy;
    cin>>a1>>b1>>c1>>a2>>b2>>c2;
    if((a1==0 && b1==0)||(a2==0 && b2==0))
        cout<<"VONGHIEM";
    else if(a1/a2==b1/b2 || a2/a1==b2/b1)
    {
        if(b1/b2!=c1/c2 || b1/b2==b2/b1)
            cout<<"VONGHIEM";
        else
            cout<<"VOSONGHIEM";
    }
    else
    {
        D=a1*b2 - a2*b1;
        Dx=c1*b2 - c2*b1;
        Dy=a1*c2 - a2*c1;
        x=Dx/D;
        y=Dy/D;
        cout<<fixed<<setprecision(2)<<x<<" "<<y;
    }
}
