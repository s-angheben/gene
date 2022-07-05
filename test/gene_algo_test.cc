#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <vector_pcim.hpp>
#include <algorithm_pcim.hpp>

// Demonstrate some basic assertions.
TEST(ShuffleSize, BasicAssertions) {
  std::vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
  std::vector<int> v2 = v1;

  unsigned seed = 1;
  std::mt19937 generator(seed);

  shuffle(v1.begin(), v1.end(), generator);

  EXPECT_EQ(v1.size(), v2.size());
}

