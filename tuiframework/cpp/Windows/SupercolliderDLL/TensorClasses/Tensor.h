/*
 * Tensor.hpp
 *
 *  Created on: 2011.01.21.
 *      Author: Adam
 */

#ifndef TENSOR_HPP_
#define TENSOR_HPP_


#include <iostream>
#include <vector>
#include <ctime>
#include <assert.h>

#include "../TemplateNumericToolbox/tnt_126/tnt.h"

namespace SdmTensor{

class Tensor{


protected:


	TNT::Array2D<double> m_data;   //!< contains unfolding of tensor
	int m_unfoldedModeDimension;   //!< dimension along which tensor is unfolded at any given moment (first dimension is 0)
	int m_numDimensions;           //!< number of dimensions the tensor has
	std::vector<int> m_tensorSize; //!< size along each dimension of tensor


                
	void get2Dcoords(int* coordinates /**<[in] array of length m_numDimensions containing coordinates in all dimensions (values start with 0) */, 
	int* coords2D /**<[out] array of length 2 containing 2-D coordinates for unfolded tensor (values start from 0)*/);
	
	/*! Returns the coordinates (in m_numDimensions number of dimensions) corresponding to 2-dimensional coordinates
	 expressed for the two-dimensional current unfolding of the tensor (i.e., stored in m_unfoldedModeDimension)
	 */	
	void getNDcoords(int* coordinates /**[in] array of length 2 containing 2-D coordinates for unfolded tensor (values start from 0)*/, 
		int* coordsND /**[out] array of length m_numDimensions containing coordinates in all dimensions (values start from 0)*/);

	
	/*! Returns the coordinates (in m_numDimensions number of dimensions) corresponding to 2-dimensional coordinates
	 expressed for a two-dimensional current unfolding of the tensor specified by rowdim
	 */	
	void getNDcoordsGeneric(int* coordinates /**[in] array of length 2 containing 2-D coordinates for unfolded tensor (values start from 0)*/, 
		int rowdim /**[in] dimension in which unfolding should be performed (dimensions start with 0)*/, 
		int* coordsND) /**[out] array of length m_numDimensions containing coordinates in all dimensions (values start from 0)*/;

                
                void getRow(int row, std::vector<double>& resu);


public:

	/*!
	Constructor that creates an empty tensor with no initialized members.
	*/
	Tensor();

	/*!
	 Constructor that creates a tensor of specified dimensions, either filled with 
	 zeros (isRandom=false) or with random values between 0 and 1 (isRandom=true)
	 */	
	Tensor(std::vector<int> size /**[in] vector of length m_numDimensions, containing the length of each dimension*/, 
		   bool isRandom = false /**[in] boolean value that determines whether or not random values are used.  False by default.*/);

	/*!
	 Virtual destructor
	 */
	virtual ~Tensor();

	/*!
	 * Creates a deep copy of the tensor and saves it into resu.
	*/
	void copy(Tensor& resu);


	int getNumDimensions();
	int getUnfoldedDimension();
	void getTensorSize(std::vector<int>& v);
	void getData(TNT::Array2D<double>& A);

	/*! Returns the element in the tensor determined by the specified coordinates
	 */
	double getElement(int* nDcoordinates /**[in] array of length m_numDimensions.  Coordinate values start from 0 */);

                
	
	/*! Returns a subtensor of the tensor specified by the fixing values along a specified dimension.
	 */
	void getSubtensor(int dimension /**[in] dimension along which coordinates are fixed (starts with 0) */, 
					  int coord_min /**[in] first coordinate taken into consideration */, 
					  int length /**[in] number of coordinates taken into consideration */, 
					  Tensor& resu /**[out] output subtensor will be stored here*/);


	/*!
	 * Sets element to a given value based on m_numDimensions-dimensional coordinates
	 */
	int setElement(int* coordinates, 
				   double value);
	
	/*!
	 * Sets element based on 2-D coordinates
	 */
	int setElement2D(int* coords2D, 
					 double value);
	
