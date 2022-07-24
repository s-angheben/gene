#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>
#include <math.h>

#include <vector_pcim.hpp>
#include <algorithm_pcim.hpp>
#include <vector_random_pcim.hpp>

int calculate_tile_to_do(int iteration, int tile_size, int lgn_size,
                         int tot_size) {
  int result = std::ceil((tot_size-lgn_size)/(tile_size-lgn_size));
  return result*iteration;
}


// THESE FUNCTIONS WORKS FOR EVERY algorithm_pcim

template <ranges::common_range T1>
bool contains(T1 &lgn, unique_ptr<T1> &tile_ptr) {
  bool result = true;
  for (auto& lgn_elem : lgn) {
    result = result && std::find((*tile_ptr).begin(), (*tile_ptr).end(),
                                 lgn_elem) != (*tile_ptr).end();
  }
  return result;
}

// check that all lgn are contained in the tile
template <ranges::common_range T1, ranges::common_range T2>
bool run_algo_contains(algorithm_pcim<T1, T2> &a, T1 &lgn) {
  bool is_always_present = true;
  a.init();
  for (int i = 0; i < a.iterations; i++) {
    a.iteration_init();
    for (int j = 0; j < a.tile_number; j++) {
      a.tile_creation(j);
      is_always_present = is_always_present && contains(lgn, a.tile_ptr);
    }
    a.iteration_end();
  }
  a.end();
  return is_always_present;
}

// count the total number of created tile
template <ranges::common_range T1, ranges::common_range T2>
int run_algo_count_tile(algorithm_pcim<T1, T2> &a) {
  int count = 0;
  a.init();
  for (int i = 0; i < a.iterations; i++) {
    a.iteration_init();
    for (int j = 0; j < a.tile_number; j++) {
      a.tile_creation(j);
      count++;
    }
    a.iteration_end();
  }
  a.end();
  return count;
}

// check that all the created tile have the same correct size
template <ranges::common_range T1, ranges::common_range T2>
int run_algo_check_tile_size(algorithm_pcim<T1, T2> &a, int tile_size) {
  bool result = true;
  a.init();
  for (int i = 0; i < a.iterations; i++) {
    a.iteration_init();
    for (int j = 0; j < a.tile_number; j++) {
      a.tile_creation(j);
      result = result && ((*a.tile_ptr).size() == tile_size);
    }
    a.iteration_end();
  }
  a.end();
  return result;
}

////////////////////////////////////////
////////////////// VFDS
////////////////////////////////////////

// Test lgn presence in the tile
TEST(lgnPresenceVfds, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_pcim a(iteration, tile_size, size, lgn, npc);

  bool result = run_algo_contains(a, lgn);

  EXPECT_EQ(result, true);
}

TEST(tileNumberVfds, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_pcim a(iteration, tile_size, size, lgn, npc);

  int to_do = calculate_tile_to_do(iteration, tile_size, lgn.size(), size);
  int done = run_algo_count_tile(a);

  EXPECT_EQ(to_do, done);
}

TEST(tileSizeVfds, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_pcim a(iteration, tile_size, size, lgn, npc);

  bool result = run_algo_check_tile_size(a, tile_size);

  EXPECT_EQ(result, true);
}
////////////////////////////////////////
////////////////// VFSI
////////////////////////////////////////

TEST(lgnPresenceVfsi, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_pcim a(iteration, tile_size, size, lgn, npc);
  a.set_tile_creation_lgn_insert();

  bool result = run_algo_contains(a, lgn);

  EXPECT_EQ(result, true);
}

TEST(tileNumberVfsi, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_pcim a(iteration, tile_size, size, lgn, npc);
  a.set_tile_creation_lgn_insert();

  int to_do = calculate_tile_to_do(iteration, tile_size, lgn.size(), size);
  int done = run_algo_count_tile(a);

  EXPECT_EQ(to_do, done);
}

TEST(tileSizeVfsi, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_pcim a(iteration, tile_size, size, lgn, npc);
  a.set_tile_creation_lgn_insert();

  bool result = run_algo_check_tile_size(a, tile_size);

  EXPECT_EQ(result, true);
}

////////////////////////////////////////
////////////////// VRI
////////////////////////////////////////

TEST(lgnPresenceVri, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_random_pcim a(iteration, tile_size, size, lgn, npc);

  bool result = run_algo_contains(a, lgn);

  EXPECT_EQ(result, true);
}

TEST(tileNumberVri, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_random_pcim a(iteration, tile_size, size, lgn, npc);

  int to_do = calculate_tile_to_do(iteration, tile_size, lgn.size(), size);
  int done = run_algo_count_tile(a);

  EXPECT_EQ(to_do, done);
}

TEST(tileSizeVri, BasicAssertions) {
  int iteration = 2;
  int tile_size = 8;
  vector<int> lgn = {2, 5};
  int size = 50;
  int npc = 10;

  vector_random_pcim a(iteration, tile_size, size, lgn, npc);

  bool result = run_algo_check_tile_size(a, tile_size);

  EXPECT_EQ(result, true);
}
