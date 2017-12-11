//
//  Evolution.hpp
//  N-Queens
//
//  Created by Matias Barcenas on 3/16/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#ifndef Evolution_hpp
#define Evolution_hpp

#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <vector>
#include <queue>
#include <iomanip>
#include <list>


// The following class represents a generic algorithm for evolution of genetics.
// The class implements generic behaviour used by genes when transgressing generations.
class Evolution {
public:

	static std::default_random_engine Randomizer;

	// The following constants represent default values the class uses for convenience.
	static const int DefaultGenerationalLimit = 1000;		// Maximum generations allowed
	static constexpr float DefaultParentPercent = 0.10;		// Parent count percentage


	// The following class denotes a set of genes, or a Genotype.
	// The class defines behaviour genes are expected to have, such as mutation,
	// crossover (or recombination), and mating, when dealing with two Genotypes.
	class Genotype {
	public:

		static constexpr float Epsilon = 0.0001;			// Used to prevent division by zero
		static const int DefaultGeneLength = 10;			// Length of genes for each genotype
		static constexpr float DefaultMutationPercent = 0.1;// Mutation chance as a percentage

		typedef std::vector<Genotype> Children;
		typedef std::vector<int> Gene;


		// The function below defines what exactly it means to be fit, and returns a value
		// indicating the fitness of any idividual instance of a Genotype.
		//
		// This method works by counting each queen in each diagonal on the board to achieve O(n).
		// Beginning with top left inremnenting down to bottom right, we count all queens
		// in the board. If there are more queens than one in the diagonal, then there are
		// (n - 1) * 2 collisions in the diagonal. The same concept applies to the other set
		// of diagonals, starting from top right to bottom left.
		// NOTICE: This method is a class method, not an instance method.
		// NOITCE: The lower the number, the better the fitness!
		static int Fitness(const Genotype& genotype);

		// Input and output methods.
		friend std::istream& operator>>(std::istream& is, Genotype &genotype);
		friend std::ostream& operator<<(std::ostream& os, const Genotype &genotype);

		// The following three methods are used for comparison of Genotypes.
		// These all work by comparing the fitness of each Genotype. This means that
		// if gene "A" has a fitness of 0.5 and gene "B" has a fitness of 0.75,
		// gene B > A. The remaining two methods follow the same concept.
		bool operator>(const Genotype &other) const;
		bool operator<(const Genotype &other) const;
		bool operator==(const Genotype &other) const;

		// The following method compares the genes of one genotype to another.
		// If the genes of the second Genotype are exactly the same as the first,
		// the method returns true, false is returned otherwise.
		Genotype& operator=(const Genotype& other);

		// The following two methods are used to access individual genes of the Genotype.
		// These both do the exact same thing, the only reason there's two is for convenience.
		int operator[](const Gene::size_type i) const;
		int gene(const Gene::size_type i) const;

		Gene::size_type length() const;

		// This method returns how fit a particular instance of a Genotype is.
		// The function relies on the fitness measure done by the class method "Fitness,"
		// which evaluates the fitness of each individual instance of a Genotype.
		float fitness() const;

		// The following methods define generic behavior expected of genotypes.
		Children mate(const Genotype &genotype) const;
		Children crossover(const Genotype &genotype) const;
		Genotype& mutate();

		// Constructors and destructor.
		Genotype(const Gene &genes);
		Genotype(const Genotype &genotype);
		Genotype(const int geneLength = DefaultGeneLength);
		~Genotype();

	protected:
		int _fitness;
		Gene _gene;
	};

	typedef std::vector<Genotype> Population;

	// I/O operators for the object
	friend std::istream& operator>>(std::istream& is, Evolution &evolution);
	friend std::ostream& operator<<(std::ostream& os, const Evolution &evolution);

	// The method below returns whether or not the algorithm is still
	// attempting to solve the problem by evolution. The method returns
	// false when the maximum amount of steps has been exceeded.
	bool solving() const;

	// solved() returns whether or not the algorithm found a solution candiate.
	// This means the genotype returned by the method "solution()" is a solution
	// for solving the problem being tackled by the genetic algorithm.
	bool solved() const;

	// This method returns the best solution that's been found thus far.
	// A perfect solution is found when the method "solved()" returns true.
	Genotype solution() const;

	// This method processes a generation. In other words, the algorithm is the
	// evolution step. It selects parents from the population pool, creates children,
	// and selects survivors for the next generation and adds them to the population pool.
	void evolve();

	// Returns the generations that have spanned since the start of evolution,
	// not counting the latest generation.
	int progress() const;

	// Returns the maximum amount of generations allowed to be spanned.
	// When the limit is surpassed, the algorithm stops solving for the solution,
	// but the best solution thus far may be obtained by calling "solution()."
	int progressLimit() const;

	// The method below attempts to find a perfect solution to the problem.
	// It attempts to find the fittest individual by evolving after each generation.
	// The method keeps evolving till it's spanned the maximum amount of generations,
	// until it's found a perfect solution, whichever comes first.
	// The method returns the fittest individual it's found up to the point of termination,
	// meaning the returned Genotype might not be a solution.
	Genotype resolve();


	Evolution(const int geneLength = Genotype::DefaultGeneLength,
			  const int generationLimit = DefaultGenerationalLimit);
	~Evolution();

protected:
	Population _population;
	int _populationSize;
	int _progressLimit;
	int _progress;


	// Randomly extracts parents from the population given. These parent candidates
	// are then subjected to tournament selection based on their fitness.
	static Population _extractParents(Population &population, int parentCount);

	// Extracts genotypes from the population given based on their fitness.
	// All individuals are ranked and the fittest survivorCount individuals are kept.
	// This directly implies I'm using a fitness-biased approach to selecting survivors.
	static Population _extractNonSurvivors(Population &population, int survivorCount);
};

#endif /* Evolution_hpp */
