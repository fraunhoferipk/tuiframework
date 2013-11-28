/*
 * SoundManager.hpp
 *
 *  Created on: 2011.01.13.
 *    Author: Adam Csapo
 */

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "./OSC/oscpkt.h"
#include "./OSC/udp.h"
#include "./TensorManagers/TensorHandler.h"
#include "./LibConfig/libconfig++.h"

using namespace std;
namespace SuperColliderWrapper{

class SoundManager{
public:

	SoundManager(const string cfgFilename, const string gradationFilename);
	~SoundManager();

	void play(int dimension, std::vector<int> gradations);
	
	//updates params based on tuning parameters, plays sound and 
	//resets params afterwards to not reflect tuning parameters
    void playTemporary(int dimension, 
		std::vector<int> gradations, 
		std::vector<double> hosvdParams, 
		std::vector<double> independentParams);	
	
	int stop();
	int quit();
	
	void startGUI();
	
	
	// GETTERS
	//total number of dimensions (including those that have no sound)	
	int getNumInstruments();
	
	//get weights from m_tensors[dimension].  Since tensors in m_tensors are hosvded, weights will contain a row of the first weight matrix
	void getWeights(int dimension, int gradation, std::vector<double>& weights);

	//get weights from m_shadowTensors[dimension].  Since tensors in m_tensors are not hosvded, weights will contain a set of raw parameters
	//if keepNotUnshadowedParamsIntact is true, then those parameters that are independent but not unshadowed are guaranteed to be returned
	//in unaltered form (if set to false, and the hosvd parameters are changed, then the newer version is returned... this is not the normal behavior)
	void getShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights, bool keepNotUnshadowedParamsIntact=true);
	
	//iparams will contain a vector of 1s and 0s.  1s mark the indices of params that are independent
	void getIndependentParams(int dimension, std::vector<int>& iparams);

	//lb will contain a vector of lower bounds for each independently tunable parameter
	void getLowerBounds(int dimension, std::vector<double>& lb, bool getJustShadow=false);
	
	//up will contain a vector of upper bounds for each independently tunable parameter
	void getUpperBounds(int dimension, std::vector<double>& ub, bool getJustShadow=false);

	//weightnames will contain a vector of strings that are the names of all params
	void getWeightNames(int dimension, std::vector< std::string >& weightnames);
	
	//returns maximum gradation number for all dimensions;
	void getNumberGradations(std::vector<int>& numgrads);

	// SETTERS
	
	//tensor is updated for given gradation, irrespective of other gradations
	void setWeights(int dimension, int gradation, std::vector<double>& weights);
	
	void setShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights);
	
	void storeManipulations(int dimension, std::vector<int> gradations, std::vector<double> hosvdParams, std::vector<double> independentParams);

	void swapGradations(int dimension, int gradation1, int gradation2);
	void copyGradations(int dimension, int gradation1, int gradation2);

	void saveChannel(const std::string& filename);

	void getDimensionInxesWithSound(std::vector<int>& diminxes);	

	void performHosvd(int dimension);
	void performHosvdNonreset(int dimension);
	void performHooi(int dimension, double compensationScale);
	bool isTensorDefined(int dimension);
	

private:

	TensorHandler* mp_tensorHandler;
	std::vector<std::string> mv_instrumentNames;
	const string cfgFile;
	const string gradFile;
	string supercolliderIP;
	int supercolliderAudioPort, supercolliderGuiPort;
	oscpkt::UdpSocket supercolliderAudioSocket;
	oscpkt::UdpSocket supercolliderGuiSocket;	

	bool sendMessageToSupercollider(oscpkt::PacketWriter& pw, int replyWait, bool isAudio=true);
};

}//end namespace SuperColliderWrapper

#endif /* SOUNDMANAGER_H_ */

