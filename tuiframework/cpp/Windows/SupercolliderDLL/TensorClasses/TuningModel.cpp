/*
 * TuningModel.cpp
 *
 *  Created on: 2011.02.16.
 *      Author: Adam
 */

#include "TuningModel.h"

namespace SdmTensor{

/*!
 * Creates an empty, tunable Tensor
 */
TuningModel::TuningModel(std::vector<int> size, 
                                std::vector<ChannelParameter> parameters,
                                bool isRandom): HooiTensor(size, isRandom){        

	m_lowerBounds = std::vector<double>();
	m_upperBounds = std::vector<double>();

	m_weightNames.clear();
	m_independentParams.clear();
	m_units.clear();

	for(int i=0; i<m_tensorSize[m_numDimensions-1]; i++){    
	        m_lowerBounds.push_back(parameters[i].getMinval());
	        m_upperBounds.push_back(parameters[i].getMaxval());
	        m_units.push_back(parameters[i].getUnit());
	}
	
	for(int i=0; i<m_tensorSize[m_numDimensions-1]; i++){		        
	        m_weightNames.push_back(parameters[i].getName());
	        m_independentParams.push_back(parameters[i].isIndependent());
	        m_unshadowedIndependentParams.push_back(parameters[i].isUnshadowed());
	}
	
	m_numGradations = m_tensorSize[0];
	
	std::cout << std::endl;
	
                m_weightCycleTrackers = std::vector<double>();
                m_weightCycleTrackers.clear();
                for(int gradation = 0; gradation<m_numGradations; gradation++){
                        m_weightCycleTrackers.push_back(0);
                }
                
	m_compensationScale = 0;
}





/*!
 * Creates a tunable Tensor with specified size.  Tensor will be filled with random values between 0 and 1
 * if requested
 * Arguments: size, names of output parameters, list of independent parameters, lower bounds of output
 * parameters, upper bounds of output parameters, bool value for random initialization
 */
TuningModel::TuningModel(std::vector<int> size,
		std::vector< std::string>& weightNames,
		std::vector<int>& independentParams,
		std::vector<int>& unshadowedIndependentParams,
		std::vector< double>& lowerBounds,
		std::vector< double>& upperBounds,
		bool isRandom): HooiTensor(size, isRandom){


	m_lowerBounds = std::vector<double>();
	m_upperBounds = std::vector<double>();

	m_weightNames.clear();
	m_independentParams.clear();
	m_units.clear();	

	for(int i=0; i<m_tensorSize[m_numDimensions-1]; i++){
		m_lowerBounds.push_back(lowerBounds[i]);
		m_upperBounds.push_back(upperBounds[i]);
		m_units.push_back("");
	}
	
	for(int i=0; i<m_tensorSize[m_numDimensions-1]; i++){
		m_weightNames.push_back(weightNames[i]);
		m_independentParams.push_back(independentParams[i]);
		m_unshadowedIndependentParams.push_back(unshadowedIndependentParams[i]);
	}

	m_compensationScale = 0;

}

TuningModel::TuningModel():HooiTensor(){
        
}

TuningModel::~TuningModel(){

	m_lowerBounds.clear();
	m_upperBounds.clear();

	m_hooiTuningSlopeBegins.clear();
	m_hooiTuningSlopeEnds.clear();
	m_hooiTuningSlopeCurrents.clear();

	m_weightNames.clear();
	m_independentParams.clear();

}


/*!
 * Returns an element from the tensor but takes into consideration its upper and lower bound and
 * cuts off any values that are out of bounds
 */
double TuningModel::getElementBounded(int* coordinates){

	int output_select = coordinates[m_numDimensions-1];
	double ub = m_upperBounds[output_select];
	double lb = m_lowerBounds[output_select];
	double res = getElement(coordinates);

	if(res < lb)
		res = lb;

	if(res > ub)
		res = ub;

	return res;
}


/*
 * Getter methods for lower and upper bounds, weights and weightnames
 */
void TuningModel::getLowerBounds(std::vector<double>& lb){

	lb.clear();
	for(int i=0; i<m_lowerBounds.size(); i++)
		lb.push_back(m_lowerBounds[i]);

}


void TuningModel::getUpperBounds(std::vector<double>& ub){

	ub.clear();
	for(int i=0; i<m_upperBounds.size(); i++)
		ub.push_back(m_upperBounds[i]);

}

