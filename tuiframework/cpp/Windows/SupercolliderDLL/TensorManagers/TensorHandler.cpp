/*
 * TensorHandler.cpp
 *
 *  Created on: 2011.02.18.
 *      Author: Adam
 */

#include "TensorHandler.h"
using namespace SdmTensor;


TensorHandler::TensorHandler(const string cfgFilename, const string gradationFilename):cfgFile(cfgFilename), gradFile(gradationFilename){


	
        // Read the channel configuration file. If there is an error, report it.
        Config cfg;
        
        try{
                cfg.readFile(cfgFile.c_str());
        }catch(const FileIOException &fioex){
                std::cerr << "I/O error while reading file." << std::endl;
        }catch(const ParseException &pex){
                std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
        }

        mv_numGradations = std::vector<int>();
        mv_numParameters = std::vector<int>();
        mv_hasSound = std::vector<bool>();
        mv_parameters = std::vector<std::vector<ChannelParameter> >();
        mv_dependencies = std::vector<std::vector<int> >();
        mv_dimensionNames = std::vector<std::string>();
        mv_instrumentNames = std::vector<std::string>();


        const Setting& root = cfg.getRoot();
        // Parse channel parameters.
        try{
                const Setting &channel = root["channel"];
                
                channel.lookupValue("gradationFilename", gradationCfgFile);
                channel.lookupValue("numDimensions", m_numInstruments);
                
                const Setting &dimensionNames = root["channel"]["dimensionNames"];
                
                for(int dimensionCtr = 0; dimensionCtr<m_numInstruments; dimensionCtr++){
                        
                        mv_parameters.push_back(std::vector<ChannelParameter>());
                        mv_dependencies.push_back(std::vector<int>());
                
                        stringstream ss;
                        ss << "dimension" << dimensionCtr;
                        string dimensionName;
                        dimensionNames.lookupValue(ss.str(), dimensionName);
                        
                        mv_dimensionNames.push_back(dimensionName);
                        
                        stringstream ss2, ss3;
                        ss2 << "gradations" << dimensionName;
                        ss3 << "hasSound" << dimensionName;
                        int numgrads, hassound;
                        channel.lookupValue(ss2.str(), numgrads);
                        channel.lookupValue(ss3.str(), hassound);
                        mv_numGradations.push_back(numgrads);
                        if(hassound == 0){
                                mv_hasSound.push_back(false);
                                mv_numParameters.push_back(0);
                                mv_instrumentNames.push_back(std::string(""));
                        }else{
                                mv_hasSound.push_back(true);
                                stringstream ss4;
                                ss4 << "numParameters" << dimensionName;
                                int numparams;
                                channel.lookupValue(ss4.str(), numparams);
                                mv_numParameters.push_back(numparams);
                        
                                stringstream ss7;
                                ss7 << "instrument" << dimensionName;
                                std::string instrName;
                                channel.lookupValue(ss7.str(), instrName);
                                mv_instrumentNames.push_back(instrName);
                        
                        
                                stringstream ss5;
                                ss5 << "dependencies" << dimensionName;
                                const Setting &dependencies = root["channel"][ss5.str()];
                                int dependencyCount = dependencies.getLength();
                                
                                for(int dependencyCtr = 0; dependencyCtr < dependencyCount; dependencyCtr++){
                                        int dependency = dependencies[dependencyCtr];
                                        mv_dependencies[dimensionCtr].push_back(dependency);
                                }
                                
                        
                                stringstream ss6;
                                ss6 << "parameters" << dimensionName;
                                const Setting &parameters = root["channel"][ss6.str()];
                                int parameterCount = parameters.getLength();
                                
                        
                                cout << "DIMENSION " << dimensionCtr << endl;
                                for(int parameterCtr = 0; parameterCtr < parameterCount; parameterCtr++){
                                        const Setting &parameter = parameters[parameterCtr];

                                        string name, unitName;
                                        double maxval, minval;
                                        int independent, unshadowed;
                                        bool indep, unsh;

                                        parameter.lookupValue("name", name);
                                        parameter.lookupValue("maxval", maxval);
                                        parameter.lookupValue("minval", minval);
                                        parameter.lookupValue("independent", independent);
                                        parameter.lookupValue("unshadowed", unshadowed);
                                        parameter.lookupValue("unit", unitName);
                                
                                        indep = (independent == 0) ? false : true;
                                        unsh = (unshadowed == 0) ?  false : true;
                                
                                        mv_parameters[dimensionCtr].push_back(ChannelParameter(name, maxval, minval, indep, unsh, unitName));
                                
                                        cout << mv_parameters[dimensionCtr][parameterCtr] << endl;
                                }
                                cout << endl;
                        }                           
                }
                
                
        }catch(const SettingNotFoundException &nfex){
                cerr << "Error while parsing main configuration file...";
                exit(0);
        }

        m_tensors = std::vector<TuningModel>();
        m_shadowTensors = std::vector<TuningModel>();
        m_tensorsDefined = std::vector<bool>(m_numInstruments);

        for(int instrumentCtr = 0; instrumentCtr< m_numInstruments; instrumentCtr++){
        
        
                if(!mv_hasSound[instrumentCtr]){
                        cout << "Instrument " << instrumentCtr << " has no sound..." << endl;
                        m_tensors.push_back(TuningModel());
                        m_shadowTensors.push_back(TuningModel());
                        m_tensorsDefined[instrumentCtr] = false;
                }else{
                
                        std::vector<int> sizes = std::vector<int>();
                        for(int depctr = 0; depctr < mv_dependencies[instrumentCtr].size(); depctr++){
                                sizes.push_back(mv_numGradations[mv_dependencies[instrumentCtr][depctr]]);
                                //cout << "pushing back into size value: " << mv_numGradations[mv_dependencies[instrumentCtr][depctr]] << endl;
                        }

                        sizes.push_back(mv_numParameters[instrumentCtr]);
                        //cout << "pushing back into size value: " << mv_numParameters[instrumentCtr] << endl;
                        
                        m_tensors.push_back(TuningModel(sizes, mv_parameters[instrumentCtr]));                
						m_shadowTensors.push_back(TuningModel(sizes, mv_parameters[instrumentCtr]));
	        

                        Config cfg2;
                        try{
                                //cfg2.readFile(gradationCfgFile.c_str());
								cfg2.readFile(gradFile.c_str());
                        }
                        catch(const FileIOException &fioex){
                                std::cerr << "I/O error while reading gradation file." << std::endl;
                        }
                        catch(const ParseException &pex){
                                std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
                        }
                        
                        const Setting& root2 = cfg2.getRoot();
                
                
                        try{
                                cout << mv_dimensionNames[instrumentCtr] << endl; 
                                const Setting &parameters = root2[mv_dimensionNames[instrumentCtr]]["tensorContent"];                          
                                int numberOfCommands = parameters.getLength();
                        
                                for(int commandCtr = 0; commandCtr < numberOfCommands; commandCtr++){
                                
                                        const Setting &command = parameters[commandCtr];
                                        int elaboratedDimension;
                                        std::vector<int> fixDimensionsVec = std::vector<int>();
                                        std::vector<double> valuesVec = std::vector<double>();

                                        command.lookupValue("elaboratedDimension", elaboratedDimension);                                          
                                        
                                        const Setting& fixDimensionsList = command["fixDimensions"];
                                        const int numFixDims = fixDimensionsList.getLength();
                                        
                                        for(int fixDimCtr = 0; fixDimCtr < numFixDims; fixDimCtr++){
                                                fixDimensionsVec.push_back(fixDimensionsList[fixDimCtr]);
                                        }
                                        
                                        const Setting& valuesList = command["values"];
                                        const int numValues = valuesList.getLength();                            
                                        
                                        cout << "instrument is " << instrumentCtr << endl;
                                        cout << "command counter is " << commandCtr << endl << endl;
                                        
                                        for(int valuesCtr = 0; valuesCtr < numValues; valuesCtr++){            
                                                valuesVec.push_back(valuesList[valuesCtr]);
                                        }                               
                                        
                                        int* fixDimensions = new int[numFixDims];
										std::copy(fixDimensionsVec.begin(), fixDimensionsVec.end(), fixDimensions);
	                                                                
										double* values = new double[numValues];
										std::copy(valuesVec.begin(), valuesVec.end(), values);
                                        
                                        m_tensors[instrumentCtr].setElements(elaboratedDimension, fixDimensions, values);
                                        
                                        m_shadowTensors[instrumentCtr].setElements(elaboratedDimension, fixDimensions, values);                 
                                        
										delete[] values;
										delete[] fixDimensions;
                        
                                }
                        }catch(const SettingNotFoundException &nfex){
                                cerr << "Incorrect settings in gradation configuration file." << endl;
                                exit(0);
                        }
	        
						m_tensors[instrumentCtr].hosvdTuningModel();
						m_tensorsDefined[instrumentCtr] = true;                
                
                        cout << "the tensor for this instrument is: " << endl;
                        cout << m_tensors[instrumentCtr] << endl;
                }
	
        }

}

