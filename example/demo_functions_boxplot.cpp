/*! \file
  \brief Boxplot display of two example functions (compared to a 1-D plot).
*/

// demo_functions_boxplot.cpp

// Copyright Paul A. Bristow 2008, 2009, 2021

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

#include <boost/svg_plot/svg_1d_plot.hpp> // For 1d plots.
#include <boost/svg_plot/svg_boxplot.hpp>  // For boxplots.

#include <vector>  // using std::vector as container for two function's data values.
#include <cmath> // using ::sin;
#include <iostream> // using std::cout; std::endl;

//[boxplot_functions_1

/*` Two example functions to show how they display as a 1-D plot and as a boxplot.
One function is effectively 1/x and the other is effectively sin(x), 
but both are scaled to avoid too much overlap when displayed as a 1-D plot.
*/

// Effectively 1/x
double f(double x)
{
    return 50 / (x);
}

// Effectively sin(x)
double g(double x)
{
    return 60 + 25 * ::sin(x * 50);
}
//] [boxplot_functions_1]

int main()
{
  using namespace boost::svg;  // For SVG colors.
  try
  {

    std::vector<double> data1, data2;  // Container for two function data-series values.
    for(double i = 0.1; i < 10; i += 0.1)
    { // Fill our vectors with 100 values:
      double fv = f(i);
      double gv = g(i);
      //std::cout << i << ' ' << fv << ' ' << gv << std::endl; // Optionally display values?
      data1.push_back(fv);
      data2.push_back(gv);
    }

    // First display as a 1D plot.
    svg_1d_plot my_1d_plot;  // To hold a SVG 1-D plot.
    my_1d_plot.title(" 1D plots of example sin functions")
      .background_border_color(cyan)
      .x_min(0).x_max(100.) // Range of values shown on plot.
      .x_major_tick(10);

    my_1d_plot.plot(data1, "[50 / x]").stroke_color(blue);  // 1/x function.
    my_1d_plot.plot(data2, "[60 + 25 * sin(50x)]").stroke_color(red); // sin(x) function.

    my_1d_plot.write("./demo_functions_1d_plot.svg");  // Write out the plot.

    // Repeat display as a boxplot.
      svg_boxplot my_boxplot; // To hold a SVG boxplot.

    my_boxplot.title("Boxplots of 1/x and sin(x) Functions")
      .background_border_color(magenta)
      .x_label("Functions")
      .y_label("Population Size");

    my_boxplot.y_range(0, 100).y_major_interval(20); // Axis information.

    my_boxplot.plot(data1, "[50 / x]"); // 1/x function.
    my_boxplot.plot(data2, "[60 + 25 * sin(50x)]"); // sin(x) function.

    my_boxplot.write("./demo_functions_boxplot.svg");  // Write out the plot.
  }
  catch (const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n  " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // int main()
