/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "deme.hh"

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
{
    mut_rate_ = mut_rate;
    pop_=std::vector<Chromosome*>();
    for(unsigned i = 0; i < pop_size; i++ ){
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
  // Add your implementation here
}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
  // Add your implementation here
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
    
  // Add your implementation here
}
