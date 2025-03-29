//khoảng cách
//a nhỏ - A lớn: 32
//số 1 và kí tự 1: 48

//isspace char
!isspace(c)

//Phân số
fixed<<setprecision(2)

//Input EOF
while(cin>>s)

//stringstream
string s;
stringstream ss(s)
string word;
while(ss>>word){}

//////////////////////////////////////////////////////////////////////////
//ARRAY
int a[10001];
int length = sizeof(a) / sizeof(a[0]);
sort(a, a+length);
*max_element(a,a+length);
distance(a, max_element(a,a+length()));

//////////////////////////////////////////////////////////////////////////
//STRING
string s;
//Xóa cache
cin.ignore();
fflush(stdin);

s.size(); = s.length();
s.append("abc");
s+="abc";
s[i];
s.find("abc"); //print vị trí đầu tiên mà "abc" xuất hiện hoặc 1 số lớn nếu ko
s.compare(t);  //print -1 0 1 (so sánh độ dài)

cin>>s;        //đọc đến khoảng trắng
getline(cin,s);//đọc đến enter

sort(s.begin().s.end()); //CBAcba321 -> 123ABCabc

//////////////////////////////////////////////////////////////////////////
//SET
set<int> s;
s.size();
s.insert(value);
s.count(value);  //print 1 or 0
s.find(value);   //print 1 or 0
set.erase(value);

*set.rbegin();   //lớn nhất
*set.begin();    //nhỏ nhất

//MULTISET
//Y hệt set, khác ở chỗ nó cho trùng value

//////////////////////////////////////////////////////////////////////////
//VECTOR
vector<int> a;
vector<int> a(m);
vector<int> a(m,0);
vec.size();
vec.pop_back();       //xóa cuối
vec.erase             //vec{1,2,3,4,5}  vec.erase(vec.begin() + 2)  -> 1 2 4 5
vec.clear();          //xóa hoàn toàn

vec.front = vec[0]
vec.back = vec[vec.size()-1]

*max_element(vec.begin(), vec.end());
distance(vec.begin(), max_element(vec.begin(),vec.end()));

vec[i];
vec.push_back(value); //thêm cuối

sort(vec.begin(), vec.end());
reverse(vec.begin(),vec.end());

//điền từ 1 đến n phần tử của vector
iota(a.begin(), a.end(), 1);


//////////////////////////////////////////////////////////////////////////
//VECTOR 2 CHIEU
//vector<vector<>>
vector<vector<int>> a(m,vector<int>(n));
a[i][j];
a.size(); //trả về m
a[n].size(); //trả về n
a.push_back(row) //m+1

a[i].push_back(value) //thêm vào hàng i n+1
//sort ngang
sort(matran[n-1].begin(), matran[n-1].end());
//sort dọc
for(int i=0;i<matran.size();i++)
    temp.push_back(matran[i][n-1]);
sort(temp.begin(), temp.end());
for(int i=0;i<matran.size();i++)
    matran[i][n-1]=temp[i];


                            function
//////////////////////////////////////////////////////////////////////////
//PRIME NUMBER
//tao vector isPrime
const int limit = 100000
vector<bool> isPrime(limit + 1,true);
void progress()
{
    isPrime[0]=isPrime[1]=false;
    for(long long i=0;i*i<=limit;i++)
        if(isPrime[i])
            for(long long j=i*i;j*j<=limit;j+=i)
                isPrime[j]=false;
}
//tao vector cac so ngto
const int limit = 100000;
vector<bool> isPrime(limit + 1, true);
vector<int> primes;

void sieve() {
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i <= limit; i++) {
        if (isPrime[i]) {
            primes.push_back(i);
            for (int j = i * 2; j <= limit; j += i) {
                isPrime[j] = false;
            }
        }
    }
}

//Dãy con tăng dài nhất LIS longestIncreasingSubsequence
//Dynamic Programming DP
int longestIncreasingSubsequenceDP(const vector<int>& arr) {
    int n = arr.size();
    vector<int> dp(n, 1); // Initialize dp array with 1

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j]) {
                dp[i] = max(dp[i], dp[j] + 1);
                cout<<i<<" "<<j<<endl;
            }
        }
    }

    return *max_element(dp.begin(), dp.end()); // Return the maximum value in dp array
}

//Dec to Bin
string dectobin(long long x)
{
    string s;
    while(x>0)
    {
        if(x%2==0)
            s+='0';
        else
            s+='1';
        x/=2;
    }
    return s;
}

//Ước của 1 số
for(long i=1;i<=n/i;i++)
{
    if(n%i==0)
        s+=i+n/i;
    if(pow(i,2)==n)
        s-=i;
}
//Ước chung nhỏ nhất của 2 số
int gcd(int a,int b)
{
    if(a==0 || b==0)
        return a + b;
    while (a != b)
    {
        if (a > b)
            a-=b;
        else
            b-=a;
    }
    return a;
}
//Bội chung lớn nhất của 2 số
(a*b)/gcd(a,b) //bội 2 số chia cho ucnn

//Tam giác Pascal giải bài tổ hợp chập n của k
int combination(int n, int k) {
    vector<vector<int>> C(n+1,vector<int>(k+1,0));
    for(int i=0;i<=n;++i)
    {
        for(int j=0;j<=min(i,k);++j)
        {
            if(j==0 || j==i)
                C[i][j] = 1;
            else
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
        }
    }
    return C[n][k];
}
