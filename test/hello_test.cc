#include <gtest/gtest.h>
#include <vector>
#include <gene_algo.hpp>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  std::vector<int> v1 = {1,2,3,4,5,6,7,8,9,10};
  std::vector<int> v2 = v1;

  std::cout << v1;

  unsigned seed = 1;
  std::mt19937 generator(seed);

  vshuffle(v1, v1.size(), generator);


  // Expect equality.
//  EXPECT_EQ(v1.size(), v2.size());
  EXPECT_EQ(v1.size(), 0);
}
