/*! \file demo_1d_limits.cpp
    \brief Demonstration of some 1D values including NaN and + and - infinity.
    \details Quickbook markup to include in documentation.
    \date 19 Feb 2009
    \author Paul A. Bristow
*/

// Copyright Paul A Bristow 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_limits_1
/*`As ever, we need a few includes to use Boost.Plot
*/

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <iostream>
  using std::cout;
  using std::endl;
  using std::hex;

#include <vector>
  using std::vector;

#include <limits>
  using std::numeric_limits;
//] [demo_1d_limits_1]

int main()
{
//[demo_1d_limits_2
/*`Some fictional data is pushed into an STL container, here `vector<double>`, including a NaN and + and - infinity:*/
  vector<double> my_data;
  my_data.push_back(-1.6);
  my_data.push_back(2.0);
  my_data.push_back(4.2563);
  my_data.push_back(-4.0);
  my_data.push_back(numeric_limits<double>::infinity());
  my_data.push_back(-numeric_limits<double>::infinity());
  my_data.push_back(numeric_limits<double>::quiet_NaN());

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.
    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

    my_1d_plot.title("Default 1D NaN and infinities Demo") // Add a string title of the plot.
      .x_range(-5, 5) // Add a range for the X-axis.
      .x_label("length (m)"); // Add a label for the X-axis.

/*`Add the one data series, `my_data` and a description, and how the data points are to marked,
here a circle with a diameter of 5 pixels.
*/
    my_1d_plot.plot(my_data, "1D limits").shape(circlet).size(5);

/*`To put a value label against each data point, switch on the option:
*/
    my_1d_plot.x_values_on(true); // Add data point value labels for the X-axis.

/*`To change the default colors (lightgray and whitesmoke) for the 'at limit' point marker
to something more conspicuous for this demonstration:
*/
    my_1d_plot.plus_inf_limit_color(blue);
    my_1d_plot.plus_inf_limit_color(pink);

/*`To use all these settings, finally write the plot to file.
*/
    my_1d_plot.write("demo_1d_limits.svg");

/*`
[note the +infinity point is marked on the far right of the plot, the -infinity on the far left, but the NaN (Not A Number) is at zero.]

To echo the new marker colors chosen:
*/
    cout << "+infinity_limit points stroke color " << my_1d_plot.plus_inf_limit_color() << endl;
    cout << "+infinity_limit points fill color " << my_1d_plot.plus_inf_limit_color() << endl;
//] [demo_1d_limits_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

//[demo_1d_limits_output

Output:

demo_1d_limits.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_limits.exe"
limit points stroke color RGB(0,0,255)
limit points fill color RGB(255,192,203)
Build Time 0:04
//] [demo_1d_limits_output]

*/

