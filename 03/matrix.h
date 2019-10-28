#pragma once

#include <cstddef>


class Matrix
{
	const size_t rows_, cols_;
	const size_t n_;
	int* data_;

	class Row
	{
		const size_t n_;
		int* data_;

	public:
		Row(size_t n, int* data)
			: n_(n), data_(data)
		{}
		
		int& operator[](size_t i)
		{
			if (i >= n_) throw std::out_of_range("");
			return data_[i];
		}
		
		const int& operator[](size_t i) const
		{
			if (i > n_) throw std::out_of_range("");
			return data_[i];
		}
	};

public:
	Matrix(size_t rows, size_t cols)
		: rows_(rows), cols_(cols),
		  n_(rows*cols),
		  data_(new int[n_])
	{}
	
	Matrix(const Matrix& m)
		: rows_(m.rows_), cols_(m.cols_),
		  n_(m.n_),
		  data_(new int[n_])
	{
		for (int i = 0; i < n_; i++)
		{
			data_[i] = m.data_[i];
		}
	}

	~Matrix()
	{
		delete [] data_;
	}


	size_t getRows() const { return rows_; }
	size_t getCols() const { return cols_; }

	
	Row operator[](size_t i)
	{
		if (i >= rows_) throw std::out_of_range("");
		return Row(cols_, data_ + cols_*i);
	}

	const Row operator[](size_t i) const
	{
		if (i >= rows_) throw std::out_of_range("");
		return Row(cols_, data_ + cols_*i);
	}


	Matrix& operator*=(int a)
	{
		for (int *ptr = data_, *end = data_ + n_; ptr < end; ptr++)
		{
			*ptr *= a;
		}

		return *this;
	}


	bool operator==(const Matrix& op) const
	{
		if (this == &op) return true;

		if (cols_ != op.cols_ ||
		    rows_ != op.rows_) return false;

		for (int i = 0; i < n_; i++)
			if (data_[i] != op.data_[i]) return false;

		return true;
	}

	bool operator!=(const Matrix& op) const
	{
		return !(*this == op);
	}
};


std::ostream& operator<<(std::ostream& out, const Matrix& m);
