#include "precompiled_header.h"
#include "neuralNetwork.h"
#include "../../core/utils.h"
#include "../../core/log.h"
#include "../../core/archivedata.h"

namespace Nexus
{
	// Set the number of inputs and weights for this neuron
	Neuron::Neuron(int numWeights)
	{
		iNumInputs = numWeights + 1;
		for (int i = 0; i < iNumInputs; i++)
		{
			vecWeight.push_back(randomClamped());
		}
	}

	NeuronLayer::NeuronLayer(int numNeurons, int numInputs)
	{
		iNumNeurons = numNeurons;
		for (int i = 0; i < iNumNeurons; i++)
		{
			vecNeuron.push_back(Neuron(numInputs));
		}
	}

	NeuralNet::NeuralNet()
	{
		numInputs = 0;
		numOutputs = 0;
		numLayers = 0;
		numNeuronsPerLayer = 0;
	}

	bool NeuralNet::create(int numInputs, int numOutputs, int numLayers, int numNeuronsPerLayer)
	{
		if (numInputs < 1)
			return false;
		if (numOutputs < 1)
			return false;
		if (numLayers < 0)
			return false;
		if (numNeuronsPerLayer < 1)
			return false;

		numInputs = numInputs;
		numOutputs = numOutputs;
		numLayers = numLayers;
		numNeuronsPerLayer = numNeuronsPerLayer;

		// Create the layers of the network
		if (numLayers > 0)
		{
			// Create first hidden layer
			layers.push_back(NeuronLayer(numNeuronsPerLayer, numInputs));

			for (int i = 0; i < numLayers - 1; i++)
			{
				layers.push_back(NeuronLayer(numNeuronsPerLayer, numNeuronsPerLayer));
			}

			// Create output layer
			layers.push_back(NeuronLayer(numOutputs, numNeuronsPerLayer));
		}
		else
		{
			// Create output layer
			layers.push_back(NeuronLayer(numOutputs, numInputs));
		}
		return true;
	}

	void NeuralNet::load(const std::string& filename)
	{
		ArchiveData ad;
		std::string err("NeuralNet::load(\"");
		err.append(filename);
		err.append("\" failed.");
		if (!ad.loadZipDisk(filename))
			Log::getPointer()->exception(err);

		if (!ad.read(numInputs))
			Log::getPointer()->exception(err);

		if (!ad.read(numOutputs))
			Log::getPointer()->exception(err);

		if (!ad.read(numLayers))
			Log::getPointer()->exception(err);

		if (!ad.read(numNeuronsPerLayer))
			Log::getPointer()->exception(err);

		// create the neural network, ready to load in the values
		create(numInputs, numOutputs, numLayers, numNeuronsPerLayer);

		// Read in number of weights
		int size = 0;
		if (!ad.read(size))
			Log::getPointer()->exception(err);

		// Read in weights
		std::vector<double> weights;
		double temp = 0;
		for (int i = 0; i < size; i++)
		{
			if (!ad.read(temp))
				Log::getPointer()->exception(err);
			weights.push_back(temp);
		}

		// Put weights into NN
		putWeights(weights);
	}

	void NeuralNet::save(const std::string& filename)
	{
		std::string err("NeuralNet::save(\"");
		err.append(filename);
		err.append("\" failed.");
		FILE* f = NULL;
		fopen_s(&f, filename.c_str(), "wb");
		if (f == NULL)
			Log::getPointer()->exception(err);

		fwrite(&numInputs, 1, sizeof(int), f);
		fwrite(&numOutputs, 1, sizeof(int), f);
		fwrite(&numLayers, 1, sizeof(int), f);
		fwrite(&numNeuronsPerLayer, 1, sizeof(int), f);

		// Retrieve weights from NN
		std::vector<double> weights;
		weights = getWeights();

		// Write out number of weights
		unsigned int size = unsigned int(weights.size());
		fwrite(&size, 1, sizeof(int), f);

		// Write all weights
		for (int i = 0; i < (int)weights.size(); i++)
		{
			fwrite(&weights[i], 1, sizeof(double), f);
		}
		fclose(f);
	}

