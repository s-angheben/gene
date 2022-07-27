#ifndef __WRITER_H__
#define __WRITER_H__

#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>
#include <future>

using namespace std;

template <ranges::common_range T1>
void print_bulk (vector<unique_ptr<T1>>& bulk, ostream& os) {
  for (auto &v : bulk) {
    for (auto &elem : *v) {
      os << elem << " ";
    }
    os << endl;
  }
}

template <ranges::common_range T1>
class bulk_writer {
public:
  vector<unique_ptr<T1>> data;

  virtual void insert(unique_ptr<T1> _tile) = 0;
  virtual ~bulk_writer();
};

template <ranges::common_range T1> bulk_writer<T1>::~bulk_writer() {}

template <ranges::common_range T1>
class bulk_cout : public bulk_writer<T1> {
public:
  const long unsigned int size;
  long unsigned int bulk_counter;

  bulk_cout (int _size) : size(_size), bulk_counter(1) {}
  void insert (unique_ptr<T1> _tile) {
    this->data.push_back(move(_tile));
    if (this->data.size() >= size) {
      print_bulk(this->data, cout);
      this->data.clear();
      bulk_counter++;
    }
  }

  ~bulk_cout() {
    if(bulk_counter != 0) {
      print_bulk(this->data, cout);
    }
  }
};


template <ranges::common_range T1>
class bulk_to_file : public bulk_writer<T1> {
public:
  ofstream tile_out_file;
  long unsigned int bulk_counter;
  const long unsigned int size;
  const string file_prefix;

  bulk_to_file (int _size, string _file_prefix) :
    size(_size), file_prefix(_file_prefix)
  {
    bulk_counter = 1;
  }

  void insert(unique_ptr<T1> _tile) {
    this->data.push_back(move(_tile));
    if (this->data.size() >= size) {
      tile_out_file.open(file_prefix + to_string(bulk_counter) + ".txt",
                         ios::out | ios::app);
      print_bulk(this->data, tile_out_file);
      tile_out_file.close();

      this->data.clear();
      bulk_counter++;
    }
  }

  ~bulk_to_file(){
    if (this->data.size() >= size) {
      tile_out_file.open(file_prefix + to_string(bulk_counter) + ".txt",
                         ios::out | ios::app);
      print_bulk(this->data, tile_out_file);
      tile_out_file.close();
    }
  }
};


template <ranges::common_range T1>
class bulk_to_file_async : public bulk_writer<T1> {
public:
  vector<unique_ptr<T1>> out;

  future<bool> data_writing;
  ofstream tile_out_file;

  long unsigned int bulk_counter;
  const long unsigned int size;
  const string file_prefix;

  bulk_to_file_async (int _size, string _file_prefix) :
    size(_size), file_prefix(_file_prefix)
  {
    //    data.resize(size);
    out.resize(0);

    bulk_counter = 1;

    data_writing = async([this]() { return print_and_clear(); });
  }

  void insert(unique_ptr<T1> _tile) {
    this->data.push_back(move(_tile));
    if (this->data.size() >= size) {
      bool already_written = data_writing.get();
      tile_out_file.open(file_prefix+to_string(bulk_counter)+".txt",
                         ios::out | ios::app);
      if(already_written) {
        out.swap(this->data);
        data_writing = async([this]() { return print_and_clear(); });
      }
      bulk_counter++;
    }
  }

  bool print_and_clear() {
    if (out.size() == 0)
      return true;

    print_bulk(out, tile_out_file);
    // for (auto& v : this->out) {
    //   if (v != nullptr) {
    //     for (auto& elem : *v) {
    //       tile_out_file << elem << " ";
    //     }
    //   }
    //   tile_out_file << endl;
    // }

    out.clear();
    tile_out_file.close();

    return true;
  }

  ~bulk_to_file_async () {
    data_writing.get();
    tile_out_file.open(file_prefix+to_string(bulk_counter)+".txt", ios::out |
                       ios::app);
    out.swap(this->data);
    print_and_clear();
    if(tile_out_file.is_open()) tile_out_file.close();
  }
};

#endif
