#pragma once

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <ostream>

// cout vector definition
std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);

// vector shuffle
void vshuffle(std::vector<int> & v, std::vector<int>::size_type n, std::mt19937 & generator);

// pcim algorithm
void pcim(int iterations, int tile_size, int n_total_probes, const std::vector<int>& lgn);
