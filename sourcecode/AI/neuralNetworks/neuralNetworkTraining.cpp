#include "precompiled_header.h"
#include "neuralNetworkTraining.h"
#include "../../core/utils.h"
#include "../../core/log.h"

namespace Nexus
{
	void NeuralNetGeneticAlgorithm::init(int populationSizeIn,
		int numInputsPerBrainIn,
		int numOutputsPerBrainIn,
		int numLayersIn,
		int numNeuronsPerLayerIn,
		double mutationRateIn,
		double mutationMaxPerturbIn,
		int numEliteIn,
		int numEliteCopiesIn,
		double crossoverRateIn)
	{
		// Clear every vector
		mvecPop.clear();
		mvecNN.clear();
		mvecNeuronSplitPoints.clear();

		// Make sure if ( (miNumElite * miNumEliteCopies)%2 == 0)
		if ((numEliteIn * numEliteCopiesIn) % 2 != 0)
			Log::getPointer()->exception("NeuralNetGeneticAlgorithm::init failed. iNumElite and iNumEliteCopies, when multiplied must give an even number.");

		// Store given params in members
		populationSize = populationSizeIn;
		numInputsPerBrain = numInputsPerBrainIn;
		numOutputsPerBrain = numOutputsPerBrainIn;
		numLayers = numLayersIn;
		numNeuronsPerLayer = numNeuronsPerLayerIn;
		mutationRate = mutationRateIn;
		mutationMaxPerturb = mutationMaxPerturbIn;
		numElite = numEliteIn;
		numEliteCopies = numEliteCopiesIn;
		crossoverRate = crossoverRateIn;
		totalElite = numElite * numEliteCopiesIn;

		// Create NNs
		for (int i = 0; i < populationSize; i++)
		{
			mvecNN.push_back(NeuralNet());
			mvecNN[i].create(numInputsPerBrain,
				numOutputsPerBrain,
				numLayers,
				numNeuronsPerLayer);

		}

		// Get number of weights in a NN
		int numWeights = mvecNN[0].getNumberOfWeights();

		// Create genomes for each brain and weights for each genome with a random value between -1 and +1
		for (int i = 0; i < populationSize; i++)
		{
			//vector<double> weights; // Will hold the genome's weight values

			// Create new gene
			mvecPop.push_back(SGenome());

			// Create each weight
			for (int j = 0; j < numWeights; j++)
			{
				// Create weight
				mvecPop[i].vecWeights.push_back(randomClamped());

				// Get this weight, for storing into NN
				//weights.push_back(mvecPop[i].vecWeights[j]);
			}

			// Put the weights into NNs
			//mvecNN[i].putWeights(weights);
			mvecNN[i].putWeights(mvecPop[i].vecWeights);
		}

		// Calculates a brain's neuron split points
		mvecNeuronSplitPoints = mvecNN[0].calculateSplitPoints();

		generationNumber = 1;
		mdTotalFitness = 0;         // Total fitness for previous generation
		bestFitness = 0;            // Best fitness for previous generation
		averageFitness = 0;         // Average fitness for previous generation
		worstFitness = 0;           // Worst fitness for previous generation
	}

	void NeuralNetGeneticAlgorithm::putInputs(int brainNumber, std::vector<double>& inputs)
	{
		if (inputs.size() != numInputsPerBrain)
			return;
		if (brainNumber >= populationSize)
			return;

		mvecNN[brainNumber].setInputs(inputs);
	}

	void NeuralNetGeneticAlgorithm::update(void)
	{
#pragma omp parallel for
		for (int i = 0; i < populationSize; ++i)
		{
			mvecNN[i].update();
		}
	}

	void NeuralNetGeneticAlgorithm::update(unsigned int index)
	{
		mvecNN[index].update();
	}

	void NeuralNetGeneticAlgorithm::newGeneration(void)
	{
		generationNumber++;

		// Sort the population genomes for scaling and elitism
		sort(mvecPop.begin(), mvecPop.end());

		calculateFitness();

		// New vector to store new population of genomes
		std::vector<SGenome> vecNewPop;

		// Add some best performing genomes from current population into new population
		insertElite(vecNewPop);

		// Fill the rest of the new population's genomes by crossover/mutation etc
		while ((int)vecNewPop.size() < populationSize)
		{
			// Get two parents
			SGenome mum = getGenomeViaRoulette();
			SGenome dad = getGenomeViaRoulette();

			// Create two babies with crossing over of parent genes
			std::vector<double> baby1;
			std::vector<double> baby2;
			crossOver(mum.vecWeights, dad.vecWeights, baby1, baby2);

			// Mutate the offspring
			mutateWeights(baby1);
			mutateWeights(baby2);

			// Insert these babies into the new population
			vecNewPop.push_back(SGenome(baby1, // Weights
				0));  // Fitness reset
			if ((int)vecNewPop.size() >= populationSize)
			{
				break;
			}
			vecNewPop.push_back(SGenome(baby2, // Weights
				0));  // Fitness reset
		}

		// Now vecNewPop contains the new genes!
		// Copy this new population into the current population
		mvecPop = vecNewPop;

		// Update the NN with weights from new population
		for (int i = 0; i < populationSize; i++)
		{
			mvecNN[i].putWeights(mvecPop[i].vecWeights);
			mvecPop[i].dFitness = 0;  // Reset fitness
		}
	}

