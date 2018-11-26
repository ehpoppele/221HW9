# 221HW9
Partners: Eli Poppele and Jon Takagi

# README
## Contents
1. Files
2. Implementations and Design
3. Results and other notes

## Files
### README.md
This file, using .md so that it looks pretty on github. I advise reading this there if you are currently using a text editor.
### cities
Should include a .hh for declarations, a .cc for implementations, and possibly a .o object file. These are largely the same as what we turned in last week, although there was an update to the reorder method to prevent a memory leak error that we didn't catch last time.
### chromosome
Should also have a header .hh, implementation .cc, and object .o that we don't care about. Contains our chromosome class, implemented as described by the assignment.
### deme
Our declarations for the deme class appear in our .hh header file, the implementation is in the .cc, and the .o object may also be floating around in the repository, because it's too easy to just git add all and end up tracking those as well.
### .tsv files
In this repository, we should have three .tsv files from Moodle: linear.tsv, five.tsv, and challenge.tsv. These were the given sets of cities that we tested our code on. The other three are the output of our tsp program: ga.tsv is the output of our genetic algorithm search, while randomized.tsv is the output of the random search implemented last week. Shortest.tsv is the actual shortest path, from whichever algorithm the program most recently used (should be ga at the time of pushing this).
### tsp
This is our actual main program. It takes the requisite three arguments, a .tsv file, a population size, and a mutation rate (between 0 and 1). There should be a .cc file which contains our implementations; this is pretty much just the framework from Moodle (I believe we didn't change it). There should also be a .o object, as well as a no-extension binary, tsp, which is the main program for this project.

## Implementations and Design
We decided to use a fitness function of fitness = (1000/distance)^3. The constant 1,000 was fairly arbitray, simply something large enough to work with the common distances in these files, and the third power was the result of a bit of testing. Lower powers produce worse results, not omptimzing as well, while higher powers do not have very apparent advantages. The purpose of raising to a power is to make sure that a path half as short as all the rest does not simply have a chance twice as good as the others when it comes to producing offspring; this bias towards shorter distances instead amplifies that difference, so twice as short produces odds 8 times as good. This is far from ideal, however, and further testing could easily produce a better function.
As for the rest of implementation, I believe it is all pretty standard. We used time-seeded standard random number generators as our default RNG. We did have to get a bit creative with the destructor on Deme; it is apparently necessary to delete each element of the vector, even though I thought the vector should do this automatically. It seems we can't precisely remember how memory works. A similar process is applied during next_generation, where we clean up the old parent generation to ensure no memory leaks there.

## Results (and other notes)
After some testing, we managed to get the GA to produce better results than the randomized search. We found that larger populations with much higher mutation rates produced the best results, but only marginally better than the random. Lower mutation rates returned something definitely worse.
There were no known bugs, unless not giving good results is a bug. Memory leak was actually tested this time, and we caught all the apparent errors there.
Compiling can be done with the make file.
