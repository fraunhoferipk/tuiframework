/*
 * HosvdTensor.cpp
 *
 *  Created on: 2011.02.14.
 *      Author: Adam
 */


#include "HosvdTensor.h"


namespace SdmTensor{

	/*!
	 * Creates an empty Tensor and sets isHosvded and hasBeenHosvded to false
	 */
HosvdTensor::HosvdTensor() : Tensor(){

	m_coreTensor = Tensor();
	isHosvded = false;
	hasBeenHosvded = false;

}

	/*!
	 * Creates a Tensor of specified size and fills it with random values between 0 and 1 if requested.  The
	 * tensor is stored in m_coreTensor, but hosvd is not performed.. hence isHosvded and hasBeenHosvded are both
	 * false
	 */
HosvdTensor::HosvdTensor(std::vector<int> size, bool isRandom) : Tensor(size, isRandom)
{
	m_coreTensor = Tensor();
	isHosvded = false;
	hasBeenHosvded = false;
	//performHosvd();
}

HosvdTensor::~HosvdTensor(){
}

	/*!
	 * Performs HOSVD of Tensor.  Results in isHosvded and hasBeenHosvded being set to true.  The function may or may
	 not "reset" the tensor before performing hosvd, depending on the input parameter (resetting means to re-multiply 
	 the core tensor with the vector of weights -- this is necessary if the weights were changed)
	 */
void HosvdTensor::performHosvd(bool reset){

	/*
	 * if Hosvd has already been performed, then remultiply first.  This way
	 * the call is guaranteed to be idempotent
	 */
	if(isHosvded && reset){
		remultiplyHosvdTensor();
	}

	m_weightingFunctions.clear();

	vector< TNT::Array2D<double> > Uts;
	TNT::Array2D<double> S;
	TNT::Array2D<double> V;


	for(int i = 0; i < m_numDimensions; i++)
	{
		unfoldN(i);

		m_weightingFunctions.push_back(TNT::Array2D<double>());
		Uts.push_back(TNT::Array2D<double>());

		TNT::performSvd(m_data, m_weightingFunctions[i], S, V);

		TNT::transpose(m_weightingFunctions[i], Uts[i]);

	}

	tprod(Uts, m_coreTensor);
	remultiplyHosvdTensor();
	isHosvded = true;
	hasBeenHosvded = true;

}


	/*!
	 * Remultiplies core tensor with weights.  Necessary whenever weighting functions are changed
	 */	
void HosvdTensor::remultiplyHosvdTensor(){

	Tensor resu = Tensor();
	m_coreTensor.tprod(m_weightingFunctions, resu);

	resu.getData(m_data);

	for(int i=0; i<m_data.dim1(); i++)
		for(int j=0; j<m_data.dim2(); j++)
			if(m_data[i][j] < 0.0000000001 && m_data[i][j] > -0.0000000001)
				m_data[i][j] = 0;

	m_unfoldedModeDimension = resu.getUnfoldedDimension();

}

bool HosvdTensor::getIsHosvded(){
        return isHosvded;
}

} // end namespace SdmTensor