	std::vector<double> NeuralNet::update(std::vector<double>& inputs)
	{
		// Stores the resultant outputs from each layer
		std::vector<double> outputs;

		int cWeight = 0;

		// First check that we have the correct amount of inputs
		if (inputs.size() != numInputs)
		{
			std::string err;
			err.append("NeuralNet::update failed. Invalid number of inputs given!\nWas expecting ");
			err.append(std::to_string(numInputs));
			err.append(" inputs but was given ");
			err.append(std::to_string(inputs.size()));

			Log::getPointer()->exception(err);
		}

		// For each layer....
		for (int i = 0; i < numLayers + 1; i++)
		{
			if (i > 0)
			{
				inputs = outputs;
			}

			outputs.clear();

			cWeight = 0;

			// For each neuron sum the (inputs * corresponding weights).Throw 
			// the total at our sigmoid function to get the output.
			for (int j = 0; j < layers[i].iNumNeurons; j++)
			{
				double netinput = 0;

				int	NumInputs = layers[i].vecNeuron[j].iNumInputs;

				// For each weight
				for (int k = 0; k < NumInputs - 1; k++)
				{
					// Sum the weights x inputs
					netinput += layers[i].vecNeuron[j].vecWeight[k] * inputs[cWeight++];
				}

				// Add in the bias
				netinput += layers[i].vecNeuron[j].vecWeight[NumInputs - 1] *
					-1; // Bias value

				// We can store the outputs from each layer as we generate them. 
				// The combined activation is first filtered through the sigmoid function
				outputs.push_back(sigmoid(netinput));
				cWeight = 0;
			}
		}

		// Update members, so we're all upto date
		setInputs(inputs);
		outputs.clear();
		outputs = outputs;

		return outputs;
	}

	void NeuralNet::setInputs(std::vector<double>& inputs)
	{
		inputs.clear();
		inputs = inputs;
	}

	void NeuralNet::update(void)
	{
		outputs = update(inputs);
	}

	std::vector<double> NeuralNet::getOutputs(void)
	{
		return outputs;
	}

	double NeuralNet::sigmoid(double value)
	{
		//return ( 1 / ( 1 + exp(-value / response)));  // response affects curve. Higher values = flatter, lower=steeper
		return (1 / (1 + exp(-value)));
	}

	std::vector<double> NeuralNet::getWeights(void)
	{
		std::vector<double> weights;

		// For each layer
		for (int i = 0; i < numLayers + 1; i++)
		{
			// For each neuron
			for (int j = 0; j < layers[i].iNumNeurons; j++)
			{
				// For each weight
				for (int k = 0; k < layers[i].vecNeuron[j].iNumInputs; k++)
				{
					weights.push_back(layers[i].vecNeuron[j].vecWeight[k]);
				}
			}
		}
		return weights;
	}

	void NeuralNet::putWeights(std::vector<double>& weights)
	{
		/* old
		int cWeight = 0;

		// For each layer
		for (int i=0; i<miNumLayers + 1; i++)
		{
			// For each neuron
			for (int j=0; j<mvecLayer[i].miNumNeurons; j++)
			{
				// For each weight
				for (int k=0; k<mvecLayer[i].mvecNeuron[j].miNumInputs; k++)
				{
					mvecLayer[i].mvecNeuron[j].mvecWeight[k] = weights[cWeight++];
				}
			}
		}
		*/
		int cWeight = 0;


		// For each layer
		for (int i = 0; i < numLayers + 1; i++)
		{
			// For each neuron
			for (int j = 0; j < layers[i].iNumNeurons; j++)
			{
				// For each weight
				for (int k = 0; k < layers[i].vecNeuron[j].iNumInputs; k++)
				{
					if (cWeight >= (int)weights.size())
					{
						break;
						Log::getPointer()->exception("NeuralNet::putWeights() failed");
					}
					layers[i].vecNeuron[j].vecWeight[k] = weights[cWeight];
					cWeight++;

				}
			}
		}
	}

	int NeuralNet::getNumberOfWeights(void)
	{
		int weights = 0;

		// For each layer
		for (int i = 0; i < numLayers + 1; i++)
		{
			// For each neuron
			for (int j = 0; j < layers[i].iNumNeurons; j++)
			{
				// For each weight
				for (int k = 0; k < layers[i].vecNeuron[j].iNumInputs; k++)
				{
					weights++;
				}
			}
		}
		return weights;
	}

	std::vector<int> NeuralNet::calculateSplitPoints()
	{
		std::vector<int> splitPoints;

		int weightCounter = 0;

		// For each layer
		for (int i = 0; i < numLayers + 1; i++)
		{
			// For each neuron
			for (int j = 0; j < layers[i].iNumNeurons; j++)
			{
				// For each weight
				for (int k = 0; k < layers[i].vecNeuron[j].iNumInputs; k++)
				{
					weightCounter++;
				}

				splitPoints.push_back(weightCounter - 1);
			}
		}
		return splitPoints;
	}
}