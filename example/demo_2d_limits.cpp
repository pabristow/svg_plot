/*! \file demo_2d_limits.cpp
    \brief Demonstration of some 2D values including NaN and + and - infinity.
    \details Quickbook markup to include in documentation.
    \date 19 Feb 2009
    \author Paul A. Bristow
*/

// Copyright Paul A Bristow 2008, 2009, 2013

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate plotting 2D 'at limts' values including NaN and + and - infinity.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_2d_limits_1

/*`An example to demonstrate plotting 2D 'at limits' values
including NaN and + and - infinity.

As ever, we need the usual includes to use Boost.Plot.

*/
//] [demo_2d_limits_1]

#include <boost/svg_plot/svg_2d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_2d_plot;
#include <iostream>
  using std::cout;
  using std::endl;

#include <map>
  using std::map;

#include <limits>
  using std::numeric_limits;

#include <utility>
  using std::make_pair;

int main()
{
//[demo_2d_limits_2
  /*`Some fictional data is pushed into an STL container, here map:*/

  map<double, double> my_data;
  /*`
  Inserting some fictional values also sorts the data.
  The map index value in [] is the x value, so mydata[x] = y.

  First some normal valued points, not 'at limits'.
  */
  my_data[1.1] = 3.2;
  my_data[4.3] = 3.1;
  my_data[0.25] = 1.4;
/*`
  Now some values including + and - infinity:
*/
  my_data[3] = numeric_limits<double>::quiet_NaN(); // marker at x = 3, y = 0
  my_data[0.] = numeric_limits<double>::quiet_NaN(); // Marker at 0,0
  my_data[1.] = numeric_limits<double>::infinity(); // Marker at 1, top
  my_data[-1] = -numeric_limits<double>::infinity(); // Marker at -1, bottom
  my_data[+numeric_limits<double>::infinity()] = +1.; // Marker at right, 1
  my_data[-numeric_limits<double>::infinity()] = -1.; // Marker at left, -1
  my_data[+(numeric_limits<double>::max)()] = +2.;  // Marker at right, 2
  my_data[-(numeric_limits<double>::max)()] = +2.; // Marker at left, 2
  my_data[-(numeric_limits<double>::max)() /2] = +3.; // Value near to max, marker left, 3
  my_data[numeric_limits<double>::infinity()] = numeric_limits<double>::infinity(); // Top right.
  my_data[-numeric_limits<double>::infinity()] = -numeric_limits<double>::infinity(); // Bottom left.

/*`
  [caution Using map (rather than multimap that allows duplicates) some assignments values overwrite,
  and so not all display as they do individually.
  In particular, an X value of quiet_NaN() causes a overwrite of the lowerest value (because NaNs never compare equal).
  So avoid NaN as an X value.]
*/

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.
    svg_2d_plot my_2d_plot; // Construct a plot with all the default constructor values.

    my_2d_plot.title("Default 2D 'at limits' NaN and infinities Demo") // Add a string title of the plot.
      .x_range(-5, 5) // Add a range for the X-axis.
      .y_range(-5, 5) // Add a range for the Y-axis
      .x_label("time (s)"); // Add a label for the X-axis.

/*`
Add the one data series, `my_data` and a description, and how the data points are to be marked,
here a circle with a diameter of 5 pixels.
*/
    svg_2d_plot_series& my_series = my_2d_plot.plot(my_data, "2D limits").shape(circlet).size(5);

/*`
We can also keep note of the plot series and use this to interrogate how many normal and how many 'at limit' values.
*/
    cout << my_series.values_count() << " normal data values in series." << endl;
    cout << my_series.limits_count() << " 'at limits' data values in series."<< endl;

/*`To put a value label against each data point, switch on the option:
*/
    //  my_2d_plot.x_values_on(true).y_values_on(true).x_values_font_size(12).y_values_font_size(12); // Add the X-axis and Y-axis values.
    // This displays x horizontally and Y downward.
    my_2d_plot.xy_values_on(true).x_values_font_size(12).y_values_font_size(12); // Add the X-axis and Y-axis values.
    // This displays X above and Y below.

/*`To change the default colors (lightgray and whitesmoke) for the 'at limit' point marker
to something more conspicuous for this demonstration:
*/
    my_2d_plot.limit_color(blue);
    my_2d_plot.limit_fill_color(pink);

/*`To use all these settings, finally write the plot to file.
*/
    my_2d_plot.write("demo_2d_limits.svg");

/*` Note the +infinity point is marked on the far right of the plot, the -infinity on the far left, but the NaN (Not A Number is at zero).

To echo the new marker colors chosen:
*/
    cout << "limit points stroke color " << my_2d_plot.limit_color() << endl;
    cout << "limit points fill color " << my_2d_plot.limit_fill_color() << endl;

//] [/demo_2d_limits_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

//[demo_2d_limits_output

Output:

Autorun "j:\Cpp\SVG\Debug\demo_2d_limits.exe"
3 normal data values in series.
9 'at limits' data values in series.
limit points stroke color RGB(0,0,255)
limit points fill color RGB(255,192,203)

*/
//] //[/demo_2d_limits_output]

