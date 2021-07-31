#ifndef MY_EIGN_H_
#define MY_EIGN_H_

class Eign {
public:
    Eign();
    ~Eign();
    /*雅格比(Jacobi)法--实对称矩阵*/
    bool getEign(double * pMatrix,int nDim, double *pdblVects, double *pdbEigenValues, double dbEps,int nJt);

    /*Matrix_Hessenberg*/
    void Matrix_Hessenberg(double *A1,int n,double *ret);
    bool Matrix_EigenValue(double *K1,int n,int LoopNumber,double Error1,double *Ret);
};
#endif