#pragma once
#include "../../precompiled_header.h"

namespace Nexus
{
	// Represents a neuron for use within CNeuralNet class
	struct Neuron
	{
		// Constructor to set the number of weights for this neuron
		Neuron(int numWeights);

		std::vector<double>  vecWeight;	// The weights
		int iNumInputs;					// Number of weights for this neuron
	};

	// Represents a layer of neurons
	struct NeuronLayer
	{
	public:
		// Constructor to create neurons for this layer
		NeuronLayer(int numNeurons, int numInputs);

		std::vector<Neuron> vecNeuron;	// The neurons
		int iNumNeurons;				// Number of neurons in this layer
	};

	// Neural network class
	class NeuralNet
	{
	public:
		NeuralNet();

		// Create a new network with random weights, ready for training or calculations
		bool create(int numInputs,		// Number of inputs
			int numOutputs,				// Number of outputs
			int numLayers,				// Number of layers
			int numNeuronsPerLayer		// Number of neurons for each layer
		);

		// Load a previously saved neural network's structure from a file
		// strFilename is the name of the previously saved neural network file (first from zip archive, then file on disk)
		// If Loading failed, an exception occurs
		void load(const std::string& filename);

		// Save current neural network structure/weights etc to a file
		// strFilename is the name of the file to save the NN to
		// If saving failed, an exception occurs
		void save(const std::string& filename);

		// Updates the neural network and returns the output values calculated from the given set input values
		// If invalid number of inputs are parsed, an exception occurs
		// Inputs should be centered around 0.0 for optimal "performance" (not speed, but quality)
		std::vector<double> update(std::vector<double>& inputs);

		// Gives this NN some inputs to work with
		// Inputs should be centered around 0.0 for optimal "performance" (not speed, but quality)
		void setInputs(std::vector<double>& inputs);

		// Updates the neural network's output values with the currently set input values
		void update(void);

		// Retrieves the calculated outputs from this brain after calling update method
		std::vector<double> getOutputs(void);

		// Returns all the weights in the network as a vector of doubles for use by the CNNGenAlg class
		std::vector<double> getWeights(void);

		// Returns the total number of weights in neural net
		int getNumberOfWeights(void);

		// Puts the given vector of weights into the neurons, for use by the CNNGenAlg class
		void putWeights(std::vector<double>& weights);

		// Calculate the points which define the individual neurons
		std::vector<int> calculateSplitPoints();

	private:
		inline double sigmoid(double value);
		int numInputs;						// Number of inputs
		int numOutputs;						// Number of outputs
		int numLayers;						// Number of layers
		int numNeuronsPerLayer;				// Number of neurons per layer
		std::vector<NeuronLayer> layers;	// Each neuron layer containing the neurons including the output layer
		std::vector<double> inputs;			// This brain's inputs
		std::vector<double> outputs;		// This brain's outputs
	};

}