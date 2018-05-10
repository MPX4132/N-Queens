//
//  Evolution.cpp
//  N-Queens
//
//  Created by Matias Barcenas on 3/16/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include "Evolution.hpp"


// ================================================================
#pragma mark - Genotype Implementation
// ================================================================
// Define the randomizer for the genotype class
std::default_random_engine Evolution::Randomizer = std::default_random_engine(0);

int Evolution::Genotype::Fitness(const Evolution::Genotype &genotype)
{
	int collisions = 0;

	std::vector<int> diagonalsL(genotype.length() * 2 - 1);
	std::vector<int> diagonalsR(genotype.length() * 2 - 1);

	for (Evolution::Genotype::Gene::size_type i = 0; i < genotype.length(); i++) {
		diagonalsL[ i + genotype.gene(i) ]++;
	}

	const Evolution::Genotype::Gene::size_type length = genotype.length() - 1;
	for (Evolution::Genotype::Gene::size_type i = 0; i < genotype.length(); i++) {
		diagonalsR[ i + (length - genotype.gene(i)) ]++;
	}

	for (const int diagonalCount : diagonalsL) {
		if (diagonalCount > 1) collisions += (diagonalCount - 1) * 2;
	}

	for (const int diagonalCount : diagonalsR) {
		if (diagonalCount > 1) collisions += (diagonalCount - 1) * 2;
	}

	return collisions;
}

std::istream& operator>>(std::istream &is, Evolution::Genotype &genotype)
{
	return is;
}

std::ostream& operator<<(std::ostream &os, const Evolution::Genotype &genotype)
{
	os << '<';
	for (std::vector<int>::size_type i = 0; i < genotype._gene.size(); i++) {
		os << genotype[i];
		if (i+1 < genotype._gene.size()) os << ", ";
	}
	os << '>';
	return os;
}

bool Evolution::Genotype::operator>(const Evolution::Genotype &other) const
{
	return this->fitness() > other.fitness();
}

bool Evolution::Genotype::operator<(const Evolution::Genotype &other) const
{
	return this->fitness() < other.fitness();
}

bool Evolution::Genotype::operator==(const Evolution::Genotype &other) const
{
	return this->fitness() == other.fitness();
}

Evolution::Genotype &Evolution::Genotype::operator=(const Evolution::Genotype &other)
{
	this->_gene = other._gene;
	this->_fitness = other._fitness;
	return *this;
}

int Evolution::Genotype::operator[](const Evolution::Genotype::Gene::size_type i) const
{
	return _gene[i];
}

int Evolution::Genotype::gene(const Evolution::Genotype::Gene::size_type i) const
{
	return _gene[i];
}

Evolution::Genotype::Gene::size_type Evolution::Genotype::length() const
{
	return _gene.size();
}

float Evolution::Genotype::fitness() const
{
	return static_cast<float>(1) / (_fitness + Evolution::Genotype::Epsilon);
}

Evolution::Genotype::Children Evolution::Genotype::mate(const Genotype &genotype) const
{

	// Get a chance at genetic crossover.
	std::vector<Evolution::Genotype> child = this->crossover(genotype);

	// Get a chance at mutation, if within our chances, mutate the children
	if (Evolution::Randomizer() % 100 <= (Evolution::Genotype::DefaultMutationPercent * 100)) {
		child[0].mutate();
		child[1].mutate();
	}

	return child;
}

Evolution::Genotype::Children Evolution::Genotype::crossover(const Evolution::Genotype &genotype) const
{
	const int crossPoint = Evolution::Randomizer() % _gene.size();

	Evolution::Genotype::Gene geneA(this->_gene);
	Evolution::Genotype::Gene geneB(genotype._gene);
	Evolution::Genotype::Gene childA, childB;

	// Copy the parents' genes up to the cross point.
	childA.insert(childA.end(), geneA.begin(), geneA.begin()+crossPoint);
	childB.insert(childB.end(), geneB.begin(), geneB.begin()+crossPoint);

	// Remove any duplicates from each other's genes.
	for (Evolution::Genotype::Gene::size_type i = 0; i < crossPoint; i++) {
		Evolution::Genotype::Gene::iterator elementA = std::find(geneA.begin(),
																 geneA.end(),
																 genotype.gene(i));
		if (elementA != geneA.end()) geneA.erase(elementA);


		Evolution::Genotype::Gene::iterator elementB = std::find(geneB.begin(),
																 geneB.end(),
																 this->gene(i));
		if (elementB != geneB.end()) geneB.erase(elementB);
	}

	// Copy the remaining genes of the alternate parent to each child.
	childA.insert(childA.end(), geneB.begin(), geneB.end());
	childB.insert(childB.end(), geneA.begin(), geneA.end());


	// Notice: The fitness of the children is being set in the constructor.
	return {Evolution::Genotype(childA), Evolution::Genotype(childB)};
}


