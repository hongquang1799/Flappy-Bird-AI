#include "neural-network/layer/Linear.h"

using namespace mc;

mc::Linear::Linear()
{
	name = "Linear";
}

mc::Linear::~Linear()
{

}

Matrix& mc::Linear::forward(Matrix& A_in, Matrix& W, Matrix& B, Matrix& S)
{
	this->A_in = A_in; // store A_in matrix for backwarding

	for (int i = 0; i < W.n_row; i++)
	{
		auto sum = B(i);

		for (int j = 0; j < W.n_col; j++)
		{
			sum += A_in(j) * W(i, j);
		}

		S(i) = sum;
	}

	return S;
}

Matrix& mc::Linear::backward(Matrix& dE2S, Matrix& dE2W)
{
	// dE2W = dE2S * A_in;
	for (int i = 0; i < dE2W.n_row; i++)
	{
		for (int j = 0; j < dE2W.n_col; j++)
		{
			dE2W(i, j) = dE2S(i) * A_in(j);
		}
	}
	return dE2W;
}
