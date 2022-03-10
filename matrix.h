#ifndef Matrix_H
#define Matrix_H
#include "fehler.h"

/*
	Ausgaben und Aktionen von Fehlern:

	Eingabe() - "Ungueltige Eingabe."
	Eingabe(const char* info) - Gibt zusaetzliche Information aus.
	SmallPosition() - "Erforderliche Position ist zu klein."
	BigPosition() - "Gesuchte Position ist zu gross."
	NotSquare() - "Matrix ist nicht quadratisch."
	DifferentDimensions() - "Matrizen haben unterschiedliche Dimensionen."
	InappropriateDimensions() - "Matrizen haben unpassende Dimensionen."

	Falls etw. unter diese Definitionen nicht passt:
	Position() - "Ungueltige Position."
	Fehler() - "Unbekannte Error."
*/

template <typename T>
class Matrix {
protected:
	unsigned int rows;
	unsigned int columns;
	T** my_matrix;

	void clear();
public:
	Matrix(); //Default-Konstruktor
	Matrix(unsigned int e_rows, unsigned int e_columns); //Ganze Matrix wird mit 0 konstruiert
	Matrix(Matrix<T> const & m); //Kopierkonstruktor

	Matrix<T>& operator = (Matrix<T> const& m);
	bool operator == (Matrix<T> const& m) const;
	bool operator != (Matrix<T> const& m) const;
	Matrix<T> operator + (Matrix<T> const& m) const;
	Matrix<T>& operator += (Matrix<T> const& m);
	Matrix<T> operator * (Matrix<T> const& m) const;
	Matrix<T>& operator *= (Matrix<T> const& m);
	Matrix<T> operator - (Matrix<T> const& m) const;
	Matrix<T>& operator -= (Matrix<T> const& m);

	unsigned int getRows() const;
	unsigned int getColumns() const;

	void setElement(unsigned int e_row, unsigned int e_column, T const value);
	T getElement(unsigned int e_row, unsigned int e_column) const;
	void setFromTo(unsigned int e_start_row, unsigned int e_start_column, unsigned int e_end_row, unsigned int e_end_column, T const value);
	void setColumn(unsigned int e_column, T const value);
	void setRow(unsigned int e_row, T const value);
	void nullMatrix();

	void transpose();
	void scalarMultiplication(T const value);
	void add(Matrix<T> const & m);
	void subtract(Matrix<T> const& m);
	void multiply(Matrix<T> const& m);

	void print() const;
	~Matrix();
};

template <typename T>
Matrix<T>::Matrix() {
	rows = columns = 0;
	my_matrix = nullptr;
}

template <typename T>
Matrix<T>::Matrix(unsigned int e_rows, unsigned int e_columns) : rows(e_rows), columns(e_columns) {
	if (rows < 2 || columns < 2) throw Eingabe("Matrix muss mindestens 2 X 2 sein.");
	my_matrix = new T * [rows];
	for (unsigned int i = 0; i < rows; ++i)
		my_matrix[i] = new T[columns];
	nullMatrix();
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> const& m) {
	rows = m.getRows();
	columns = m.getColumns();

	my_matrix = new T * [rows];
	for (unsigned int i = 0; i < rows; ++i)
		my_matrix[i] = new T[columns];

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			my_matrix[i][j] = m.getElement(i, j);
	}
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T> const& m) {
	if (this == &m) return *this;

	clear(); //Speicher freigeben

	rows = m.getRows();
	columns = m.getColumns();
	my_matrix = new T * [rows];
	for (unsigned int i = 0; i < rows; ++i)
		my_matrix[i] = new T[columns];

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			my_matrix[i][j] = m.getElement(i, j);
	}

	return *this;
}

template <typename T>
bool Matrix<T>::operator == (Matrix<T> const& m) const {
	if (this == &m) return true;

	if (rows != m.getRows() || columns != m.getColumns())
		throw DifferentDimensions();
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			if (my_matrix[i][j] != m.getElement(i, j)) return false;
	}

	return true;
}

template <typename T>
bool Matrix<T>::operator != (Matrix<T> const& m) const {
	if (this == &m) return false;

	if (rows != m.getRows() || columns != m.getColumns())
		throw DifferentDimensions();
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			if (my_matrix[i][j] != m.getElement(i, j)) return true;
	}

	return false;
}

template <typename T>
Matrix<T> Matrix<T>::operator + (Matrix<T> const& m) const {
	if (rows != m.getRows() || columns != m.getColumns())
		throw InappropriateDimensions();
	Matrix<T> temp_matrix(rows, columns);

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			temp_matrix.my_matrix[i][j] = my_matrix[i][j] + m.getElement(i, j);
	}

	return temp_matrix;
}

