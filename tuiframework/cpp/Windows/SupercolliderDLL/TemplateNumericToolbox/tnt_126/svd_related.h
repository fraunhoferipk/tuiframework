/*
 * svd_related.h
 *
 *  Created on: 2011.03.31.
 *      Author: Adam
 */

#ifndef SVD_RELATED_H_
#define SVD_RELATED_H_

#include <cmath>
//#include "tnt_array2d_utils.h"
#include "../jama125/jama_svd.h"

namespace TNT
{

template <class T>
void performSvd(const TNT::Array2D<T>& A, TNT::Array2D<T>& U, TNT::Array2D<T>& S, TNT::Array2D<T>& V, bool noCutoff = false){

	double tol = 0.00001;

	int m = A.dim1();
	int n = A.dim2(); // JAMA::SVD works only if m >= n

	//std::cout << "m and n are " << m << ", " << n << std::endl;

	if(m >= n){
		JAMA::SVD<T> interm_svd = JAMA::SVD<T>(A);
		interm_svd.getU(U);
		interm_svd.getS(S);
		interm_svd.getV(V);

	}else{

		TNT::Array2D<T> datacopy = TNT::Array2D<T>();
		//std::cout << "about to transpose" << std::endl;
		TNT::transpose(A, datacopy);

		JAMA::SVD<T> interm_svd = JAMA::SVD<T>(datacopy);
		interm_svd.getV(U);
		interm_svd.getS(S);
		interm_svd.getU(V);


	}

	if(!noCutoff){

		int numSingValues = 0;
		for(int i = 0; i < S.dim1(); i++){
			if(S[i][i] > tol)
				numSingValues++;
		}


		if((U.dim1() < U.dim2()) || (numSingValues < U.dim2())){

			//std::cout << "doing svd economisation" << std::endl;
			//std::cout << "sizes of S and U are " << S.dim1() << " x " << S.dim2() << ", " << U.dim1() << " x " << U.dim2() << std::endl;

			TNT::Array2D<T> tmpS(S);
			S = TNT::Array2D<T>(numSingValues, numSingValues, 0.0);

			for(int j = 0; j < numSingValues; j++){

				S[j][j] = tmpS[j][j];
			}

			TNT::Array2D<T> tmpU(U);

			int finalColNum = (numSingValues < tmpU.dim2()) ? numSingValues : tmpU.dim2();

			U = TNT::Array2D<T>(tmpU.dim1(), finalColNum, 0.0);

			for(int j = 0; j < tmpU.dim1(); j++)
				for(int k = 0; k < finalColNum; k++)
					U[j][k] = tmpU[j][k];
		}
	}
}

template <class T>
void performSvd(const TNT::Array2D<T>& A, TNT::Array2D<T>& U, TNT::Array2D<T>& S, TNT::Array2D<T>& V, int numSingVals){

	double tol = 0.00001;

	performSvd(A, U, S, V);

	int numSingValues = S.dim1();

	if(numSingValues > numSingVals){

		//std::cout << "doing svd economisation" << std::endl;

		TNT::Array2D<T> tmpS(S);
		S = TNT::Array2D<T>(numSingVals, numSingVals, 0.0);

		for(int j = 0; j < numSingVals; j++){

			S[j][j] = tmpS[j][j];
		}


		TNT::Array2D<T> tmpU(U);

		int finalColNum = (numSingVals < tmpU.dim2()) ? numSingVals : tmpU.dim2();

		U = TNT::Array2D<T>(tmpU.dim1(), finalColNum, 0.0);

		for(int j = 0; j < tmpU.dim1(); j++)
			for(int k = 0; k < finalColNum; k++)
				U[j][k] = tmpU[j][k];
	}

}

template <class T>
void pseudoInverse(TNT::Array2D<T>& A, TNT::Array2D<T>& resu){

	TNT::Array2D<T> U,S,V,aux;

	int dim1 = A.dim1();
	int dim2 = A.dim2();
	std::vector<int> sz;
	sz.push_back(dim1); sz.push_back(dim2);
	//HosvdTensor X = HosvdTensor(sz);
	//X.resizeWithNewData(A, sz, 0);
	//X.performSvd(U,S,V, true);
	performSvd(A,U,S,V,true);

	TNT::Array2D<T> newS(S.dim2(), S.dim1(), 0.0);
	TNT::Array2D<T> newU(U.dim2(), U.dim1(), 0.0);

	TNT::transpose(U, newU);

	for(int i=0; i<S.dim2(); i++){
		for(int j=0; j<S.dim1(); j++){
			newS[i][j] = S[j][i];
			if((i==j) && abs(S[j][i]) > 0.0000001)
				newS[i][j] = 1.0 / newS[i][j];

		}
	}

	//std::cout << "size of U, S and V are: " << U.dim1() << " x " << U.dim2() << ", " << S.dim1() << " x " << S.dim2() << ", " << V.dim1() << " x " << V.dim2() << ", " << std::endl;
	//std::cout << "size of newU, newS and V are: " << newU.dim1() << " x " << newU.dim2() << ", " << newS.dim1() << " x " << newS.dim2() << ", " << V.dim1() << " x " << V.dim2() << ", " << std::endl;

	aux = TNT::matmult(V, newS);
	resu = TNT::matmult(aux, newU);


}


}


#endif /* SVD_RELATED_H_ */
