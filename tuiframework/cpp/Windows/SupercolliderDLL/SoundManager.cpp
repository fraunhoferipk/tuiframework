/*
 * SoundManager.cpp
 *
 *  Created on: 2013.10.30.
 *      Author: Adam Csapo
 */

#include "./SoundManager.h"

namespace SuperColliderWrapper{

SoundManager::SoundManager(const string cfgFilename, const string gradationFilename): cfgFile(cfgFilename), gradFile(gradationFilename){

        // Read the channel configuration file. If there is an error, report it.
        Config cfg;

		cout << "configfile is " << cfgFilename << endl;
		cout << "gradationfile is " << gradationFilename << endl;
                
        try{
                cfg.readFile(cfgFile.c_str());
        }catch(const FileIOException &){
                std::cerr << "I/O error while reading file." << std::endl;
        }catch(const ParseException &pex){
                std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
        }


        const Setting& root = cfg.getRoot();
        // Parse IP address and port number
        try{
                const Setting &channel = root["channel"];
                channel.lookupValue("IPaddress", supercolliderIP);
                channel.lookupValue("audioPortNum", supercolliderAudioPort);
                channel.lookupValue("guiPortNum", supercolliderGuiPort);                
                
                
        }catch(const SettingNotFoundException &){
                cerr << "could not read supercollider IP or port number...";
                exit(0);
        }
                
        supercolliderAudioSocket.connectTo(supercolliderIP, supercolliderAudioPort);
        supercolliderGuiSocket.connectTo(supercolliderIP, supercolliderGuiPort);        
                
        if (!supercolliderAudioSocket.isOk()) {
                cerr << "Error connection to port " << supercolliderAudioPort << ": " << supercolliderAudioSocket.errorMessage() << "\n";
        } 

        if (!supercolliderGuiSocket.isOk()) {
                cerr << "Error connection to port " << supercolliderGuiPort << ": " << supercolliderGuiSocket.errorMessage() << "\n";
        } 


        mp_tensorHandler = new TensorHandler(cfgFile, gradFile);
        
        mv_instrumentNames = std::vector<std::string>();
        mp_tensorHandler->getInstrumentNames(mv_instrumentNames);

        startGUI();

        return;
}

SoundManager::~SoundManager(){

        delete mp_tensorHandler;

}


int SoundManager::getNumInstruments(){ //total number of dimensions (including those that have no sound)

        return mp_tensorHandler->getNumInstruments();
}

bool SoundManager::sendMessageToSupercollider(oscpkt::PacketWriter& pw, int replyWait, bool isAudio){

        bool ok;

        if (pw.isOk()) {
                if(isAudio){
                        cout << "calling audio client..." << endl;
                        ok = supercolliderAudioSocket.sendPacket(pw.packetData(), pw.packetSize());
                }else{
                        ok = supercolliderGuiSocket.sendPacket(pw.packetData(), pw.packetSize());
                }
        }

        if(ok){
                cout << "Client: sent " << "\n";
        }        
        
        // wait for a reply ?
        if (supercolliderAudioSocket.receiveNextPacket(replyWait)) {
                oscpkt::PacketReader pr(supercolliderAudioSocket.packetData(), supercolliderAudioSocket.packetSize());
                oscpkt::Message *incoming_msg;
                while (pr.isOk() && (incoming_msg = pr.popMessage()) != 0) {
                        //cout << "Client: received " << *incoming_msg << "\n";
                }
        }

		return true;

}


void SoundManager::startGUI(){
        oscpkt::PacketWriter pkt; 
        oscpkt::Message msg;                  
        std::string cmd("/setDimensionNamesTunable");                                
        msg.init(cmd);

        std::vector<std::string> dimnamesTunable = std::vector<std::string>();
        mp_tensorHandler->getDimensionNamesWithSound(dimnamesTunable);
        
        msg.pushInt32(dimnamesTunable.size());
        for(unsigned int i=0; i<dimnamesTunable.size(); i++){
                msg.pushStr(dimnamesTunable[i]);
        }

        pkt.addMessage(msg);
        sendMessageToSupercollider(pkt, 10, false);

        /////////////////////////

        oscpkt::PacketWriter pkt11; 
        oscpkt::Message msg11;                  
        std::string cmd11("/setDimensionNames");                                
        msg11.init(cmd11);

        std::vector<std::string> dimnames = std::vector<std::string>();
        mp_tensorHandler->getDimensionNames(dimnames);
        
        std::vector<int> hasSound = std::vector<int>();
        
        msg11.pushInt32(dimnames.size());
        for(unsigned int i=0; i<dimnames.size(); i++){
                msg11.pushStr(dimnames[i]);
                if(mp_tensorHandler->isTensorDefined(i)){
                        hasSound.push_back(1);
                }else{
                        hasSound.push_back(0);
                }
        }
        
        for(unsigned int i=0; i<dimnames.size(); i++){
                msg11.pushInt32(hasSound[i]);
        }
        
        

        pkt11.addMessage(msg11);
        sendMessageToSupercollider(pkt11, 10, false);

        /////////////////////////


        
        oscpkt::PacketWriter pkt2; 
        oscpkt::Message msg2;                  
        std::string cmd2("/setNumberGradations");                                
        msg2.init(cmd2);

        std::vector<int> numGradations = std::vector<int>();
        mp_tensorHandler->getNumberGradations(numGradations);
        
        msg2.pushInt32(numGradations.size());
        for(unsigned int i=0; i<numGradations.size(); i++){
                msg2.pushInt32(numGradations[i]);
        }

        pkt2.addMessage(msg2);
        sendMessageToSupercollider(pkt2, 10, false);


        /////////////////////////
        
        oscpkt::PacketWriter pkt3; 
        oscpkt::Message msg3;                  
        std::string cmd3("/setIndependentParameters");                                
        msg3.init(cmd3);

        int numInstruments = mp_tensorHandler->getNumInstruments();
        msg3.pushInt32(numInstruments);

        std::vector<std::vector<std::string> > weightNamess = std::vector<std::vector<std::string> >(numInstruments);
        std::vector<std::vector<double> > lowerBoundss = std::vector<std::vector<double> >(numInstruments);
        std::vector<std::vector<double> > upperBoundss = std::vector<std::vector<double> >(numInstruments);
        std::vector<std::vector<std::string> > paramUnitss = std::vector<std::vector<std::string> >(numInstruments);

        int dimension;
        
        for(int i=0; i<numInstruments; i++){
                if(mp_tensorHandler->isTensorDefined(i)){

                        dimension = i;        
                        weightNamess[i] = std::vector<std::string>();
                        lowerBoundss[i] = std::vector<double>();
                        upperBoundss[i] = std::vector<double>();
                        paramUnitss[i] = std::vector<std::string>();
                                   
                        mp_tensorHandler->getIndependentParamNames(dimension, weightNamess[i]);
                        mp_tensorHandler->getLowerBounds(dimension, lowerBoundss[i], true);
                        mp_tensorHandler->getUpperBounds(dimension, upperBoundss[i], true);
                        mp_tensorHandler->getParamUnits(i, paramUnitss[i], true);
                        
                        msg3.pushInt32(weightNamess[i].size());
                }
                else{
                        weightNamess[i] = std::vector<std::string>();
                        lowerBoundss[i] = std::vector<double>();
                        upperBoundss[i] = std::vector<double>();
                        weightNamess[i].clear();
                        lowerBoundss[i].clear();
                        upperBoundss[i].clear();
                        msg3.pushInt32(0);
                }
        }
        
        //for(int i=0; i<dimInxesWithSound.size(); i++){
        for(int i=0; i<numInstruments; i++){
                for(unsigned int j=0; j<weightNamess[i].size(); j++){
                        msg3.pushStr(weightNamess[i][j]);                        
                }
        }
        
        for(int i=0; i<numInstruments; i++){
                for(unsigned int j=0; j<weightNamess[i].size(); j++){        
                        msg3.pushDouble(lowerBoundss[i][j]);
                        msg3.pushDouble(upperBoundss[i][j]);
                }
        }
        
        for(int i=0; i<numInstruments; i++){
                for(unsigned int j=0; j<weightNamess[i].size(); j++){
                        msg3.pushStr(paramUnitss[i][j]);
                }
        }

        pkt3.addMessage(msg3);
        sendMessageToSupercollider(pkt3, 10, false);        
        
        ////////////////////////////////
        
        oscpkt::PacketWriter pkt20;         
        oscpkt::Message msg20;
        std::string cmd20("/startGUI");                        
        msg20.init(cmd20);
        
        pkt20.addMessage(msg20);
        sendMessageToSupercollider(pkt20, 10, false);        

}



void SoundManager::playTemporary(int dimension, std::vector<int> gradations, std::vector<double> hosvdParams, std::vector<double> independentParams){

        std::vector<int> relevantGradations = std::vector<int>();
        mp_tensorHandler->getRelevantGradations(dimension, gradations, relevantGradations);
        
        int gradation = relevantGradations[0];

        std::vector<double> prevHosvdWeights = std::vector<double>();        
        mp_tensorHandler->getWeights(dimension, gradation, prevHosvdWeights);

        
        std::vector<double> prevIndependentWeights = std::vector<double>();
        mp_tensorHandler->getShadowWeights(dimension, gradations, prevIndependentWeights, true);
        
        
        //now set new weights, get playing parameters, and reset original weights before returning.
        mp_tensorHandler->setWeights(dimension, gradation, hosvdParams);
        mp_tensorHandler->setShadowWeights(dimension, gradations, independentParams);// ,true); only when debugging...
        
        
        std::vector<double> performanceParameters = std::vector<double>();
        mp_tensorHandler->getPerformanceParameters(dimension, gradations, performanceParameters);
        
        cout << "performance parameters are: ";
        for(unsigned int i=0; i<performanceParameters.size();i++){
                cout << performanceParameters[i] << ", ";
        }
        cout << endl << endl;
        
        //reset original weights...
        mp_tensorHandler->setWeights(dimension, gradation, prevHosvdWeights);
        mp_tensorHandler->setShadowWeights(dimension, gradations, prevIndependentWeights);
        
        
        //now send OSC command to play sound...
        oscpkt::PacketWriter pkt; 
        oscpkt::Message msg;                  
        std::string cmd(mv_instrumentNames[dimension]);                                
        msg.init(cmd);

        msg.pushInt32(performanceParameters.size());
        for(unsigned int i=0; i<performanceParameters.size(); i++){
                msg.pushDouble(performanceParameters[i]);
        }
        
        pkt.addMessage(msg);
        cout << "command name is: " << mv_instrumentNames[dimension] << endl;
        sendMessageToSupercollider(pkt, 10, true);          

}


void SoundManager::storeManipulations(int dimension, std::vector<int> gradations, std::vector<double> hosvdParams, std::vector<double> independentParams){
        //same as playTemporary, only 1. original weights are not reset, and 2. no sound is played...        
        
        //std::vector<int> relevantGradations = std::vector<int>();
        //mp_tensorHandler->getRelevantGradations(dimension, gradations, relevantGradations);
        //int gradation = relevantGradations[0];
        
        
        mp_tensorHandler->setWeights(dimension, gradations, hosvdParams);
        mp_tensorHandler->setShadowWeights(dimension, gradations, independentParams);// ,true); only when debugging...
        //in playTemporary, setShadowWeights was enough, because getPerformanceParameters afterwards took shadowed parameters into consideration
        //by properly calling getElementProtected(...) when necessary.  Here, on the other hand, we want to actually change the regular, hosvded tensor as well!
        // --> we need something like the inverse operation of getPerformanceParameters(...)
        mp_tensorHandler->storeShadowWeights(dimension, gradations, true);       // only when debugging
        
        
}

void SoundManager::play(int dimension, std::vector<int> gradations){
        
        std::vector<double> performanceParameters = std::vector<double>();
        mp_tensorHandler->getPerformanceParameters(dimension, gradations, performanceParameters);
        
        cout << "performance parameters are: ";
        for(unsigned int i=0; i<performanceParameters.size();i++){
                cout << performanceParameters[i] << ", ";
        }
        cout << endl << endl;
        
        //now send OSC command to play sound...
        oscpkt::PacketWriter pkt; 
        oscpkt::Message msg;                  
        std::string cmd(mv_instrumentNames[dimension]);                                
        msg.init(cmd);

        msg.pushInt32(performanceParameters.size());
        for(unsigned int i=0; i<performanceParameters.size(); i++){
                msg.pushDouble(performanceParameters[i]);
        }
        
        pkt.addMessage(msg);
        cout << "command name is: " << mv_instrumentNames[dimension] << endl;
        sendMessageToSupercollider(pkt, 10, true);
        
}

int SoundManager::stop(){

        //send OSC command to stop sound...
        oscpkt::PacketWriter pkt; 
        oscpkt::Message msg;                  
        msg.init("/stop");
        
        pkt.addMessage(msg);
        sendMessageToSupercollider(pkt, 10, true);
		return 0;
}

int SoundManager::quit(){

                if(supercolliderAudioSocket.isOk()) {
                        
                        oscpkt::PacketWriter pkt; 
                        oscpkt::Message msg;
                         
                        pkt.addMessage(msg.init("/quit"));
                        sendMessageToSupercollider(pkt, 10);
                        
                }

	std::cout << "sound has been played..." << std::endl;
	return 0;
}


/*void SoundManager::setMask(std::vector<bool> mask){

	//mp_filegen->setMask(mask);

}

void SoundManager::getMask(std::vector<bool>& mask){

	//mp_filegen->getMask(mask);

}*/

void SoundManager::getNumberGradations(std::vector<int>& numgrads){

	mp_tensorHandler->getNumberGradations(numgrads);
}

void SoundManager::getLowerBounds(int dimension, std::vector<double>& lb, bool getJustShadow){

	mp_tensorHandler->getLowerBounds(dimension, lb, getJustShadow);

}

void SoundManager::getUpperBounds(int dimension, std::vector<double>& ub, bool getJustShadow){

	mp_tensorHandler->getUpperBounds(dimension, ub, getJustShadow);

}


void SoundManager::getWeights(int dimension, int gradation, std::vector<double>& weights){

	mp_tensorHandler->getWeights(dimension, gradation, weights);

}


void SoundManager::getShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights, bool keepNotUnshadowedParamsIntact){
        
        mp_tensorHandler->getShadowWeights(dimension, gradations, weights, keepNotUnshadowedParamsIntact);

}
	
