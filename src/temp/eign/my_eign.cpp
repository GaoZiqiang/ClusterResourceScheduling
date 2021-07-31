#include <iostream>
#include <vector>
#include <stdlib.h>

#include <math.h>
#include <map>
#include <string.h>

#include "my_eign.h"

using namespace std;

Eign::Eign() {
    cout << "创建了一个Eign对象!" << endl;
}


/**
* @brief 求实对称矩阵的特征值及特征向量的雅克比法
* 利用雅格比(Jacobi)方法求实对称矩阵的全部特征值及特征向量
* @param pMatrix				长度为n*n的数组，存放实对称矩阵
* @param nDim					矩阵的阶数
* @param pdblVects				长度为n*n的数组，返回特征向量(按列存储)
* @param dbEps					精度要求
* @param nJt					整型变量，控制最大迭代次数
* @param pdbEigenValues			特征值数组
* @return
*/
bool Eign::getEign(double * pMatrix,int nDim, double *pdblVects, double *pdbEigenValues, double dbEps,int nJt) {
    for(int i = 0; i < nDim; i ++)
    {
        pdblVects[i*nDim+i] = 1.0f;
        for(int j = 0; j < nDim; j ++)
        {
            if(i != j)
                pdblVects[i*nDim+j]=0.0f;
        }
    }

    int nCount = 0;		//迭代次数
    while(1)
    {
        //在pMatrix的非对角线上找到最大元素
        double dbMax = pMatrix[1];
        int nRow = 0;
        int nCol = 1;
        for (int i = 0; i < nDim; i ++)			//行
        {
            for (int j = 0; j < nDim; j ++)		//列
            {
                double d = fabs(pMatrix[i*nDim+j]);

                if((i!=j) && (d> dbMax))
                {
                    dbMax = d;
                    nRow = i;
                    nCol = j;
                }
            }
        }

        if(dbMax < dbEps)     //精度符合要求
            break;

        if(nCount > nJt)       //迭代次数超过限制
            break;

        nCount++;

        double dbApp = pMatrix[nRow*nDim+nRow];
        double dbApq = pMatrix[nRow*nDim+nCol];
        double dbAqq = pMatrix[nCol*nDim+nCol];

        //计算旋转角度
        double dbAngle = 0.5*atan2(-2*dbApq,dbAqq-dbApp);
        double dbSinTheta = sin(dbAngle);
        double dbCosTheta = cos(dbAngle);
        double dbSin2Theta = sin(2*dbAngle);
        double dbCos2Theta = cos(2*dbAngle);

        pMatrix[nRow*nDim+nRow] = dbApp*dbCosTheta*dbCosTheta +
                                  dbAqq*dbSinTheta*dbSinTheta + 2*dbApq*dbCosTheta*dbSinTheta;
        pMatrix[nCol*nDim+nCol] = dbApp*dbSinTheta*dbSinTheta +
                                  dbAqq*dbCosTheta*dbCosTheta - 2*dbApq*dbCosTheta*dbSinTheta;
        pMatrix[nRow*nDim+nCol] = 0.5*(dbAqq-dbApp)*dbSin2Theta + dbApq*dbCos2Theta;
        pMatrix[nCol*nDim+nRow] = pMatrix[nRow*nDim+nCol];

        for(int i = 0; i < nDim; i ++)
        {
            if((i!=nCol) && (i!=nRow))
            {
                int u = i*nDim + nRow;	//p
                int w = i*nDim + nCol;	//q
                dbMax = pMatrix[u];
                pMatrix[u]= pMatrix[w]*dbSinTheta + dbMax*dbCosTheta;
                pMatrix[w]= pMatrix[w]*dbCosTheta - dbMax*dbSinTheta;
            }
        }

        for (int j = 0; j < nDim; j ++)
        {
            if((j!=nCol) && (j!=nRow))
            {
                int u = nRow*nDim + j;	//p
                int w = nCol*nDim + j;	//q
                dbMax = pMatrix[u];
                pMatrix[u]= pMatrix[w]*dbSinTheta + dbMax*dbCosTheta;
                pMatrix[w]= pMatrix[w]*dbCosTheta - dbMax*dbSinTheta;
            }
        }

        //计算特征向量
        for(int i = 0; i < nDim; i ++)
        {
            int u = i*nDim + nRow;		//p
            int w = i*nDim + nCol;		//q
            dbMax = pdblVects[u];
            /*pdblVects:特征向量*/
            pdblVects[u] = pdblVects[w]*dbSinTheta + dbMax*dbCosTheta;
            pdblVects[w] = pdblVects[w]*dbCosTheta - dbMax*dbSinTheta;
        }

    }

    //对特征值进行排序以及重新排列特征向量,特征值即pMatrix主对角线上的元素
//        std::map<double,int> mapEigen;
    map<double,int> mapEigen;
    for(int i = 0; i < nDim; i ++)
    {
        pdbEigenValues[i] = pMatrix[i*nDim+i];

        mapEigen.insert(make_pair( pdbEigenValues[i],i ) );
    }

    double *pdbTmpVec = new double[nDim*nDim];
//        std::map<double,int>::reverse_iterator iter = mapEigen.rbegin();
    map<double,int>::reverse_iterator iter = mapEigen.rbegin();
    for (int j = 0; iter != mapEigen.rend(),j < nDim; ++iter,++j)
    {
        for (int i = 0; i < nDim; i ++)
        {
            pdbTmpVec[i*nDim+j] = pdblVects[i*nDim + iter->second];
        }

        //特征值重新排列
        pdbEigenValues[j] = iter->first;
    }

    //设定正负号
    for(int i = 0; i < nDim; i ++)
    {
        double dSumVec = 0;
        for(int j = 0; j < nDim; j ++)
            dSumVec += pdbTmpVec[j * nDim + i];
        if(dSumVec<0)
        {
            for(int j = 0;j < nDim; j ++)
                pdbTmpVec[j * nDim + i] *= -1;
        }
    }

    memcpy(pdblVects,pdbTmpVec,sizeof(double)*nDim*nDim);
    delete []pdbTmpVec;

    return 1;
}