	void NeuralNetGeneticAlgorithm::crossOver(std::vector<double>& mum, std::vector<double>& dad, std::vector<double>& baby1, std::vector<double>& baby2)
	{
		// If the parents are the same, just return the parent's as the babies
		// Also return parents as children if the crossover rate was not reached
		if (mum == dad || randFloat() > crossoverRate)
		{
			baby1 = mum;
			baby2 = dad;
			return;
		}

		// Get two crossover points within the weights
		int cp1 = int(mvecNeuronSplitPoints[randInt(0, int(mvecNeuronSplitPoints.size()) - 2)]);
		int cp2 = int(mvecNeuronSplitPoints[randInt(0, int(mvecNeuronSplitPoints.size()) - 1)]);

		for (int i = 0; i < (int)mum.size(); i++)
		{
			// If outside the two crossover points, keep same genes
			if (i<cp1 || i>cp2)
			{
				baby1.push_back(mum[i]);
				baby2.push_back(dad[i]);
			}
			// Lets crossover
			else
			{
				baby1.push_back(dad[i]);
				baby2.push_back(mum[i]);
			}
		}
	}

	void NeuralNetGeneticAlgorithm::mutateWeights(std::vector<double>& genomeWeights)
	{
		// Loop through each weight
		for (int i = 0; i < (int)genomeWeights.size(); i++)
		{
			// Mutate it?
			if (randFloat() < mutationRate)
			{
				genomeWeights[i] += (randomClamped() * mutationMaxPerturb);
			}
		}
	}

	void NeuralNetGeneticAlgorithm::insertElite(std::vector<SGenome>& newPopulation)
	{
		// Make sure even numbers, otherwise roulette wheel selection will crash...
		if ((numElite * numEliteCopies) % 2 == 0)
		{
			int iGenomeIndex = numElite;
			while (iGenomeIndex > 0)
			{
				for (int i = 0; i < numEliteCopies; i++)
				{
					newPopulation.push_back(mvecPop[populationSize - 1 - iGenomeIndex]);
				}
				iGenomeIndex--;
			}
		}
	}

	SGenome NeuralNetGeneticAlgorithm::getGenomeViaRoulette(void)
	{
		double slice = (double)randFloat() * mdTotalFitness;

		SGenome selectedGenome;

		double dFitnessSoFar = 0;
		for (int i = 0; i < populationSize; i++)
		{
			dFitnessSoFar += mvecPop[i].dFitness;

			if (dFitnessSoFar >= slice)
			{
				selectedGenome = mvecPop[i];
				break;
			}
		}
		return selectedGenome;
	}

	void NeuralNetGeneticAlgorithm::calculateFitness(void)
	{
		mdTotalFitness = 0;
		worstFitness = 99999999.0f;
		bestFitness = -99999999.0f;
		for (int i = 0; i < populationSize; i++)
		{
			mdTotalFitness += mvecPop[i].dFitness;
			if ((float)mvecPop[i].dFitness > bestFitness)
				bestFitness = (float)mvecPop[i].dFitness;
			if ((float)mvecPop[i].dFitness < worstFitness)
				worstFitness = (float)mvecPop[i].dFitness;
		}
		averageFitness = float(mdTotalFitness / populationSize);
	}

	void NeuralNetGeneticAlgorithm::adjustBrainFitness(int brainNumber, float amount)
	{
		if (brainNumber >= populationSize)
			return;

		mvecPop[brainNumber].dFitness += double(amount);
		// Keep fitness above zero, otherwise roulette wheel screws up
		if (mvecPop[brainNumber].dFitness < 0)
			mvecPop[brainNumber].dFitness = 0;
	}

	void NeuralNetGeneticAlgorithm::setBrainFitness(int brainNumber, float value)
	{
		if (brainNumber >= populationSize)
			return;
		if (value < 0)
			mvecPop[brainNumber].dFitness = 0;
		else
			mvecPop[brainNumber].dFitness = value;
	}

	float NeuralNetGeneticAlgorithm::getBrainFitness(int brainNumber)
	{
		if (brainNumber >= populationSize)
			return 0;
		return float(mvecPop[brainNumber].dFitness);
	}

	std::vector<double> NeuralNetGeneticAlgorithm::getOutputs(int brainNumber)
	{
		std::vector<double> outputs;
		if (brainNumber >= populationSize)
			return outputs;

		outputs = mvecNN[brainNumber].getOutputs();
		return outputs;
	}

	void NeuralNetGeneticAlgorithm::saveBestBrain(const std::string& strFilename)
	{
		// Find index to highest fitness brain
		double dCurFitness = 0;
		int iBestBrain = 0;
		for (int b = 0; b < populationSize; b++)
		{
			double dBrainFitness = getBrainFitness(b);
			if (dCurFitness < dBrainFitness)
			{
				dCurFitness = dBrainFitness;
				iBestBrain = b;
			}
		}

		// Save the brain with the highest fitness
		mvecNN[iBestBrain].save(strFilename);
	}

	void NeuralNetGeneticAlgorithm::saveBrain(const std::string& filename, unsigned int brainNumber)
	{
		std::string err("NeuralNetGeneticAlgorithm::saveBrain(\"");
		err.append(filename);
		err.append("\", ");
		err.append(std::to_string(brainNumber));
		err.append(") failed. ");

		if (brainNumber >= (unsigned int)populationSize)
		{
			err.append(" Invalid brain number given");
			Log::getPointer()->exception(err);
		}

		mvecNN[brainNumber].save(filename);
	}

	//! Loads an individual brain into all of the Gen algs brains
	void NeuralNetGeneticAlgorithm::loadBrainIntoAll(const std::string& filename)
	{
		for (int b = 0; b < populationSize; b++)
		{
			mvecNN[b].load(filename);
		}
	}
}