template <typename T>
Matrix<T>& Matrix<T>::operator += (Matrix<T> const& m) {
	add(m);
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator * (Matrix<T> const& m) const {
	if (columns != m.getRows()) throw InappropriateDimensions();

	Matrix<T> temp_matrix(rows, m.getColumns());
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < m.getColumns(); ++j) {
			T sum = 0;
			for (unsigned int k = 0; k < m.getRows(); ++k)
				sum += my_matrix[i][k] * m.getElement(k, j);
			temp_matrix.my_matrix[i][j] = sum;
		}
	}

	return temp_matrix;
}

template <typename T>
Matrix<T>& Matrix<T>::operator *= (Matrix<T> const& m) {
	multiply(m);
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator - (Matrix<T> const& m) const{
	if (rows != m.getRows() || columns != m.getColumns())
		throw InappropriateDimensions();
	Matrix<T> temp_matrix(rows, columns);

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			temp_matrix.my_matrix[i][j] = my_matrix[i][j] - m.my_matrix[i][j];
	}

	return temp_matrix;
}

template <typename T>
Matrix<T>& Matrix<T>::operator -= (Matrix<T> const& m) {
	subtract(m);
	return *this;
}

template <typename T>
unsigned int Matrix<T>::getRows() const {
	return rows;
}

template <typename T>
unsigned int Matrix<T>::getColumns() const {
	return columns;
}

template <typename T>
void Matrix<T>::setElement(unsigned int e_row, unsigned int e_column, T const value) {
	if (e_row >= rows || e_column >= columns) throw BigPosition();
	my_matrix[e_row][e_column] = value;
}

template <typename T>
T Matrix<T>::getElement(unsigned int e_row, unsigned int e_column) const {
	return my_matrix[e_row][e_column];
}

template <typename T>
void Matrix<T>::setFromTo(unsigned int e_start_row, unsigned int e_start_column, unsigned int e_end_row, unsigned int e_end_column, T const value) {
	//Eingabe Pruefung:

	if (e_start_row >= rows || e_start_column >= columns
		|| e_end_row >= rows || e_end_column >= columns)
		throw Eingabe("Zu grosse Indexe.");

	//Ende Pruefung

	if (e_start_row != e_end_row) {
		if (e_start_row > e_end_row) {
			unsigned int temp = e_start_row;
			e_start_row = e_end_row;
			e_end_row = temp;

			temp = e_start_column;
			e_start_column = e_end_column;
			e_end_column = temp;
		}

		for (unsigned int j = e_start_column; j < columns; ++j) {
			my_matrix[e_start_row][j] = value;
		}

		for (unsigned int i = e_start_row + 1; i < e_end_row; ++i) {
			for (unsigned int j = 0; j < columns; ++j)
				my_matrix[i][j] = value;
		}

		for (unsigned int j = 0; j <= e_end_column; ++j)
			my_matrix[e_end_row][j] = value;
	}
	else {
		if (e_start_column > e_end_column) {
			unsigned int temp = e_start_column;
			e_start_column = e_end_column;
			e_end_column = temp;
		}
		for (unsigned int j = e_start_column; j <= e_end_column; ++j)
			my_matrix[e_start_row][j] = value;
	}
}

template <typename T>
void Matrix<T>::nullMatrix() {
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			my_matrix[i][j] = 0;
	}
}

template <typename T>
void Matrix<T>::setColumn(unsigned int e_column, T const value) {
	if (e_column >= columns) throw BigPosition();
	for (unsigned int i = 0; i < rows; ++i)
		my_matrix[i][e_column] = value;
}

template <typename T>
void Matrix<T>::setRow(unsigned int e_row, T const value) {
	if (e_row >= rows) throw BigPosition();
	for (unsigned int j = 0; j < columns; ++j)
		my_matrix[e_row][j] = value;
}

template <typename T>
void Matrix<T>::transpose() {
	T **temp_matrix = new T*[rows];
	for (unsigned int i = 0; i < rows; ++i)
		temp_matrix[i] = new T[columns];

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			temp_matrix[i][j] = my_matrix[i][j];
	}

	clear();

	unsigned int temp = rows;
	rows = columns;
	columns = temp;

	my_matrix = new T * [rows];
	for (unsigned int i = 0; i < rows; ++i)
		my_matrix[i] = new T[columns];

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			my_matrix[i][j] = temp_matrix[j][i];
	}

	for (unsigned int i = 0; i < columns; ++i)
		delete[] temp_matrix[i];
	delete[] temp_matrix;
}

template <typename T>
void Matrix<T>::scalarMultiplication(T const value) {
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			my_matrix[i][j] *= value;
	}
}

template <typename T>
void Matrix<T>::add(Matrix<T> const& m) {
	if (rows != m.getRows() || columns != m.getColumns()) throw DifferentDimensions();
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			my_matrix[i][j] += m.getElement(i, j);
	}
}

