#pragma once
#include "../../precompiled_header.h"
#include "neuralNetwork.h"

namespace Nexus
{
	// Represents a single brain's genes
	struct Genome
	{
		Genome() :dFitness(0) {}
		Genome(std::vector <double> w, double f) : vecWeights(w), dFitness(f) {}
		//overload '<' used for sorting
		friend bool operator<(const Genome& lhs, const Genome& rhs)
		{
			return (lhs.dFitness < rhs.dFitness);
		}

		std::vector<double>	vecWeights;
		double dFitness;

	};

	// Genetic algorithm class for NN training
	class NeuralNetGeneticAlgorithm
	{
	public:
		// Initialises the genetic algorithm with all the NNs and settings, ready for training
		void init(int populationSizeIn,			// Population size
			int numInputsPerBrainIn,				// Num inputs per brain
			int numOutputsPerBrainIn,				// Num outputs per brain
			int numLayersIn = 1,					// Num layers
			int numNeuronsPerLayerIn = 10,		// Num neurons per layer
			double mutationRateIn = 0.1,			// Mutation rate
			double mutationMaxPerturbIn = 0.3,	// Mutation max perturbation
			int numEliteIn = 3,					// Num elite
			int numEliteCopiesIn = 2,				// Elite copies
			double crossoverRateIn = 0.7f			// Crossover rate
		);

		// Gives a brain it's inputs
		// Inputs should be centered around 0.0 for optimal "performance" (not speed, but quality)
		void putInputs(int brainNumber, std::vector<double>& inputs);

		// Update all brains
		void update(void);

		// Updates a single brain
		void update(unsigned int index);

		// Retrieves the outputs of a brain in the form of a vector
		std::vector<double> getOutputs(int brainNumber);

		// Generates a new generation of brains from the current one by "breeding" the best, mutating, generating offspring etc
		void newGeneration(void);

		// Adjusts a brain's fitness by the value given
		void adjustBrainFitness(int brainNumber, float amount);

		// Set a brain's fitness
		void setBrainFitness(int brainNumber, float value);

		// Returns the given brain's current fitness value
		float getBrainFitness(int brainNumber);

		// Retrieves current generation number
		int getGenerationNumber(void) { return generationNumber; }

		// Returns best fitness of last generation
		float getBestFitness(void) { return bestFitness; }

		// Returns average fitness of last generation
		float getAvrFitness(void) { return averageFitness; }

		// Returns worst fitness for previous generation
		float getWorstFitness(void) { return worstFitness; }

		// Returns the total number of elite NN's copied each new population
		int getNumElite(void) { return totalElite; }

		// Returns the population size given to this class on call to init()
		int getPopSize(void) { return populationSize; }

		// Saves the best brain to the given filename, for loading into a CNeuralNet object
		// If saving fails, an exception occurs
		void saveBestBrain(const std::string& strFilename);

		// Saves the specified brain number to the given filename
		// If saving fails, an exception occurs
		void saveBrain(const std::string& filename, unsigned int brainNumber);

		// Loads an individual brain into all of the genetic algorithm's brains (first from zip, then file)
		void loadBrainIntoAll(const std::string& filename);

	private:
		void calculateFitness(void);							// Calculates best/worst/avr etc of current population
		void insertElite(std::vector<Genome>& newPopulation);	// Inserts the some of the best performing genomes into a vector of SGenome representing the new population
		Genome getGenomeViaRoulette(void);						// Returns a genome from the current population via random roulette method

		// Creates a couple of babies from the given mvecWeights of mum and dad 
		void crossOver(std::vector<double>& mum, std::vector<double>& dad, std::vector<double>& baby1, std::vector<double>& baby2);
		void mutateWeights(std::vector<double>& genomeWeights);  // Mutates a genome's weights

		int populationSize;         // Population size
		int numInputsPerBrain;      // Number of inputs per brain
		int numOutputsPerBrain;     // Number of outputs per brain
		int numLayers;              // Number of layers (try 1)
		int numNeuronsPerLayer;     // Number of neurons per layer (try 10)
		double mutationRate;        // Mutation rate (try 0.1)
		double mutationMaxPerturb;  // Mutation max perturbation (try 0.3)
		int numElite;               // Number of elite
		int numEliteCopies;         // Elite copies
		int totalElite;             // Total number of elite NN's copied each new population
		double crossoverRate;       // Crossover rate (try 0.7)

		std::vector<Genome> vecPop;				// Population genomes
		std::vector<NeuralNet> vecNN;			// Brains
		std::vector<int> vecNeuronSplitPoints;  // A vector of integers which represents the boundaries of each neuron within a brain
		// Used on newGeneration to prevent crossover in the middle of a neuron

		int generationNumber;       // Current generation number
		double mdTotalFitness;      // Total fitness of entire previous generation
		float bestFitness;          // Best fitness for previous generation
		float averageFitness;		// Average fitness for previous generation
		float worstFitness;         // Worst fitness for previous generation
	};

}