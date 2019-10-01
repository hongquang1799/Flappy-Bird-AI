#include "neural-network/matrix/Matrix.h"


mc::Matrix::Matrix()
{

}

mc::Matrix::Matrix(size_t row, size_t col)
{
	allocateMemory(row, col);
}

mc::Matrix::~Matrix()
{
}

float * mc::Matrix::get() const
{
	return data.get();
}

size_t mc::Matrix::size() const
{
	return n_row * n_col;
}

mc::Matrix& mc::Matrix::allocateMemory(size_t row, size_t col)
{
	n_row = row;
	n_col = col;

	data = std::shared_ptr<float>(new float[row * col],
		[&](float* ptr)
	{		
		delete[] ptr;
	});

	return *this;
}

void mc::Matrix::set(float value)
{
	for (int i = n_row * n_col - 1; i >= 0; i--)
	{
		data.get()[i] = value;
	}
}

void mc::Matrix::set(const float * arr)
{
	memcpy(data.get(), arr, sizeof(float) * n_row * n_col);
}

void mc::Matrix::set(const std::initializer_list<std::initializer_list<float>>& list)
{
	size_t i = 0;
	for (auto& row_elements : list)
	{
		for (auto& value : row_elements)
		{
			(*this)(i++) = value;
		}
	}
}

void mc::Matrix::mutate(Matrix source, float rate, float range)
{
	for (int i = n_row * n_col - 1; i >= 0; i--)
	{
		if ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) <= rate)
		{
			data.get()[i] = source.get()[i] - range + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (range + range)));
		}		
	}
}

void mc::Matrix::randomize(float min, float max)
{
	for (int i = n_row * n_col - 1; i >= 0; i--)
	{
		data.get()[i] = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}
}

float& mc::Matrix::operator()(size_t r, size_t c) const
{
	return data.get()[r * n_col + c];
}

float& mc::Matrix::operator()(size_t i) const
{
	return data.get()[i];
}

//float * mc::Matrix::operator[](size_t r)
//{
//	return data.get() + r * n_col;
//}

void mc::Matrix::log()
{
	for (int i = 0; i < n_row; i++)
	{
		for (int j = 0; j < n_col; j++)
		{
			if (j == n_col - 1)
				DEBUG_LOG("%f\n", data.get()[i * n_col + j]);
			else
				DEBUG_LOG("%f|", data.get()[i * n_col + j]);
		}
	}
}

void PrintToOutput(const char* szFormat, ...)
{
	/*char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugString(szBuff);*/
}