	/*! Getter method that can be obtained to get the weights of the tuning model.  Weights are interpreted 
	 differently depending on whether the model is HOSVD-ed, HOOI-ed or HOOI-compensated.  The three cases 
	 are as follows:
	 
	 Before HOSVD (valid for all shadow tensors in application): getElementBounded() for every output select
	 After HOSVD only: A single row of the first weighting matrix
	 After HOOI: 		A single row of the first weighting matrix (always just one element)
	 After HOOI compensation: The first value of a single row of the first weighting matrix
	 */	
void TuningModel::getWeights(int gradation, std::vector<double>& weights){

	weights.clear();

	if(!isHooied){

		if(isHosvded){
			for(int i=0; i < m_weightingFunctions[0].dim2(); i++){
				weights.push_back((m_weightingFunctions[0])[gradation][i]);
			}
		}

		else{
			//most probably getting weights for shadow tensor, since hosvd has not been performed
			int* coords = new int[m_numDimensions];
			coords[0] = gradation;
			for(int i=1; i < m_numDimensions; i++)
				coords[i] = 0;

			for(int i=0; i < m_independentParams.size(); i++){
				if(m_independentParams[i] == 1){
					coords[m_numDimensions-1] = i;
					weights.push_back(getElementBounded(coords));
				}
			}

			delete[] coords;

		}
	}else{
		if(!isCompensated)
			weights.push_back((m_hooiWeightingFunctions[0])[gradation][0]);
		else
			weights.push_back((m_compensatedWeightingFunctions[0])[gradation][0]);
	}
}


void TuningModel::getWeightNames(std::vector< std::string >& weightnames){

	//if HOSVD was performed (as in the case of all tensors except shadow tensors)
	//the output contains empty strings only, since the HOSVD model does not define special meanings
	//to any of its weights

	weightnames.clear();

	if(!isHosvded){
		//shadow tensor case
		for(int i=0; i<m_weightNames.size(); i++){
			if(m_independentParams[i] == 1)
				weightnames.push_back(m_weightNames[i]);
		}
	}

	else{
		//normal case -- with HOSVD
		for(int i=0; i < m_weightingFunctions[0].dim2(); i++){
			weightnames.push_back("");
		}
	}
}


void TuningModel::getIndependentParamNames(std::vector< std::string >& paramnames){

        paramnames.clear();

        for(int i=0; i<m_weightNames.size(); i++){
                if(m_independentParams[i] == 1){
	        paramnames.push_back(m_weightNames[i]);
	}
        }

}


void TuningModel::getUnits(std::vector< std::string >& resu){

        resu.clear();

        for(int i=0; i<m_units.size(); i++){
                resu.push_back(m_units[i]);
        }

}


/*
 * Setter method for weights
 */
void TuningModel::setWeights(std::vector<int> gradations, std::vector<double>& weights, bool shadowTensor){

        /*weights are interpreted differently depending on whether the model is HOSVD-ed, HOOI-ed
        *or HOOI-compensated.  The three cases are as follows:
        *
        *Before HOSVD and shadowTensor=TRUE 
        *              (valid for all shadow tensors in application): weights contains only those output parameters which are independent
        *After HOSVD only: A single row of the first weighting matrix
        *After HOOI: 		A single row of the first weighting matrix (always just one element)
        *After HOOI compensation: The first value of a single row of the first weighting matrix.  In this case
        *						the core tensor is also modified (subtensor K) so that the original parameter space
        *						can be discovered in meaningful directions
        */

        int gradation = gradations[0]; //if tensor was already HOSVDed or HOOIed, in this case we need only the first gradation
        //otherwise we need all relevant gradations to set the correct values!

        std::vector<int> ranks = std::vector<int>();
        int wlen = weights.size();
        int wfun_dim2;

        if(!isHooied){
                
	if(isHosvded){
		
	        assert(!shadowTensor);
	        
                        //HOSVD only case: a full row is modified in the first weighting matrix                        
                        wfun_dim2 = m_weightingFunctions[0].dim2();
                        
                        assert(wlen == wfun_dim2);

                        for(int i=0; i < wfun_dim2; i++)
                                (m_weightingFunctions[0])[gradation][i] = weights[i];
                        remultiplyHosvdTensor();
                
                }else{
                        //shadow case... only because tensor hasn't been hosvded... used to be kind of dangerous...
                        if(shadowTensor){
                        
							int* fixDimensions = new int[m_numDimensions];
							for(int i=0; i < m_numDimensions-1; i++){
                                        fixDimensions[i] = gradations[i]+1;
                            }
							fixDimensions[m_numDimensions-1] = 0;

							int ctr = 0;
							for(int i=0; i < m_independentParams.size(); i++){
									if(m_independentParams[i] == 1){
                                                fixDimensions[m_numDimensions-1] = i+1;
												setElements(0, gradation, fixDimensions, weights[ctr]);
												ctr++;
									}
                                }

							delete[] fixDimensions;

                        }

	}
        }else{
	
                assert(!shadowTensor);

                if(!isCompensated){
                        //HOOI case, no compensation - a single value is modified in a single row of the first
	        //weighting matrix
	        (m_hooiWeightingFunctions[0])[gradation][0] = weights[0];
	}else{
	        //IF HOOI WAS COMPENSATED
	        //change in weight value is calculated in an integer value of NOTCHES.  The actual
	        //size of a notch depends on the compensation scale
	        
	        double weightDiff = weights[0] - (m_compensatedWeightingFunctions[0])[gradation][0];
	        int weightChangeDirection = 1;
	        if(weightDiff < 0){
		weightDiff = -1*weightDiff;
		weightChangeDirection = -1;
	        }
	        std::cout << "weight changed from " << (m_compensatedWeightingFunctions[0])[gradation][0] << " to " << weights[0] << std::endl;
	        
	        int numNotches = (int)(100*weightDiff / m_compensationScale);
	        
	        double current, begin, end;
	        
	        int notchesLeft = 0; //number of notches left between current and either begin or end in the direction of modification
	        int totalNotches = 0; //total notches from beginning to end
	        double notchSize = 0; // size of one notch in units between begin and end
	        int beginEndOrder; // 1 if incrementing weights corresponds to incrementing current (i.e., end > begin), and -1 otherwise
	        bool reverseDirection; // true if limit (either begin or end) is reached
			
	        if(numNotches > 5){			
		
		for(int i=0; i<m_hooiTuningSlopeBegins.size(); i++){


		        begin = m_hooiTuningSlopeBegins[i];
		        end = m_hooiTuningSlopeEnds[i];
		        current = m_hooiTuningSlopeCurrents[i];
		        
		        totalNotches = 10*abs((begin-end));
		        
		        if(begin != end){
		                
		        
		                // means that positive change direction means incrementing current
		                beginEndOrder = 1;
                		if(end < begin){
		                        // means that positive change direction actually means reducing current
		                        beginEndOrder = -1;
		                }
		                
		                notchSize = 0.1;
		                notchesLeft = (weightChangeDirection > 0) ? 10*abs(current - end) : 10*abs(begin - current);
		        
		                reverseDirection = false;
		                std::cout << numNotches << ", " << notchesLeft << " num notches and notches left..." << std::endl;
		                std::cout << "begin end and current are: " << begin << ", " << end << ", " << current << std::endl;
		                std::cout << "weight change direction is: " << weightChangeDirection << std::endl;
		        
		                if(notchesLeft == 0){
		                        notchesLeft = totalNotches;
		                        
		                        reverseDirection = true;
		                        std::cout << "reversing direction" << std::endl;
                		        
                		        std::cout << numNotches << ", " << notchesLeft << " num notches and notches left..." << std::endl;
		                }

		                if(notchesLeft < numNotches){
		                        std::cout << "reversing direction because not enough notches left" << std::endl;
		                        reverseDirection = true;
		                        
		                        int oldNotchesLeft = notchesLeft;
		                        
		                        numNotches = numNotches - notchesLeft;
		                        numNotches = numNotches % totalNotches;
		                        
		                        //detect when updates get "stuck" and give them a boost
		                        while(numNotches == 0 || (numNotches == oldNotchesLeft)){
		                                numNotches = KISS%totalNotches;
		                        }
		                }
		        
		                if(reverseDirection){
		                        //swap begin and end for later runs
		                        m_hooiTuningSlopeBegins[i] = end;
		                        m_hooiTuningSlopeEnds[i] = begin;
		                        
		                        
		                        cout << "numNotches, notchSize are " << numNotches << ", " << notchSize << std::endl;
		                        
		                        if(weightChangeDirection > 0){
		                                current = end - beginEndOrder * numNotches * notchSize;
		                        }else{
		                                current = begin + beginEndOrder * numNotches * notchSize;
		                        }
		                
		                       
                                                        if( weights[0] - (m_compensatedWeightingFunctions[0])[gradation][0] > 0){
                                                                m_weightCycleTrackers[gradation] = m_weightCycleTrackers[gradation] + 1;
                                                        }else{
                                                                m_weightCycleTrackers[gradation] = m_weightCycleTrackers[gradation] - 1;
                                                        }

		                        (m_compensatedWeightingFunctions[0])[gradation][0] = weights[0] - (0.4*m_weightCycleTrackers[gradation] * totalNotches* m_compensationScale / 100.0);
                                                        
		                        //(m_compensatedWeightingFunctions[0])[gradation][0] = weights[0];           
		                
		                }else{
		                        current = current + beginEndOrder * weightChangeDirection * numNotches * notchSize;
		                        (m_compensatedWeightingFunctions[0])[gradation][0] = weights[0];
		                }
		        
		                m_hooiTuningSlopeCurrents[i] = current;
		                cout << "begin, end and current are: " << begin << ", " << end << ", " <<  current << std::endl;

		                /*if(begin > end and !reverseDirection){
			        assert(current <= begin);
			        assert(current >= end);
		                }else{
		                        if(begin < end and reverseDirection){
		                                assert(current >= begin);
			                assert(current <= end);
		                        }
		                }
		                
                                                if(begin < end and !reverseDirection){
                                                        cout << "begin, end and current are: " << begin << ", " << end << ", " <<  current << std::endl;
			        assert(current >= begin);
			        assert(current <= end);
		                }else{
		                        if(begin > end and reverseDirection){
                			assert(current <= begin);
			                assert(current >= end);
			        }
		                }
		                
                                                if(begin==end){
			        assert(current == begin);
			        assert(m_hooiTuningSlopeBegins[i] + m_hooiTuningSlopeEnds[i] == 0);
		                }*/
		        }		                
	                }

			

		//do NOT compensate for errors, because we do not want to set the single control
		//back to the explore the same nodes...
		//compensateHooiErrors(m_hooiTuningSlopeCurrents, false);
			
		//instead, just update K...
		//std::cout << "OK in TuningModel.cpp before updating K" << std::endl;
		updateK(m_hooiTuningSlopeCurrents);
		//std::cout << "OK in TuningModel.cpp after updating K" << std::endl;
		
		remultiplyHooiTensor();
	        }	
	}
        }
        

        
        std::cout << "WEIGHTS SET!" << std::endl;
}

void TuningModel::setWeightsSelective(std::vector<int> gradations, std::vector<double>& weights){

        if(isHosvded){

                int gradation = gradations[0];

                const int numcoords = gradations.size() + 1;
                int* coords = new int[numcoords];
                for(int i = 0; i<numcoords-1; i++){
                        coords[i] = gradations[i];
                }
                coords[numcoords - 1] = 0;

                int coords2Dstart[2], coords2Dend[2];
                get2Dcoords(coords, coords2Dstart);
                coords[numcoords - 1] = m_weightNames.size() - 1;
                get2Dcoords(coords, coords2Dend);
         
				delete[] coords;
        
                std::vector<double> row;
                getRow(coords2Dstart[0], row);        
        
                std::vector<int> maingradations = std::vector<int>();
                maingradations.push_back(gradation);
                setWeights(maingradations, weights, false);
        
                //now reset all other values...
                int inxes[2];
                for(int i=0; i<coords2Dstart[1]; i++){
                        inxes[0] = coords2Dstart[0]; inxes[1] = i;
                        setElement2D(inxes, row[i]);
                }


                for(int i=coords2Dend[1]+1; i<row.size(); i++){
                        inxes[0] = coords2Dend[0]; inxes[1] = i;
                        setElement2D(inxes, row[i]);
                }
        
                hosvdTuningModel(false); //do not reset tensor...   
                
                
        }  
        
}


/*!
 Swaps two gradations within the tuning model.
 */
void TuningModel::swapGradations(int gradation1, int gradation2){

		
	bool doHooiAgain = false;
	if (isHooied) {
		hosvdTuningModel(false); //hosvd without resetting
		doHooiAgain = true;
	}
	
	
	bool hosvdAgain = false;
	if(isHosvded){
		hosvdAgain = true;
	}
	
	unfoldN(0);
	m_data.swapRows(gradation1, gradation2);
	if(hosvdAgain){
		isHosvded = false; // set this to false so that remultiplication does not occur
		//(that's important because the core tensor and weighting functions are still in their
		//previous state)
		hosvdTuningModel();
	}

	if(doHooiAgain){
		hooiTuningModel(m_compensationScale);
	}
	
}