	/*!
	 * Sets multiple elements with one call.  For details, see parameter explanations.
	 * Example: in a 3-dimensional tensor of size 2-by-2-by-5, setElements(0, [0,0,2], [3,4])
	 * results in changing values in coordinates (0,0,1) and (0,1,1) to 3, and values in coordinates
	 * (1,0,1) and (1,1,1) to 4.
	 */
	void setElements(int elaboratedDimension /**[in] dimension in which inserted values are specified (see third parameter) */, 
					 int* fixDimensions /**[in] array of integers of length m_numDimensions.  If a value is n, and n>0, only those 
										 elements are changed whose coordinate in the given dimension is equal to n-1 */, 
					 double* values /**[in] array of values to be inserted, of length equal to length of dimension number elaboratedDimension*/);

	/*!
	 * Sets multiple elements with one call.  For details, see parameter explanations.
	 * Example: in a 3-dimensional tensor of size 2-by-2-by-5, setElements(0, 1, [0,0,2], 3)
	 * results in changing the values of coordinates (1,0,1) and (1,1,1) to 3
	 */
	void setElements(int dimension /**[in] specifies a single dimension (starts with 0)*/, 
					 int coordinate /**[in] specifies a single coordinate within dimension specified by dimension*/, 
					 int* fixDimensions /**[in] array with a single integer for each dimension in the tensor. If a value is 
										 n, and n>0, only those elements are changed whose coordinate in the given 
										 dimension is equal to n-1  */, 
					 double value /**[in] */);

	/*!
	 * Sets multiple elements with one call.  For details, see parameter explanations.
	 */
	void setSubtensor(int dimension /**[in] dimension along which coordinates are fixed (starts with 0) */, 
					  int coord_min /**[in] first coordinate taken into consideration */, 
					  int length /**[in] number of coordinates taken into consideration */, 
					  Tensor& toSet /**[in] input tensor containing values to be inserted*/);


	/*!
	 * Unfolds the tensor along the dimension specified by parameter dimension (values start with 0)
	 */
	void unfoldN(int dimension);

	/*!
	 * Resizes tensor according to parameter tensorSize, fills its values according to the values
	 * contained in parameter data, and sets m_unfoldedDimension according to parameter unfoldedDimension.
	 * Note that it is assumed that data does not have to have the same dimensions as the tensor does once it is unfolded;
	 * the tensor is fully resized appropriately, and filled with entirely new values.  However, the function does NOT change
	 * the value of m_numDimensions, hence the number of dimensions should still be the same.
	 */
	void resizeWithNewData(const TNT::Array2D<double>& data, std::vector<int> tensorSize, int unfoldedDimension);

	/*!
	 * Calculates tensor product of tensor and all weighting matrices contained in parameter vector v.
	 * Result is written to output parameter resu
	 */
	void tprod(const std::vector< TNT::Array2D<double> >& v, Tensor& resu);
	
	/*!
	 * Calculates tensor product of tensor and all weighting matrices contained in parameter vector v, but
	 * skips a single dimension specified by parameter skipDim (indexing starts from 0)
	 * Result is written to output parameter resu
	 */
	void tprod(const std::vector< TNT::Array2D<double> >& v, int skipDim, Tensor& resu);
	
	/*!
	 * Calculates n-mode tensor product along a specified dimension between the tensor and a weighting
	 * matrix stored in parameter arr
	 */
	void tprodN(TNT::Array2D<double>& arr, int dimension, Tensor& resu);

	/*!
	 * Subtracts tensor contained in parameter secondTerm from this tensor element by element.  Result
	 * is stored in output parameter resu.
	 */
	void subtract(Tensor& secondTerm, Tensor& resu);
	
	/*!
	 * Returns Frobenius-norm of this tensor
	 */
	double norm();
	
	/*!
	 * Concatenates this tensor with another one stored in parameter secondTerm, along the dimension
	 * specified by parameter dimension.  This means that the two tensors must have the same size along
	 * all dimensions except this single dimension.
	 * Result is stored in output parameter resu
	 */
	void concat(Tensor& secondTerm, int dimension, Tensor& resu);

	/*!
	 * Enables printing of a tensor
	 */
	friend std::ostream& operator<<(std::ostream &s, const Tensor &T);
	
	/*!
	 * Enables deep equality check
	 */
	bool operator==(Tensor& T);
	
	/*!
	 * Prints the size of the tensor after echoing its name
	 */
	void printSize(std::string title);

};

} // end namespace SdmTensor


//


#endif /* TENSOR_HPP_ */
