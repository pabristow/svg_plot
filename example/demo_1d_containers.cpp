/*! \file demo_1d_containers.cpp
  \brief An example to demonstrate simple 1D plotting using a range of different STL containers.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2012, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_containers_1

/*`First a few includes to use Boost.Plot and various STL containers:
*/
#include <boost/svg_plot/svg_1d_plot.hpp>
  // using namespace boost::svg;
#include <boost/array.hpp>
  using boost::array;
#include <vector>
  using std::vector;
#include <set>
  using std::set; // Automatically sorted - though this is not useful to the plot process.
  using std::multiset; // At present using std::multiset, allowing duplicates, plot does not indicate duplicates.
  // With 2_D data in std::multimap, duplicate values are usefully displayed.
#include <list>
  using std::list;
#include <deque>
  using std::deque;

//] [/demo_1d_containers_1]

int main()
{
  {
//[demo_1d_containers_2

/*`STL vector is used as the container,
and fictional values are inserted using push_back.
Since this is a 1-D plot the order of data values is not important.
*/
  std::vector<float> values;
  values.push_back(3.1f);
  values.push_back(-5.5f);
  values.push_back(8.7f);
  values.push_back(0.5f);

/*`The constructor initializes a new 1D plot, called `my_plot`, and also sets all the many default values.
*/
  
  using namespace boost::svg;
  boost::svg::svg_1d_plot my_plot;

/*`Setting (member) functions are fairly self-explanatory:
Title provides a title at the top for the whole plot,
and plot adds a (unnamed) data series (naming isn't very useful if there is only one data series).
*/
  my_plot.title("vector&lt;float&gt; example");
/*`
[note One must insert the XML character entity equivalents of &lt; for < and &gt; for >).]
*/
  my_plot.plot(values);

/*`Write the SVG to a file.
*/
  my_plot.write("./demo_1d_vector_float.svg");
//] [/demo_1d_containers_2]
  }

  {
//[demo_1d_containers_3

  static boost::array<const long double, 4> values {3.1L,-5.5L, 8.7L, 0.5L};
  //! \note Using @c long double provokes an expected warning about conversion from 'T' to 'const double', possible loss of data.
  boost::svg::svg_1d_plot my_plot;
  my_plot.title("array&lt;long double&gt; example");
  my_plot.plot(values);
  my_plot.write("./demo_1d_array_long_double.svg");
//] [/demo_1d_containers_3]
  }

  {
//[demo_1d_containers_4
/*`If the container type is a set, then it can be filled with insert:*/
  std::set<double> values;
  values.insert(-8.4);
  values.insert(-2.3);
  values.insert(0.1);
  values.insert(5.6);
  values.insert(7.8);

  boost::svg::svg_1d_plot my_plot;
  my_plot.title("set&lt;double&gt; example");
  my_plot.plot(values);
  my_plot.write("./demo_1d_set_double.svg");
//] [/demo_1d_containers_4]
  }

  {
//[demo_1d_containers_5
/*`If the container type is a list, then it can be filled with push_back or push_front:*/
  std::list<double> values;
  values.push_back(-8.4);
  values.push_back(-2.3);
  values.push_back(0.1);
  values.push_back(5.6);
  values.push_back(7.8);
  boost::svg::svg_1d_plot my_plot;
  my_plot.title("list&lt;double&gt; example");
  my_plot.plot(values);
  my_plot.write("./demo_1d_list_double.svg");
//] [/demo_1d_containers_5]
  }

  {
//[demo_1d_containers_6
/*`If the container type is a deque, then it can be filled with push_back or push_front:*/
  std::deque<double> values;
  values.push_front(-8.4);
  values.push_front(-2.3);
  values.push_front(0.1);
  values.push_front(5.6);
  values.push_front(7.8);

  boost::svg::svg_1d_plot my_plot;
  my_plot.title("deque&lt;double&gt; example");
  my_plot.plot(values);
  my_plot.x_label("X values as doubles");

  my_plot.write("./demo_1d_deque_double.svg");
//] [/demo_1d_containers_6]
  }
  return 0;
} // int main()

/*
Sample output:
//[demo_1d_containers_output

demo_1d_containers.cpp
demo_1d_containers.vcxproj -> I:\Cpp\SVG_plot\svg_plot\x64\Release\demo_1d_containers.exe
Autorun "I:\Cpp\SVG_plot\svg_plot\x64\Release\demo_1d_containers.exe"
Plot written to file ./demo_1d_vector_float.svg.
Plot written to file ./demo_1d_array_long_double.svg.
Plot written to file ./demo_1d_set_double.svg.
Plot written to file ./demo_1d_list_double.svg.
Plot written to file ./demo_1d_deque_double.svg.
//] [/demo_1d_containers_output]
*/