TensorHandler::~TensorHandler(){

	m_tensors.clear();
	m_tensorsDefined.clear();
	m_shadowTensors.clear();

}

int TensorHandler::getNumInstruments(){

	return m_numInstruments;
}


double TensorHandler::getElement(int dimension, int* coordinates){
	return m_tensors[dimension].getElementBounded(coordinates);
}


double TensorHandler::getElementProtected(int dimension, int* coordinates){
	return m_shadowTensors[dimension].getElementBounded(coordinates);
}

void TensorHandler::getWeights(int dimension, int gradation, std::vector<double>& weights){

	m_tensors[dimension].getWeights(gradation, weights);

}


void TensorHandler::getRelevantGradations(int dimension, std::vector<int> gradations, std::vector<int>& grads){

        grads.clear();
        std::vector<int> deps = mv_dependencies[dimension];
        
        for(int i = 0; i<deps.size(); i++){
                grads.push_back(gradations[deps[i]]);
        }

}

void TensorHandler::getShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights, bool keepNotUnshadowedParamsIntact){
        
        //gradations contains all gradations, e.g. softness=2, roughness=5, stickiness=3.  But the given dimension may not actually
        //depend on all of these values... therefore only those gradations should be used which are relevant to the indexing of the tensor

        std::vector<int> grads = std::vector<int>();        
        getRelevantGradations(dimension, gradations, grads);
        
        int gradation = grads[0];  
        
        weights = std::vector<double>();
        weights.clear();
        
        std::vector<int> iparams;
        std::vector<int> unshadowedIparams;
	
        m_tensors[dimension].getIndependentParams(iparams);
        m_tensors[dimension].getUnshadowedIndependentParams(unshadowedIparams);
	
	
        const int sz = grads.size() + 1;
        int* coords = new int[sz];
        std::copy(grads.begin(), grads.end(), coords);
        coords[sz-1] = 0;
                
        double w;
        //cout << "Weights are: ";
        for(int i=0; i<mv_numParameters[dimension]; i++){
                coords[sz-1] = i;
                if(iparams[i]==1){ // if parameter is independent
                        if(keepNotUnshadowedParamsIntact && unshadowedIparams[i]==0){//if not unshadowed and this does actually matter
                                w = getElementProtected(dimension, coords);
                                //cout << w << ", ";
                                weights.push_back(w);
                        }else{
                                w = getElement(dimension, coords);
                                //cout << w << ", ";
                                weights.push_back(w);
                        }
                
                }
                
        }
        cout << endl << endl;
		delete[] coords;
                
        //cout << "size of normalweights: " << normalweights.size() << endl;
        //cout << "size of shadowedweights: " << shadowedweights.size() << endl;		
        //cout << "size of iparams: " << iparams.size() << endl;	
        //cout << "size of unshadowedIparams: " << unshadowedIparams.size() << endl;
        //cout << "size of weights is: " << weights.size() << endl;       
        
}


