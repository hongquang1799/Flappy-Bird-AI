#include "neural-network/layer/Dense.h"
#include "neural-network/layer/Linear.h"
#include "neural-network/layer/AtvtFunc.h"

using namespace mc;

mc::Dense::Dense(size_t n_row, size_t n_col)
{
	this->n_row = n_row;
	this->n_col = n_col;

	W.allocateMemory(n_row, n_col).randomize(-1.0f, 1.0f);
	B.allocateMemory(1, n_row).randomize(0.0f, 1.0f);
	S.allocateMemory(1, n_row);
	A.allocateMemory(1, n_row);
	dE2S.allocateMemory(1, n_row);
	dE2W.allocateMemory(n_row, n_col);

	linear = new Linear();
}

mc::Dense::~Dense()
{
	delete linear;
	delete atvt;
}

void mc::Dense::Log()
{
	DEBUG_LOG("Layer: Type-Dense\n");
	DEBUG_LOG("       Shape-%dx%d\n", n_row, n_col);
	DEBUG_LOG("       Weight\n");
	W.log();
	DEBUG_LOG("       Bias\n");
	B.log();
	DEBUG_LOG("       Activation-%s\n", atvt->name.c_str());
}

Matrix& mc::Dense::Forward(Matrix& A_in)
{
	// linear forward S = W * A_in + B
	S = linear->forward(A_in, W, B, S);
	/*DEBUG_LOG("After %s\n", linear->name.c_str());
	S.log();*/

	// activation forward A = activation(S)
	A = atvt->forward(S, A);
	/*DEBUG_LOG("After %s\n", atvt->name.c_str());
	A.log();*/

	return A;
}

Matrix& mc::Dense::Backward(Matrix& dE2A)
{
	// activation backward
	dE2S = atvt->backward(dE2A, dE2S, S, A);

	// calculate dError for next backfowarding (of previous layer) dE2A = dE2S x W(Transpose)
	dE2A.allocateMemory(1, n_col);
	
	for (int i = 0; i < n_col; i++)
	{
		float sum = 0.0f;
		for (int j = 0; j < n_row; j++)
		{
			sum += dE2S(j) * W(j, i);
		}
		dE2A(i) = sum;
	}

	// linear backward
	dE2W = linear->backward(dE2S, dE2W);

	return dE2A;
}

Matrix mc::Dense::GetWeights() const
{
	return W;
}

Matrix mc::Dense::GetBiases() const
{
	return B;
}