template <typename T>
void Matrix<T>::subtract(Matrix<T> const& m) {
	if (rows != m.getRows() || columns != m.getColumns()) throw DifferentDimensions();
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			my_matrix[i][j] -= m.getElement(i, j);
	}
}

template <typename T>
void Matrix<T>::multiply(Matrix<T> const& m) {
	if (columns != m.getRows()) throw InappropriateDimensions();

	T **temp_matrix = new T * [rows];
	for (unsigned int i = 0; i < rows; ++i)
		temp_matrix[i] = new T[columns];

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			temp_matrix[i][j] = my_matrix[i][j];
	}

	clear();

	columns = m.getColumns();
	my_matrix = new T * [rows];
	for (unsigned int i = 0; i < rows; ++i)
		my_matrix[i] = new T[columns];

	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j) {
			T sum = 0;
			for (unsigned int k = 0; k < m.getRows(); ++k)
				sum += temp_matrix[i][k] * m.getElement(k, j);
			my_matrix[i][j] = sum;
		}
	}

	for (unsigned int i = 0; i < columns; ++i)
		delete[] temp_matrix[i];
	delete[] temp_matrix;
}

template <typename T>
void Matrix<T>::print() const {
	for (unsigned int i = 0; i < rows; ++i) {
		for (unsigned int j = 0; j < columns; ++j)
			cout << my_matrix[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

template <typename T>
void Matrix<T>::clear() {
	for (unsigned int i = 0; i < rows; ++i)
		delete[] my_matrix[i];
	delete[] my_matrix;
}

template <typename T>
Matrix<T>::~Matrix() {
	clear();
}

/*
	Klasse ist nur fuer numerischen Typen moeglich!
	---------------------------------------------------------------------------------------------------

	Indexbindung beginnt sich mit 0 und endet mit rows/columns - 1;
	Das heisst fuer Matrix 3 X 5 hat Element in der 1. Zeile 4. Spalte Indexe [0][3];
	Wenn man Element der 2. Zeile 1. Spalte bekommen will, muss man |-> getElement(1, 0) <-| schreiben ;

	rows - Anzahl Zeilen;
	columns - Anzahl Spalten;
	value - Wert;

	Erzeugung einer neuen Matrix konstruiert immer eine Nullmatrix;
	---------------------------------------------------------------------------------------------------

	e_ - Eingegebene/Uebergebene Dateien;

	---------------------------------------------------------------------------------------------------

	Methoden Erklaerungen:
		-> getRows() - Gibt Anzahl Zeilen (unsigned int) zurueck;
		-> getColumns() - Gibt Anzahl Spalten (unsigned int) zurueck;

		-> setElement(...) - Setzt Element in der Matrix mit ueberegebenem Wert;
		-> getElement(...) - Gibt gewuenschte Element der Matrix zurueck (Schablonen Datentyp);
		-> setFromTo(...) - Setzt ab irgendwelchem Element wie "Eine Schlange" bis einem anderen Element; Funktioniert
							auch in der entgegene Richtung;

							|-------------------------------------------------------------------------------------------|
							|Beispiel:																					|
							|	->Es gibt Nullmatrix 5 X 3:					->Es gibt folgende Matrix:					|
							|																							|
							|			0	0	0									4	8	56							|
							|			0	0	0									32	0	0							|
							|			0	0	0									2	11	1							|
							|			0	0	0									31	9	2							|
							|			0	0	0									0	0	0							|
							|																							|
							|	->Funktionsanwendung:						->Funktionsanwendung(entgegengerichtet):	|
							|		setFromTo(0, 2, 3, 0, 7);					setFromTo(4, 0, 1, 2, 5);				|
							|																							|
							|	->Matrixumwandlung:							->Matrixumwnadlung:							|
							|																							|
							|			0	0	7									4	8	56							|
							|			7	7	7									32	0	5							|
							|			7	7	7									5	5	5							|
							|			7	0	0									5	5	5							|
							|			0	0	0									5	0	0							|
							|																							|
							|Ende Bespiel-------------------------------------------------------------------------------|

		-> setColumn(...) - Setzt Spalte mit uebergebenem Wert;
		-> setRow(...) - Setzt Spalte mit uebergebenem Wert;
		-> setDiagonal(bool main_diagonal, T value) - Setzt Hauptdiagonale, wenn main_diagonal == 1;
													  andere diagonale wenn haupt == -1;
		-> nullMatrix() - Setzt ganze Matrix mit dem Wert 0;
		-> unitMatrix() - Wandelt Matrix in eine Einheits-Matrix gleicher Dimension um;

		-> transpose() - Transponiert die Matrix;
		-> scalarMultiplication(...) - Multiplizierung mit einem Skalar;
		-> add(...) - Matrixaddition;
		-> subtract(...) - Matrixsubtraktion;
		-> multiply(...) - Matrixmultiplikation;

		-> print() - Matrixausgabe
*/

#endif