#pragma once

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>

// cout vector definition
std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);

// initialize random generator
std::mt19937 setup_generator();

// setup the other_probes vector
void setup_other_probes(std::vector<int>& other_probes, const std::vector<int>& lgn);

// createTail
void create_tail(std::vector<int>& tile, int index, const std::vector<int>& other_probes, const std::vector<int>& lgn, int subset_size, std::mt19937 generator);

// calculate tail to do and add element to the end of other_probes if necessary
int tail_to_do(std::vector<int>& other_probes, int subset_size, std::vector<int>& frequency);

// pcim algorithm
void pcim(int iterations, int tile_size, int n_total_probes, const std::vector<int>& lgn);
