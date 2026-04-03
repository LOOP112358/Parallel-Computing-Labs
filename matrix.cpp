#include <iostream>
#include <vector>
#include <time.h>
#include <cmath> 

using namespace std;

// 计时函数
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

void run_test(int n) {
    cout << "--- 正在测试规模 N = " << n << " ---" << endl;

    // 1. 初始化数据
    vector<vector<double>> matrix(n, vector<double>(n));
    vector<double> vec_in(n), res_trivial(n, 0), res_opt(n, 0);

    for(int i = 0; i < n; i++) {
        vec_in[i] = i ;
        for(int j = 0; j < n; j++) matrix[i][j] = i + j;
    }

    // 2. 平凡算法测试
    double start = get_time();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res_trivial[i] += matrix[j][i] * vec_in[j];
        }
    }
    double end = get_time();
    double t1 = end - start;

    // 3. 优化算法测试
    start = get_time();
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            res_opt[i] += matrix[j][i] * vec_in[j];
        }
    }
    end = get_time();
    double t2 = end - start;

    // 4. 正确性校验
    bool correct = true;
    for(int i=0; i<n; i++) {
        if(abs(res_trivial[i] - res_opt[i]) > 1e-6) correct = false;
    }

    cout << "结果正确性: " << (correct ? "通过" : "失败") << endl;
    cout << "平凡耗时: " << t1 << "s, 优化耗时: " << t2 << "s" << endl;
    cout << "当前加速比: " << t1 / t2 << endl << endl;
}

int main() {
    // 定义你想测试的规模，比如从 100 到 3000
    int sizes[] = {100, 500, 1000, 2000, 3000}; 
    for (int n : sizes) {
        cout << "--- 当前测试规模 N = " << n << " ---" << endl;
        run_test(n); // 把计时的逻辑封装在这个函数里
    }
    return 0;
}