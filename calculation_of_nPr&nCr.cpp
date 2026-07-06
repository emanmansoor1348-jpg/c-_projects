#include<iostream>
using namespace std;

template<typename T>
T nPr(T n, T r) {
    if(r > n || r < 0) return 0;
    
    T result = 1;
    for(T i = 0; i < r; i++) {
        result *= (n - i);
    }
    return result;
}

template<typename T>
T nCr(T n, T r) {
    if(r > n || r < 0) return 0;
    if(r == 0 || r == n) return 1;
    
    if(r > n - r) r = n - r;
    
    T result = 1;
    for(T i = 0; i < r; i++) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

int main() {
    cout << "=== With int ===\n";
    cout << "5P3 = " << nPr<int>(5, 3) << "\n";    // 60
    cout << "5C3 = " << nCr<int>(5, 3) << "\n";    // 10
    
    cout << "\n=== With long long ===\n";
    cout << "20P5 = " << nPr<long long>(20, 5) << "\n";   // 1860480
    cout << "20C5 = " << nCr<long long>(20, 5) << "\n";   // 15504
    
    cout << "\n=== With double ===\n";
    cout << "10P3 = " << nPr<double>(10, 3) << "\n";      // 720
    cout << "10C3 = " << nCr<double>(10, 3) << "\n";      // 120
}