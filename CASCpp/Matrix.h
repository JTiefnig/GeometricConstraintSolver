#pragma once
#include <vector>
#include <exception>

typedef std::size_t uint; // std::size_t // needed for iteration



// Februar 2019 
// Author Johannes Tiefnig
// Simple flexible Matrix implementation



template <class T> 
class Matrix
{
protected:
	
	int m; // Rows // Reihen // Zeilen
	int n; // Columns // Spalten
	
	std::vector<std::vector<T>> v;

public:

	Matrix<T>(int M, int N);
	Matrix<T>(int M, int N, const T& );
	Matrix<T>();
	Matrix(const Matrix<T>&);
	Matrix<T>(Matrix<T> &&); // Move constructor

	Matrix<T>& operator=(Matrix<T>&& other);

	virtual ~Matrix();

	int rows() const;
	int cols() const;

	void resize(int, int);


	T getVal(int i, int j) const;

	T& operator() (int i, int j);

	std::vector<T>& operator [] (int);
	const std::vector<T>& operator [] (int) const;

	// Math Operators
	
	Matrix<T> operator + (Matrix<T>& b) const;
	Matrix<T> operator - (Matrix<T>& b) const;
	Matrix<T> operator * (Matrix<T>& b) const;
	Matrix<T> operator += (const Matrix<T>&);
	Matrix<T> operator -= (const Matrix<T>&);
	Matrix<T> operator *= (const Matrix<T>&);

	Matrix<T> operator * (const T & s) const;
	Matrix<T> operator *= (const T & s);


	void RowSwap(int i1, int i2);
	void ColumnSwap(int j1, int j2);

	void operator = (const Matrix<T>& temp);

	Matrix<T> transpose();

};



// Nummeric interface?? with conversion operator would be nice

class MatrixXd : public Matrix<double>
{
public:


	MatrixXd(int m, int n)
		:Matrix(m, n)
	{}
};


// thinking about another base class structure
// for more efficient data storage
// row vector.. not the best
//

class Vector3d : public MatrixXd
{

public:
	Vector3d(double x, double y, double z)
		:MatrixXd(3, 1) // Column Vector
	{ 
		v[0][0] = x;
		v[1][0] = y;
		v[2][0] = z;
	}
};



class Vector2d : public MatrixXd
{
public:
	Vector2d(double x, double y)
		:MatrixXd(2, 1) // Column Vector
	{
		v[0][0] = x;
		v[1][0] = y;
	}
};



class MathException : public std::exception
{
public:
	MathException(const char* msg)
		:exception(msg)
	{}
};




template <class T>
Matrix<T>::Matrix(int M, int N)
	:m(M), n(N), v(M)
{
	for (int i = 0; i < M; i++)
		v[i].resize(N);
}

template<class T>
inline Matrix<T>::Matrix(int M, int N, const T & temp)
	:m(M), n(N), v(M)
{
	for (int i = 0; i < M; i++)
	{
		v[i].resize(N);
		for (int j = 0; j < N; j++)
			v[i][j] = temp;
	}

}

template<class T>
Matrix<T>::Matrix()
{
}

template<class T>
inline Matrix<T>::Matrix(Matrix<T>&& temp)
	:v(std::move(temp.v)), m(std::exchange(temp.m)), n(std::exchange(temp.n))
{

}

template<class T>
inline Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other)
{
	v = std::move(other.v);
	m = std::exchange(other.m);
	n = std::exchange(other.n);

	return *this;
}

template<class T>
Matrix<T>::~Matrix()
{

}

template<class T>
Matrix<T>::Matrix(const Matrix & temp)
	:m(temp.m), n(temp.n), v(temp.v)
{
}

template<class T>
int Matrix<T>::rows() const
{
	return this->m;
}

template<class T>
int Matrix<T>::cols() const
{
	return this->N;
}

template<class T>
void Matrix<T>::resize(int M, int N)
{
	v.resize(M);

	for (int i = 0; i < M; i++)
		v[i].resize(N);

	m = M;
	n = N;
}




template<class T>
T Matrix<T>::getVal(int i, int j) const
{
	return v[i][j];
}

template<class T>
T & Matrix<T>::operator()(int i, int j)
{
	return v[i][j];
}

template<class T>
std::vector<T>& Matrix<T>::operator[](int i)
{
	return v[i];
}

template<class T>
const std::vector<T>& Matrix<T>::operator[](int i) const
{
	return v[i];
}


template<class T>
Matrix<T> Matrix<T>::operator+(Matrix<T> & b) const
{
	if (n != b.n || m != b.m)
		throw MathException("Matrix dimensions did not Match");

	Matrix ret(this->m, this->n);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ret[i][j] = v[i][j] + b[i][j];

	return ret;
}


template<class T>
Matrix<T> Matrix<T>::operator-(Matrix<T> & b) const
{
	if (n != b.n || m != b.m)
		throw MathException("Matrix dimensions did not Match");

	Matrix ret(this->m, this->n);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ret(i, j) = v[i][j] - b[i][j];

	return ret;
}

template<class T>
Matrix<T> Matrix<T>::operator*(Matrix<T>& b) const
{
	if (this->N != b.M)
		throw MathException("Matrix dimensions did not Match");

	int nm = this->M;
	int nn = b.N;
	Matrix ret(nm, nn);

	for (int i = 0; i < nm; i++)
	{
		for (int j = 0; j < nn; j++)
		{
			for (int c = 0; c < b.m; c++)
			{
				ret[i][j] += v[i][c] * b[c][j];
			}
		}
	}
}


template<class T>
Matrix<T> Matrix<T>::operator*(const T & s) const
{
	Matrix ret(this->m, this->n);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ret[i][j] = v[i][j] * s;

	return ret;
}

template<class T>
Matrix<T> Matrix<T>::operator*=(const T & s)
{
	*this = (*this) * s;

	return *this;
}

template<class T>
inline Matrix<T> Matrix<T>::operator+=(const Matrix<T>&)
{
	return *this = *this + m;
}

template<class T>
inline Matrix<T> Matrix<T>::operator-=(const Matrix<T>&)
{
	return *this = *this - m;
}

template<class T>
inline Matrix<T> Matrix<T>::operator*=(const Matrix<T>&)
{
	return *this = *this * m;
}


template<class T>
void Matrix<T>::RowSwap(int i1, int i2)
{
	if (i1 == i2) return;

	T temp;
	for (int j = 0; j < n; j++)
	{
		temp = v[i1][j];
		v[i1][j] = v[i2][j];
		v[i2][j] = temp;
	}
}

template<class T>
void Matrix<T>::ColumnSwap(int j1, int j2)
{
	if (j1 == j2) return;

	T temp;
	for (int i = 0; i < m; i++)
	{
		temp = v[i][j1];
		v[i][j1] = v[i][j2];
		v[i][j2] = temp;
	}
}



template<class T>
void Matrix<T>::operator=(const Matrix<T> & temp)
{

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			v[i][j] = temp[i][j];

}

template<class T>
Matrix<T> Matrix<T>::transpose()
{
	Matrix ret(this->N, this->M);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ret[j][i] = v[i][j];


	return ret;
}



