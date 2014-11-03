#ifndef AUXILIARY_H
#define AUXILIARY_H
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <iterator>
#include <typeinfo>

// The almighty Auxiliary class.
// Created mostly to avoid boost dependancies on the sampler
// Oh yes and the name Auxiliary is uber cool, so there's that.
// The sampler uses vectors and all vector to ublas matrix transformations are done within the class
// Name it and there probably is a utility for it in here
// Matrixes, vector conversions, and the boost library is loaded here
namespace ublas = boost::numeric::ublas;

template<typename T>
class Auxiliary
{
    public:
        //Multiply two row vectors
        std::vector< std::vector<T> > product(std::vector<T> &A , std::vector< std::vector<T> > &B);
        //Multiply matrix with scalar
        std::vector< std::vector<T> > productScalar(std::vector< std::vector<T> > &A , double scalar);
        //Subtract a vector with a scalar
        std::vector<T> matrixSubtraction(std::vector<T> &A , std::vector<T> &B);
        //Subtract a matrix to a scalar
        std::vector< std::vector<T> > matrixSubtraction(std::vector< std::vector<T> > &A , std::vector< std::vector<T> > &B);
        //Add a matrix to a matrix
        std::vector< std::vector<T> > matrixAddition(std::vector< std::vector<T> > &A , std::vector< std::vector<T> > &B);
        //Add a vector to a vector
        std::vector<T> matrixAddition(std::vector<T>  &A , std::vector<T> &B);
        //Multiply a vector with a scalar
        std::vector<T> productScalar(std::vector<T> &A , double scalar);
        //std::vector to ublas::matrix convertor
        boost::numeric::ublas::matrix<T> vectorToUblasMatrix(std::vector<T> &A);
        //ublas::matrix convertor to std::vector
        std::vector<T> ublasMatrixToVector(boost::numeric::ublas::matrix<T> &A);
        //ublas::matrix convertor to multidimensional vector
        std::vector< std::vector<T> > ublasMatrixToMatrix(boost::numeric::ublas::matrix<T> &A);
        //multidim vec to ublas::matrix
        boost::numeric::ublas::matrix<T> matrixToUblasMatrix(std::vector< std::vector<T> > &A);
        //vector transpose
        std::vector< std::vector<T> > transpose(std::vector<T> &A);
        //determinant of matrix
        double determinant(std::vector< std::vector<T> > B);
        //inverse of a matrix
        bool inverse(const ublas::matrix<T>& input, ublas::matrix<T>& inverse);
        //first part of marginal
        double getFirstPart(std::vector<double> mu, std::vector<double> z, std::vector< std::vector<double> > covariance);
    protected:
    private:
};

//multiply two row vectors, output a matrix
template <typename T>
std::vector< std::vector<T> > Auxiliary<T>::product(std::vector<T> &A , std::vector< std::vector<T> > &B){
    boost::numeric::ublas::matrix<T> mat  = vectorToUblasMatrix(A);
    boost::numeric::ublas::matrix<T> mat2 = matrixToUblasMatrix(B);
    boost::numeric::ublas::matrix<T> tempMat = boost::numeric::ublas::prod(mat, mat2);
    return ublasMatrixToMatrix(tempMat);
}

//multiply two row vectors, output a matrix
template <typename T>
std::vector< std::vector<T> > Auxiliary<T>::transpose(std::vector<T> &A){
    boost::numeric::ublas::matrix<T> mat  = vectorToUblasMatrix(A);
    boost::numeric::ublas::matrix<T> mat2 = boost::numeric::ublas::trans(mat);
    return ublasMatrixToMatrix(mat2);
}

//subtract two row vectors
template <typename T>
std::vector<T> Auxiliary<T>::matrixSubtraction(std::vector<T> &A , std::vector<T> &B){
    boost::numeric::ublas::matrix<T> mat  = vectorToUblasMatrix(A);
    boost::numeric::ublas::matrix<T> mat2 = vectorToUblasMatrix(B);
    boost::numeric::ublas::matrix<T> tempMat = mat-mat2;
    return ublasMatrixToVector(tempMat);
}

//subtract two row vectors
template <typename T>
std::vector< std::vector<T> > Auxiliary<T>::matrixSubtraction(std::vector< std::vector<T> > &A , std::vector< std::vector<T> > &B){
    boost::numeric::ublas::matrix<T> mat  = matrixToUblasMatrix(A);
    boost::numeric::ublas::matrix<T> mat2 = matrixToUblasMatrix(B);
    boost::numeric::ublas::matrix<T> tempMat = mat-mat2;
    return ublasMatrixToMatrix(tempMat);
}

//Addtwo row vectors
template <typename T>
std::vector< std::vector<T> > Auxiliary<T>::matrixAddition(std::vector< std::vector<T> > &A , std::vector< std::vector<T> > &B){
    boost::numeric::ublas::matrix<T> mat  = matrixToUblasMatrix(A);
    boost::numeric::ublas::matrix<T> mat2 = matrixToUblasMatrix(B);
    boost::numeric::ublas::matrix<T> tempMat = mat+mat2;
    return ublasMatrixToMatrix(tempMat);
}

//Addtwo row vectors
template <typename T>
 std::vector<T> Auxiliary<T>::matrixAddition(std::vector<T>  &A , std::vector<T> &B){
    boost::numeric::ublas::matrix<T> mat  = vectorToUblasMatrix(A);
    boost::numeric::ublas::matrix<T> mat2 = vectorToUblasMatrix(B);
    boost::numeric::ublas::matrix<T> tempMat = mat+mat2;
    return ublasMatrixToVector(tempMat);
}