void SoundManager::getIndependentParams(int dimension, std::vector<int>& iparams){

	mp_tensorHandler->getIndependentParams(dimension, iparams);
}
	

void SoundManager::getWeightNames(int dimension, std::vector< std::string >& weightnames){

	mp_tensorHandler->getWeightNames(dimension, weightnames);
}



void SoundManager::getDimensionInxesWithSound(std::vector<int>& diminxes){
                
                mp_tensorHandler->getDimensionInxesWithSound(diminxes);

}

void SoundManager::setWeights(int dimension, int gradation, std::vector<double>& weights){

	mp_tensorHandler->setWeights(dimension, gradation, weights);

}

void SoundManager::setShadowWeights(int dimension, std::vector<int> gradations, std::vector<double>& weights){

	mp_tensorHandler->setShadowWeights(dimension, gradations, weights);

}


bool SoundManager::isTensorDefined(int dimension){

	return mp_tensorHandler->isTensorDefined(dimension);

}

/*void SoundManager::saveManipulations(int dimension, const std::string& filename){

	mp_tensorHandler->saveManipulations(dimension, filename);

}

void SoundManager::loadManipulations(int dimension, const std::string& filename){

	mp_tensorHandler->loadManipulations(dimension, filename);
}*/

void SoundManager::swapGradations(int dimension, int gradation1, int gradation2){
	mp_tensorHandler->swapGradations(dimension, gradation1, gradation2);
}