void TensorHandler::getIndependentParams(int dimension, std::vector<int>& iparams){
	iparams = std::vector<int>();
	m_tensors[dimension].getIndependentParams(iparams);

}

void TensorHandler::setWeights(int dimension, int gradation, std::vector<double>& weights){
        
        std::vector<int> maingradations = std::vector<int>();
        maingradations.push_back(gradation);
        m_tensors[dimension].setWeights(maingradations, weights, false);
        
}


void TensorHandler::setWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights){

        //if hosvd parameters were changed, care must be taken to RESET those values of the
        //tensor which do not belong to the same gradations.  For example, if the parameters were changed for
        //softness=2 and stickiness=3, then those parameters that used to be valid for softness=2 and stickiness = 1,2,4,...
        //should be left unchanged...

        std::vector<int> grads = std::vector<int>();        
        getRelevantGradations(dimension, gradations, grads);
        int gradation = grads[0];  

        m_tensors[dimension].setWeightsSelective(grads, weights);

        
}


void TensorHandler::setShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights, bool debug){

        std::vector<int> grads = std::vector<int>();        
        getRelevantGradations(dimension, gradations, grads);
        int gradation = grads[0];  

        if(debug){
                cout << "gradation is " << gradation << endl;
                cout << "BEFORE" << endl;
                cout << m_shadowTensors[dimension];
        }

        m_shadowTensors[dimension].setWeights(grads, weights, true);
        
        if(debug){
                cout << "AFTER" << endl;
                cout << m_shadowTensors[dimension];        
        }
}