void Eign::Matrix_Hessenberg(double *A1,int n,double *ret) {

    int i,j,k,MaxNumber;

    double temp,*A;

    A=new double[n*n];

    for (i=0;i<n;i++)

    {

        k=i*n;

        for (j=0;j<n;j++)

        {

            A[k+j]=A1[k+j];

        }

    }

    for (k=1;k<n-1;k++)

    {

        i=k-1;

        MaxNumber=k;

        temp=abs(A[k*n+i]);

        for (j=k+1;j<n;j++)

        {

            if (abs(A[j*n+i])>temp)

            {

                temp=abs(A[j*n+i]);

                MaxNumber=j;

            }

        }

        ret[0]=A[MaxNumber*n+i];

        i=MaxNumber;

        if (ret[0]!=0)

        {

            if (i!=k)

            {

                for(j=k-1;j<n;j++)

                {

                    temp=A[i*n+j];

                    A[i*n+j]=A[k*n+j];

                    A[k*n+j]=temp;

                }

                for(j=0;j<n;j++)

                {

                    temp=A[j*n+i];

                    A[j*n+i]=A[j*n+k];

                    A[j*n+k]=temp;

                }

            }

            for (i=k+1;i<n;i++)

            {

                temp=A[i*n+k-1]/ret[0];

                A[i*n+k-1]=0;

                for (j=k;j<n;j++)

                {

                    A[i*n+j]-=temp*A[k*n+j];

                }

                for (j=0;j<n;j++)

                {

                    A[j*n+k]+=temp*A[j*n+i];

                }

            }

        }



    }

    for (i=0;i<n;i++)

    {

        k=i*n;

        for (j=0;j<n;j++)

        {

            ret[k+j]=A[k+j];

        }

    }

    delete []A;

}

