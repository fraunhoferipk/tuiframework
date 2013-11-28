

#ifndef CHANNELPARAMETER_HPP_
#define CHANNELPARAMETER_HPP_

#include <string>
#include <iostream>

class ChannelParameter{
public:

        ChannelParameter(std::string name, double maxval, double minval, bool isIndependent, bool isUnshadowed, std::string unitName);
        std::string getName();
        double getMaxval();
        double getMinval();
        bool isIndependent();
        bool isUnshadowed();
        std::string getUnit();
        
        friend std::ostream& operator<<(std::ostream& out, const ChannelParameter& cp){
                return out << cp.name << ": " << cp.minval << "--" << cp.maxval << ", unit: " << cp.unit << ", independent: " << cp.independent << ", unshadowed: " << cp.unshadowed;
        }
        
private:

        std::string name;
        std::string unit;
        double maxval, minval;
        bool independent, unshadowed;
};

#endif