/*
 * TuningModel.hpp
 *
 *  Created on: 2011.02.16.
 *      Author: Adam
 */

#ifndef TUNINGMODEL_HPP_
#define TUNINGMODEL_HPP_


#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <iostream>
#include <fstream>

#include "HooiTensor.h"
#include "../TemplateNumericToolbox/tnt_126/tnt.h"
#include "randomness.h"
#include "../TensorManagers/ChannelParameter.h"

namespace SdmTensor{

//! Implements the SDM tuning model.  The class is ultimately a Tensor object that inherits through HosvdTensor and HooiTensor, so that it can be HOSVDed, HOOIed and HOOI compensated.
class TuningModel : public HooiTensor{

private:
        //one weight cycle tracker for each gradation
        std::vector<double> m_weightCycleTrackers;
	
protected:

        int m_numGradations;

    std::vector<double> m_lowerBounds;//!< lower bound of each output parameter (length of vector is the same as the size of the last dimension in the tensor this class represents)
    std::vector<double> m_upperBounds;//!< upper bound of each output parameter (length of vector is the same as the size of the last dimension in the tensor this class represents)
    std::vector<double> m_hooiTuningSlopeBegins;//!< extrema of values in K on one end (pertains only to coordinates (0,0,...,0,:) of K)
    std::vector<double> m_hooiTuningSlopeEnds;//!< extrema of values in K on the other end (pertains only to coordinates (0,0,...,0,:) of K)
    std::vector<double> m_hooiTuningSlopeCurrents;	//!< current values of K
    std::vector<std::string> m_units;

    std::vector< std::string > m_weightNames;//!< names of output parameters
    
	/// vector whose length equals the number of output parameters. Contains a 1 for each output parameter that can be tuned manually even 
	/// after HOSVD, HOOI, etc. (e.g., the volume should typically be modifiable)
	std::vector<int> m_independentParams;		
	
	/// vector whose length equals the number of output parameters. Contains a 1 for each output parameter that can be tuned manually even after HOSVD, 
	/// HOOI, etc., but whose value is also updated through the normal tuning process (i.e., if HOSVD or HOOI parameters are changed).  For example, the 
	/// volume should not be unshadowed, since then the tuning process could create uncomfortably loud sounds
	std::vector<int> m_unshadowedIndependentParams; 
	
    double m_compensationScale;						//!< determines frequency of loops within adaptive tuning model

    void initTuningSlopes();

public:

    /*!
     * Creates an empty, tunable Tensor
     */
	TuningModel();
	
	/*!
	 * Creates a tunable Tensor with specified size.  Tensor will be filled with random values between 0 and 1
	 * if requested
	 * Arguments: size, names of output parameters, list of independent parameters, lower bounds of output
	 * parameters, upper bounds of output parameters, bool value for random initialization
	 */
	TuningModel(std::vector<int> size,
			std::vector< std::string>& weightNames,
			std::vector<int>& independentParams,
			std::vector<int>& unshadowedIndependentParams,
			std::vector< double>& lowerBounds,
			std::vector< double>& upperBounds,
			bool isRandom = false);

                TuningModel(std::vector<int> size, 
                                                std::vector<ChannelParameter> parameters,
                                                bool isRandom = false);


	~TuningModel();
	
	/*!
	 * Returns an element from the tensor but takes into consideration its upper and lower bound and
	 * cuts off any values that are out of bounds
	 */
	double getElementBounded(int* coordinates);
	
	/*
	 * Getter methods for lower and upper bounds, weights and weightnames
	 */
	void getLowerBounds(std::vector<double>& lb);
	void getUpperBounds(std::vector<double>& ub);
	
	/*! Getter method that can be obtained to get the weights of the tuning model.  Weights are interpreted 
	 differently depending on whether the model is HOSVD-ed, HOOI-ed or HOOI-compensated.  The three cases 
	 are as follows:
	 
	 Before HOSVD (valid for all shadow tensors in application): getElementBounded() for every output select
	 After HOSVD only: A single row of the first weighting matrix
	 After HOOI: 		A single row of the first weighting matrix (always just one element)
	 After HOOI compensation: The first value of a single row of the first weighting matrix
	 */	
	void getWeights(int gradation, std::vector<double>& weights);
	void getWeightNames(std::vector< std::string >& weightnames);
	void getIndependentParamNames(std::vector< std::string >& paramnames);
	
	/*weights are interpreted differently depending on whether the model is HOSVD-ed, HOOI-ed
                *or HOOI-compensated.  The three cases are as follows:
        
                *Before HOSVD and shadowTensor=TRUE 
                *(valid for all shadow tensors in application): weights contains only those output parameters which are independent
                *After HOSVD only: A single row of the first weighting matrix
                *After HOOI: 		A single row of the first weighting matrix (always just one element)
                *After HOOI compensation: The first value of a single row of the first weighting matrix.  In this case
                *						the core tensor is also modified (subtensor K) so that the original parameter space
                *						can be discovered in meaningful directions
                */
	void setWeights(std::vector<int> gradations, std::vector<double>& weights, bool shadowTensor = false);
	
	
	// prerequisite: tensor must be hosvded for this method to do anything
	// weights contains hosvd parameters, but when remultiplying, changes are applied only selectively in relation to specified set of gradations
	void setWeightsSelective(std::vector<int> gradations, std::vector<double>& weights);
	
	/*!
	 Swaps two gradations within the tuning model.
	 */
	void swapGradations(int gradation1, int gradation2);
	
	/*!
	 Copies gradation1 to gradation2 within the tuning model.
	 */
	void copyGradation(int gradation1, int gradation2);
	
	/*!
	 Interpolates between two gradations within the tuning model.
	 */
	void interpolateGradations(int gradation1, int gradation2, int resultGradation, double degree, double factor);
	
	
	void getGradation(int gradation, std::vector<double>& params);
	void setGradation(int gradation, std::vector<double>& params);
	void saveGradation(int gradation, const std::string& filename);
	void loadGradation(int gradation, const std::string& filename);

	
	
	void getIndependentParams(std::vector<int>& resu);
	
	void getUnits(std::vector<std::string>& resu);
	
	
	/*! 
	 places in output vector resu the values of m_unshadowedIndependentParams
	 */
	void getUnshadowedIndependentParams(std::vector<int>& resu);

	void hosvdTuningModel(bool reset = true);
	void hooiTuningModel(double compensationScale);
	
                friend std::ostream& operator<<(std::ostream& out, const TuningModel& tm){
                        for(unsigned int i = 0; i<tm.m_weightNames.size(); i++){
                                out << tm.m_weightNames[i] << ", ";
                        }
                        
                        out << endl << "lower bounds: ";

                        for(unsigned int i = 0; i<tm.m_weightNames.size(); i++){
                                out << tm.m_lowerBounds[i] << ", ";
                        }
                        
                        out << endl << "upper bounds: ";

                        for(unsigned int i = 0; i<tm.m_weightNames.size(); i++){
                                out << tm.m_upperBounds[i] << ", ";
                        }                        

                        out << endl << "independent params: ";

                        for(unsigned int i = 0; i<tm.m_weightNames.size(); i++){
                                out << tm.m_independentParams[i] << ", ";
                        }                        
        
                        out << endl << "unshadowed params: ";

                        for(unsigned int i = 0; i<tm.m_weightNames.size(); i++){
                                out << tm.m_unshadowedIndependentParams[i] << ", ";
                        }
                        
                        out << endl << endl;
                        
                        out << tm.m_data;
        
                        return out;
                }
	

};

} // end namespace SdmTensor


#endif /* TUNINGMODEL_HPP_ */
