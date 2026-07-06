#include<iostream>
using namespace std;

void frequency(int a[], int n) {
    for(int i = 0; i < n; i++) {
        int count = 0;
        for(int j = 0; j < n; j++) {
            if(a[i] == a[j]) {
                count++;  
            } 
        }
        cout << a[i] << " occurs " << count << " times" << endl;
    }
}
int main() {
    int n;
    cout << "Input the number of elements: ";
    cin >> n;
    int a[n];
    for(int i = 0; i < n; i++) {
        cout << "element - " << i << ": ";
        cin >> a[i];
    }
    cout << "The frequency of all elements of an array:" << endl;
    frequency(a, n);
    return 0;
}