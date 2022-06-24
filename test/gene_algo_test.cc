#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <gene_algo.hpp>

// Demonstrate some basic assertions.
TEST(ShuffleSize, BasicAssertions) {
  std::vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
  std::vector<int> v2 = v1;

  unsigned seed = 1;
  std::mt19937 generator(seed);

  shuffle(v1.begin(), v1.end(), generator);

  EXPECT_EQ(v1.size(), v2.size());
}

TEST(tail_to_do_number, BasicAssertions) {
  std::vector<int> f;

  std::vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
  int subset_size1 = 5;

  int tail_number1 = tail_to_do(v1, subset_size1, f);
  EXPECT_EQ(v1.size(), 10);
  EXPECT_EQ(tail_number1, 2);

  std::vector<int> v2 (40);
  iota(v2.begin(), v2.end(), 1);
  int subset_size2 = 15;

  int tail_number2 = tail_to_do(v2, subset_size2, f);

  EXPECT_EQ(v2.size(), 45);
  EXPECT_EQ(tail_number2, 3);
}
