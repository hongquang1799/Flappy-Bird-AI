#ifndef FNN_H
#define FNN_H

#include "neural-network/matrix/Matrix.h"
#include "neural-network/optimizer/Optimizer.h"

namespace mc
{
	//namespace opt { class Optimizer; }
	namespace loss { class LossFunc; }

	class Sequential /*: public Model*/
	{
	public:
		Sequential();
		virtual ~Sequential();

		void SetInputUnit(size_t unit);

		class Dense * AddDense(size_t unit, const std::string& activation);
	
		class Dense * GetDense(size_t index);

		void SetLoss(const std::string& loss);
		
		void SetOptimizer(const std::string& optimize, float learning_rate);

		Matrix Predict(Matrix& input);
		
		void Train(Matrix& input, Matrix& target);

		void Log();
	private:
		size_t inputUnit;

		std::vector<class Dense*> listOfDenses;

		loss::LossFunc * lossFunc;

		opt::Optimizer * optimizer;
	};
}

#endif
