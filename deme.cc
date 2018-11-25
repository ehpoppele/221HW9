/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */
#include <algorithm>
#include <cmath>
#include "chromosome.hh"
#include "deme.hh"
#include <random>
#include <chrono>

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
{
    mut_rate_ = mut_rate;
    pop_=std::vector<Chromosome*>();
    for(unsigned i = 0; i < pop_size; i++ ) {
	Chromosome* new_chrom = new Chromosome(cities_ptr);
	pop_.push_back(new_chrom);
    }
}

// Clean up as necessary
Deme::~Deme()
{
    // deme member variables:
    // pop_ is a smart vector, don't need to delete it
    // mut_rate is a primitive, don't need to delete it
    // default_random_engine is a mystery.

    // Add your implementation here
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine generator (seed);
    std::vector<Chromosome*> new_pop({});
    for(unsigned int i = 0; i < pop_.size() / 2; i++ ) {
    	auto father = select_parent();
    	double mut_roll = std::generate_canonical<double,std::numeric_limits<double>::digits>(generator);
        // mut_roll is a random in [0,1)
    	if(mut_roll < mut_rate_) {
    	    father -> mutate();
    	}
    	auto mother = select_parent();
    	mut_roll = std::generate_canonical<double,std::numeric_limits<double>::digits>(generator);
    	if(mut_roll < mut_rate_) {
    	    mother -> mutate();
    	}
    	auto children = father -> recombine(mother);
    	new_pop.push_back(children.first);
    	new_pop.push_back(children.second);
    }
    pop_ = new_pop;
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
    return *std::max_element(pop_.begin(), pop_.end(),
            [] (Chromosome* a, Chromosome* b)
    {
	return a->get_fitness() > b->get_fitness();
    }
            );
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
    int s = 0;
    for(auto f: pop_) {
	s += f->get_fitness();
    }
    // s is now the sum of fitness

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<int> distribution(0, pop_.size());
    //generate a random value
    unsigned int p = distribution(rng);
    for(unsigned int i = 0; i < pop_.size(); i++) {
	p = -pop_.at(i)->get_fitness();
	if(p < 0) {
	    return pop_.at(i);
	}
    }
    return pop_.at(pop_.size() - 1);
}
