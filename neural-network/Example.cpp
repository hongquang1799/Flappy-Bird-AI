
#include "neural-network/matrix/Matrix.h"
#include "neural-network/Sequential.h"
#include "neural-network/layer/Dense.h"
#include "neural-network/layer/AtvtFunc.h"
#include "neural-network/loss/LossFunc.h"
#include "neural-network/optimizer/Optimizer.h"
#include "time.h"

using namespace mc;

void Example()
{
	int n_row = 1;
	int n_col = 2;

	Matrix input(n_row, n_col);
	input.set({ {0.05f, 0.1f} });

	Sequential model;
	model.SetInputUnit(n_col);

	auto d1 = model.AddDense(2, "sigmoid");
	d1->GetWeights().set({ {0.15f, 0.2f},
						   {0.25f, 0.3f} });
	d1->GetBiases().set({ {0.35f, 0.35f} });

	auto d2 = model.AddDense(2, "sigmoid");
	d2->GetWeights().set({ {0.4f, 0.45f},
						   {0.5f, 0.55f} });
	d2->GetBiases().set({ { 0.6f, 0.6f } });

	model.SetLoss("meanSquaredError");
	model.SetOptimizer("sgd", 0.5f);
	model.Log();

	Matrix output = model.Predict(input);

	Matrix target(1, 2);
	target.set({ {0.01f, 0.99f} });

	model.Train(input, target);
	
	DEBUG_LOG("Output:\n");
	output.log();
}

//void main()
//{
//	srand(time(NULL));
//
//	Example();
//
//	getchar();
//}