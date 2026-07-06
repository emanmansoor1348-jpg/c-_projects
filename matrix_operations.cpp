#include <iostream>
using namespace std;

class Matrix {
private:
    int rows, cols;
    int mat[10][10];

public:
    void input() {
        cout << "Enter number of rows and columns: ";
        cin >> rows >> cols;

        cout << "Enter matrix elements:\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cin >> mat[i][j];
            }
        }
    }

    void display() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }

    Matrix add(Matrix m) {
        Matrix result;
        result.rows = rows;
        result.cols = cols;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.mat[i][j] = mat[i][j] + m.mat[i][j];
            }
        }
        return result;
    }

    Matrix subtract(Matrix m) {
        Matrix result;
        result.rows = rows;
        result.cols = cols;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.mat[i][j] = mat[i][j] - m.mat[i][j];
            }
        }
        return result;
    }

    Matrix multiply(Matrix m) {
        Matrix result;
        result.rows = rows;
        result.cols = m.cols;

        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                result.mat[i][j] = 0;
                for (int k = 0; k < cols; k++) {
                    result.mat[i][j] += mat[i][k] * m.mat[k][j];
                }
            }
        }
        return result;
    }

    int getRows() { return rows; }
    int getCols() { return cols; }
};

int main() {
    Matrix A, B, C;

    cout << "Enter First Matrix:\n";
    A.input();

    cout << "\nEnter Second Matrix:\n";
    B.input();

    if (A.getRows() == B.getRows() && A.getCols() == B.getCols()) {
        cout << "\nAddition Result:\n";
        C = A.add(B);
        C.display();

        cout << "\nSubtraction Result:\n";
        C = A.subtract(B);
        C.display();
    } else {
        cout << "\nAddition and Subtraction not possible!\n";
    }

    if (A.getCols() == B.getRows()) {
        cout << "\nMultiplication Result:\n";
        C = A.multiply(B);
        C.display();
    } else {
        cout << "\nMultiplication not possible!\n";
    }
    return 0;
}