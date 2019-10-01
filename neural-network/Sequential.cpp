#include "neural-network/Sequential.h"
#include "neural-network/layer/Dense.h"
#include "neural-network/layer/AtvtFunc.h"
#include "neural-network/loss/LossFunc.h"


using namespace mc;
using namespace mc::opt;
using namespace mc::loss;

mc::Sequential::Sequential() : 
	lossFunc(NULL),
	optimizer(NULL)
{
}

mc::Sequential::~Sequential()
{
	for (auto dense : listOfDenses)
	{
		delete dense;
	}

	delete lossFunc;
	delete optimizer;
}

Matrix mc::Sequential::Predict(Matrix& input)
{
	Matrix A_in = input;
	for (auto dense : listOfDenses)
	{
		A_in = dense->Forward(A_in);
	}

	return A_in;
}

void mc::Sequential::Train(Matrix& input, Matrix& target)
{
	Matrix predict = Predict(input);
	Matrix dE = lossFunc->Gradient(predict, target);
	DEBUG_LOG("dError:\n");
	dE.log();
	

	for (auto it = this->listOfDenses.rbegin(); it != this->listOfDenses.rend(); it++)
	{
		Dense * dense = (*it);
		dE = dense->Backward(dE);

		optimizer->Optimize(dense->dE2W, dense->W, dense->dE2S, dense->B);

		DEBUG_LOG("W:\n");
		dense->W.log();
	}
}

void mc::Sequential::SetInputUnit(size_t unit)
{
	inputUnit = unit;
}

Dense * mc::Sequential::AddDense(size_t unit, const std::string& activation)
{
	Dense * dense = NULL;

	if (listOfDenses.size() == 0)
	{
		dense = new Dense(unit, inputUnit);
		listOfDenses.push_back(dense);
	}
	else
	{
		dense = new Dense(unit, listOfDenses.back()->n_row);
		listOfDenses.push_back(dense);
	}

	if (activation == "sigmoid")
	{
		dense->atvt = new Sigmoid();
	}
	else if (activation == "relu")
	{
		dense->atvt = new ReLU();
	}
	else if (activation == "tanh")
	{
		dense->atvt = new Tanh();
	}
	else if (activation == "softmax")
	{
		dense->atvt = new Softmax();
	}

	dense->atvt->name = activation;

	return dense;
}

Dense * mc::Sequential::GetDense(size_t index)
{
	return listOfDenses.at(index);
}

void mc::Sequential::SetLoss(const std::string& loss)
{
	if (loss == "meanSquaredError")
	{
		lossFunc = new MeanSquarredError();
	}
	else if (loss == "crossEntropy")
	{
		lossFunc = new CrossEntropy();
	}
}

void mc::Sequential::SetOptimizer(const std::string& optimize, float learning_rate)
{
	if (optimize == "sgd")
	{
		optimizer = new SGD(learning_rate);
	}
	else if (optimize == "momentum")
	{
		optimizer = new Momentum(learning_rate, 0.9f);
	}
	else if (optimize == "adam")
	{
		optimizer = new Adam(learning_rate, 0.1f);
	}

	optimizer->learning_rate = learning_rate;
}

void mc::Sequential::Log()
{
	DEBUG_LOG("Input Unit: %d\n", inputUnit);
	for (auto dense : listOfDenses)
	{
		dense->Log();
	}


}


