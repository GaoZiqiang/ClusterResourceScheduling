#include<iostream>
#include <vector>
#include <stdlib.h>

#include <math.h>
#include <typeinfo>
#include <map>
#include <string.h>
using namespace std;

//接下来把一般实矩阵的QR分解按函数的形式稍稍改写一下，输入是一般mxn实矩阵A，以及矩阵的行数m列数n，输出是QR形式的正交矩阵和上三角矩阵的乘积，

void Maqr(double ** A, int m, int n)//进行一般实矩阵QR分解的函数
{
    int i, j, k, nn, jj;
    double u, alpha, w, t;
    double** Q = new double*[m];   //动态分配内存空间
    for (i = 0; i<m; i++) Q[i] = new double[m];
    if (m < n)
    {
        cout << "\nQR分解失败！" << endl;
        exit(1);
    }
    //保证列数<=行数
    for (i = 0; i <= m - 1; i++)
        for (j = 0; j <= m - 1; j++)
        {
            Q[i][j] = 0.0;
            if (i == j) Q[i][j] = 1.0;
        }
    //初始的Q矩阵就是一个单位的m阶方阵
    nn = n;
    if (m == n) nn = m - 1;
    for (k = 0; k <= nn - 1; k++)//在大循环k：0~m当中，进行H矩阵的求解，左乘Q，以及左乘A
    {

        u = 0.0;
        for (i = k; i <= m - 1; i++)
        {
            w = fabs(A[i][k]);
            if (w > u) u = w;
        }
        alpha = 0.0;
        for (i = k; i <= m - 1; i++)
        {
            t = A[i][k] / u; alpha = alpha + t * t;
        }
        if (A[k][k] > 0.0) u = -u;
        alpha = u * sqrt(alpha);
        if (fabs(alpha) + 1.0 == 1.0)
        {
            cout << "\nQR分解失败！" << endl;
            exit(1);
        }

        u = sqrt(2.0*alpha*(alpha - A[k][k]));
        if ((u + 1.0) != 1.0)
        {
            A[k][k] = (A[k][k] - alpha) / u;
            for (i = k + 1; i <= m - 1; i++) A[i][k] = A[i][k] / u;

            //以上就是H矩阵的求得，实际上程序并没有设置任何数据结构来存储H矩
            //阵，而是直接将u向量的元素赋值给原A矩阵的原列向量相应的位置，这样做
            //这样做是为了计算左乘矩阵Q和A
            for (j = 0; j <= m - 1; j++)
            {
                t = 0.0;
                for (jj = k; jj <= m - 1; jj++)
                    t = t + A[jj][k] * Q[jj][j];
                for (i = k; i <= m - 1; i++)
                    Q[i][j] = Q[i][j] - 2.0*t*A[i][k];
            }
//左乘矩阵Q，循环结束后得到一个矩阵，再将这个矩阵转置一下就得到QR分解中的Q矩阵
//也就是正交矩阵

            for (j = k + 1; j <= n - 1; j++)
            {
                t = 0.0;
                for (jj = k; jj <= m - 1; jj++)
                    t = t + A[jj][k] * A[jj][j];
                for (i = k; i <= m - 1; i++)
                    A[i][j] = A[i][j] - 2.0*t*A[i][k];
            }
            //H矩阵左乘A矩阵，循环完成之后，其上三角部分的数据就是上三角矩阵R
            A[k][k] = alpha;
            for (i = k + 1; i <= m - 1; i++)  A[i][k] = 0.0;
        }
    }
    for (i = 0; i <= m - 2; i++)
        for (j = i + 1; j <= m - 1; j++)
        {
            t = Q[i][j]; Q[i][j] = Q[j][i]; Q[j][i] = t;
        }
    //QR分解完毕，然后在函数体里面直接将Q、R矩阵打印出来
    cout << "---Q矩阵---" << endl;
    for (i = 0; i<m; i++)
    {
        for (j = 0; j<n; j++)
        {
            cout << "    " << Q[i][j];
        }
        cout << endl;
    }
    cout << endl;
    cout << "---R矩阵---" << endl;
    for (i = 0; i<m; i++)
    {
        for (j = 0; j<n; j++)
        {
            cout << "    " << A[i][j];
        }
        cout << endl;
    }
}

int main()//主函数的调用
{

    double a = 0.333333;
    cout << "a:" << a << endl;
    int m, n;
//    cout << "输入矩阵的行数m，列数n" << endl;
//    cin >> m >> n;
    m = 4;
    n = 4;
    double** A = new double* [m];
    for (int i = 0; i < m; i++)A[i] = new double[n];
//    A = {{1,1,3,5},{1,1,3,5},{1/3,1/3,1,2},{1/5,1/5,1/2,1}};
    double B[16] = {1, 1, 3, 5, 1, 1, 3, 5, 0.333333, 0.333333, 1, 2, 0.2, 0.2, 0.5, 1};
    int cnt = 0;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            A[i][j] = B[cnt];
            cnt ++;
        }
    for (int i = 0; i < m; i++) {
        cout << endl;
        for (int j = 0; j < n; j++) {
            cout << A[i][j] << endl;
        }
    }
//    cout << "输入矩阵A的每一个元素" << endl;
//    for (int i = 0; i < m; i++)
//        for (int j = 0; j < n; j++)
//            cin >> A[i][j];
    Maqr(A, m, n);

//    system("pause");
    return 0;
}

