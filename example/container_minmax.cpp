/*! \file container_minmax.cpp
    \brief Finding minmax of an STL container.
    \author Paul A Bristow
*/

// Copyright Paul A Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
//#  pragma warning(disable : 4512) //  assignment operator could not be generated
#endif

#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;
 // minmax_element finds both min and max elements more efficiently than separately.

#include <algorithm>
  using std::min_element;
  using std::max_element;

#include <iostream>
  using std::cout;
  using std::endl;

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

  template<class T1, class T2>
  std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
  { // Output a pair of values.
         os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      return os;
  } // std::ostream& operator<<

  std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p)
  { // Output a pair of double values.
      std::streamsize precision = os.precision(3); // Save & use rather than default precision(6)
      os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      os.precision(precision); // Restore.
      return os;
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


template <typename T> // T an STL container: array, vector ...
size_t show(const T& container)
{ // Show all the containers values.
  for (typename T::const_iterator it = container.begin(); it != container.end(); it++)
  {
    cout << *it << ' ';
  }
  cout << endl;
  cout << container.size() << " values in container. " << endl;
  return container.size();
} // Container Data series to plot.

//template <typename T> // T an STL container: container of containers.
//size_t show_all(vector<T> containers)
//{ // Show all the containers values.
//  for (size_t i = 0; i < containers.size(); i++)
//  {
//    show(containers[i]);
//  }
//  return containers.size();
//} // Container Data series to plot.

template <typename T> // T an STL container: container of containers.
size_t show_all(const T& containers)
{ // Show all the containers values.
  for (typename T::const_iterator it = containers.begin(); it != containers.end(); it++)
  {
    show(*it);
  }
  return containers.size();
} // Container Data series to plot.

// Pointer version is not needed - iterator version is used instead.
//template <typename T> // T an STL container: array, vector ...
//size_t show_part(T const* begin, T const* end) // pointers.
//{
//  int count = 0;
//  while (begin != end)
//  {
//    count++;
//    cout << *begin << ' ';
//    ++begin;
//  }
//  cout << endl;
//  cout << count << " values in container used. " << endl;
//  return count;
//}// Container Data series to plot.

template <typename iter> // T an STL container: array, vector ...
size_t show_part(iter begin, iter end) // Iterators.
{// Show some or all of the containers values.
  size_t count = 0;
  while (begin != end)
  {
    count++;
    cout << *begin << ' ';
    ++begin;
  }
  cout << endl;
  cout << count << " values in container used. " << endl;
  return count;
}// Container Data series to plot.


// typedef std::iterator_traits<iter>::value_type would be double, etc

template <typename iter> // into an STL container: array, vector, set ...
pair<double, double> s(iter begin, iter end) // Data series to plot.
{
  iter it = min_element(begin, end);
  cout << "min_element " << *it << endl;
  std::pair<iter, iter> result = boost::minmax_element(begin, end);
  //pair<const T::const_iterator, const T::const_iterator > result = boost::minmax_element(begin, end);
  // error C2825: 'T': must be a class or namespace when followed by '::'
  cout << "minmax_elements " << *result.first << ' ' << *result.second << endl;

  std::pair<double, double > minmax;
  minmax.first = *result.first;
  minmax.second = *result.second;
  return minmax; // pair<double, double>
} // template <class T> int s  T an STL container: array, vector ...

template <class T> // T an STL container: for example: array<float>, vector<double> ...
pair<double, double> s(const T& container) // Data series to plot.
{
  pair<typename T::const_iterator, typename T::const_iterator> result = boost::minmax_element(container.begin(), container.end());
  cout << "minmax_elements " << *result.first << ' ' << *result.second << endl;
  std::pair<double, double > minmax; // Convert type of container T to double.
  minmax.first = *result.first;
  minmax.second = *result.second;
  return minmax; // pair<double, double>
} // template <class T> int s  T an STL container: array, vector ...

template <typename T> // T an STL container: array, vector ...
std::pair<double, double> scale(const T& container) // Container Data series
{
  //typedef typename <const T& >::const_iterator container_iterator;
  //pair< T, T > result = boost::minmax_element(container.begin(), container.end());
  //pair< vector_iterator, vector_iterator > result = boost::minmax_element(container.begin(), container.end());
  //cout << "Autoscale min is " << *(result.first) << endl;
  //cout << "Max is  " << *(result.second) << endl;
  //T::const_iterator it;
  //it = min_element(container.begin(), container.end());
  //cout << "min " << *it << endl;
  //it = max_element(container.begin(), container.end());
  //cout << "max " << *it << endl;

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
std::pair<double, double> s_all(const T& containers) // Container of containers of Data series.
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
  // transform(my_data.begin(), my_data.end(), data2.begin(), bind1st(multiplies<double>(), 2.3));
  copy(my_data.begin(), my_data.end(), std::back_inserter(my_data_2));
  copy(my_data.begin(), my_data.end(), std::ostream_iterator<double>(cout, " "));
  cout << endl << my_data.size() << " values in my_data. " << endl;
  copy(my_data_2.begin(), my_data_2.end(), std::ostream_iterator<double>(cout, " "));
  std::transform(my_data_2.begin(), my_data_2.end(), my_data_2.begin(), std::bind1st(multiplies<double>(), 2.3));
  copy(my_data_2.begin(), my_data_2.end(), ostream_iterator<double>(cout, " "));

  cout << endl << my_data.size() << " values in my_data. " << endl;

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
    cout << *si << ' ';
  }
  std::cout << endl;
  std::cout << my_set.size() << " values in my_set. " << std::endl; // 8 values in my_set.

  mm = s(my_set); // Display range.
  std::cout << mm << std::endl; //  1.2, 8.9
  std::cout <<"first " << *my_set.begin()  << ", last " << *(--my_set.end()) << std::endl;



  return 0;
} // int main()

/*



*/

