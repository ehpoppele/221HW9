#include "cities.hh"
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>


//returns the distance between two city coord pairs
double Cities::dist_between(coord_t city_a, coord_t city_b) const
{
    return std::hypot(static_cast<double>(city_a.first-city_b.first), static_cast<double>(city_a.second-city_b.second));//Cast as doubles so hypot returns doubles
}

//This function is given a permutation vector as an argument and returns a
//new cities object based on that vector, one which is equivalent to the
//current cities object, with the ordering of elements changed to reflect
//the permutation vector.
Cities Cities::reorder(const permutation_t& ordering) const
{
    auto permuted_cities = new Cities();
    std::vector<coord_t> permuted_elmts = {};//empty vector
    //Copy in the reordered elements
    for (int i : ordering)
    {
        permuted_elmts.push_back(this->city_elements_.at(i));//Takes element i of original cities object and puts it in
    }
    //Then copy any elements that weren't reordered
    for (long unsigned int i = permuted_elmts.size(); i < (this->city_elements_.size()); i++)
    {
        permuted_elmts.push_back(this->city_elements_.at(i-1));//since i is a size here, we need to scale it back to refer to the elements
    }
    permuted_cities->city_elements_ = permuted_elmts;
    return *permuted_cities;
}

//Returns a random permutation of ints 0 to len-1
//This is implemented by making an ordered vector of said ints
//Then using the shuffle function on that vector, and then
//returning it after the STL has done all the hard work for us.
Cities::permutation_t Cities::random_permutation(unsigned len) const
{
    //first create the ordered vector
    permutation_t permuted_nums;
    for (unsigned int i = 0; i < len; i++)
    {
        permuted_nums.push_back(i);
    }
    //Establish our rng; we use a time-based seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    //And then use the shuffle function
    std::shuffle(permuted_nums.begin(),permuted_nums.end(), rng);
    return permuted_nums;
}

//Returns the total path distance for a given ordering of cities.
double Cities::total_path_distance(const permutation_t& ordering) const
{
    auto city_path = reorder(ordering).city_elements_;
    double distance = dist_between(city_path.at(0), city_path.at(city_path.size()-1));//Initialize distance as dist between first and last
    for (unsigned int i = 1; i < city_path.size(); i++)//loop the distance between city i and the city before it, from the second to the last city.
    {
        distance += dist_between(city_path.at(i), city_path.at(i-1));
    }
    return distance;
}
int Cities::size() const {
    return this -> city_elements_.size();
}

//reads from the input istream
//while the stream is not done, make pairs of values
//relies on in reading a whole number
//as explained to me by robert
std::istream& operator>> (std::istream& in, Cities& cities_ls){
    int x;
    while(in >> x) {
        Cities::coord_t pair;
        pair.first = x;
        in >> pair.second;
        cities_ls.city_elements_.push_back(pair);
    }
    return in;
}

//writes tab separated list
//each pair in the elements is written on a newline as tab separated values
std::ostream& operator<< (std::ostream& os, Cities& cities_ls){
    for(Cities::coord_t elm: cities_ls.city_elements_){
        os << elm.first << "\t" << elm.second << "\n";
    }
    return os;
}

//helper function that returns elements, used for testing
std::vector<Cities::coord_t> Cities::get_elements() const
{
    return this->city_elements_;
}
