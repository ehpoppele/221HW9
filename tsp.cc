/*
 * Main file for Traveling-Salesperson solver.
 * Read a cities file in TSV format into a Cities object, then run
 * of several solver algorithms on it and output the best result.
 */

#include "cities.hh"
#include "deme.hh"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <numeric>

//////////////////////////////////////////////////////////////////////////////
// Check whether a specific ordering reduces the total path distance in cities
// from a previous best distance. If so, print out the iteration number and
// new distance, update the best distance, and return true.
// Returns false if no improvement was found.
bool is_improved(const Cities& cities,
                 const Cities::permutation_t& ordering,
                 double& best_dist,
                 uint64_t iter)
{
  const auto dist = cities.total_path_distance(ordering);
  if (dist < best_dist) {
    std::cout << iter << "\t" << dist << std::endl;
    best_dist = dist;
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
// exhaustive_search searches niter randomized operdinges on the given cities
// The best cities permutation is returned.
Cities::permutation_t randomized_search(const Cities& cities, uint64_t niter)
{
  auto best_ordering = Cities::permutation_t(cities.size());
  auto best_dist = 1e100;

  for (uint64_t i = 0; i < niter; ++i) {
    auto ordering = cities.random_permutation(cities.size());
    if (is_improved(cities, ordering, best_dist, i)) {
      best_ordering = ordering;
    }
  }

  return best_ordering;
}

//////////////////////////////////////////////////////////////////////////////
// exhaustive_search searches every single permutation of the cities.
// The best cities permutation is returned.
Cities::permutation_t exhaustive_search(const Cities& cities)
{
  auto ordering = Cities::permutation_t(cities.size());
  std::iota(ordering.begin(), ordering.end(), 0);
  auto best_ordering = ordering;
  auto best_dist = 1e100;
  uint64_t i = 0;

  do {
    i++;
    if (is_improved(cities, ordering, best_dist, i)) {
      best_ordering = ordering;
    }
  } while (std::next_permutation(ordering.begin(), ordering.end()));

  return best_ordering;
}


//////////////////////////////////////////////////////////////////////////////
// ga_search uses a genetic algorithm to solve the traveling salesperson
// problem for a given list of cities.
// This function then creates a randomly generated population of permutations
// for traveling to those cities.
// The function also requires a population size and mutation rate.
// indicates how aggressively the population's individuals mutate.  The
// function then repeatedly evolves the population to generate increasingly
// better (i.e. shorter total distances) city permutations.
// The best cities permutation is returned.
Cities::permutation_t ga_search(const Cities& cities,
                                unsigned iters,
                                unsigned pop_size,
                                double mutation_rate)
{
  auto best_dist = 1e100;
  auto best_ordering = Cities::permutation_t(cities.size());

  Deme deme(&cities, pop_size, mutation_rate);

  // Evolve the population to make it fitter and keep track of
  // the shortest distance generated
  for (long i = 1; i <= iters/pop_size; ++i) {
    deme.compute_next_generation();    // generate next generation

    // Find best individual in this population
    const auto ordering = deme.get_best()->get_ordering();
    if (is_improved(cities, ordering, best_dist, i * pop_size)) {
      best_ordering = ordering;
    }
  }
  return best_ordering;
}


//////////////////////////////////////////////////////////////////////////////


auto randomized_search(Cities city, unsigned int NUM_ITER) {
        const int number_of_cities = city.size();

        Cities best_city({});
        double best_distance = std::numeric_limits<double>::infinity();
        Cities::permutation_t order = city.random_permutation(number_of_cities);
        Cities::permutation_t best_ordering = order;
        // In each iteration,
        for(unsigned int i = 0; i < NUM_ITER; i++) {
        //     it generates a new random permutation;
            order = city.random_permutation(number_of_cities);
            city = city.reorder(order);
            //     evaluates the distance to travel the cities along this ordering;
            double dist = city.total_path_distance(order);
            //     compares it to the best (shortest) distance found so far,
            //     and if it's shorter,
            if(dist < best_distance) {
            //         replaces the cities object with the newly-reordered cities and
            //         prints out to the screen the iteration number,
            //         followed by a space (or tab) and the total distance of the new path.
                best_distance = dist;
                best_ordering = order;
                best_city = city;
                std::cout << i << "\t" << best_distance << "\n";
            }
        }
        return best_ordering;
}

int main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << "Required arguments: filename for cities, population size, and mutation rate\n";
    return -1;
  }

  std::ifstream fs(argv[1]);
  Cities cities({});
  fs >> cities;
  fs.close();
  const auto pop_size = atoi(argv[2]);
  const auto mut_rate = atof(argv[3]);
  constexpr unsigned NUM_ITER = 100000;


//  const auto best_ordering = exhaustive_search(cities);
// const auto best_ordering = ga_search(cities, NUM_ITER, pop_size, mut_rate);
 const auto best_ordering = randomized_search(cities, NUM_ITER);

  auto out = std::ofstream("shortest.tsv");
  if (!out.is_open()) {
    std::cerr << "Can't open output file to record shortest path!\n";
    return -2;
  }
  Cities best_ordered_city = cities.reorder(best_ordering);
  out << best_ordered_city;

  return 0;
}
