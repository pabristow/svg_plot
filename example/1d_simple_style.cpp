/*! \file 1d_simple_style.cpp

  \brief Simple 1D plot from two vectors of doubles.
*/

//#define BOOST_SVG_POINT_DIAGNOSTICS
#define BOOST_SVG_LEGEND_DIAGNOSTICS

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_1d_plot.hpp>
//using namespace boost::svg;
//using boost::svg::svg_1d_plot;
#include <vector>
//using std::vector;

int main()
{
  // Construct two containers using std::vector.
  std::vector<double> reds_times;
  std::vector<double> blues_times;

  // Fill containers with ficticious data.
  reds_times.push_back(3.1);
  reds_times.push_back(4.2);
  blues_times.push_back(2.1);
  blues_times.push_back(7.8);

  using namespace boost::svg; // Access to SVG named colors and to svg_1d_plot.

  // Construct plot.
  svg_1d_plot my_plot;

  // Set a few features for the plot.
  my_plot.background_border_color(black)
     .title("Race Times")
     .legend_on(true) // Want a legend box,
    .legend_title("Teams")
         .x_range(-1, 11); // Offset to ensure that origin and max 10 are visible.

  // Add data to plot.
  my_plot.plot(reds_times, "Reds").stroke_color(red);
  my_plot.plot(blues_times, "Blues").stroke_color(blue);

  // Write plot to file.
  my_plot.write("./1d_simple_style.svg");
  return 0;
} // int main()
