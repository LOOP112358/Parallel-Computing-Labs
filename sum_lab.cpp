#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>

using namespace std;

// 高精度计时函数
double get_time_s() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// 1. 平凡算法（链式累加）
double sum_trivial(const vector<double>& a, int n) {
    double total = 0;
    for (int i = 0; i < n; i++) {
        total += a[i];
    }
    return total;
}

// 2. 优化算法：双路累加（利用超标量并行）
double sum_dual_way(const vector<double>& a, int n) {
    double sum1 = 0, sum2 = 0;
    for (int i = 0; i < n ; i += 2) {
        sum1 += a[i];
        sum2 += a[i + 1];
    }
    return sum1 + sum2;
}

// 3. 递归/二分累加思想（迭代实现，避免栈溢出）
double sum_pairwise(vector<double> a, int n) {
    for (int m = n; m > 1; m = (m + 1) / 2) { // 向上取整处理奇数
        for (int i = 0; i < m / 2; i++) {
            a[i] = a[i * 2] + a[i * 2 + 1];
        }
    }
    return a[0];
}

void run_sum_test(int n) {
    cout << "--- 测试规模 N = " << n << " ---" << endl;
    vector<double> a(n);
    for (int i = 0; i < n; i++) a[i] = i ;

    // 为了让计时更有说服力，重复计算 100 次
    int repeat = 100;

    // 测试平凡算法
    double start = get_time_s();
    double res1 = 0;
    for(int r=0; r<repeat; r++) res1 = sum_trivial(a, n);
    double t1 = (get_time_s() - start) / repeat;

    // 测试双路累加
    start = get_time_s();
    double res2 = 0;
    for(int r=0; r<repeat; r++) res2 = sum_dual_way(a, n);
    double t2 = (get_time_s() - start) / repeat;

    // 测试递归/对等累加（由于它会修改原数组，测一次即可或备份数据）
    start = get_time_s();
    double res3 = sum_pairwise(a, n);
    double t3 = get_time_s() - start;

    cout << "平凡算法耗时: " << t1 << "s, 结果: " << res1 << endl;
    cout << "双路优化耗时: " << t2 << "s, 结果: " << res2 << endl;
    cout << "二分递归耗时: " << t3 << "s, 结果: " << res3 << endl;
    cout << "双路加速比: " << t1 / t2 << endl;
    
    // 进阶思考：结果差异
    if(abs(res1 - res2) > 1e-9) {
        cout << "注意：由于浮点数运算次序改变，结果存在微小差异！" << endl;
    }
    cout << endl;
}

int main() {
    // 测试不同规模，建议测到 10^7 或 10^8 级别
    vector<int> sizes = {1024, 16384, 131072, 1048576};
    for (int n : sizes) {
        run_sum_test(n);
    }
    return 0;
}