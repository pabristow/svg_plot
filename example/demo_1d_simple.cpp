/*! \file demo_1d_simple.cpp
    \brief Example of a simple 1D plot of two vectors of data.
    \details Creates file demo_1d_simple.svg
    \author Jacob Voytko and Paul A. Bristow
    \date 2007
  */

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// An example to demonstrate very simple 1D settings.
// See also demo_1d_plot.cpp for a wider range of use.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_simple_1

/*`First we need a few includes to use Boost.Plot, and an STL 1D container vector.
*/

#include <boost/svg_plot/svg_1d_plot.hpp>
using namespace boost::svg;

#include <vector>
using std::vector;

int main()
{ // Construct two STL containers for the two data series to plot.
  vector<double> dan_times;
  vector<double> elaine_times;

  // Fill the two containers with some data:
  dan_times.push_back(3.1);
  dan_times.push_back(4.2);
  elaine_times.push_back(2.1);
  elaine_times.push_back(7.8);

  svg_1d_plot my_plot; // Construct a plot.

  my_plot.legend_on(true) // Set title and legend, and X-axis range and label.
         .title("Race Times")
         .x_label("time (sec)")
         .x_range(-1, 11);
  // There are hundreds of other possible options here!

  // Add the two containers of data to the plot, choosing two different colors.
  my_plot.plot(dan_times, "Dan").stroke_color(blue);
  my_plot.plot(elaine_times, "Elaine").stroke_color(orange);

  my_plot.write("./demo_1d_simple.svg"); // Finally write the plot to a file.

  return 0;
} // int main()

//] [/demo_1d_simple_1]


