#ifndef LINEAR_H
#define LINEAR_H

#include "neural-network/matrix/Matrix.h"
#include "string"

namespace mc
{
	class Linear
	{
	public:
		Linear();
		virtual ~Linear();

		Matrix& forward(Matrix& A_in, Matrix& W, Matrix& B, Matrix& S);
		Matrix& backward(Matrix& dE2S, Matrix& dE2W);

		std::string name;
	private:
		Matrix A_in; // output matrix from previous layer
	};
}

#endif
