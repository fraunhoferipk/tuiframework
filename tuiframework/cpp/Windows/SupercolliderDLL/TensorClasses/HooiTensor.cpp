/*
 * HooiTensor.cpp
 *
 *  Created on: 2011.03.07.
 *      Author: Adam
 */

#include "HooiTensor.h"


namespace SdmTensor{

/*!
 * Creates an empty Tensor and sets isHooied, hasBeenHooied and isCompensated to false
 */
HooiTensor::HooiTensor(): HosvdTensor(){
	isHooied = false;
	hasBeenHooied = false;
	isCompensated = false;
}

/*!
 * Creates a Tensor of specified size and fills it with random values between 0 and 1 if requested.
 * Sets isHooied, hasBeenHooied and isCompensated to false.
 */
HooiTensor::HooiTensor(std::vector<int> size, bool isRandom): HosvdTensor(size, isRandom){

	m_hooiCoreTensor = Tensor();
	m_compensatedCoreTensor = Tensor();
	isHooied = false;
	hasBeenHooied = false;
	isCompensated = false;
}


bool HooiTensor::getIsHooied(){
        return isHooied;
}


HooiTensor::~HooiTensor(){
}

/*!
 * Performs HOOI of Tensor.  Results in isHooied and hasBeenHooied being set to true
 */
void HooiTensor::performHooi(int dimension, int newrank, double compensationScale, bool doCompensate){

	
	unfoldN(0);
	vector < TNT::Array2D<double> > Us;
	vector < TNT::Array2D<double> > Ust;

	for(int i = 0; i < m_numDimensions; i++){

		TNT::Array2D<double> U, S, V;
		unfoldN(i);
		TNT::performSvd(m_data, U, S, V);
		Us.push_back(TNT::Array2D<double>());
		TNT::transpose(U, Us[i]);

	}

	double diffsize = 0;
	double diffsize_difference = 10.0;
	double previous_diffsize = diffsize_difference;

	while(diffsize_difference > 0.001){

		for(int i = 0; i < m_numDimensions; i++){

			tprod(Us, i, m_hooiCoreTensor);

			TNT::Array2D<double> newU;
			TNT::Array2D<double> newS;
			TNT::Array2D<double> newV;
			unfoldN(i);

			if(i == dimension){
				TNT::performSvd(m_data, newU, newS, newV, newrank);
			}
			else{
				TNT::performSvd(m_data, newU, newS, newV);
			}
			TNT::transpose(newU, Us[i]);

		}

		for(int i = 0; i < Us.size(); i++){
			Ust.push_back(Array2D<double>());
			TNT::transpose(Us[i], Ust[i]);
		}

		Tensor hooiCoreTensor_tmp;
		tprod(Us, hooiCoreTensor_tmp);
		hooiCoreTensor_tmp.tprod(Ust, m_hooiCoreTensor);
		unfoldN(0);

		Tensor diffTensor;
		subtract(m_hooiCoreTensor, diffTensor);

		diffsize = diffTensor.norm();

		previous_diffsize = diffsize;

		diffsize_difference = abs(previous_diffsize - diffsize);


	}

	tprod(Us, m_hooiCoreTensor);
	m_hooiWeightingFunctions.clear();
	for(int i = 0; i < m_numDimensions; i++){
		m_hooiWeightingFunctions.push_back(Ust[i]);
	}

	isHooied = true;
	hasBeenHooied = true;
	remultiplyHooiTensor();

	
	if (doCompensate) {
		std::vector<double> dummyvec;
		compensateHooiErrors(dummyvec, true);
	}
	
}

/*!
 * Remultiplies HOOI core tensor or compensated core tensor with appropriate weights depending on values
 * of isHooied and isCompensated. Necessary whenever weighting functions are changed
 */
void HooiTensor::remultiplyHooiTensor(){

	Tensor resu = Tensor();

	if(!isCompensated){

		m_hooiCoreTensor.tprod(m_hooiWeightingFunctions, resu);
		resu.getData(m_data);

	}else{

		m_compensatedCoreTensor.tprod(m_compensatedWeightingFunctions, resu);
		resu.getData(m_data);
	}

	for(int i=0; i<m_data.dim1(); i++){
		for(int j=0; j<m_data.dim2(); j++){

			if(m_data[i][j] < 0.0000000001 && m_data[i][j] > -0.0000000001)
				m_data[i][j] = 0;
		}
	}

	m_unfoldedModeDimension = resu.getUnfoldedDimension();
}

/*!
* Compensates for rank reduction errors by augmenting core tensor along its last dimension by tensor K,
* and the whole resulting core tensor along its first dimension by tensor D.  The first and last weighting
* matrices are also augmented.  By choosing the free parameters in K, the slope of the hyperline controlled
* by the first column of the first weighting matrix can be manipulated.
*/
void HooiTensor::compensateHooiErrors(std::vector<double>& K_values, bool randKvalues){

	prepareCompensatedWeightingFunctions();

	if(randKvalues){
		std::vector<double> dummyvec;
		prepareCompensatedCoreTensor(dummyvec, true);
	}else{
		prepareCompensatedCoreTensor(K_values, randKvalues);

	}

	Tensor tempTensor1, tempTensor2;
	Tensor compensatedCoreTensor_p01;
	m_compensatedCoreTensor.getSubtensor(0,0,1,compensatedCoreTensor_p01);

	//either do this
	compensatedCoreTensor_p01.tprod(m_compensatedWeightingFunctions, 0, tempTensor1);
	tempTensor1.tprodN(m_hooiWeightingFunctions[0], 0, tempTensor2);

	//or you could do this, since the bottom of m_compensatedWeightingFunctions is all 0...
	//m_compensatedCoreTensor.tprod(m_compensatedWeightingFunctions, tempTensor2);


	remultiplyHosvdTensor();
	subtract(tempTensor2, tempTensor1);
	remultiplyHooiTensor();

	remultiplyHooiTensor();

	Array2D<double> Rinv, tmppinv;
	TNT::pseudoInverse(m_R, Rinv);

	tempTensor1.tprodN(Rinv, 0, tempTensor2);

	TNT::pseudoInverse(m_compensatedWeightingFunctions[m_numDimensions-1], tmppinv);
	tempTensor2.tprodN(tmppinv, m_numDimensions-1, tempTensor1);

	Tensor* prevTen;
	Tensor* curTen;
	Tensor* auxTen;
	prevTen = &tempTensor1;
	curTen = &tempTensor2;

	for(int i=1; i<m_numDimensions-1; i++){
		TNT::pseudoInverse(m_compensatedWeightingFunctions[i], tmppinv);
		(*prevTen).tprodN(tmppinv, i, *curTen);
		auxTen = prevTen;
		prevTen = curTen;
		curTen = auxTen;
	}

	compensatedCoreTensor_p01.concat(*prevTen, 0, m_compensatedCoreTensor);

	Tensor tmp2 = Tensor();
	remultiplyHosvdTensor();
	copy(tmp2);

	isCompensated = true;
	remultiplyHooiTensor();

	assert(*this == tmp2);

}


/*!
 * Augments appropriate weighting matrices for compensation
 */
void HooiTensor::prepareCompensatedWeightingFunctions(){

	//copy weighting functions into m_compensatedWeightingFunctions
	m_compensatedWeightingFunctions.clear();

	int dim1, dim2;
	dim1 = m_hooiWeightingFunctions[0].dim1();
	dim2 = m_hooiWeightingFunctions[0].dim2();

	m_compensatedWeightingFunctions.push_back(Array2D<double>(dim1, 1+dim1, 0.0));

	for(int j = 0; j<dim1; j++)
		for(int k=0; k<dim2; k++)
			(m_compensatedWeightingFunctions[0])[j][k] = (m_hooiWeightingFunctions[0])[j][k];


	int rdim1 = m_compensatedWeightingFunctions[0].dim1();
	int rdim2 = m_compensatedWeightingFunctions[0].dim2() - 1;

	m_R = Array2D<double>(dim1, dim1, 0.0);
	srand(time(NULL));
	for(int i=0; i<dim1; i++){
		for(int j=0; j<dim1; j++){
			m_R[i][j] = (rand() % 101) / 100.0;
			(m_compensatedWeightingFunctions[0])[i][j+1] = m_R[i][j];
		}
	}//let's hope that R is full-rank


	int newdim2;
	for(int i=1; i<m_numDimensions-1; i++){
		dim1 = m_hooiWeightingFunctions[i].dim1();
		dim2 = m_hooiWeightingFunctions[i].dim2();
		newdim2 = dim2;
		if(dim1 >= dim2)
			newdim2 = dim1 + 1;

		m_compensatedWeightingFunctions.push_back(Array2D<double>(dim1, newdim2, 0.0));
		srand(time(NULL));
		for(int j=0; j<dim1; j++){
			for(int k=0; k<dim2; k++){
				(m_compensatedWeightingFunctions[i])[j][k] = (m_hooiWeightingFunctions[i])[j][k];
			}
			for(int k=dim2; k<newdim2; k++){
				(m_compensatedWeightingFunctions[i])[j][k] = (rand() % 101) / 100.0;
			}
		}
	}


	dim1 = m_hooiWeightingFunctions[m_numDimensions-1].dim1();
	dim2 = m_hooiWeightingFunctions[m_numDimensions-1].dim2();

	m_compensatedWeightingFunctions.push_back(Array2D<double>(dim1, dim1+dim2, 0.0));

	for(int j = 0; j<dim1; j++)
		for(int k=0; k<dim2; k++)
			(m_compensatedWeightingFunctions[m_numDimensions-1])[j][k] = (m_hooiWeightingFunctions[m_numDimensions-1])[j][k];

	for(int j=0; j<dim1; j++){
		for(int k=dim2; k<dim1+dim2; k++){
			if(j+dim2==k)
				(m_compensatedWeightingFunctions[m_numDimensions-1])[j][k] = 1.0;
		}
	}

}

/*!
 * Augments HOOI core tensor so that it may be filled with K and D for error compensation
 * if randKvalues is true, random values are used in K, otherwise, K is filled using K_values
 */
void HooiTensor::prepareCompensatedCoreTensor(std::vector<double>& K_values, bool randKvalues){


	std::vector< std::vector<int> > leftoverSizes;
	std::vector<int> hooiCoreSize;

	//////////////////////////////////////////////////////////////
	////// PREPARE SIZE OF K AND AUGMENTATION SIZES //////////////
	//////////////////////////////////////////////////////////////

	m_coreTensor.getTensorSize(m_K_size);
	m_hooiCoreTensor.getTensorSize(hooiCoreSize);

	m_K_size[0] = 1;
	m_K_size[m_numDimensions-1] = m_hooiWeightingFunctions[m_numDimensions-1].dim1();

	for(int i=1; i<m_numDimensions-1; i++){
		m_K_size[i] = m_compensatedWeightingFunctions[i].dim2();
	}

	leftoverSizes.push_back(hooiCoreSize);
	for(int i=1; i<m_numDimensions-1; i++){

		leftoverSizes.push_back(std::vector<int>());
		leftoverSizes[i].push_back(1);
		for(int j=1; j<m_numDimensions; j++){
			if(j>i)
				leftoverSizes[i].push_back(leftoverSizes[i-1][j]);
			else{
				if(j<i)
					leftoverSizes[i].push_back(m_K_size[j]);
				else
					leftoverSizes[i].push_back(m_K_size[j] - hooiCoreSize[j]);
			}

		}

	}


	/////////////////////////////////////////////////
	//// PREPARE K //////////////////////////////////
	/////////////////////////////////////////////////

	Tensor K = Tensor(m_K_size);

	//fill K tensor
	K.unfoldN(0);
	int dim2 = 1;
	for(int j=1; j<m_numDimensions; j++)
		dim2 = dim2 * m_K_size[j];

	int coords[2] = {0, 0};

	if(randKvalues){
		for(int j=0; j<dim2; j++){
			coords[1] = j;
			K.setElement2D(coords, ((rand() % 101) / 100.0));
		}
	}else{
		std::vector<int> sz;
		K.getTensorSize(sz);

		int* fixDimensions = new int[sz.size()];
		int elaboratedDimension = m_numDimensions-1;
		for(int i=0; i<sz.size()-1; i++)
			fixDimensions[i] = 1;

		double* vals = new double[K_values.size()];
		for(int i=0; i<K_values.size(); i++)
			vals[i] = K_values[i];

		for(int i=0; i<sz[m_numDimensions-1]; i++){
			K.setElements(elaboratedDimension, fixDimensions, vals);
		}

		delete[] fixDimensions;
		delete[] vals;
	}

	/////////////////////////////////////////////////
	//// PREPARE D //////////////////////////////////
	/////////////////////////////////////////////////

	m_coreTensor.getTensorSize(m_D_size);
	m_D_size[0] = m_hooiWeightingFunctions[0].dim1();
	m_D_size[m_numDimensions-1] = m_hooiWeightingFunctions[m_numDimensions-1].dim1() + m_hooiWeightingFunctions[m_numDimensions-1].dim2();

	for(int i=1; i<m_numDimensions-1; i++)
		m_D_size[i] = m_K_size[i];

	Tensor D = Tensor(m_D_size);


	/////////////////////////////////////////////////
	//// AUGMENT CORE TENSOR ////////////////////////
	/////////////////////////////////////////////////



	Tensor tmpTensor1, tmpTensor2;

	m_hooiCoreTensor.copy(tmpTensor1);

	for(int i=1; i<m_numDimensions-1; i++){
		bool doAppendLeftover = true;
		for(int j=0; j<leftoverSizes[i].size(); j++){
			if(leftoverSizes[i][j] == 0)
				doAppendLeftover = false;
		}

		if(doAppendLeftover){
			Tensor ten = Tensor(leftoverSizes[i], true);
			tmpTensor1.concat(ten, i, tmpTensor2);
			tmpTensor2.copy(tmpTensor1);
		}
	}


	tmpTensor1.copy(m_compensatedCoreTensor);


	m_compensatedCoreTensor.concat(K, m_numDimensions-1, tmpTensor1);

	//tmpTensor1.printSize("S | K");
	//D.printSize("D");

	tmpTensor1.concat(D, 0, m_compensatedCoreTensor);

}
	
	
void HooiTensor::updateK(std::vector<double>& K_values){
	
	std::vector<int> compensatedCoreSize;
	m_compensatedCoreTensor.getTensorSize(compensatedCoreSize);	
	
	int numdims = compensatedCoreSize.size();
	int lastdimlen = compensatedCoreSize[numdims-1];
	int kvalslen = K_values.size();
	
	int* coordinates = new int[numdims];
	for (int step=0; step<numdims; step++) {
		coordinates[step] = 0;
	}
	
	//std::cout << "OK in HooiTensor.cpp before loop" << std::endl;
	
	for (int step=0; step < K_values.size(); step++) {
		coordinates[numdims-1] = lastdimlen - step - 1;
		m_compensatedCoreTensor.setElement(coordinates, K_values[kvalslen - step - 1]);
		//std::cout << "Loop " << step << " was OK" << std::endl;
	}
	
	delete coordinates;
	
}
	

/*!
 * prints compensated weighting functions
 */
void HooiTensor::printCompensatedWeightingFunctions(){

	for(int i=0; i<m_compensatedWeightingFunctions.size(); i++){

		std::cout << "compensated weighting function in dimension " << i << " is:" << std::endl;
		std::cout << m_compensatedWeightingFunctions[i] << std::endl << std::endl;

	}
}

/*!
 * prints HOOI weighting functions
 */
void HooiTensor::printHooiWeightingFunctions(){

	for(int i=0; i<m_hooiWeightingFunctions.size(); i++){

		std::cout << "hooi weighting function in dimension " << i << " is:" << std::endl;
		std::cout << m_hooiWeightingFunctions[i] << std::endl << std::endl;

	}

}

}//end namespace SdmTensor
