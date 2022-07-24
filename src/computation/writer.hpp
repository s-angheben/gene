#ifndef __WRITER_H__
#define __WRITER_H__

#include <fstream>
#include <memory>
#include <ranges>
#include <vector>
#include <future>

using namespace std;

template <ranges::common_range T1>
class bulk_writer {
public:
  virtual void insert(unique_ptr<T1> _tile) = 0;
  virtual ~bulk_writer();
};

template <ranges::common_range T1> bulk_writer<T1>::~bulk_writer() {}

template <ranges::common_range T1>
class bulk_to_file_async : public bulk_writer<T1> {
public:
  vector<unique_ptr<T1>> data;

  vector<unique_ptr<T1>> out;
  future<bool> data_writing;
  ofstream tile_out_file;

  int bulk_counter;
  const long unsigned int size;
  const string file_prefix;

  bulk_to_file_async (int _size, string _file_prefix) :
    size(_size), file_prefix(_file_prefix)
  {
    //    data.resize(size);
    //out.resize(0);

    bulk_counter = 1;

    data_writing = async([this]() { return print_and_clear(); });
  }

  void insert(unique_ptr<T1> _tile) {
    data.push_back(move(_tile));
    if (data.size() >= size) {
      bool already_written = data_writing.get();
      tile_out_file.open(file_prefix+to_string(bulk_counter)+".txt",
                         ios::out | ios::app);
      if(already_written) {
        out.swap(data);
        data_writing = async([this]() { return print_and_clear(); });
      }
      bulk_counter++;
    }
  }

  bool print_and_clear() {
    if (out.size() == 0) return true;

    for (auto& v : out) {
      if (v != nullptr) {
        for (auto& elem : *v) {
          tile_out_file << elem << " ";
        }
      }
    }
    tile_out_file << endl;

    out.resize(0);
    tile_out_file.close();

    return true;
  }

  ~bulk_to_file_async () {
    bool already_written = data_writing.get();
    tile_out_file.open(file_prefix+to_string(bulk_counter)+".txt", ios::out |
                       ios::app);
    out.swap(data);
    print_and_clear();
    if(tile_out_file.is_open()) tile_out_file.close();
  }
};

#endif
