#include <iostream>
#include <math.h>

using namespace std;

/*************************************************
Function:eigen
Description:计算矩阵的特征值和特征向量

Input:输入矩阵、矩阵维数
Return:矩阵的特征值和特征向量
*************************************************/
double* eigen(double *p,int n){    //使用和法求矩阵特征根和特征向量
    double *A1,*A2,*sum1,*sum2,*W;
    A1=new double[n*n];
    A2=new double[n];
    sum1=new double[n];
    sum2=new double[n];
    W=new double[n+1];
    double sum3,sum4,lambda;
    sum3=sum4=0.0;
    int i,j;

    /*列向归一化*/
    for(i=0;i<n;i++){
        sum1[i]=0.0;
        for(j=0;j<n;j++)
            sum1[i]+=p[j*n+i];
        for(j=0;j<n;j++)
            A1[j*n+i]=p[j*n+i]/sum1[i];
    }

    /*按行求和*/
    for(i=0;i<n;i++){
        sum2[i]=0.0;
        for(j=0;j<n;j++)
            sum2[i]+=A1[i*n+j];
    }

    /*再次归一化，近似特征向量*/
    for(i=0;i<n;i++)
        sum3=sum3+sum2[i];
    for(i=0;i<n;i++)
        W[i]=sum2[i]/sum3;

    /*求特征值*/
    for(i=0;i<n;i++){
        A2[i]=0.0;
        for(j=0;j<n;j++)
            A2[i]+=p[i*n+j]*W[j];
    }

    /*归一化*/
    for(i=0;i<n;i++)
        sum4+=A2[i]/W[i];
    lambda=sum4/n;
    W[n]=lambda;

    return W;
}

/*************************************************
Function:AHP
Description:计算各指标的权重

Input:
    param@p：输入矩阵
    param@n：输入矩阵的维数
    param@RI：n与RI对应表

Return:
    praram@indicator_weights：各指标的权重值
*************************************************/
double* AHP(int n,double *p,double *RI) {
    int m,i,j;
    double CI,CR,*q,*A;

    /*求判断矩阵的特征根及特征向量*/
    q=eigen(p,n);    //求准则层判断矩阵的特征根及特征向量
    A=q;

    /*一致性检验*/
    CI=(q[n]-n)/(n-1);
    CR=CI/RI[n-1];// 为什么对应的是RI[n-1]呢
    if(CR<0.1){
        cout.setf(ios::fixed,ios::floatfield);
        cout.precision(4);
        cout<<"判断矩阵CR="<<CR<<','<<"通过一致性检验！"<<endl;
    }
    else{
        cout.setf(ios::fixed,ios::floatfield);
        cout.precision(4);
        cout<<"判断矩阵CR="<<CR<<','<<"未通过一致性检验！"<<endl;
    }

    /*获取指标权重*/
    double *indicator_weights = new double[n];
    cout<<"各指标的权重为："<<endl;
    for(i=0;i<n;i++){
        cout.setf(ios::left,ios::adjustfield);    //设置输出为左对齐
        cout.setf(ios::fixed,ios::floatfield);    //设置以定点数格式输出
        cout.precision(4);  //设置精度
        // cout << A[i] << endl;
        indicator_weights[i] = A[i];
    }

    /*返回各指标的权重值*/
    return indicator_weights;
}


