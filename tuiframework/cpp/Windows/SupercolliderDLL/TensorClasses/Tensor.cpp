/*
 * Tensor.cpp
 *
 *  Created on: 2011.01.24.
 *      Author: Adam
 */

#include "Tensor.h"

namespace SdmTensor{

/*!
 Constructor that creates an empty tensor with no initialized members.
*/
Tensor::Tensor()
{
}


/*!
 Constructor that creates a tensor of specified dimensions, either filled with 
 zeros (isRandom=false) or with random values between 0 and 1 (isRandom=true)
 */	
Tensor::Tensor(std::vector<int> tensorSize, bool isRandom)
{


	m_numDimensions = tensorSize.size(); //initialize m_numDimensions
	m_unfoldedModeDimension = 0; //initialize m_unfoldedModeDimension

	//initialize m_tensorSize
	//calculate number of columns in 0-mode unfolding
	int matrixRows = tensorSize[0], matrixCols = 1;
	m_tensorSize.push_back(tensorSize[0]);
	for(int i=1; i < m_numDimensions; i++){
		m_tensorSize.push_back(tensorSize[i]);
		matrixCols *= tensorSize[i];
	}

	//initialize m_data
	m_data = TNT::Array2D<double>(matrixRows, matrixCols, 0.0);

	//if isRandom is true, then fill with random values between 0 and 1
	if(isRandom){

		srand(time(NULL));
		for(int i=0; i<matrixRows; i++)
			for(int j=0; j<matrixCols; j++){
				m_data[i][j] = (rand() % 101) / 100.0;
			}
	}
}

/*!
 Virtual destructor
 */
Tensor::~Tensor()
{
}

/*!
 * Creates a deep copy of the tensor and saves it into resu
*/
void Tensor::copy(Tensor& resu)
{
	resu = Tensor(m_tensorSize);
	resu.resizeWithNewData(m_data, m_tensorSize, m_unfoldedModeDimension);
}


/*
 * Getter methods for m_numDimensions, m_unfoldedDimension and m_tensorSize
 */
int Tensor::getNumDimensions(){
	return m_numDimensions;
}

int Tensor::getUnfoldedDimension()
{
	return m_unfoldedModeDimension;
}


void Tensor::getTensorSize(std::vector<int>& v){
	v.clear();
	for(int i=0; i<m_tensorSize.size(); i++){
		v.push_back(m_tensorSize[i]);
	}

}

void Tensor::getData(TNT::Array2D<double>& M)
{
	M = TNT::Array2D<double>(m_data.dim1(), m_data.dim2(), 0.0);
	for(int i=0; i<m_data.dim1(); i++)
		for(int j=0; j<m_data.dim2(); j++)
			M[i][j] = m_data[i][j];
}


/*! Returns the element in the tensor determined by the specified coordinates
*/
double Tensor::getElement(int* coordinates)
{
	int coords2D[2] = {0, 0};
	get2Dcoords(coordinates, coords2D);
	return m_data[coords2D[0]][coords2D[1]];
}

/*
*/
void Tensor::getRow(int row, std::vector<double>& resu)
{
                resu.clear();
	int coords2D[2] = {row, 0};
	for(int i=0; i<m_data.dim2(); i++){
	        resu.push_back(m_data[row][i]);
	
	}
	
}


/*! Returns a subtensor of the tensor specified by the fixing values along a specified dimension.
*/
void Tensor::getSubtensor(int dimension, int coord_min, int length, Tensor& resu)
{
	std::vector<int> sz = std::vector<int>(m_tensorSize);
	//sz[dimension] = length - coord_min;
	sz[dimension] = length;

	resu = Tensor(sz);

	unfoldN(dimension);
	resu.unfoldN(dimension);
	int coords[2] = {0, 0};

	for(int j=0; j<sz[dimension];j++){
		coords[0] = j;
		for(int k = 0; k < m_data.dim2(); k++){
			coords[1] = k;
			resu.setElement2D(coords, m_data[j+coord_min][k]);
		}
	}
}


/*
 * Setter methods
 */

/*!
* Sets element to a given value based on m_numDimensions-dimensional coordinates
*/
int Tensor::setElement(int* coordinates, double value)
{

	int coords2D[2] = {0,0};
	get2Dcoords(coordinates, coords2D);
	
	if(coords2D[0] == -1)
		return -1;

	m_data[coords2D[0]][coords2D[1]] = value;
	return 0;

}

/*!
* Sets element based on 2-D coordinates
*/
int Tensor::setElement2D(int* coords2D, double value)
{

	m_data[coords2D[0]][coords2D[1]] = value;
	return 0;

}

/*!
* Sets multiple elements with one call.  For details, see parameter explanations.
* Example: in a 3-dimensional tensor of size 2-by-2-by-5, setElements(0, [0,0,2], [3,4])
* results in changing values in coordinates (0,0,1) and (0,1,1) to 3, and values in coordinates
* (1,0,1) and (1,1,1) to 4.
*/
void Tensor::setElements(int elaboratedDimension, int* fixDimensions, double* values)
{

	int twoDcoords[2];
	int* nDcoords = new int[m_numDimensions];
	int sentinel = 0;
	

	for(int xc = 0; xc < m_data.dim1(); xc++){
		for(int yc = 0; yc < m_data.dim2(); yc++){

			sentinel = 0;

			twoDcoords[0] = xc; twoDcoords[1] = yc;
			getNDcoords(twoDcoords, nDcoords);

			for(int fixDim_inx = 0; fixDim_inx < m_numDimensions; fixDim_inx++)
				if(fixDimensions[fixDim_inx] > 0)
					if(nDcoords[fixDim_inx] != fixDimensions[fixDim_inx] - 1)
						sentinel = 1;

			if(sentinel==0)
				m_data[xc][yc] = values[nDcoords[elaboratedDimension]];
		}
	}

	delete[] nDcoords;
}

/*!
* Sets multiple elements with one call.  For details, see parameter explanations.
* Example: in a 3-dimensional tensor of size 2-by-2-by-5, setElements(0, 1, [0,0,2], 3)
* results in changing the values of coordinates (1,0,1) and (1,1,1) to 3
*/
void Tensor::setElements(int dimension, int coordinate, int* fixDimensions, double value){

	int twoDcoords[2];
	int* nDcoords = new int[m_numDimensions];
	int sentinel = 0;

	for(int xc = 0; xc < m_data.dim1(); xc++){
		for(int yc = 0; yc < m_data.dim2(); yc++){

			sentinel = 0;

			twoDcoords[0] = xc; twoDcoords[1] = yc;
			getNDcoords(twoDcoords, nDcoords);

			for(int fixDim_inx = 0; fixDim_inx < m_numDimensions; fixDim_inx++)
				if(fixDimensions[fixDim_inx] > 0)
					if(nDcoords[fixDim_inx] != fixDimensions[fixDim_inx] - 1)
						sentinel = 1;

			if(nDcoords[dimension] != coordinate)
				sentinel = 1;

			if(sentinel==0)
				m_data[xc][yc] = value;
		}
	}

	delete[] nDcoords;
}

/*!
* Sets multiple elements with one call.  For details, see parameter explanations.
*/
void Tensor::setSubtensor(int dimension, int coord_min, int length, Tensor& toSet){

	std::vector<int> sz = std::vector<int>(m_tensorSize);
	sz[dimension] = length;

	unfoldN(dimension);
	toSet.unfoldN(dimension);
	TNT::Array2D<double> toSetData;
	toSet.getData(toSetData);


	for(int j=0; j<length;j++){
		for(int k = 0; k < m_data.dim2(); k++){
			m_data[j+coord_min][k] = toSetData[j][k];
		}
	}

}


/*!
 * Unfolds the tensor along the dimension specified by parameter dimension (values start with 0)
 */
void Tensor::unfoldN(int dimension){

	int* coordsNd = new int[m_numDimensions];
	int coords2d[2];
	int new_coords2d[2];

	int prev_unfoldedModeDimension = m_unfoldedModeDimension;
	int prev_matrixRows = m_data.dim1();
	int prev_matrixCols = m_data.dim2();

	m_unfoldedModeDimension = dimension;

	int matrixCols = 1;
	for(int i = 0; i < m_numDimensions; i++)
		if(i != dimension)
			matrixCols *= m_tensorSize[i];

	TNT::Array2D<double> new_m_data(m_tensorSize[dimension], matrixCols, 0.0);

	for(int colnum = 0; colnum < m_data.dim2(); colnum++){
		for(int rownum = 0; rownum < m_data.dim1(); rownum++){

			coords2d[0] = rownum;
			coords2d[1] = colnum;
			getNDcoordsGeneric(coords2d, prev_unfoldedModeDimension, coordsNd);
			get2Dcoords(coordsNd, new_coords2d);

			new_m_data[new_coords2d[0]][new_coords2d[1]] = m_data[rownum][colnum];
		}
	}

	delete[] coordsNd;

	m_data = new_m_data;

}

	
/*!
* Resizes tensor according to parameter tensorSize, fills its values according to the values
* contained in parameter data, and sets m_unfoldedDimension according to parameter unfoldedDimension.
* Note that it is assumed that data does not have to have the same dimensions as the tensor does once it is unfolded;
* the tensor is fully resized appropriately, and filled with entirely new values.  However, the function does NOT change
* the value of m_numDimensions, hence the number of dimensions should still be the same.
*/
void Tensor::resizeWithNewData(const TNT::Array2D<double>& data, std::vector<int> tensorSize, int unfoldedDimension){


	for(int i = 0; i < tensorSize.size(); i++)
		m_tensorSize[i] = tensorSize[i];

	m_unfoldedModeDimension = unfoldedDimension;
	m_data = TNT::Array2D<double>(data.dim1(), data.dim2(), 0.0);

	for(int i=0; i<m_data.dim1(); i++)
		for(int j=0; j<m_data.dim2(); j++)
			m_data[i][j] = data[i][j];

}

/*!
* Calculates tensor product of tensor and all weighting matrices contained in parameter vector v.
* Result is written to output parameter resu
*/
void Tensor::tprod(const std::vector< TNT::Array2D<double> >& v, Tensor& resu){

	std::vector<int> size;
	std::vector< std::vector<int> > tempResultSizes;

	tempResultSizes.push_back(m_tensorSize);

	for(int i=0; i < v.size(); i++){

		size.push_back(v[i].dim1());

		tempResultSizes.push_back(std::vector<int>());

		for(int j = 0; j < v.size(); j++){
			if (j < i+1)
				tempResultSizes[i+1].push_back(v[j].dim1());
			else
				tempResultSizes[i+1].push_back(v[j].dim2());
		}
	}

	resu = Tensor(size);
	unfoldN(0);
	TNT::Array2D<double> tmp(m_data);
	resu.resizeWithNewData(tmp, tempResultSizes[0], 0);

	for(int i=0; i<m_numDimensions; i++){
		resu.unfoldN(i);
		resu.getData(tmp);
		resu.resizeWithNewData(TNT::matmult(v[i], tmp), tempResultSizes[i+1], i);
	}

	resu.unfoldN(0); //important so that only m_data must be copied into an existing tensor when resu is only temporary
}

/*!
* Calculates tensor product of tensor and all weighting matrices contained in parameter vector v, but
* skips a single dimension specified by parameter skipDim (indexing starts from 0)
* Result is written to output parameter resu
*/
void Tensor::tprod(const std::vector< TNT::Array2D<double> >& v, int skipDim, Tensor& resu){

	std::vector<int> size;
	std::vector< std::vector<int> > tempResultSizes;

	tempResultSizes.push_back(m_tensorSize);

	for(int i=0; i < v.size(); i++){

		size.push_back(v[i].dim1());
		tempResultSizes.push_back(std::vector<int>());

		for(int j = 0; j < v.size(); j++){
			if (j < i+1)
				tempResultSizes[i+1].push_back(v[j].dim1());
			else
				tempResultSizes[i+1].push_back(v[j].dim2());
		}

		tempResultSizes[i+1][skipDim] = m_tensorSize[skipDim];

	}

	size[skipDim] = m_tensorSize[skipDim];

	resu = Tensor(size);
	unfoldN(0);
	TNT::Array2D<double> tmp(m_data);
	TNT::Array2D<double> tmp2;
	resu.resizeWithNewData(tmp, tempResultSizes[0], 0);

	for(int i=0; i<m_numDimensions; i++){
		resu.unfoldN(i);
		resu.getData(tmp);
		if(i != skipDim)
			tmp2 = TNT::matmult(v[i], tmp);
		else
			tmp2 = TNT::Array2D<double>(tmp);
		resu.resizeWithNewData(tmp2, tempResultSizes[i+1], i);
	}

	resu.unfoldN(0); //important so that only m_data must be copied into an existing tensor when resu is only temporary
}


