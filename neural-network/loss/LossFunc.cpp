#include "neural-network/loss/LossFunc.h"

using namespace mc;


Matrix mc::loss::MeanSquarredError::Get(Matrix& predict, Matrix& target)
{
	Matrix E = Matrix(predict.n_row, predict.n_col);
	for (int j = 0; j < E.n_col; j++)
	{
		float delta = target(j) - predict(j);
		E(j) = 0.5f * delta * delta;
	}

	return E;
}

Matrix mc::loss::MeanSquarredError::Gradient(Matrix& predict, Matrix& target)
{
	Matrix dE = Matrix(predict.n_row, predict.n_col);
	for (int j = 0; j < dE.n_col; j++)
	{
		float delta = target(j) - predict(j);
		dE(j) = -delta;
	}

	return dE;
}

Matrix mc::loss::CrossEntropy::Get(Matrix& predict, Matrix& target)
{
	return predict;
}

Matrix mc::loss::CrossEntropy::Gradient(Matrix& predict, Matrix& target)
{
	return predict;
}
