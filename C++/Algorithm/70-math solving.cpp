#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

const int MAX_N = 1000000;
vector<double> dp(MAX_N + 1);

void preprocess() {
    dp[1] = sqrt(1);
    for (int i = 2; i <= MAX_N; ++i) {
        dp[i] = sqrt(i + dp[i - 1]);
    }
}

int main() {
    preprocess(); // Tính trước các giá trị cho tất cả n từ 1 đến MAX_N

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        cout << fixed << setprecision(5) << dp[n] << endl;
    }

    return 0;
}
