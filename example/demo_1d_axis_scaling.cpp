/*! \file demo_1d_axis_scaling.cpp

  \brief Demonstration of 1D plot with axis scaling.
  \details See auto_1d_containers.cpp for an example axis_scaling with multiple data series.

  \author Paul A Bristow

  \date 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_axis_scaling_1

/*`This example shows the use of functions scale_axis to find suitable axis limits.
Normally one would use autoscaling, but there are conceivable circumstances when
one would want to check on the scale_axis algorithm's choice of axis,
and perhaps intervene in the process.

First some includes to use Boost.Plot
(and some others only needed for this example).
*/

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <boost/svg_plot/show_1d_settings.hpp>
// Only needed for showing which settings in use.
// void boost::svg::show_1d_plot_settings(svg_1d_plot&);
 // (Also provides operator<< for std::pair).

#include <boost/algorithm/minmax.hpp>
 using boost::minmax;
#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;

#include <iostream> // for debugging.
  using std::cout;
  using std::endl;
  using std::boolalpha;

#include <limits>
  using std::numeric_limits;

#include <vector>
  using std::vector;
#include <set>
  using std::multiset;

#include <utility>
  using std::pair;

#include <boost/svg_plot/detail/auto_axes.hpp>
  using boost::svg::show; // A single STL container.
  using boost::svg::show_all; // Multiple STL containers.
  // using boost::svg::range; // Find min and max of a STL container.
  using boost::svg::range_all;// Find min and max of multipler STL containers.
//] [/demo_1d_axis_scaling_1]

void scale_axis(double min_value, double max_value, // Input range
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, int* major_ticks, // All 4 updated.
               bool origin, // If true, ensures that zero is a tick value.
               double tight, // Allows user to avoid a small fraction over a tick using another tick.
               int min_ticks, // Minimum number of ticks.
               int steps); // Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.

double tol100eps = 1000 * numeric_limits<double>::epsilon(); // suitable tight value.

