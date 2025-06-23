#include<bits/stdc++.h>
using namespace std;

float cal(int a,int b, int c, int d)
{
    return pow(abs(c-a),2) + pow(abs(d-b),2);
}
int main()
{
    int n;
    cin>>n;
    vector<float> x(n),y(n);
    for(int i=0;i<n;i++)
        cin>>x[i]>>y[i];
    float z,t=99999999;
    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
        {
            z=cal(x[i],y[i],x[j],y[j]);
            if(z<t)
                t=z;
        }
    cout<<fixed<<setprecision(2)<<t;
}
