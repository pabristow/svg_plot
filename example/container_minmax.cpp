/*! \file container_minmax.cpp
    \brief Finding minmax of an STL container.
    \details Provide specialized operator<< for pairs.
*/

// Copyright Paul A Bristow 2008, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;
 // minmax_element finds both min and max elements more efficiently than separately.

#include <algorithm>
  using std::min_element;
  using std::max_element;
  // using std::transform

#include <iostream>
  //using std::cout;
  //using std::endl;

#include <vector>
  using std::vector;

#include <utility>
  using std::pair;

#include <iterator> // for std::iterator_traits;
  using std::iterator;
  using std::iterator_traits;
  using std::ostream_iterator;

#include <set>
  using std::multiset;

#include <functional>
  using std::multiplies;

#include <limits>
  using std::numeric_limits;

//namespace detail
//{

  //std::ostream& operator<< (std::ostream&, const std::pair<double, double>&);
  //template<class T1, class T2> std::ostream& operator<< (std::ostream&, std::pair<T1, T1>&);

  //! Output both items of a pair separated by a comma, for example: \code 1.2, 3.4 \endcode
  //! Permits chaining.
  template<class T1, class T2>
  std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
  { // Output a pair of values.
         os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      return os; // Permit chaining.
  } // std::ostream& operator<<

  //! Output both items of a pair of doubles separated by a comma, for example: \code 1.2, 3.4 \endcode
  //! Permits chaining.
  std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p)
  { // Output a pair of double values.
      std::streamsize precision = os.precision(3); // Save & use rather than default precision(6)
      os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      os.precision(precision); // Restore.
      return os; // Permit chaining.
  } // std::ostream& operator<<

  // Maybe better as:
  //template<typename charT, typename traits, typename T1, typename T2>
  //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<T1, T2>& p)
  //{
  //  return os << p.first << " " << p.second;
  //}
  //
  //// Explicit double, double.
  //template<typename charT, typename traits>
  //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<double, double>& p)
  //{
  //  return os << p.first << " " << p.second;
  //}
  // but OK for this purpose.
//} // namespace detail

