#ifndef DENSE_H
#define DENSE_H

#include "neural-network/layer/Layer.h"
#include "neural-network/matrix/Matrix.h"

namespace mc
{
	class Dense : public Layer
	{
	public:
		Dense(size_t n_row, size_t n_col);
		virtual ~Dense();

		Matrix& Forward(Matrix& A);
		Matrix& Backward(Matrix& dE2A);

		Matrix GetWeights() const;
		Matrix GetBiases() const;

		void Log();

		size_t n_row;
		size_t n_col;

		// weight 
		Matrix W;	

		// bias
		Matrix B;

		// weighted sum
		Matrix S;

		// activation (of weighted sum)
		Matrix A;

		// gradient to B (derivative of E wrt S)
		Matrix dE2S;

		// gradient to W (derivative of E wrt W)
		Matrix dE2W; 
	
		class Linear * linear;
		class AtvtFunc * atvt;
	};
}

#endif