int main()
{
  using namespace boost::svg;
  try
  {
//[demo_1d_axis_scaling_2
  /*`This example uses a few types of containers to demonstrate axis_scaling.
  axis_scaling must inspect the container in order to find axis ranges that will be suitable.
  First we create a container and fill with some fictional data.
  */
  vector<double> my_data;
  // Initialize my_data with some entirely fictional data.
  my_data.push_back(0.2);
  my_data.push_back(1.1); // [1]
  my_data.push_back(4.2); // [2]
  my_data.push_back(3.3); // [3]
  my_data.push_back(5.4); // [4]
  my_data.push_back(6.5); // [5]
  show(my_data); // Show entire container contents,
  // 6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5
//] [/demo_1d_axis_scaling_2]

//[demo_1d_axis_scaling_3
  multiset<double> my_set;
  // Initialize my_set with some entirely fictional data.
  my_set.insert(1.2);
  my_set.insert(2.3);
  my_set.insert(3.4);
  my_set.insert(4.5);
  my_set.insert(5.6);
  my_set.insert(6.7);
  my_set.insert(7.8);
  my_set.insert(8.9);
  // Show the set.
  multiset<double>::const_iterator si;
  show(my_set); // for two different types of container.
  // 8 values in container: 1.2 2.3 3.4 4.5 5.6 6.7 7.8 8.9
/*` Show can also display just a part of the container contents.
*/
  // show(&my_data[0], &my_data[my_data.size()]); // pointers - wrong! > all data ;-)
  show(&my_data[0], &my_data[my_data.size()-1]); // pointers, all data.
  show(&my_data[1], &my_data[5]); // pointers, part data.
  show(my_data.begin(), my_data.end()); // iterators.
  show(++(my_data.begin()), --(my_data.end())); // Just the 4 middle values.

  vector<double>::const_iterator idb = my_data.begin();
  vector<double>::const_iterator ide = my_data.end();
  show(idb, ide); // All
  ++idb; // Move to 2nd value.
  --ide; // move back from last value.
  show(idb, ide); // Just the 4 middle values.
//] [/demo_1d_axis_scaling_3]

//[demo_1d_axis_scaling_4

  /*`Is is possible to find the minimum and maximum values in a container using the STL functions min & max
  or more conveniently and efficiently with boost::minmax_element:
  */

  typedef vector<double>::const_iterator vector_iterator;
  pair<vector_iterator, vector_iterator> result = boost::minmax_element(my_data.begin(), my_data.end());
  cout << "The smallest element is " << *(result.first) << endl; // 0.2
  cout << "The largest element is  " << *(result.second) << endl; // 6.5

  // axis_scaling using two double min and max values.
  double min_value = *(my_data.begin());
  double max_value = *(--my_data.end());
  cout << "axis_scaling 1 min " << min_value << ", max = " << max_value << endl;

/*` and to apply these values to the axis_scaling algorithm using by plot to choose the axes limits and ticks.
*/
  double axis_min_value; // Values to be updated by function `scale_axis`.
  double axis_max_value;
  double axis_tick_increment;
  int axis_ticks;
  
  scale_axis(min_value, max_value,
    &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    false, tol100eps, 6); // Display range.
  cout << "Axis_scaled 2 min " << axis_min_value << ", max = " << axis_max_value << ", increment " << axis_tick_increment << endl;
/*`It is also possible to use this with containers that use iterators and whose contents are ordered in ascending value,
axis_scaling using first and last in container, for example, set, map, multimap, or a sorted vector or array.
A number of variations are shown below, mainly by way of testing.
*/
  scale_axis(*my_data.begin(),*(--my_data.end()),
    &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    false, tol100eps, 6); // Display range.
  cout << "Axis_scaled 3 min " << axis_min_value << ", max = " << axis_max_value << ", increment " << axis_tick_increment << endl;

  // axis_scaling using two begin & end iterators into STL container,
  // scale_axis does finding min and max.
  scale_axis(my_data.begin(), my_data.end(), // Input range
    &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks, // to update.
    true, // check for non-finite
    3., // autoscale_plusminus = 3 sd
    false, // Do not include origin
    tol100eps, // tight
    6, // steps at default.
    0); // Display range.
  cout << "Axis_scaled 4 min " << axis_min_value << ", max = " << axis_max_value << ", increment " << axis_tick_increment << endl;

  // axis_scaling using two begin & end iterators into STL container,
  // scale_axis does finding min and max.
  scale_axis(my_data[1], my_data[4], // Only middle part of the container used, ignoring 1st and last values.
    &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    true, tol100eps, 6); // Display range.
  cout << "Axis_scaled 5 min " << axis_min_value << ", max = " << axis_max_value << ", increment " << axis_tick_increment << endl;

  // axis_scaling using whole STL container,
  // scale_axis does finding min and max.
  scale_axis(my_data, &axis_min_value, &axis_max_value, &axis_tick_increment, &axis_ticks,
    true, 3., false, tol100eps, 6); // Display range.
  cout << "Axis_scaled 6 min " << axis_min_value << ", max = " << axis_max_value << ", increment " << axis_tick_increment << endl;

  svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

  // One could intercept and change any values calculated by scale_axis here?
  // Set the plot to use range and interval from the scale_axis values.

  /*`The axis range thus computed can be inserted directly into the plot using the `range` and `x_major_interval` functions.
  */

  my_1d_plot.x_range(axis_min_value, axis_max_value)
    .x_major_interval(axis_tick_increment);

  //my_1d_plot.x_autoscale(false);  // Ensure autoscale values are *not* recalculated for the plot.
  //] [/demo_1d_axis_scaling_4]

  //[demo_1d_axis_scaling_5

  // Set some axis_scaling parameters:
  my_1d_plot.x_with_zero(false);
  my_1d_plot.x_min_ticks(10);
  my_1d_plot.x_steps(0);
  my_1d_plot.x_tight(0.001);

  // Show the flags just set.
  cout << (my_1d_plot.x_with_zero() ? "x_with_zero, " : "not x_with_zero, ")
    << my_1d_plot.x_min_ticks() << " x_min_ticks, "
    << my_1d_plot.x_steps() << " x_steps, "
    << my_1d_plot.x_tight() << " tightness." << endl;

  my_1d_plot.x_autoscale(my_data);  // Use all my_data to autoscale.
  cout << "Axis_scaled " // Show the results of autoscale:
    "min " << my_1d_plot.x_auto_min_value()
    << ", max "<< my_1d_plot.x_auto_max_value()
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 0, max 6.5, interval 0.5

  my_1d_plot.x_autoscale(my_data.begin(), my_data.end());  // Use all my_data to autoscale.

  cout << "Axis_scaled " // Show the results of autoscale:
    "min " << my_1d_plot.x_auto_min_value()
    << ", max "<< my_1d_plot.x_auto_max_value()
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 0, max 6.5, interval 0.5

  my_1d_plot.x_autoscale(my_data[1], my_data[4]);  // Use only part of my_data to autoscale.

  cout << "Axis_scaled " // Show the results of autoscale:
    "min " << my_1d_plot.x_auto_min_value()
    << ", max "<< my_1d_plot.x_auto_max_value()
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 1, max 5.5, interval 0.5

  my_1d_plot.x_autoscale(true);  // Ensure autoscale values are used for the plot.

  my_1d_plot.plot(my_data, "Auto 1D"); // Add the one data series.
  cout << "Axis_scaled " // Show the results of autoscale:
    " min " << my_1d_plot.x_auto_min_value()
    << ", max "<< my_1d_plot.x_auto_max_value()
    << ", interval " << my_1d_plot.x_auto_tick_interval() << endl; // Autoscaled min 1, max 5.5, interval 0.5

  my_1d_plot.plot(my_set.begin(), my_set.end(), "Auto 1D"); // Add another data series from my_set.
  my_1d_plot.plot(my_set, "Auto 1D"); // Add another whole data series from my_set.
  my_1d_plot.plot(&my_data[1], &my_data[4], "Auto 1D"); // Add part (1,2 3 but *not* 4) of the one data series.
  //my_1d_plot.plot(&my_set[1], &my_set[4], "Auto 1D"); // operator[] is not defined for set container!

  my_1d_plot.write("demo_1d_axis_scaling.svg"); // Write the plot to file.

  using boost::svg::detail::operator<<; // Needed for output a pair.
  cout << "x_range() " << my_1d_plot.x_range() << endl; // x_range() 1, 5.5

  //show_1d_plot_settings(my_1d_plot); // For *all* settings.
//] [/demo_1d_axis_scaling_5]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n  " << e.what() << std::endl;
  }
  return 0;
} // int main()
/*
//[demo_1d_axis_scaling_output

Autorun "j:\Cpp\SVG\Debug\demo_1d_axis_scaling.exe"
6 values in container: 0.2 1.1 4.2 3.3 5.4 6.5
8 values in container: 1.2 2.3 3.4 4.5 5.6 6.7 7.8 8.9
0.2 1.1 4.2 3.3 5.4 : 5 values used.
1.1 4.2 3.3 5.4 : 4 values used.
0.2 1.1 4.2 3.3 5.4 6.5 : 6 values used.
1.1 4.2 3.3 5.4 : 4 values used.
0.2 1.1 4.2 3.3 5.4 6.5 : 6 values used.
1.1 4.2 3.3 5.4 : 4 values used.
The smallest element is 0.2
The largest element is  6.5
axis_scaling 1 min 0.2, max = 6.5
Axis_scaled 2 min 0, max = 7, increment 1
Axis_scaled 3 min 0, max = 7, increment 1
Axis_scaled 4 min 0, max = 7, increment 1
Axis_scaled 5 min 0, max = 6, increment 1
Axis_scaled 6 min 0, max = 7, increment 1
not x_with_zero, 10 x_min_ticks, 0 x_steps, 0.001 tightness.
Axis_scaled min 0, max 6.5, interval 0.5
Axis_scaled min 0, max 6.5, interval 0.5
Axis_scaled min 1, max 5.5, interval 0.5
Axis_scaled  min 1, max 5.5, interval 0.5
x_range() 1, 5.5

//] [/demo_1d_axis_scaling_output]

*/