//! Show all the containers values.
//! \tparam T an STL container: array, vector ...
template <typename T> // T an STL container: array, vector ...
size_t show(const T& container)
{
  for (typename T::const_iterator it = container.begin(); it != container.end(); it++)
  {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;
  std::cout << container.size() << " values in container. " << std::endl;
  return container.size();
} // Container data-series to plot.

//template <typename T> // T an STL container: container of containers.
//size_t show_all(vector<T> containers)
//{ // Show all the containers values.
//  for (size_t i = 0; i < containers.size(); i++)
//  {
//    show(containers[i]);
//  }
//  return containers.size();
//} // Container data-series to plot.

//! Show all the containers of containers values.
//! \tparam T an STL container: container of containers, array, vector ...
template <typename T>
size_t show_all(const T& containers)
{
  for (typename T::const_iterator it = containers.begin(); it != containers.end(); it++)
  {
    show(*it);
  }
  return containers.size();
} // Container data-series to plot.

// Pointer version is not needed - iterator version is used instead.
//template <typename T> // T an STL container: array, vector ...
//size_t show_part(T const* begin, T const* end) // pointers.
//{
//  int count = 0;
//  while (begin != end)
//  {
//    count++;
//    std::cout << *begin << ' ';
//    ++begin;
//  }
//  std::cout << std::end
//  std::cout << count << " values in container used. " << std::end
//  return count;
//}// Container data-series to plot.

template <typename iter> // T an STL container: array, vector ...
size_t show_part(iter begin, iter end) // Iterators.
{// Show some or all of the containers values.
  size_t count = 0;
  while (begin != end)
  {
    count++;
    std::cout << *begin << ' ';
    ++begin;
  }
  std::cout << std::endl;
    std::cout << count << " values in container used. " << std::endl;
  return count;
}// Container data-series to plot.

// typedef std::iterator_traits<iter>::value_type would be double, etc

template <typename iter> // into an STL container: array, vector, set ...
pair<double, double> s(iter begin, iter end) // data-series to plot.
{
  iter it = min_element(begin, end);
  std::cout << "min_element " << *it << std::endl;
  std::pair<iter, iter> result = boost::minmax_element(begin, end);
  //pair<const T::const_iterator, const T::const_iterator > result = boost::minmax_element(begin, end);
  // error C2825: 'T': must be a class or namespace when followed by '::'
  std::cout << "minmax_elements " << *result.first << ' ' << *result.second << std::endl;

  std::pair<double, double > minmax;
  minmax.first = *result.first;
  minmax.second = *result.second;
  return minmax; // pair<double, double>
} // template <class T> int s  T an STL container: array, vector ...

template <class T> // T an STL container: for example: array<float>, vector<double> ...
pair<double, double> s(const T& container) // data-series to plot.
{
  pair<typename T::const_iterator, typename T::const_iterator> result = boost::minmax_element(container.begin(), container.end());
  std::cout << "minmax_elements " << *result.first << ' ' << *result.second << std::endl;
  std::pair<double, double > minmax; // Convert type of container T to double.
  minmax.first = *result.first;
  minmax.second = *result.second;
  return minmax; // pair<double, double>
} // template <class T> int s  T an STL container: array, vector ...

template <typename T> // T an STL container: array, vector ...
std::pair<double, double> scale(const T& container) // Container data-series
{
  //typedef typename <const T& >::const_iterator container_iterator;
  //pair< T, T > result = boost::minmax_element(container.begin(), container.end());
  //pair< vector_iterator, vector_iterator > result = boost::minmax_element(container.begin(), container.end());
  //std::cout << "Autoscale min is " << *(result.first) << std::end
  //std::cout << "Max is  " << *(result.second) << std::end
  //T::const_iterator it;
  //it = min_element(container.begin(), container.end());
  //std::cout << "min " << *it << std::end
  //it = max_element(container.begin(), container.end());
  //std::cout << "max " << *it << std::end

  //pair<double, double> mm;
  //mm.first= *(minmax_element(container.begin(), container.end()).first);
  //mm.second= *(minmax_element(container.begin(), container.end()).second);
  // But this calls minmax_element twice.

  pair<typename T::const_iterator, typename T::const_iterator> result = boost::minmax_element(container.begin(), container.end());

  pair<double, double> minmax;
  minmax.first = *result.first;
  minmax.second = *result.second;
  return minmax;
} // template <class T> int scale_axis,  T an STL container: array, vector  set, map ...

template <class T> // T an STL container: array, vector, set, map ...
std::pair<double, double> s_all(const T& containers) // Container of containers of data-series.
{
  std::pair<double, double> minmax((std::numeric_limits<double>::max)(), (std::numeric_limits<double>::min)());
  for (typename T::const_iterator it = containers.begin(); it != containers.end(); it++)
  {
    std::pair<double, double> mm = s(*it); // Scale of this container.
    minmax.first = (std::min)(mm.first, minmax.first); //
    minmax.second = (std::max)(mm.second, minmax.second);
  }
  return minmax;
} // template <class T> int scale_axis,  T an STL container: array, vector ...

int main()
{
  std::vector<double> my_data;
  // Initialize my_data with some entirely fictional data.
  my_data.push_back(0.2);
  my_data.push_back(1.1); // [1]
  my_data.push_back(4.2); // [2]
  my_data.push_back(3.3); // [3]
  my_data.push_back(5.4); // [4]
  my_data.push_back(6.5); // [5]

  std::vector<double> my_data_2;
  std::vector<double> my_data_3;
  // transform(my_data.begin(), my_data.end(), data2.begin(), bind1st(multiplies<double>(), 2.3));
  copy(my_data.begin(), my_data.end(), std::back_inserter(my_data_2));
  copy(my_data.begin(), my_data.end(), std::ostream_iterator<double>(std::cout, " "));
  std::cout << "\n" << my_data.size() << " values in my_data. " << std::endl;
  std::copy(my_data_2.begin(), my_data_2.end(), std::ostream_iterator<double>(std::cout, " "));
  const double d = 2.3; // Multiply all items by a constant.
  std::transform(my_data_2.begin(), my_data_2.end(), my_data_2.begin(), [d](double i) { return i * d; });

  copy(my_data_2.begin(), my_data_2.end(), ostream_iterator<double>(std::cout, " "));

  std::cout << "\n" << my_data.size() << " values in my_data. " << std::endl;

  vector<vector<double> > my_containers;

  my_containers.push_back(my_data);
  my_containers.push_back(my_data_2);

  std::cout << my_containers.size() << " containers." << std::endl;

  show_all(my_containers);

  std::cout << s_all(my_containers) << std::endl;

  typedef std::vector<double>::const_iterator vector_iterator;
  std::pair< vector_iterator, vector_iterator > result
          = boost::minmax_element(my_data.begin(), my_data.end());
  std::cout << "The smallest element is " << *(result.first) << std::endl; // 0.2
  std::cout << "The largest element is  " << *(result.second) << std::endl; // 6.5

  // Autoscaling using two double min and max values.
  double first_value = *(my_data.begin());
  double last_value = *(--my_data.end());
  std::cout << "First value " << first_value << ", last = " << last_value << std::endl;

  // Using first and last in container,
  // assuming the data are ordered in ascending value,
  // either because they have been sorted, for std::vector or array,
  // or by being sorted as inserted, for example, std::map, std::multimap.
  //s(*my_data.begin(),*(--my_data.end()));

  // Using two begin & end iterators into STL container,
  std::pair<double, double> mm = s(my_data.begin(), my_data.end() );
  std::cout << mm << std::endl;
  std::cout << mm.first << ' ' << mm.second << std::endl;

  // Using two begin & end pointer into STL container,
  // scale_axis does finding min and max.
  mm = s(&my_data[1], &my_data[4]); // Only middle part of the container used.

  // Autoscaling using whole STL container,
  // scale_axis does finding min and max.
  mm = s(my_data); // Display range.

  std::multiset<double> my_set;
  // Initialize my_set with some entirely fictional data.
  my_set.insert(2.3);
  my_set.insert(7.8);
  my_set.insert(3.4);
  my_set.insert(4.5);
  my_set.insert(1.2);
  my_set.insert(5.6);
  my_set.insert(6.7);
  my_set.insert(8.9);
  // Show the set.
  std::multiset<double>::const_iterator si;
  for (si = my_set.begin(); si != my_set.end(); si++)
  {
    std::cout << *si << ' ';
  }
  std::cout << std::endl;
  std::cout << my_set.size() << " values in my_set. " << std::endl; // 8 values in my_set.

  mm = s(my_set); // Display range.
  std::cout << mm << std::endl; //  1.2, 8.9
  std::cout <<"first " << *my_set.begin()  << ", last " << *(--my_set.end()) << std::endl;

  return 0;
} // int main()

/*

1>container_minmax.cpp
1>Generating code
1>Previous IPDB not found, fall back to full compilation.
1>All 347 functions were compiled because no usable IPDB/IOBJ from previous compilation was found.
1>Finished generating code
1>container_minmax.vcxproj -> I:\Cpp\SVG_plot\svg_plot\x64\Release\container_minmax.exe
1>Autorun "I:\Cpp\SVG_plot\svg_plot\x64\Release\container_minmax.exe"
1>0.2 1.1 4.2 3.3 5.4 6.5
1>6 values in my_data.
1>0.2 1.1 4.2 3.3 5.4 6.5 0.46 2.53 9.66 7.59 12.42 14.95
1>6 values in my_data.
1>2 containers.
1>0.2 1.1 4.2 3.3 5.4 6.5
1>6 values in container.
1>0.46 2.53 9.66 7.59 12.42 14.95
1>6 values in container.
1>minmax_elements 0.2 6.5
1>minmax_elements 0.46 14.95
1>0.2, 14.9
1>The smallest element is 0.2
1>The largest element is  6.5
1>First value 0.2, last = 6.5
1>min_element 0.2
1>minmax_elements 0.2 6.5
1>0.2, 6.5
1>0.2 6.5
1>min_element 1.1
1>minmax_elements 1.1 4.2
1>minmax_elements 0.2 6.5
1>1.2 2.3 3.4 4.5 5.6 6.7 7.8 8.9
1>8 values in my_set.
1>minmax_elements 1.2 8.9
1>1.2, 8.9
1>first 1.2, last 8.9
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

*/

