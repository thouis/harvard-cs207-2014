#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <omp.h>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include "CS207/Util.hpp"

// RAII helper class for timing code
struct Timer {
  std::string msg;
  CS207::Clock clock;
  Timer(const std::string& s) : msg(s) {
    clock.start();
  }
  ~Timer() {
    double elapsed = clock.seconds();
    std::cout << msg << ": " << elapsed << "s" << std::endl;
  }
};


// Generic parallel-transform implementation that checks the iterator category
template <class Iter, class Function>
void parallel_transform(Iter first, Iter last, Function f) {
  // Check (at compile-time) the category of the iterator
  using category = typename std::iterator_traits<Iter>::iterator_category;
  static_assert(std::is_same<category, std::random_access_iterator_tag>::value,
                "parallel_transform requires random-access iterators!");

#pragma omp parallel for
  for (auto it = first; it < last; ++it)
    *it = f(*it);
}


int main()
{
  unsigned N = 100000000;
  std::vector<double> a;

  // Normal loop
  a = std::vector<double>(N,4);
  { Timer timer("Serial");
    for (auto it = a.begin(); it < a.end(); ++it)
      *it = std::exp(std::sqrt(*it));
  }

  // Parallel loop
  a = std::vector<double>(N,4);
  { Timer timer("OMP Parallel");
#pragma omp parallel for
    for (auto it = a.begin(); it < a.end(); ++it)
      *it = std::exp(std::sqrt(*it));
  }

  // Wrapped in a function
  a = std::vector<double>(N,4);
  { Timer timer("Parallel_Transform");
    // Create a lambda function and call parallel_transform
    auto func = [](double ai) { return std::exp(std::sqrt(ai)); };
    parallel_transform(a.begin(), a.end(), func);
  }

  // Create a boost iterator and call the function
  a = std::vector<double>(N,4);
  { Timer timer("Parallel_Transform Transform_Iter");
    // Make a trivial, but still random-access iterator pair
    auto identity = [](double& ai) -> double& { return ai; };
    auto begin = boost::make_transform_iterator(a.begin(), identity);
    auto end   = boost::make_transform_iterator(a.end(),   identity);
    // Call the parallel_transform, still works even with weird iterators!
    auto func = [](double ai) { return std::exp(std::sqrt(ai)); };
    parallel_transform(begin, end, func);
  }

  /*
  // This won't compile!
  a = std::vector<double>(N,4);
  { Timer timer("Won't compile!");
    // Filter iterators are (at best) bi-directional, not random-access!
    auto f_true = [](double) { return true; };
    auto begin = boost::make_filter_iterator(f_true, a.begin(), a.end());
    auto end   = boost::make_filter_iterator(f_true, a.begin(), a.end());
    // Create a lambda function and call parallel_transform
    auto func = [](double ai) { return std::exp(std::sqrt(ai)); };
    parallel_transform(begin, end, func);
  }
  */

  return 0;
}
