/*
    Copyright (C) 2010, 2011, 2012 The Fraunhofer Institute for Production Systems and
    Design Technology IPK. All rights reserved.

    This file is part of the TUIFramework library.
    It includes a software framework which contains common code
    providing generic functionality for developing applications
    with a tangible user interface (TUI).
    designed for creating applications with a tangible user interface (TUI).
    
    The TUIFramework library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The TUIFramework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with the TUIFramework.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "VectorList3d.h"


VectorList3d::VectorList3d()
 {
        m_vectorList = std::vector<Vector3d>();
 
 }

VectorList3d::VectorList3d(const std::vector<Vector3d>& v):m_vectorList(v)
{
        
}


VectorList3d::~VectorList3d() {}


double VectorList3d::size(){
        return m_vectorList.size();
}


const std::vector<Vector3d>& VectorList3d::getVectorList() const{
        return m_vectorList;    
}
// for testing purposes
void VectorList3d::printValues() {
    //std::printf("(x,y,z) = (%.3f, %.3f, %.3f)", this->x, this->y, this->z);
}

std::ostream & VectorList3d::serialize(std::ostream & os) const {

    os << m_vectorList.size();
    
    std::vector<Vector3d>::const_iterator i = m_vectorList.begin();    
    std::vector<Vector3d>::const_iterator e = m_vectorList.end();    
    while (i != e) {
        os << " " << *i ;
        ++i;
    }
    return os;
}

std::istream & VectorList3d::deSerialize(std::istream & is) {
    
    int i = 0;
    m_vectorList.clear();
    
    is >> i;
    std::cout << "==========> vector size" << i << std::endl;

    
    while (i != 0) {
        Vector3d v;
        v.deSerialize(is);
        std::cout << "==========> " << v << std::endl;
        this->m_vectorList.push_back(v);
        --i;
    }
    
    return is;
}



double VectorList3d::getAverageAngleChange(){


        double len1, len2, dp;
        Vector3d cp;
        double sinval, cosval, sum;
        int ctr;
        
        sum = 0.0;
        ctr = 0;

        std::vector<Vector3d>::const_iterator i = m_directionVectorList.begin();    
        std::vector<Vector3d>::const_iterator e = m_directionVectorList.end() - 1;    
        while (i != e) {

                Vector3d firstVector = Vector3d((*(i)).getX(), (*(i)).getY(), (*(i)).getZ());
                Vector3d secondVector = Vector3d((*(i+1)).getX(), (*(i+1)).getY(), (*(i+1)).getZ());

                len1 = firstVector.absoluteValue();
                len2 = secondVector.absoluteValue();
                
                dp = firstVector.dotProduct(secondVector);
                cp =  firstVector.crossProduct(secondVector);

                sinval = cp.absoluteValue() / (len1*len2);
                cosval = dp / (len1*len2);
                
                double newval;
                
                if(cosval > 0){
                        sum += sinval / 2;
                        std::cout << "change in angle: " << sinval/2 << std::endl;
                }else{
                        sum += 1 - (sinval / 2);
                        std::cout << "change in angle: " << 1 - sinval/2 << std::endl;
                }
                
                
                ctr++;
                
                ++i;
        }
        
        return sum / ctr;

}

void VectorList3d::computeDirections(){
        
        m_directionVectorList = std::vector<Vector3d>();
        
        double dx, dy, dz;
        double x1, y1, z1, x2, y2, z2;
        
        std::vector<Vector3d>::const_iterator i = m_vectorList.begin();    
        std::vector<Vector3d>::const_iterator e = m_vectorList.end() - 1;    
        while (i != e) {

                Vector3d destinationVector = Vector3d((*(i+1)).getX(), (*(i+1)).getY(), (*(i+1)).getZ());
                
                destinationVector.subtract(*i);
                
                m_directionVectorList.push_back(Vector3d(destinationVector.getX(), destinationVector.getY(), destinationVector.getZ()));
                
                
                ++i;
        }       
        
        //std::cout << "number of directions are: " << m_directionVectorList.size() << std::endl;

}



