/*
 * HooiTensor.hpp
 *
 *  Created on: 2011.03.07.
 *      Author: Adam
 */

#ifndef HOOITENSOR_HPP_
#define HOOITENSOR_HPP_


#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <assert.h>

#include "Tensor.h"
#include "HosvdTensor.h"
#include "../TemplateNumericToolbox/tnt_126/tnt.h"
#include "../TemplateNumericToolbox/jama125/jama_svd.h"


namespace SdmTensor{

class HooiTensor : public HosvdTensor{

protected:

        Tensor m_hooiCoreTensor;	//!< HOOI core tensor	
        vector< TNT::Array2D<double> > m_hooiWeightingFunctions;  //!< HOOI weighting functions	
        vector< TNT::Array2D<double> > m_compensatedWeightingFunctions;	//!< compensated weighting functions
        Tensor m_compensatedCoreTensor; //!< compensated core tensor

        bool isHooied; //!< true if HOOI has been computed and is effective
        bool hasBeenHooied;	//!< true if HOOI was computed at some point (in this case some parameters for the tuning model will remain unchanged)
        bool isCompensated;	//!< true if rank reduction by HOOI was compensated for

        TNT::Array2D<double> m_R;	//!< matrix R, which contains all columns of the first weighting matrix except the first column, after compensation	
        std::vector<int> m_K_size;	//!< vector containing size of each dimension of extended part of upper subtensor in core tensor
        std::vector<int> m_D_size;	//!< vector containing size of each dimension of extended part of lower subtensor in core tensor

        /*!
        * Augments appropriate weighting matrices for compensation
        */
        void prepareCompensatedWeightingFunctions();
	
        /*
        Changes just K within core tensor.  The input parameter is just a vector because it contains only
        those elements of K that belong to places where all other coordinates are 0.
        */
        void updateK(std::vector<double>& K_values);
	
        /*!
         * Augments HOOI core tensor so that it may be filled with K and D for error compensation
         * if randKvalues is true, random values are used in K, otherwise, K is filled using K_values
         */
        void prepareCompensatedCoreTensor(std::vector<double>& K_values, bool randKvalues);



public:

        /*!
        * Creates an empty Tensor and sets isHooied, hasBeenHooied and isCompensated to false
        */
        HooiTensor();

        /*!
        * Creates a Tensor of specified size and fills it with random values between 0 and 1 if requested.
        * Sets isHooied, hasBeenHooied and isCompensated to false.
        */
        HooiTensor(std::vector<int> size, bool isRandom = false);

        virtual ~HooiTensor();

        /*!
        * Performs HOOI of Tensor.  Results in isHooied and hasBeenHooied being set to true
        */	
        void performHooi(int dimension, int newrank, double compensationScale, bool doCompensate=true);

        /*!
        * Remultiplies HOOI core tensor or compensated core tensor with appropriate weights depending on values
        * of isHooied and isCompensated. Necessary whenever weighting functions are changed
        */
        void remultiplyHooiTensor();

        /*!
        * Compensates for rank reduction errors by augmenting core tensor along its last dimension by tensor K,
        * and the whole resulting core tensor along its first dimension by tensor D.  The first and last weighting
        * matrices are also augmented.  By choosing the free parameters in K, the slope of the hyperline controlled
        * by the first column of the first weighting matrix can be manipulated.
        */
        void compensateHooiErrors(std::vector<double>& K_values /**[in] vector containing values in elements of K where all coordinates other than the last
                one are equal to 0 (first coordinates everywhere else, hence this parameter is like a row-vector)*/, 
                bool randKvalues) /**[in] if true, K_values is ignored and random values are used*/;

        /*!
        * prints compensated weighting functions
        */
        void printCompensatedWeightingFunctions();

        /*!
        * prints hooi weighting functions
        */
        void printHooiWeightingFunctions();
        
        bool getIsHooied();

};

}// end namespace SdmTensor

#endif /* HOOITENSOR_HPP_ */
