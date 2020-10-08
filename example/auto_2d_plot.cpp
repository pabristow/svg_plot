/*! \file auto_2d_plot.cpp
  \brief An example to demonstrate autoscaling with *multiple* STL containers.
  \details See also demo_2d_autoscaling.cpp, auto_1d_plot.cpp and auto_1d_container.cpp.
  \author Paul A Bristow
*/

// Copyright Paul A Bristow 2008, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[auto_2d_plot_1

/*`First we need a few includes to use Boost.Plot
*/

#include <boost/svg_plot/svg_2d_plot.hpp>
  using namespace boost::svg;

#include <utility>
 // using std::pair;
#include <map>
 // using std::map;
#include <set>
//  using std::multiset;
#include <iostream>
//  using std::cout;
//  using std::endl;

#include <limits>
//  using std::numeric_limits;

#include <boost/math/special_functions.hpp>
  //using boost::math::isfinite;

  // Getting the max and min of x and y data points.
template <typename T> // T an STL container: array, vector ...
void s(T& container, // Container Data series to plot - entire container.
               // (not necessarily ordered, so will find min and max).
               double* x_min,  double* x_max,
               double* y_min,  double* y_max
               )
{
  typedef typename T::const_iterator iter;
  std::pair<iter, iter> result = boost::minmax_element(container.begin(), container.end());
  // minmax_element is efficient for maps because can use knowledge of being sorted,
  // BUT only if it can be assumed that no values are 'at limits',
  // infinity, NaN, max_value, min_value, denorm_min.
  // Otherwise it is necessary to inspect all values individually.
  std::pair<const double, double> px = *result.first;
  std::pair<const double, double> py = *result.second;
  *x_min = px.first;
  *x_max = py.first;
  *y_min = px.second;
  *y_max = py.second;

  std::cout << "s x_min " << *x_min << ", x_max " << *x_max << std::endl; // x_min 1, x_max 7.3
  std::cout << "s y_min " << *y_min << ", y_max " << *y_max << std::endl; // y_min 3.2, y_max 9.1
} // template <class T> int scale_axis  T an STL container: array, vector ...
//] [/auto_2d_plot_1]

int main()
{
//[auto_2d_plot_2
  /*`This example uses a single map to demonstrate autoscaling.
  We create a map to hold our data series.
  */
  std::map<const double, double> my_map;
  /*`
  Inserting some fictional values also sorts the data.
  The index value in [] is the x value.
  */
  my_map[1.1] = 3.2;
  my_map[7.3] = 9.1;
  my_map[2.1] = 5.4;

/*`Also include some 'at limits' values that might confuse autoscaling.
*/
  my_map[99.99] = std::numeric_limits<double>::quiet_NaN();
  my_map[999.9] = std::numeric_limits<double>::infinity();
  my_map[999.] = +std::numeric_limits<double>::infinity();

  /*`Next a 2D plot is created using defaults for the very many possible settings.
  */
  try
  { // try'n'catch clocks are needed to ensure error messages from any exceptions are shown.

  /*`Construct `myplot` and add at least a title,
    specify the both X and Y axes are to use autoscaling,
    and add the one data-series to be plotted.
  */
  svg_2d_plot my_plot;
  my_plot.title("Autoscale example 1"); // Add a title.
  my_plot.xy_autoscale(my_map); // Specify that both x and y axes are to use autoscaling,
  my_plot.plot(my_map); // Add the one data series to be plotted
  my_plot.write("./auto_2d_plot_1.svg"); // And write the SVG image to a file.

  /*`We can show the ranges used by autoscaling; */
  std::cout << "X min " << my_plot.x_range().first << ", X max " << my_plot.x_range().second << std::endl;
  std::cout << "Y min " << my_plot.y_range().first << ", Y max "  << my_plot.y_range().second << std::endl;

  /*`Had we know that there were no 'at limits' values, we could have chosen to skip the checks.
  This might be important for speed if there are thousands of data values.
  */
  my_plot.autoscale_check_limits(false);  // Skip checks for speed.
/*`The possible cost is that it will fail at run-time if there are any infinite or NaNs.
We could also chose to autoscale either of the axes separately, for example:
*/

  /*`But we might consider the Y-axis range of 3 to 10 a little ugly? so instead
  */
  svg_2d_plot my_plot_2;
  my_plot_2.title("Autoscale example 2"); // Add a title.
  my_plot_2.xy_autoscale(my_map); // Specify that both x and y axes are to use autoscaling.
  my_plot_2.plot(my_map); // Add the one data series to be plotted.

/*`and specify:*/

  my_plot_2.y_autoscale(0.4, 9.3); // autoscale using two doubles.

/*`which will chose a neater scale range from 0 to 10 for the Y axis. */

/*`It is also possible to fully control the factors used by autoscaling with function @c scale_axis.
 For examples, see \example\demo_1d_axis_scaling.cpp */

  my_plot_2.write("./auto_2d_plot_2.svg"); // And write another SVG image to another file.

//] [/auto_2d_plot_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
}

/*
//[auto_2d_plot_output
Output:

Autorun "j:\Cpp\SVG\Debug\auto_2d_plot.exe"
Checked: x_min 1.1, x_max 7.3, y_min 3.2, y_max 9.1, 3 'good' values, 3 values at limits.
X min 1, X max 8
Y min 3, Y max 10
X min 1, X max 8
Y min 0, Y max 10
//] [/auto_2d_plot_output]

*/
