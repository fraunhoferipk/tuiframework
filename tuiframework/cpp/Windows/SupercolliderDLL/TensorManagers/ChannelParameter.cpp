
#include "ChannelParameter.h"

ChannelParameter::ChannelParameter(std::string nm, double max, double min, bool indep, bool unsh, std::string unitName){

                name = nm;
                maxval = max;
                minval = min;
                independent = indep;
                unshadowed = unsh;
                unit = unitName;

}

std::string ChannelParameter::getName(){
                return name;
}

double ChannelParameter::getMaxval(){
                return maxval;
}

double ChannelParameter::getMinval(){
                return minval;
}

bool ChannelParameter::isIndependent(){
                return independent;
}

bool ChannelParameter::isUnshadowed(){
                return unshadowed;
}

std::string ChannelParameter::getUnit(){
                return unit;
}