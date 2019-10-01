#ifndef ATVT_FUNC_H
#define ATVT_FUNC_H

#include "neural-network/matrix/Matrix.h"
#include "string"

namespace mc
{
	class AtvtFunc
	{
	public:
		AtvtFunc() {};
		virtual ~AtvtFunc() {};

		virtual Matrix& forward(Matrix& S, Matrix& A) = 0;
		virtual Matrix& backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A) = 0;

		std::string name;
	protected:
	
	};

	class Sigmoid : public AtvtFunc
	{
	public:
		Sigmoid() { name = "Sigmoid"; }
		virtual ~Sigmoid() {}

		Matrix& forward(Matrix& S, Matrix& A) override;
		Matrix& backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A) override;
	};

	class ReLU : public AtvtFunc
	{
	public:
		ReLU() { name = "ReLU"; }
		virtual ~ReLU() {}

		Matrix& forward(Matrix& S, Matrix& A) override;
		Matrix& backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A) override;
	};

	class Tanh : public AtvtFunc
	{
	public:
		Tanh() { name = "Tanh"; }
		virtual ~Tanh() {}

		Matrix& forward(Matrix& S, Matrix& A) override;
		Matrix& backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A) override;
	};

	class Softmax : public AtvtFunc
	{
	public:
		Softmax() {}
		virtual ~Softmax() {}

		Matrix& forward(Matrix& S, Matrix& A) override;
		Matrix& backward(Matrix& dE2A, Matrix& dE2S, Matrix& S, Matrix& A) override;
	};
}

#endif
