#include "matrix.h"
#include <cmath>

template <typename T> 
class SquareMatrix: public Matrix<T>{
public:
	SquareMatrix();
	SquareMatrix(unsigned int e_dimension);
	void setDiagonal(bool main_diagonal, T const value);
	void unitMatrix();
	//void inverse();
	T determinante();
};

template <typename T>
SquareMatrix<T>::SquareMatrix(): Matrix<T>::Matrix(){}

template <typename T>
SquareMatrix<T>::SquareMatrix(unsigned int e_dimension) : Matrix<T>::Matrix(e_dimension, e_dimension) {}

template <typename T>
void SquareMatrix<T>::setDiagonal(bool main_diagonal, T const value) {
	if (main_diagonal) {
		for (unsigned int i = 0; i < this->rows; ++i)
			this->my_matrix[i][i] = value;
	}
	else {
		for (unsigned int i = 0; i < this->rows; ++i)
			this->my_matrix[i][this->rows - i - 1] = value;
	}
}

template <typename T>
void SquareMatrix<T>::unitMatrix() {
	for (unsigned int i = 0; i < this->rows; ++i) {
		this->my_matrix[i][i] = 1;
	}
}


template <typename T>
T SquareMatrix<T>::determinante() {
	T det = 0;
	if (this->rows == 2) {
		det = this->my_matrix[0][0] * this->my_matrix[1][1] -
				this->my_matrix[0][1] * this->my_matrix[1][0];
		return det;
	}
	if (this->rows == 3) {
		det = this->my_matrix[0][0] * this->my_matrix[1][1] * this->my_matrix[2][2] +
			this->my_matrix[1][0] * this->my_matrix[2][1] * this->my_matrix[0][2] +
			this->my_matrix[0][1] * this->my_matrix[1][2] * this->my_matrix[2][0] -
			this->my_matrix[0][2] * this->my_matrix[1][1] * this->my_matrix[2][0] -
			this->my_matrix[0][1] * this->my_matrix[1][0] * this->my_matrix[2][2] -
			this->my_matrix[1][2] * this->my_matrix[2][1] * this->my_matrix[0][0];
		return det;
	}

	for(unsigned int i = 0; i < this->rows; ++i){
		SquareMatrix<T> M(this->rows - 1);
		for (unsigned int k = 0; k < i ; ++k) {
			for (unsigned int j = 0; j < M.getColumns(); ++j)
				M.my_matrix[k][j] = this->my_matrix[k][j + 1];
		}
		for (unsigned int k = i; k < M.getRows(); ++k) {
			for (unsigned int j = 0; j < M.getColumns(); ++j)
				M.my_matrix[k][j] = this->my_matrix[k + 1][j + 1];

		}
		
		det += this->my_matrix[i][0] * pow(-1, i + 1 + 1) * M.determinante();
	}

	return det;
}