// saves shadowed weights for given dimension and gradations in normal tensor...
void TensorHandler::storeShadowWeights(int dimension, std::vector<int> gradations, bool debug){

        std::vector<int> relevantGradations = std::vector<int>();
        getRelevantGradations(dimension, gradations, relevantGradations);
               
        
        std::vector<int> iparams = std::vector<int>();
        m_tensors[dimension].getIndependentParams(iparams);
        
        if(debug){
                cout << "BEFORE" << endl;
                cout << m_tensors[dimension];
        }
        
        
        const int numcoords = relevantGradations.size() + 1;
        int* coords = new int[numcoords];
        for(int i = 0; i<numcoords-1; i++){
                coords[i] = relevantGradations[i];
        }
        coords[numcoords - 1] = 0;

        double w; int c;
        for(int i=0; i<mv_numParameters[dimension]; i++){
                coords[numcoords-1] = i;
                
                if(iparams[i]==1){ // if parameter is independent, then we must store it in m_tensors[dimension]...
                
                        w = getElementProtected(dimension, coords);                        
                        m_tensors[dimension].setElement(coords, w);
                        
                }
        }
                

        m_tensors[dimension].hosvdTuningModel(false); //do not reset tensor...
        
        if(debug){
                cout << "AFTER" << endl;
                cout << m_tensors[dimension];        
        } 

		delete[] coords;


}


void TensorHandler::getWeightNames(int dimension, std::vector< std::string >& weightnames){

	m_tensors[dimension].getWeightNames(weightnames);

}


void TensorHandler::getIndependentParamNames(int dimension, std::vector< std::string >& paramnames){

        m_tensors[dimension].getIndependentParamNames(paramnames);

}

void TensorHandler::getInstrumentNames(std::vector<std::string>& resu){
        resu.clear();
        
        for(int i = 0; i<mv_instrumentNames.size(); i++){
                resu.push_back(mv_instrumentNames[i]);
        }

}


bool TensorHandler::isTensorDefined(int dimension){


	return m_tensorsDefined[dimension];

}


/*void TensorHandler::saveManipulations(int dimension, const std::string& filename){


	std::ofstream ofs(filename.c_str());
	boost::archive::text_oarchive oa(ofs);

	oa << m_tensors[dimension] << m_shadowTensors[dimension];


}

void TensorHandler::loadManipulations(int dimension, const std::string& filename){

	std::ifstream ifs(filename.c_str());
	boost::archive::text_iarchive ia(ifs);

	ia >> m_tensors[dimension] >> m_shadowTensors[dimension];

}*/


void TensorHandler::swapGradations(int dimension, int gradation1, int gradation2){

	m_tensors[dimension].swapGradations(gradation1, gradation2);
	m_shadowTensors[dimension].swapGradations(gradation1, gradation2);
}

void TensorHandler::copyGradations(int dimension, int gradation1, int gradation2){

	m_tensors[dimension].copyGradation(gradation1, gradation2);
	m_shadowTensors[dimension].copyGradation(gradation1, gradation2);
}


/*void TensorHandler::interpolateManipulations(int dimension, int gradation1, int gradation2,
		int resultGradation, double degree, double factor){

	m_tensors[dimension].interpolateGradations(gradation1, gradation2, resultGradation, degree, factor);

}*/

/*void TensorHandler::getGradation(int dimension, int gradation, std::vector<double>& params){
	m_tensors[dimension].getGradation(gradation, params);

}

void TensorHandler::setGradation(int dimension, int gradation, std::vector<double>& params){
	m_tensors[dimension].setGradation(gradation, params);

}*/