Evolution::Genotype& Evolution::Genotype::mutate()
{
	const int pointA = Evolution::Randomizer() % _gene.size();
	const int pointB = Evolution::Randomizer() % _gene.size();

	const int geneA = _gene[pointA];
	const int geneB = _gene[pointB];

	_gene[pointA] = geneB;
	_gene[pointB] = geneA;

	_fitness = Evolution::Genotype::Fitness(*this);

	return *this;
}

Evolution::Genotype::Genotype(const Evolution::Genotype::Gene &genes)
{
	_gene = genes;
	_fitness = Evolution::Genotype::Fitness(*this);
}

Evolution::Genotype::Genotype(const Evolution::Genotype &genotype)
{
	*this = genotype;
}

Evolution::Genotype::Genotype(const int geneLength):
_fitness(geneLength), // Assume worst case scenario
_gene(geneLength)
{
	// Generate the genes for the gene pool.
	// The expression below generates numbers from 0 to n-1,
	// where n is the size of the vector.
	std::iota(_gene.begin(), _gene.end(), 0);

	// Randomly rearrange the elements.
	// Once all the genes have been created, we can rearrange them.
	std::shuffle(_gene.begin(), _gene.end(), Evolution::Randomizer);

	// Update the fitness of the genotype instance.
	// This will update the fitness based on the base class's fitness function.
	_fitness = Evolution::Genotype::Fitness(*this);
}

Evolution::Genotype::~Genotype()
{

}


// ================================================================
#pragma mark - Evolution Implementation
// ================================================================
std::istream& operator>>(std::istream& is, Evolution &evolution)
{
	return is;
}

std::ostream& operator<<(std::ostream& os, const Evolution &evolution)
{
	Evolution::Genotype solution(evolution.solution());

	os << std::setprecision(2) << std::fixed;
	os << std::setw(4) << evolution.progress() << ' ';
	
	if (evolution.solved())
		os << solution;
	else
		os << "<NO SOLUTION>";


	return os;
}

bool Evolution::solving() const
{
	return !solved() && progress() < progressLimit();
}

bool Evolution::solved() const
{
	return solution().fitness() >= 1;
}

Evolution::Genotype Evolution::solution() const
{
	return _population.front();
}

void Evolution::evolve()
{
	// Select parents for the next generation.
	const int parentCount = _populationSize * Evolution::DefaultParentPercent;
	Population parents = _extractParents(_population, parentCount);


	// Reserve memory for the parent's children (optimization)
	_population.reserve(_population.size() + parents.size());


	// Generate new children with the parents selected.
	for (int i = 0; i < parents.size() / 2; i++) {
		const Evolution::Genotype parentA = parents[i * 2];
		const Evolution::Genotype parentB = parents[i * 2 + 1];
		const Evolution::Population children(parentA.mate(parentB));

		// Insert the children into the population
		_population.insert(_population.end(), children.begin(), children.end());
	}


	// Select survivors for the next generation, and prepare population.
	_extractNonSurvivors(_population, _populationSize);


	// Increment the generation counter.
	_progress++;
}

int Evolution::progress() const
{
	return _progress;
}

int Evolution::progressLimit() const
{
	return _progressLimit;
}

Evolution::Genotype Evolution::resolve()
{
	while (solving()) evolve();
	return solution();
}

Evolution::Population Evolution::_extractParents(Population &population, int parentCount)
{
	// NOTICE: THIS METHOD DOES NOT CHECK FOR LOW POPULATION QUANTITY AND MAY FAULT AT SUCH POINT.
	Evolution::Population parents;

	// Assure there's an even number of parents.
	if (parentCount % 2) parentCount++;

	// Randomly pick candidate parents.
	while (parentCount--) {
		std::priority_queue<Evolution::Genotype> candidates;

		for (int i = 0; i < 3; i++) {
			Evolution::Population::iterator candidate = population.begin() + (Evolution::Randomizer() % population.size());
			candidates.push(*candidate);
		}

		// Select the top candidate as a parent and delete it from the queue
		parents.push_back(candidates.top());
	}

	// Pair the parents based on some method. I'm using the fittest parents
	// as each other's mate, sorting them sequentially based on fitness.
	std::sort(parents.begin(), parents.end(), [](const Evolution::Genotype &a, const Evolution::Genotype &b) -> bool {
		return a > b;
	});

	return parents;
}

Evolution::Population Evolution::_extractNonSurvivors(Population &population, int survivorCount)
{
	std::sort(population.begin(), population.end(), [](const Evolution::Genotype &a, const Evolution::Genotype &b) -> bool {
		return a > b;
	});

	Evolution::Population dead(population.begin() + survivorCount, population.end());
	population.erase(population.begin() + survivorCount, population.end());

	return dead;
}

Evolution::Evolution(const int geneLength, const int generationLimit):
_population((geneLength > 10)? (geneLength * 10) : 100, Genotype(geneLength)),
_populationSize((geneLength > 10)? (geneLength * 10) : 100),
_progressLimit(generationLimit),
_progress(0)
{
	// Generate a seed and seed the randomizer
	Evolution::Randomizer = std::default_random_engine(static_cast<unsigned int>(std::clock()));
}

Evolution::~Evolution()
{}
