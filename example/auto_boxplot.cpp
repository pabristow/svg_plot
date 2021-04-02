/*! \file auto_boxplot.cpp

   \brief An example to demonstrate boxplot settings, including auto-scaling.
   \details See also:
     example @c auto_1d_containers.cpp for an example autoscaling with multiple data-series.
     example @c demo_boxplot.cpp for a wider range of use.

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[auto_boxplot_1

//!`First we need a few includes to use Boost.Plot
//! (and some others only needed for this example).
*/

#include <boost/quan/meas.hpp> // measurement class
#include <boost/quan/unc.hpp> // uncertain class

#include <boost/svg_plot/svg_boxplot.hpp>
  using boost::svg::svg_boxplot;

#include <boost/svg_plot/detail/pair.hpp>
// (Provides operator<< for std::pair).

#include <boost/algorithm/minmax.hpp>
 using boost::minmax;
#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;

#include <boost/svg_plot/detail/auto_axes.hpp>
  using boost::svg::show; // A single STL container.
  using boost::svg::show_all; // Multiple STL containers.
 // using boost::svg::range; // Find min and max of a STL container.
  using boost::svg::range_all;// Find min and max of multipler STL containers.

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

//] [/auto_boxplot_1]

void scale_axis(double min_value, double max_value, // input
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, // updated.
               bool origin, double tight, int min_ticks, int steps); 

constexpr double tol100eps = 1000 * numeric_limits<double>::epsilon(); // Suitable tight value.

int main()
{
  using namespace boost::svg; // Especially convenient for SVG colors.

  //[auto_boxplot_2
  /*`
  This example uses containers to demonstrate autoscaling.
  Autoscaling must inspect the container in order to find axis ranges that will be suitable.
  First we create a container and fill with some fictional data.
  */
  vector<double> my_data;
  // Initialize my_data with some entirely fictional data.
  my_data.push_back(0.2); // [0]
  my_data.push_back(1.1); // [1]
  my_data.push_back(4.2); // [2]
  my_data.push_back(3.3); // [3]
  my_data.push_back(5.4); // [4]
  my_data.push_back(6.5); // [5]
  my_data.push_back(6.8); // [6]
  my_data.push_back(6.9); // [7]
  my_data.push_back(7.2); // [8]
  my_data.push_back(7.3); // [9]
  my_data.push_back(8.1); // [10]
  my_data.push_back(8.5); // [11]

  /*`Not included is an 'at limit' value that could confuse autoscaling.
  Obviously we do not want the plot range to include infinity.
  // my_data.push_back(numeric_limits<double>::infinity()); // [12]
  */
  try
  { // Ensure error, warning and information messages from svg_plot are displayed by the catch block.
    
    double mn; // Ready to be updated by mnmx;
    double mx;
    int good = mnmx(my_data.begin(), my_data.end(), &mn, &mx);
    cout << good << " good values, " << my_data.size() - good << " limit values."
      << " min value = " << mn << ", max = " << mx << std::endl;
    // 12 good values, 0 limit values. min value = 0.2, max = 8.5

    svg_boxplot my_boxplot; // Construct a plot with all the default constructor values.
    my_boxplot.title("Auto boxplot");
    my_boxplot.y_label("Values");
    my_boxplot.y_autoscale(my_data);  // Compute autoscale values for the plot.
    //my_boxplot.y_autoscale(my_data.begin(), my_data.end());  // Compute autoscale values for the plot.
    // my_boxplot.y_autoscale(std::make_pair(0., 10.));
    //my_boxplot.y_autoscale(0., 9.);  // Compute autoscale values for the plot.
    cout << boolalpha << "Use y autoscale " << my_boxplot.y_autoscale() << "."<< std::endl;
    my_boxplot.plot(my_data, "Auto boxplot"); // Add the one data-series, and give it a title.
    my_boxplot.write("auto_boxplot.svg"); // Write the plot to file.

    /*`It may be useful to display that range chosen by autoscaling. */
    using boost::svg::detail::operator<<; // For displaying std::pair.
    cout << "y_range() " << my_boxplot.y_range() << std::endl; // x_range() 
  }
  catch(const std::exception& e)
  { // Error, warning and information messages are displayed by the catch block.
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  //] [/auto_boxplot_2]

  return 0;
} // int main()

/*

//[auto_boxplot_output

Compiling...
auto_boxplot.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\auto_boxplot.exe"
12 good values, 0 limit values. min value = 0.2, max = 8.5
Use y autoscale true.
Min outlier fences <-1.7 or >12.6333
Min extreme fences <-7.075 or >18.0083
y_range() 0, 9
Build Time 0:02
Build log was saved at "file://j:\Cpp\SVG\auto_boxplot\Debug\BuildLog.htm"

//] [auto_boxplot_output]

*/

