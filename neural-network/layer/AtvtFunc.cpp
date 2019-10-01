#include "neural-network/layer/AtvtFunc.h"

using namespace mc;

Matrix& mc::Sigmoid::forward(Matrix& S, Matrix& A)
{
	for (int j = 0; j < S.n_col; j++)
	{
		float x = S(j);
		A(j) = 1.0f / (1.0f + exp(-x));
		
	}

	return A;
}

mc::Matrix& mc::Sigmoid::backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A)
{
	// S matrix can be used or not
	// dE2S = dE2A * A * (1 - A)

	for (int j = 0; j < dE2A.n_col; j++)
	{
		dE2S(j) = dE2A(j) * A(j) * (1 - A(j));
	}

	return dE2S;
}

Matrix& mc::ReLU::forward(Matrix& S, Matrix& A)
{
	for (int j = 0; j < S.n_col; j++)
	{
		float x = S(j);
		A(j) = x > 0 ? x : 0;
	}

	return A;
}

Matrix& mc::ReLU::backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A)
{
	// S matrix can be used or not
	// dE2S = dE2A * 1 if S >= 0
	// dE2S = dE2A * 0 if S < 0

	for (int j = 0; j < dE2A.n_col; j++)
	{
		dE2S(j) = S(j) > 0 ? dE2A(j) : 0.f;
	}

	return dE2S;
}

Matrix& mc::Tanh::forward(Matrix& S, Matrix& A)
{
	for (int j = 0; j < S.n_col; j++)
	{
		float x = S(j);
		A(j) = tanhf(S(j));
	}

	return A;
}

Matrix& mc::Tanh::backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A)
{
	// S matrix can be used or not
	// dE2S = dE2A * (1 - A^2);

	for (int j = 0; j < dE2A.n_col; j++)
	{
		dE2S(j) = dE2A(j) * (1 - A(j) * A(j));
	}

	return dE2S;
}

Matrix& mc::Softmax::forward(Matrix& S, Matrix& A)
{
	float sum = 0.f;
	for (int j = 0; j < S.n_col; j++)
	{
		float x = S(j);
		float y = exp(S(j));
		A(j) = y;

		sum += y;
	}

	for (int j = 0; j < S.n_col; j++)
	{
		A(j) /= sum;
	}

	return A;
}

Matrix& mc::Softmax::backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A)
{
	// researching
	return dE2S;
}