void SoundManager::copyGradations(int dimension, int gradation1, int gradation2){
	mp_tensorHandler->copyGradations(dimension, gradation1, gradation2);
}

/*void SoundManager::interpolateManipulations(int dimension,
		int gradation1, int gradation2, int resultGradation,
		double degree, double factor){

	mp_tensorHandler->interpolateManipulations(dimension, gradation1, gradation2, resultGradation,
			degree, factor);
}*/


/*void SoundManager::getGradation(int dimension, int gradation, std::vector<double>& params){
	mp_tensorHandler->getGradation(dimension, gradation, params);

}


void SoundManager::saveGradation(int dimension, int gradation, const std::string& filename){
	mp_tensorHandler->saveGradation(dimension, gradation, filename);
}

void SoundManager::loadGradation(int dimension, int gradation, const std::string& filename){
	mp_tensorHandler->loadGradation(dimension, gradation, filename);
}*/


 void SoundManager::saveChannel(const std::string& filename){
 
        mp_tensorHandler->saveChannel(filename);
}

/*void SoundManager::insertGradation(int dimension, int afterGradation, const std::string& configfilename){

        mp_tensorHandler->insertGradation(dimension, afterGradation, configfilename);
}*/


void SoundManager::performHosvd(int dimension){

	mp_tensorHandler->performHosvd(dimension);

}

void SoundManager::performHosvdNonreset(int dimension){

	std::cout << "SoundManager::performHosvdNonreset called" << std::endl;
	mp_tensorHandler->performHosvdNonreset(dimension);
}

void SoundManager::performHooi(int dimension, double compensationScale){

	std::cout << "SoundManager::performHooi called" << std::endl;
	mp_tensorHandler->performHooi(dimension, compensationScale);

}



/*
// C-functions which create and dispose instantiations of the class

extern "C" int MY_DLL m_SoundManagerDefault(SoundTypes st, const string& path)
{
    return (int) new SoundManager(st, path);
}


extern "C" int MY_DLL m_SoundManager(const string& path, std::vector< std::string > filenames, SoundTypes st)
{
    return (int) new SoundManager(st, filenames, path);
}


extern "C" void MY_DLL m_DisposeSoundManager(SoundManager* pObject)
{
    if(pObject != NULL)
    {
        delete pObject;
        pObject = NULL;
    }
}


// C-functions which act as a bridge for each public method

extern "C" void MY_DLL m_play(SoundManager* pObject, int* gradations, int numGradations)
{
    if(pObject != NULL)
    {
        pObject->play(gradations, numGradations);
    }
}

*/

} //end namespace SoundWrapper