bool Eign::Matrix_EigenValue(double *K1,int n,int LoopNumber,double Error1,double *Ret) {

    int i,j,k,t,m,Loop1;

    double b,c,d,g,xy,p,q,r,x,s,e,f,z,y,temp,*A;

    A=new double[n*n];

    Matrix_Hessenberg(K1,n,A);// 将方阵化为上三角Hessenberg矩阵

    m=n;

    Loop1=LoopNumber;

    while(m!=0)

    {
        t=m-1;

        while(t>0) {

            temp=abs(A[(t-1)*n+t-1]);

            temp+=abs(A[t*n+t]);
            /*控制精度？*/
            // 怎么控制？？？
            temp=temp*Error1;

            if (abs(A[t*n+t-1])>temp) {
                t--;
            }

            else {
                break;
            }

        }

        if (t==m-1)

        {

            Ret[(m-1)*2]=A[(m-1)*n+m-1];

            Ret[(m-1)*2+1]=0;

            m-=1;

            Loop1=LoopNumber;

        }

        else if(t==m-2)

        {

            b=-A[(m-1)*n+m-1]-A[(m-2)*n+m-2];

            c=A[(m-1)*n+m-1]*A[(m-2)*n+m-2]-A[(m-1)*n+m-2]*A[(m-2)*n+m-1];

            d=b*b-4*c;

            y=sqrt(abs(d));

            if (d>0)

            {

                xy=1;

                if (b<0)

                {

                    xy=-1;

                }

                Ret[(m-1)*2]=-(b+xy*y)/2;

                Ret[(m-1)*2+1]=0;

                Ret[(m-2)*2]=c/Ret[(m-1)*2];

                Ret[(m-2)*2+1]=0;

            }

            else

            {

                Ret[(m-1)*2]=-b/2;

                Ret[(m-2)*2]=-b/2;

                Ret[(m-1)*2+1]=y/2;

                Ret[(m-2)*2+1]=-y/2;

            }

            m-=2;

            Loop1=LoopNumber;

        }

        else

        {

            if (Loop1<1)

            {

                return false;

            }

            Loop1--;

            j=t+2;

            while (j<m)

            {

                A[j*n+j-2]=0;

                j++;

            }

            j=t+3;

            while (j<m)

            {

                A[j*n+j-3]=0;

                j++;

            }

            k=t;

            while (k<m-1)

            {

                if (k!=t)

                {

                    p=A[k*n+k-1];

                    q=A[(k+1)*n+k-1];

                    if (k!=m-2)

                    {

                        r=A[(k+2)*n+k-1];

                    }

                    else

                    {

                        r=0;

                    }

                }

                else

                {

                    b=A[(m-1)*n+m-1];

                    c=A[(m-2)*n+m-2];

                    x=b+c;

                    y=b*c-A[(m-2)*n+m-1]*A[(m-1)*n+m-2];

                    p=A[t*n+t]*(A[t*n+t]-x)+A[t*n+t+1]*A[(t+1)*n+t]+y;

                    q=A[(t+1)*n+t]*(A[t*n+t]+A[(t+1)*n+t+1]-x);

                    r=A[(t+1)*n+t]*A[(t+2)*n+t+1];

                }

                if (p!=0 || q!=0 || r!=0)

                {

                    if (p<0)

                    {

                        xy=-1;

                    }

                    else

                    {

                        xy=1;

                    }

                    s=xy*sqrt(p*p+q*q+r*r);

                    if (k!=t)

                    {

                        A[k*n+k-1]=-s;

                    }

                    e=-q/s;

                    f=-r/s;

                    x=-p/s;

                    y=-x-f*r/(p+s);

                    g=e*r/(p+s);

                    z=-x-e*q/(p+s);

                    for (j=k;j<m;j++)

                    {

                        b=A[k*n+j];

                        c=A[(k+1)*n+j];

                        p=x*b+e*c;

                        q=e*b+y*c;

                        r=f*b+g*c;

                        if (k!=m-2)

                        {

                            b=A[(k+2)*n+j];

                            p+=f*b;

                            q+=g*b;

                            r+=z*b;

                            A[(k+2)*n+j]=r;

                        }

                        A[(k+1)*n+j]=q;

                        A[k*n+j]=p;

                    }

                    j=k+3;

                    if (j>m-2)

                    {

                        j=m-1;

                    }

                    for (i=t;i<j+1;i++)

                    {

                        b=A[i*n+k];

                        c=A[i*n+k+1];

                        p=x*b+e*c;

                        q=e*b+y*c;

                        r=f*b+g*c;

                        if (k!=m-2)

                        {

                            b=A[i*n+k+2];

                            p+=f*b;

                            q+=g*b;

                            r+=z*b;

                            A[i*n+k+2]=r;

                        }

                        A[i*n+k+1]=q;

                        A[i*n+k]=p;

                    }

                }

                k++;

            }

        }

    }

    delete []A;

    return true;

}





int main() {
    Eign *eign = new Eign();

    double matrix[16] = {1, 1, 3, 5, 1, 1, 3, 5, 1 / 3, 1 / 3, 1, 2, 1 / 5, 1 / 5, 1 / 2, 1};

    double pdblVects[16];
    // double pdblVects[16] = {0.54710883, -0.56838358, 0.51063738, 0.51063738, 0.54710883, -0.46838358, 0.51063738, 0.51063738, 0.54710883, -0.56838358, 0.51063738, 0.51063738, 0.54710853, -0.56838358, 0.51063738, 0.51063738};
    double pdbEigenValues[4];
    // double pdbEigenValues[4] = {0, 0, 0, 0};
    double dbEps = 100.0;
    int nJt = 100;

    int n = 4;
    double Ret[16];
    double Error = 2.2204460492503131e-016;
    eign->Matrix_EigenValue(matrix,4,1000,Error,Ret);
    // cout << sizeof(Ret) / sizeof(Ret[0]) << endl;
    int Ret_length = sizeof(Ret) / sizeof(Ret[0]);
    // 遍历输出特征值
    for (int i = 0;i < Ret_length; i++)
        cout << Ret[i] << endl;




    //切记:C++中,数组变量名为数组中第一个元素的地址，即数组名本身就是一个指针
//    eign->getEign(matrix, 4, pdblVects, pdbEigenValues, dbEps, nJt);
//
//    int pdbEigenValues_length = sizeof(pdbEigenValues) / sizeof(pdbEigenValues[0]);
//    for (int i =0; i < pdbEigenValues_length; i++)
//        cout << pdbEigenValues[i] << endl;
//
//    int pdblVects_length = sizeof(pdblVects) / sizeof(pdblVects[0]);
//    for (int i = 0; i < pdblVects_length; i++)
//        cout << pdblVects[i] << endl;
//    cout << sizeof(pdbEigenValues)/sizeof(pdbEigenValues[0]) << endl;
//    cout << "pdblVects[0]:" << pdblVects[0] << endl;
};