	/*!
	 Copies gradation1 to gradation2 within the tuning model.
	 */
void TuningModel::copyGradation(int gradation1, int gradation2){

	bool doHooiAgain = false;
	if (isHooied) {
		hosvdTuningModel(false); //hosvd without resetting
		doHooiAgain = true;
	}
	
	bool hosvdAgain = false;
	if(isHosvded){
		hosvdAgain = true;
		//remultiplyHosvdTensor();
	}

	unfoldN(0);
	m_data.copyRow(gradation1, gradation2);
	if(hosvdAgain){
		isHosvded = false; // set this to false so that remultiplication does not occur
		//(that's important because the core tensor and weighting functions are still in their
		//previous state)
		hosvdTuningModel();
	}
	
	if(doHooiAgain){
		hooiTuningModel(m_compensationScale);
	}
}

	
	/*!
	 Interpolates between two gradations within the tuning model.
	 */
void TuningModel::interpolateGradations(int gradation1, int gradation2, int resultGradation,
			double degree, double factor){

	hooiTuningModel(factor);
	std::vector<double> weights1;
	std::vector<double> weights2;

	getWeights(gradation1, weights1);
	getWeights(gradation2, weights2);

	double diff = weights2[0] - weights1[0];
	double newWeight = weights1[0] + degree*diff;

	std::vector<double> newWeights;
	newWeights.push_back(newWeight);

                std::vector<int> resultGradations = std::vector<int>();
                resultGradations.push_back(resultGradation);

	setWeights(resultGradations, newWeights);

}


void TuningModel::getGradation(int gradation, std::vector<double>& params){
	//places all values in a row of the first weight matrix into params, irrespective of whether or not the
	//model was HOSVD-d or HOOI-ed or HOOI compensated

	unfoldN(0);
	params.clear();
	cout << "mdata dim2 size is " << m_data.dim2() << endl;;
	for(int i=0; i<m_data.dim2(); i++)
	        params.push_back(m_data[gradation][i]);

}

void TuningModel::setGradation(int gradation, std::vector<double>& params){

	unfoldN(0);
	for(int i=0; i<m_data.dim2(); i++)
		m_data[gradation][i] = params[i];

	bool doHooi = false;
	if(isHooied){
		doHooi = true;
	}

	if(isHosvded){
		//isHosvded = false; // do this so that weights do not get remultiplied
		hosvdTuningModel(false); //hosvd without resetting
	}
	if(doHooi)
		hooiTuningModel(m_compensationScale);
}


void TuningModel::saveGradation(int gradation, const std::string& filename){

	/*unfoldN(0);

	std::ofstream file(filename.c_str());
	boost::archive::text_oarchive oa(file);

	double element;
	for(int i=0; i<m_data.dim2(); i++){
		element = m_data[gradation][i];
		oa << element;
	}*/

}

void TuningModel::loadGradation(int gradation, const std::string& filename){

	/*unfoldN(0);

	std::ifstream file(filename.c_str());
	boost::archive::text_iarchive ia(file);

	for(int i=0; i<m_data.dim2(); i++){
		ia >> m_data[gradation][i];
	}

	if(isHosvded){
		//isHosvded = false; // do this so that weights do not get remultiplied
		hosvdTuningModel(false); //hosvd without resetting
	}*/

}


void TuningModel::getIndependentParams(std::vector<int>& resu){

        resu.clear();
	
        for(int i=0; i<m_independentParams.size(); i++){
	resu.push_back(m_independentParams[i]);
        }
}

void TuningModel::getUnshadowedIndependentParams(std::vector<int>& resu){

	resu.clear();
	for(int i=0; i<m_unshadowedIndependentParams.size(); i++)
		resu.push_back(m_unshadowedIndependentParams[i]);

}


void TuningModel::hosvdTuningModel(bool reset){

	bool haveToInitTuningSlopes = false;
	if(!hasBeenHosvded)
		haveToInitTuningSlopes = true;

	performHosvd(reset);
	isHooied = false; // set isHooied to false so that hosvd controls will be used

	if(haveToInitTuningSlopes) //if system has never been Hosvded, only then initialize the tuning slopes
		initTuningSlopes();
}


void TuningModel::hooiTuningModel(double compensationScale){

	m_compensationScale = compensationScale;
	performHooi(0, 1, compensationScale, false);
	compensateHooiErrors(m_hooiTuningSlopeBegins, false);
	
	/*for(int gradation = 0; gradation < m_numGradations; gradation++){
	        m_weightCycleTrackers[gradation] = (m_compensatedWeightingFunctions[0])[gradation][0];
	}*/

}

void TuningModel::initTuningSlopes(){


	m_hooiTuningSlopeBegins.push_back(0.0);
	m_hooiTuningSlopeEnds.push_back(0.0);
	m_hooiTuningSlopeCurrents.push_back(0.0);
	
	srand(time(NULL));
	int posneg;
	for(int i = 1; i < m_weightingFunctions[m_numDimensions-1].dim1(); i++){
		posneg = KISS % 2;
		
		m_hooiTuningSlopeBegins.push_back((KISS % 3) + 2);
		if (posneg != 0) {
			m_hooiTuningSlopeBegins[i] = -1*m_hooiTuningSlopeBegins[i];
		}

		m_hooiTuningSlopeEnds.push_back(-1*m_hooiTuningSlopeBegins[i]);
		m_hooiTuningSlopeCurrents.push_back(m_hooiTuningSlopeBegins[i]);
		
	}
}


} //end namespace SdmTensor