/*void TensorHandler::saveGradation(int dimension, int gradation, const std::string& filename){

	m_tensors[dimension].saveGradation(gradation, filename);
	m_shadowTensors[dimension].saveGradation(gradation, filename);

}

void TensorHandler::loadGradation(int dimension, int gradation, const std::string& filename){

	m_tensors[dimension].loadGradation(gradation, filename);
	m_shadowTensors[dimension].loadGradation(gradation, filename);
}*/

void TensorHandler::saveChannel(const std::string& filename){

                ofstream outstream;
                outstream.open(filename.c_str());
                
                
                for(int i=0; i<mv_dimensionNames.size(); i++){
                
                
                        if(mv_hasSound[i]){
                
                                std::vector<int> gradations = std::vector<int>();
                                for(int j=0; j<mv_dimensionNames.size(); j++){
                                        gradations.push_back(0);
                                }
                        
                                outstream << mv_dimensionNames[i] << " = " << std::endl << "{" << std::endl;
                                outstream << "\ttensorContent = (" << std::endl;
                                
                                std::vector<int> deps = mv_dependencies[i];
                                std::vector<int> maxvals = std::vector<int>();
                                std::vector<int> grads = std::vector<int>();
                                
                                for(int j=0; j<deps.size(); j++){
                                        grads.push_back(1);
                                }
                                
                                int rowcnt = mv_numGradations[i];
                                maxvals.push_back(mv_numGradations[i]);
                                
                                for(int j=1; j<deps.size(); j++){
                                        rowcnt = rowcnt*mv_numGradations[deps[j]];
                                        maxvals.push_back(mv_numGradations[deps[j]]);
                                }
                                
                                for(int j=0; j<rowcnt; j++){
                                
                                        for(int k=0; k<deps.size(); k++){
                                                gradations[deps[k]] = grads[k] - 1;
                                        }
                                
                                        outstream << "\t\t{ elaboratedDimension = " << deps.size() << ";" << std::endl;
                                        outstream << "\t\tfixDimensions = (";
                                        
                                        for(int k=0; k<grads.size(); k++){
                                                outstream << grads[k] << ", "; 
                                        }
                                        outstream << "0);" << std::endl;
                                        
                                        outstream << "\t\tvalues = (";
                                        std::vector<double> perfparams = std::vector<double>();
                                        getPerformanceParameters(i, gradations, perfparams);
                                        
                                        for(int k=0; k<perfparams.size(); k++){
                                        
                                                double val = perfparams[k];
                                                std::ostringstream sstream;
                                                sstream << val;
                                                
                                                std::string strtof(".");
                                                int pos = sstream.str().find(strtof);
                                                
                                                if(pos == std::string::npos){
                                                        sstream << ".0";
                                                }
                                        
                                                if(k == perfparams.size() - 1){
                                                        outstream << sstream.str() << ");" << std::endl;        
                                                }else{
                                                        outstream << sstream.str() << ", ";
                                                }
                                        }
                                
                                        if(j == rowcnt-1){
                                                outstream << "\t\t}" << std::endl;
                                        }else{
                                                outstream << "\t\t}," << std::endl << std::endl;
                                        }
                                        
                                        incrementNaryCombination(maxvals, grads);
                                
                                }
                                
                                
                                outstream << "\t);" << std::endl;
                                outstream << "};" << std::endl;
                        
                        }
                
                }

                outstream.close();

}

/*void TensorHandler::insertGradation(int dimension, int afterGradation, const std::string& configfilename){

                ofstream outstream;
                outstream.open(configfilename.c_str());
                
                
                
                
                outstream.close();

}*/


void TensorHandler::performHosvd(int dimension){
	m_tensors[dimension].hosvdTuningModel();
}

void TensorHandler::performHosvdNonreset(int dimension){
	std::cout << "TensorHandler::performHosvdNonreset called" << std::endl;
	m_tensors[dimension].hosvdTuningModel(false);
}

void TensorHandler::performHooi(int dimension, double compensationScale){

	m_tensors[dimension].hooiTuningModel(compensationScale);

}

void TensorHandler::getLowerBounds(int dimension, std::vector<double>& lb, bool getJustShadow){

	lb.clear();
	
	if (getJustShadow) {
		std::vector<double> temp;
		std::vector<int> iparams;
	
		m_tensors[dimension].getLowerBounds(temp);
		m_tensors[dimension].getIndependentParams(iparams);
		
		for (int i=0; i<temp.size(); i++) {
			if (iparams[i]==1) {
				lb.push_back(temp[i]);
			}
		}
	}
	else {
		m_tensors[dimension].getLowerBounds(lb);
	}

}