	/*!
	 * Calculates n-mode tensor product along a specified dimension between the tensor and a weighting
	 * matrix stored in parameter arr
	 */	
void Tensor::tprodN(TNT::Array2D<double>& arr, int dimension, Tensor& resu){

	std::vector<int> newTensorSize;
	for(int i=0; i<m_numDimensions; i++)
		newTensorSize.push_back(m_tensorSize[i]);

	newTensorSize[dimension] = arr.dim1();

	unfoldN(dimension);

	resu = Tensor(newTensorSize);
	resu.resizeWithNewData(TNT::matmult(arr, m_data), newTensorSize, dimension);

	resu.unfoldN(0); //important so that only m_data must be copied into an existing tensor when resu is only temporary
}

/*!
 * Subtracts tensor contained in parameter secondTerm from this tensor element by element.  Result
 * is stored in output parameter resu.
 */
void Tensor::subtract(Tensor& secondTerm, Tensor& resu){

	TNT::Array2D<double> arr;
	TNT::Array2D<double> res;

	secondTerm.getData(arr);

	res = m_data - arr;

	resu = Tensor(secondTerm);
	resu.resizeWithNewData(res, m_tensorSize, m_unfoldedModeDimension);

}

/*!
 * Returns Frobenius-norm of this tensor
 */
double Tensor::norm(){
	return TNT::norm(m_data);
}

/*!
 * Concatenates this tensor with another one stored in parameter secondTerm, along the dimension
 * specified by parameter dimension.  This means that the two tensors must have the same size along
 * all dimensions except this single dimension.
 * Result is stored in output parameter resu
 */
void Tensor::concat(Tensor& secondTerm, int dimension, Tensor& resu){

	std::vector<int> sz2;
	secondTerm.getTensorSize(sz2);


	////////////// PERFORM SANITY CHECK ////////////////////////////

	assert(sz2.size() == m_numDimensions);
	for(int i=0; i<m_numDimensions; i++)
		if(i != dimension)
			assert(sz2[i] == m_tensorSize[i]);

	////////////// END SANITY CHECK ////////////////////////////

	//unfold along dimension after non-matching dimension.  This way the coordinates of the non-matching
	//dimension will change at the largest frequency in the unfolding
	int unfoldDimension = dimension + 1;
	if (unfoldDimension > m_numDimensions-1)
		unfoldDimension = 0;

	int blocksizeA = m_tensorSize[dimension];
	int blocksizeB = sz2[dimension];
	int blocksizeC = blocksizeA + blocksizeB;

	unfoldN(unfoldDimension);
	secondTerm.unfoldN(unfoldDimension);

	TNT::Array2D<double> secondTermData;
	secondTerm.getData(secondTermData);

	int numrows = m_data.dim1();
	int numcols = m_data.dim2() + secondTermData.dim2();

	int numblocks = m_data.dim2() / blocksizeA;
	int numblocks2 = secondTermData.dim2() / blocksizeB;
	assert(numblocks == numblocks2);

	TNT::Array2D<double> result_data(numrows, numcols, 0.0);
	int ycoord, ycoordOrig;

	for(int blocki=0; blocki<numblocks; blocki++){
		for(int blockai=0; blockai<blocksizeA; blockai++){
			ycoord = blocki*blocksizeC+blockai;
			ycoordOrig = blocki*blocksizeA+blockai;
			for(int xcoord=0; xcoord<numrows; xcoord++)
				result_data[xcoord][ycoord] = m_data[xcoord][ycoordOrig];

		}

		for(int blockbi=0; blockbi<blocksizeB; blockbi++){
			ycoord = blocksizeA*(blocki+1)+blocksizeB*blocki+blockbi;
			ycoordOrig = blocki*blocksizeB+blockbi;
			for(int xcoord=0; xcoord<numrows; xcoord++)
				result_data[xcoord][ycoord] = secondTermData[xcoord][ycoordOrig];
		}
	}

	std::vector<int> resultsz = std::vector<int>(m_tensorSize);
	resultsz[dimension] = m_tensorSize[dimension]+sz2[dimension];

	resu = Tensor(resultsz);
	resu.resizeWithNewData(result_data, resultsz, unfoldDimension);

}

/*!
 * Enables printing of a tensor
 */
std::ostream& operator<<(std::ostream &s, const Tensor &T)
{
	s << T.m_data;
    return s;
}

/*!
 * Enables deep equality check
 */
bool Tensor::operator==(Tensor& T){

	if(m_numDimensions != T.getNumDimensions())
		return false;

	std::vector<int> sz;
	T.getTensorSize(sz);
	for(int i=0; i<m_tensorSize.size(); i++){
		if(m_tensorSize[i] != sz[i])
			return false;
	}

	unfoldN(0);
	T.unfoldN(0);

	TNT::Array2D<double> data2;
	T.getData(data2);

	double diff;
	for(int i=0; i<m_data.dim1(); i++){
		for(int j=0; j<m_data.dim2(); j++){
			diff = m_data[i][j] - data2[i][j];
			if (diff > 0.00001 || diff < -0.00001)
				return false;
		}

	}

	return true;
}


/*!
 * Prints the size of the tensor after echoing title
 */
void Tensor::printSize(std::string title){

	std::cout << "Size of tensor " << title << " is: ";
	for(int i=0; i<m_tensorSize.size(); i++)
		std::cout << m_tensorSize[i] << " x ";
	std::cout << std::endl << std::endl;

}


///////////////////////////////////////////////////////////////////////////
////////////////////////// PROTECTED METHODS //////////////////////////
///////////////////////////////////////////////////////////////////////////

/*! Returns the two-dimensional coordinates of the two-dimensional 1-mode unfolding of tensor
*/
void Tensor::get2Dcoords(int* coordinates, int* coords2D)
{

	int row_inx = coordinates[m_unfoldedModeDimension];
	int col_inx = 0, aux = 0;


	for(int i = 0; i < m_numDimensions; i++){
		if(i != m_unfoldedModeDimension){

			aux = coordinates[i];

			if(i < m_unfoldedModeDimension){
				for(int j = i+1; j < m_unfoldedModeDimension; j++)
					aux = aux * m_tensorSize[j];
			}
			else{
				for(int j = i+1; j < m_numDimensions; j++){
					aux = aux * m_tensorSize[j];
				}
				for(int j = 0; j < m_unfoldedModeDimension; j++){
					aux = aux * m_tensorSize[j];
				}
			}
			col_inx += aux;

		}
	}

	*coords2D++ = row_inx;
	*coords2D = col_inx;
	return;
}


/*! Returns the coordinates (in m_numDimensions number of dimensions) corresponding to 2-dimensional coordinates
expressed for the two-dimensional current unfolding of the tensor (i.e., stored in m_unfoldedModeDimension)
*/	
void Tensor::getNDcoords(int* coordinates, int* coordsND)
{

	//first dimension's coordinate is the first 2D coordinate
	coordsND[m_unfoldedModeDimension] = coordinates[0];
	int ycoord = coordinates[1];

	//listToTraverse contains indices from m_unfoldedModeDimension+1 until the last one, then from 0 until
	//m_unfoldedModeDimension-1
	int* listToTraverse = new int[m_numDimensions - 1];
	for(int i = 0; i < (m_numDimensions - m_unfoldedModeDimension - 1); i++){
		listToTraverse[i] = m_unfoldedModeDimension + 1 + i;
	}
	for(int i = (m_numDimensions - m_unfoldedModeDimension - 1); i < (m_numDimensions-1);i++){
		listToTraverse[i] = i - (m_numDimensions - m_unfoldedModeDimension - 1);
	}

	//iterate backwards through listToTraverse, filling coordsND with the appropriate values
	for(int i = (m_numDimensions - 2); i >= 0; i--){
		coordsND[listToTraverse[i]] = ycoord % m_tensorSize[listToTraverse[i]];
		ycoord = (ycoord - coordsND[listToTraverse[i]]) / m_tensorSize[listToTraverse[i]];
	}

	delete[] listToTraverse;

}

/*! Returns the coordinates (in m_numDimensions number of dimensions) corresponding to 2-dimensional coordinates
expressed for a two-dimensional current unfolding of the tensor specified by rowdim
*/	
void Tensor::getNDcoordsGeneric(int* coordinates, int rowdim, int* coordsND)
{

	int tmp_unfoldedModeDimension = m_unfoldedModeDimension;
	m_unfoldedModeDimension = rowdim;

	//call getNDCoords after temporarily setting m_unfoldedModeDimension to desired value
	getNDcoords(coordinates, coordsND);
	m_unfoldedModeDimension = tmp_unfoldedModeDimension;

}



} // end namespace SdmTensor

