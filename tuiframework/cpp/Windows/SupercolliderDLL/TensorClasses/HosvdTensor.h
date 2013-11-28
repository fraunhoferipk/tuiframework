/*
 * HosvdTensor.hpp
 *
 *  Created on: 2011.02.14.
 *      Author: Adam
 */

#ifndef HOSVDTENSOR_HPP_
#define HOSVDTENSOR_HPP_

#include <iostream>
#include <vector>
#include <cmath>

#include "Tensor.h"
#include "../TemplateNumericToolbox/tnt_126/tnt.h"


namespace SdmTensor{

class HosvdTensor : public Tensor{

protected:

	Tensor m_coreTensor; //!< core tensor of N dimensions	
	vector< TNT::Array2D<double> > m_weightingFunctions;	//!< contains weighting functions in all N dimensions
	bool isHosvded;	//!< true if performHosvd has been called at least once
	bool hasBeenHosvded; //!< true if Hosvd was computed at some point


public:

	/*!
	 * Creates an empty Tensor and sets isHosvded and hasBeenHosvded to false
	 */
	HosvdTensor();
	
	/*!
	 * Creates a Tensor of specified size and fills it with random values between 0 and 1 if requested.  The
	 * tensor is stored in m_coreTensor, but hosvd is not performed.. hence isHosvded and hasBeenHosvded are both
	 * false
	 */
	HosvdTensor(std::vector<int> size, bool isRandom = false);

	virtual ~HosvdTensor();

	/*!
	 * Performs HOSVD of Tensor.  Results in isHosvded and hasBeenHosvded being set to true.  The function may or may
	 not "reset" the tensor before performing hosvd, depending on the input parameter (resetting means to re-multiply 
	 the core tensor with the vector of weights -- this is necessary if the weights were changed)
	 */
	void performHosvd(bool reset = true);
	
	/*!
	 * Remultiplies core tensor with weights.  Necessary whenever weighting functions are changed
	 */
	void remultiplyHosvdTensor();

                bool getIsHosvded();


};

} //end namespace SdmTensor

#endif /* HOSVDTENSOR_HPP_ */