void TensorHandler::getUpperBounds(int dimension, std::vector<double>& ub, bool getJustShadow){
	
	ub.clear();
	
	if (getJustShadow) {
		std::vector<double> temp;
		std::vector<int> iparams;
		
		m_tensors[dimension].getUpperBounds(temp);
		m_tensors[dimension].getIndependentParams(iparams);
		
		for (int i=0; i<temp.size(); i++) {
			if (iparams[i]==1) {
				ub.push_back(temp[i]);
			}
		}		
	}
	else {
		m_tensors[dimension].getUpperBounds(ub);
	}

	
}


void TensorHandler::getParamUnits(int dimension, std::vector<std::string>& resu, bool getJustShadow){

                resu.clear();
        
	if (getJustShadow) {
		std::vector<std::string> temp;
		std::vector<int> iparams;
		
		m_tensors[dimension].getUnits(temp);
		m_tensors[dimension].getIndependentParams(iparams);
		
		for (int i=0; i<temp.size(); i++) {
			if (iparams[i]==1) {
			        resu.push_back(temp[i]);
			}
		}		
	}
	else {
		m_tensors[dimension].getUnits(resu);
	}

}


void TensorHandler::createTensors(){


	
}

void TensorHandler::getPerformanceParameters(int dimension, std::vector<int> gradations, std::vector<double>& performanceParameters){
        
        std::vector<int> relevantGradations = std::vector<int>();
        getRelevantGradations(dimension, gradations, relevantGradations);
        
        performanceParameters.clear();        
        
        std::vector<int> iparams = std::vector<int>();
        std::vector<int> unshadowedparams = std::vector<int>();
        m_tensors[dimension].getIndependentParams(iparams);
        m_tensors[dimension].getUnshadowedIndependentParams(unshadowedparams);
        
        const int numparams = iparams.size();
        const int numcoords = relevantGradations.size() + 1;
        
        int* coords = new int[numcoords];
        for(int i = 0; i<numcoords-1; i++){
                coords[i] = relevantGradations[i];
        }
        coords[numcoords - 1] = 0;

       
        double w;
        
        for(int i=0; i<numparams; i++){
        
                coords[numcoords-1] = i;
                
                if(iparams[i]==1){ // if parameter is independent
                        w = getElementProtected(dimension, coords);
                        performanceParameters.push_back(w);
                }else{
                        w = getElement(dimension, coords);
                        performanceParameters.push_back(w);
                }
        }

		delete[] coords;

}


void TensorHandler::getDimensionNames(std::vector< std::string >& dimnames){
        dimnames.clear();
        for(int i = 0; i<mv_dimensionNames.size(); i++){
                dimnames.push_back(mv_dimensionNames[i].c_str());
        }

}

void TensorHandler::getDimensionNamesWithSound(std::vector< std::string >& dimnames){

        dimnames.clear();
        for(int i = 0; i<mv_dimensionNames.size(); i++){
                if(mv_hasSound[i]){
                        dimnames.push_back(mv_dimensionNames[i].c_str());
                }
        }

}

void TensorHandler::getNumberGradations(std::vector< int>& numgrads){
        numgrads.clear();
        for(int i = 0; i<mv_numGradations.size(); i++){
                numgrads.push_back(mv_numGradations[i]);
        }
}

void TensorHandler::getDimensionInxesWithSound(std::vector<int>& diminxes){
        diminxes.clear();
        for(int i = 0; i<mv_hasSound.size(); i++){
                if(mv_hasSound[i]){
                        diminxes.push_back(i);
                }
        }

}


void TensorHandler::incrementNaryCombination(std::vector<int>& maxvalues, std::vector<int>& currentNaryAndResu){

        int numvalues = maxvalues.size();
        for(int i=1; i<numvalues+1; i++){
                if(currentNaryAndResu[numvalues-i] < maxvalues[numvalues-i]){
                        currentNaryAndResu[numvalues-i] = currentNaryAndResu[numvalues-i] + 1;
                        return;
                }
                else{
                        currentNaryAndResu[numvalues-i] = 1;
                }
        }

}


