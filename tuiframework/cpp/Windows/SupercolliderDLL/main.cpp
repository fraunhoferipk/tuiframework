/*
 * mainNORMAL.cpp
 *
 *  Created on: 2011.04.09.
 *      Author: adam
 */


#include <vector>
#include <cmath>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "./OSC/oscpkt.h"
#include "./OSC/udp.h"
#include "SoundManager.h"
#include "ExportedFunctions.h"
using namespace std;
using namespace oscpkt;

const int PORT_NUM = 57130;

int main (int argc, char* argv[]) {

        if(argc < 2){
                cout << "Please supply the name of the configuration file" << endl;
                exit(0);
        }

        cout << "Opening file " << argv[1] << endl;
        
        SuperColliderWrapper::SoundManager* mp_soundmgr;
        
        
        mp_soundmgr = new SuperColliderWrapper::SoundManager(argv[1], argv[2]);

        UdpSocket sock; 
        sock.bindTo(PORT_NUM);
        
        if (!sock.isOk()) {
                cerr << "Error opening port " << PORT_NUM << ": " << sock.errorMessage() << "\n";
        } else {
                cout << "Server started, will listen to packets on port " << PORT_NUM << std::endl;
                PacketReader pr;
                PacketWriter pw;
                
                //std::vector<int> diminxes = std::vector<int>();
                std::vector<double> hosvdWeights = std::vector<double>();
                std::vector<double> shadowWeights = std::vector<double>();                                
                int diminx;
                
                bool messageHandled;
                
                while (sock.isOk()) {      
                        if (sock.receiveNextPacket(30 /* timeout, in ms */)) {
                                pr.init(sock.packetData(), sock.packetSize());
                                oscpkt::Message *msg;
                                while (pr.isOk() && (msg = pr.popMessage()) != 0) {
                                        int dimension, numDimensions, parameter, gradation, gradation2, hosvdRnk, numParams;
                                        std::string gradationsString, hosvdStr, paramStr, filepath;
                                        std::vector<int> gradations;
                                        float value;
                                        messageHandled = false; 
                                        if (msg->match("/getWeights").popInt32(dimension).popInt32(numDimensions).popStr(gradationsString).isOkNoMoreArgs()) {
                                        
                                                cout << "Server: received /getWeights from " << sock.packetOrigin() << endl;
                                        

                                                int pos = 0; int newpos;
                                                int len = gradationsString.size();
                                                gradations.clear();
                                                const std::string pattern = ";";
                                                std::string toparse = gradationsString.substr(pos, len);
                                                for(int i=0; i<numDimensions; i++){
                                                        newpos = toparse.find(pattern);
                                                        int val = atoi(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        gradations.push_back(val);
                                                }
                                                
                                                gradation = gradations[dimension];                                                  
                                                
                                                mp_soundmgr->getWeights(dimension, gradation, hosvdWeights);
                                                mp_soundmgr->getShadowWeights(dimension, gradations, shadowWeights);
                                                
                                                
                                                Message repl; repl.init("/setWeights").pushInt32(hosvdWeights.size());
                                                for(int i = 0; i<hosvdWeights.size(); i++){
                                                        repl.pushDouble(hosvdWeights[i]);
                                                }

                                                for(int i = 0; i<shadowWeights.size(); i++){
                                                        repl.pushDouble(shadowWeights[i]);
                                                }
                                                
                                                pw.init().addMessage(repl);
                                                sock.sendPacketTo(pw.packetData(), pw.packetSize(), sock.packetOrigin());
                                                messageHandled = true;
                                        }
                                        
                                        if (msg->match("/playWeights").popInt32(dimension).popInt32(hosvdRnk).popInt32(numParams).popStr(hosvdStr).popStr(paramStr).popStr(gradationsString).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /playWeights from " << sock.packetOrigin() << endl;
                                                
                                                numDimensions = mp_soundmgr->getNumInstruments();

                                                int pos = 0; int newpos;
                                                int len = gradationsString.size();
                                                gradations.clear();
                                                const std::string pattern = ";";
                                                std::string toparse = gradationsString.substr(pos, len);
                                                for(int i=0; i<numDimensions; i++){
                                                        newpos = toparse.find(pattern);
                                                        int val = atoi(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        gradations.push_back(val);
                                                }
                                                
                                                std::vector<double> hosvdParams = std::vector<double>();
                                                hosvdParams.clear();
                                                
                                                pos = 0; newpos = 0;
                                                len = hosvdStr.size();
                                                toparse = hosvdStr.substr(pos, len);
                                                for(int i=0; i<hosvdRnk; i++){
                                                        newpos = toparse.find(pattern);
                                                        double val = atof(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        hosvdParams.push_back(val);
                                                }                                                                                            

                                                std::vector<double> independentParams = std::vector<double>();
                                                independentParams.clear();

                                                pos = 0; newpos = 0;
                                                len = paramStr.size();
                                                toparse = paramStr.substr(pos, len);
                                                for(int i=0; i<numParams; i++){
                                                        newpos = toparse.find(pattern);
                                                        double val = atof(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        independentParams.push_back(val);
                                                }
                                                
                                                mp_soundmgr->playTemporary(dimension, gradations, hosvdParams, independentParams);
                                                
                                                messageHandled = true;
                                        
                                        }

                                        if (msg->match("/playSound").popInt32(dimension).popStr(gradationsString).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /playSound from " << sock.packetOrigin() << endl;
                                                
                                                numDimensions = mp_soundmgr->getNumInstruments();
                                                int pos = 0; int newpos;
                                                int len = gradationsString.size();
                                                gradations.clear();
                                                const std::string pattern = ";";
                                                std::string toparse = gradationsString.substr(pos, len);
                                                for(int i=0; i<numDimensions; i++){
                                                        newpos = toparse.find(pattern);
                                                        int val = atoi(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        gradations.push_back(val);
                                                }
                                                
                                                
                                                mp_soundmgr->play(dimension, gradations);
                                                
                                                messageHandled = true;
                                        
                                        }


                                        if (msg->match("/keepWeights").popInt32(dimension).popInt32(hosvdRnk).popInt32(numParams).popStr(hosvdStr).popStr(paramStr).popStr(gradationsString).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /keepWeights from " << sock.packetOrigin() << endl;
                                                
                                                numDimensions = mp_soundmgr->getNumInstruments();

                                                int pos = 0; int newpos;
                                                int len = gradationsString.size();
                                                gradations.clear();
                                                const std::string pattern = ";";
                                                std::string toparse = gradationsString.substr(pos, len);
                                                for(int i=0; i<numDimensions; i++){
                                                        newpos = toparse.find(pattern);
                                                        int val = atoi(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        gradations.push_back(val);
                                                }
                                                
                                                std::vector<double> hosvdParams = std::vector<double>();
                                                hosvdParams.clear();
                                                
                                                pos = 0; newpos = 0;
                                                len = hosvdStr.size();
                                                toparse = hosvdStr.substr(pos, len);
                                                for(int i=0; i<hosvdRnk; i++){
                                                        newpos = toparse.find(pattern);
                                                        double val = atof(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        hosvdParams.push_back(val);
                                                }                                                                                            

                                                std::vector<double> independentParams = std::vector<double>();
                                                independentParams.clear();

                                                pos = 0; newpos = 0;
                                                len = paramStr.size();
                                                toparse = paramStr.substr(pos, len);
                                                for(int i=0; i<numParams; i++){
                                                        newpos = toparse.find(pattern);
                                                        double val = atof(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        independentParams.push_back(val);
                                                }
                                                
                                                mp_soundmgr->storeManipulations(dimension, gradations, hosvdParams, independentParams);
                                                
                                                messageHandled = true;
                                        
                                        }

                                        
                                        if (msg->match("/modifyWeights").popInt32(dimension).popInt32(numDimensions).popStr(gradationsString).popInt32(parameter).popFloat(value).isOkNoMoreArgs()){
                                
                                        
                                                cout << "Server: received /modifyWeights from " << sock.packetOrigin() << endl;
                                                
                                                
                                                int pos = 0; int newpos;
                                                int len = gradationsString.size();
                                                gradations.clear();
                                                const std::string pattern = ".";
                                                std::string toparse = gradationsString.substr(pos, len);
                                                for(int i=0; i<numDimensions; i++){
                                                        newpos = toparse.find(pattern);
                                                        int val = atoi(toparse.substr(pos, newpos).c_str());
                                                        toparse = toparse.substr(newpos+1, toparse.size() - newpos);
                                                        gradations.push_back(val);
                                                }
                                                
                                                gradation = gradations[dimension];                                         

                                                cout << "Dimension is: " << dimension << ", " << endl;
                                                cout << "Gradations are: " << gradationsString << endl;
                                                cout << "Gradation in this dimension is " << gradation << endl;
                                                cout << "Parameter number and value are: " << parameter << ", " << value << endl;
                                                
                                                hosvdWeights.clear();
                                                shadowWeights.clear();
                                                
                                                mp_soundmgr->getWeights(dimension, gradation, hosvdWeights);
                                                mp_soundmgr->getShadowWeights(dimension, gradations, shadowWeights);

                                                cout << "hosvd weights were:" << endl;
                                                for(int i=0; i<hosvdWeights.size(); i++){
                                                        cout << hosvdWeights[i] << ", ";
                                                }
                                                cout << endl << endl;
                                                
                                                cout << "shadow weights were:" << endl;
                                                for(int i=0; i<shadowWeights.size(); i++){
                                                        cout << shadowWeights[i] << ", ";
                                                }
                                                cout << endl;
                                                
                                                std::vector<double> newHosvdWeights = std::vector<double>();
                                                std::vector<double> newShadowWeights = std::vector<double>();
                                                
                                                newHosvdWeights.clear();
                                                newShadowWeights.clear();
                                                           
                                                for(int i = 0; i<hosvdWeights.size(); i++){
                                                        newHosvdWeights.push_back(hosvdWeights[i]);
                                                }
                                                newHosvdWeights[parameter] = value;                                         
                                                                                                
                                                mp_soundmgr->setWeights(dimension, gradation, newHosvdWeights);
                                                std::vector<double> newestHosvdWeights;
                                                mp_soundmgr->getWeights(dimension, gradation, newestHosvdWeights);                                                
                                                mp_soundmgr->getShadowWeights(dimension, gradations, newShadowWeights); //problem used to be here  
                                                          
                                                cout << "hosvd weights are:" << endl;
                                                for(int i=0; i<newestHosvdWeights.size(); i++){
                                                        cout << newestHosvdWeights[i] << ", ";
                                                }
                                                cout << endl << endl;
                                                
                                                cout << "shadow weights are:" << endl;
                                                for(int i=0; i<newShadowWeights.size(); i++){
                                                        cout << newShadowWeights[i] << ", ";
                                                }
                                                cout << endl;   

                                                Message repl; repl.init("/refreshWeights").pushInt32(newShadowWeights.size());
                                                for(int i = 0; i<newShadowWeights.size(); i++){
                                                        repl.pushDouble(newShadowWeights[i]);
                                                }
                                                
                                                pw.init().addMessage(repl);
                                                sock.sendPacketTo(pw.packetData(), pw.packetSize(), sock.packetOrigin());
                                                messageHandled = true;
                                                
                                                mp_soundmgr->setWeights(dimension, gradation, hosvdWeights);                                                                                                
                                                
                                        }
                                        
                                        if (msg->match("/hooiDimension").popInt32(dimension).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /hooiDimension from " << sock.packetOrigin() << endl;
                                                
                                                mp_soundmgr->performHooi(dimension, 0.01);
                                                
                                                messageHandled = true;
                                        
                                        }
                                        
                                        if (msg->match("/saveChannel").popStr(filepath).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /saveChannel from " << sock.packetOrigin() << endl;
                                                cout << "filename is " << filepath << std::endl;
                                                
                                                mp_soundmgr->saveChannel(filepath);
                                                
                                                messageHandled = true;
                                        
                                        }
                                        
                                        if (msg->match("/copyGradation").popInt32(dimension).popInt32(gradation).popInt32(gradation2).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /copyGradation from " << sock.packetOrigin() << endl;
                                                cout << "dimension is " << dimension << std::endl;
                                                cout << "gradation1 is " << gradation << std::endl;
                                                cout << "gradation2 is " << gradation2 << std::endl;
                                                
                                                mp_soundmgr->copyGradations(dimension, gradation, gradation2);
                                                
                                                messageHandled = true;
                                        
                                        }         
                                        
                                        if (msg->match("/swapGradations").popInt32(dimension).popInt32(gradation).popInt32(gradation2).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /swapGradations from " << sock.packetOrigin() << endl;
                                                cout << "dimension is " << dimension << std::endl;
                                                cout << "gradation1 is " << gradation << std::endl;
                                                cout << "gradation2 is " << gradation2 << std::endl;
                                                
                                                mp_soundmgr->swapGradations(dimension, gradation, gradation2);
                                                
                                                messageHandled = true;
                                        
                                        }           
                                        
                                        
                                        /*if (msg->match("/insertGradation").popInt32(dimension).popInt32(gradation).popStr(filepath).isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /insertGradation from " << sock.packetOrigin() << endl;
                                                cout << "dimension is " << dimension << std::endl;
                                                cout << "gradation is " << gradation << std::endl;
                                                
                                                mp_soundmgr->insertGradation(dimension, gradation, filepath);
                                                
                                                //mp_soundmgr->saveChannel(filepath);
                                                
                                                messageHandled = true;
                                        
                                        }*/ 
                                        
                                        if (msg->match("/stopSound").isOkNoMoreArgs()){
                                        
                                                cout << "Server: received /stopSound from " << sock.packetOrigin() << endl;
                                                
                                                mp_soundmgr->stop();
                                                
                                                messageHandled = true;
                                        
                                        }                   
                                        
                                        if (msg->match("/quitProgram")){
                                                cout << "Server: received /quitProgram from " << sock.packetOrigin() << endl;
                                                messageHandled = true;
                                                cout << "exiting..." << endl;
                                                exit(0);
                                        }
                                        
                                        if(!messageHandled){
                                                //cout << "Server: unhandled message: " << *msg << "\n";
                                                cout << "Unhandled message" << endl;
                                        }
                                }
                        }
                }
        }

}
