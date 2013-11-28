/*
 * TensorHandler.hpp
 *
 *  Created on: 2011.02.18.
 *      Author: Adam
 */

#ifndef TENSORHANDLER_HPP_
#define TENSORHANDLER_HPP_

#include <vector>
#include <fstream>
#include "../TensorClasses/TuningModel.h"
#include "../TensorManagers/ChannelParameter.h"
using namespace SdmTensor;

#include "../LibConfig/libconfig++.h"
using namespace libconfig;

class TensorHandler{
public:

	TensorHandler(const string cfgFilename, const string gradationFilename);
	virtual ~TensorHandler();
	
	
	double getElement(int dimension, int* coordinates); // gets elements from tensor of given dimension
	double getElementProtected(int dimension, int* coordinates); // gets elements from shadow tensor of given dimension
	int getNumInstruments(); //total number of dimensions (including those that have no sound)     
                
	void getPerformanceParameters(int dimension, std::vector<int> gradations, std::vector<double>& performanceParameters);

	void getWeights(int dimension, int gradation, std::vector<double>& weights);
	
	void getShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights, bool keepNotUnshadowedParamsIntact=true);
		
	//iparams will contain a vector of 1s and 0s.  1s mark the indices of params that are independent
	void getIndependentParams(int dimension, std::vector<int>& iparams);
	
	//tensor is updated for given gradation, irrespective of other gradations
	void setWeights(int dimension, int gradation, std::vector<double>& weights);
	
	//same as previous method, but this one saves new parameters only in relation to the full set of gradations
	//for example, if the tensor has two input dimensions (softness, stickiness), and the weights for gradation 2 of softness are
	//changed, and stickiness in the gradations vector is set to 1, then the parameters for stickiness=0,2,3,... are left unchanged
	void setWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights);
	
	
	void setShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights, bool debug=false);
	
	
	void storeShadowWeights(int dimension, std::vector<int> gradations, bool debug=false);
	
	void getWeightNames(int dimension, std::vector< std::string >& weightnames);

	void getIndependentParamNames(int dimension, std::vector< std::string >& paramnames);

	void swapGradations(int dimension, int gradation1, int gradation2);
	void copyGradations(int dimension, int gradation1, int gradation2);
	
	void saveChannel(const std::string& filename);
	
	void performHosvd(int dimension);
	void performHosvdNonreset(int dimension);
	void performHooi(int dimension, double compensationScale);

	bool isTensorDefined(int dimension); // must return false if dimension cannot be tuned...

	void getLowerBounds(int dimension, std::vector<double>& lb, bool getJustShadow=false);
	void getUpperBounds(int dimension, std::vector<double>& ub, bool getJustShadow=false);
	void getParamUnits(int dimension, std::vector<std::string>& resu, bool getJustShadow=false);
	
	void getDimensionNames(std::vector< std::string >& dimnames);
	void getDimensionNamesWithSound(std::vector< std::string >& dimnames);	
	void getNumberGradations(std::vector< int>& numgrads);
	
	void getDimensionInxesWithSound(std::vector<int>& diminxes);
	
	void getInstrumentNames(std::vector<std::string>& resu);
	
	//gradations contains all gradations, e.g. softness=2, roughness=5, stickiness=3.  But the given dimension may not actually
	//depend on all of these values... therefore only those gradations should be used which are relevant to the indexing of the tensor
	// this method produces a vector of these gradations.  the first one is always the one that represents dimension.
	void getRelevantGradations(int dimension, std::vector<int> gradations, std::vector<int>& grads);

protected:

	std::vector<TuningModel> m_tensors;
	std::vector<TuningModel> m_shadowTensors; // shadow tensor kept for weights which cannot be changed
	// using tuning process (can only be changed directly)
	std::vector<bool> m_tensorsDefined;
	int m_numInstruments;
	
	void createTensors();
	const string cfgFile;
	const string gradFile;
	string gradationCfgFile;
	std::vector<std::vector<double> > mv_variables;
        
        std::vector<std::vector<ChannelParameter> > mv_parameters;
        std::vector<std::string> mv_dimensionNames;
        std::vector<int> mv_numParameters; //number of parameters per dimension
        std::vector<int> mv_numGradations;
        std::vector<bool> mv_hasSound;	
        std::vector<std::vector<int> > mv_dependencies;
        std::vector<std::string> mv_instrumentNames;
        
        void incrementNaryCombination(std::vector<int>& maxvalues, std::vector<int>& currentNaryAndResu);
};


#endif /* TENSORHANDLER_HPP_ */