//Returns the product of a row vector with scalar
template<typename T>
std::vector<T> Auxiliary<T>::productScalar(std::vector<T> &A , double scalar){
    boost::numeric::ublas::matrix<T> mat = vectorToUblasMatrix(A)*scalar;
    return ublasMatrixToVector(mat);
}

//Returns the product of a matrix with scalar
template<typename T>
std::vector< std::vector<T> > Auxiliary<T>::productScalar(std::vector< std::vector<T> > &A , double scalar){
    boost::numeric::ublas::matrix<T> mat = matrixToUblasMatrix(A)*scalar;
    return ublasMatrixToMatrix(mat);
}

//Self explanatory
template <typename T>
boost::numeric::ublas::matrix<T> Auxiliary<T>::vectorToUblasMatrix(std::vector<T> &A){
    typename vector<T>::iterator it; //Explicitly define the iterator
    boost::numeric::ublas::matrix<T> mat(A.size(),1);
    int counter = 0;
    for(it=A.begin(); it!=A.end(); it++ ){
        mat(counter,0) = *it;
        counter++;
    }
    return mat;
}
//Self explanatory
template<typename T>
std::vector<T> Auxiliary<T>::ublasMatrixToVector(boost::numeric::ublas::matrix<T> &A){
    std::vector<T> vectie;
    for(int i=0;i<A.size1();i++)
        vectie.push_back(A(i,0));
    return vectie;
}

//Self explanatory
template<typename T>
std::vector< std::vector<T> > Auxiliary<T>::ublasMatrixToMatrix(boost::numeric::ublas::matrix<T> &A){
    std::vector< std::vector<T> > vectie;
    for(int i=0;i<A.size1();i++){
        std::vector<T> temp;
        for(int j=0;j<A.size2();j++)
            temp.push_back(A(i,j));
        vectie.push_back(temp);
    }
    return vectie;
}
//Self explanatory
template <typename T>
boost::numeric::ublas::matrix<T> Auxiliary<T>::matrixToUblasMatrix(std::vector< std::vector<T> > &A){
    //typename vector<T>::iterator it; //Explicitly define the iterator
    int externalSize  = A.size();
    int internalSize  = A[0].size();
    boost::numeric::ublas::matrix<T> mat(externalSize, internalSize);
    for(int i=0;i<externalSize;i++)
        for(int j=0;j<internalSize;j++)
            mat(i,j) = A[i][j];

    return mat;
}
// Calculate the determinant of a square matrix using its LU decomposition.
// Relevant links http://en.wikipedia.org/wiki/LU_decomposition
template <typename T>
double Auxiliary<T>::determinant(std::vector< std::vector<T> > B){
    boost::numeric::ublas::matrix<T> A = matrixToUblasMatrix(B);
    //Calculate determinant of a square matrix
    assert(A.size1() == A.size2() && " Matrix dimensions must be equal");
    //the size of the pivots
    boost::numeric::ublas::permutation_matrix<std::size_t> pivots(A.size1());

    int is_singular = boost::numeric::ublas::lu_factorize(A, pivots);

    if(is_singular) return 0.0;

    double det = 1.0;
    const std::size_t sz = pivots.size();
    for(std::size_t i=0;i<sz;i++){
        if(pivots(i)!=i)
            det*= -1.0;
        det*= A(i,i);
    }
    return det;
}
// Calculate the inverse of a square matrix using its LU decomposition.
// Relevant links http://en.wikipedia.org/wiki/LU_decomposition <3
 template<typename T>
 bool Auxiliary<T>::inverse(const ublas::matrix<T>& input, ublas::matrix<T>& inverse) {
 	using namespace boost::numeric::ublas;
 	typedef permutation_matrix<std::size_t> pmatrix;
 	// create a working copy of the input
 	matrix<T> A(input);
 	// create a permutation matrix for the LU-factorization
 	pmatrix pm(A.size1());

 	// perform LU-factorization
 	int res = lu_factorize(A,pm);
        if( res != 0 ) return false;

 	// create identity matrix of "inverse"
 	inverse.assign(ublas::identity_matrix<T>(A.size1()));

 	// backsubstitute to get the inverse
 	lu_substitute(A, pm, inverse);
 	return true;
 }

 template<typename T>
 double Auxiliary<T>::getFirstPart(std::vector<double> mu, std::vector<double> z, std::vector< std::vector<double> > covariance){
     using namespace boost::numeric::ublas;
     //convert to ublas matrixes so that we can work with boost operatios
     // TODO Ugly conversions, need to think alternative implementation
     ublas::matrix<T> mean      = vectorToUblasMatrix(mu);
     ublas::matrix<T> obs       = vectorToUblasMatrix(z);
     ublas::matrix<T> cov       = matrixToUblasMatrix(covariance);
     ublas::matrix<T> meanTrans = ublas::trans(mean-obs);
     ublas::matrix<T> inverseMat(cov.size1(), cov.size2());
     this->inverse(cov, inverseMat);


     meanTrans = ublas::prod(meanTrans,inverseMat);
     mean = mean-obs;

     meanTrans = ublas::prod(meanTrans, mean);
     std::vector<double> returnvalue = ublasMatrixToVector(meanTrans);

     return returnvalue[0];
 }


#endif // AUXILIARY_H
