/*! \file demo_2d_lines.cpp
    \brief Demonstration of 2D 'function' lines, for example a least-squares fit line.
    \details Adding lines and curves to a plot.
    \author Paul A. Bristow
    \date Mar 2009
*/

// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

#ifdef _MSC_VER
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4512) // assignment operator could not be generated
#  pragma warning (disable : 4100) // unreferenced formal parameter
#endif

//[demo_2d_lines_1

/*`First we need some includes to use Boost.Plot and C++ Standard Library:
*/#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <boost/svg_plot/show_2d_settings.hpp>
// using boost::svg::show_2d_plot_settings - Only needed for showing which settings in use.

#include <map>
  using std::map;
  using std::multimap;

#include <iostream>
   using std::cout;
   using std::endl;
   using std::boolalpha;
//] [/demo_2d_lines_1]

int main()
{
  try
  {
//[demo_2d_lines_2

/*`
This shows how to add lines to a plot, for example, for example a least-squares 'function' fit line.
The data is roughly a straight line through the origin.
*/

  map<double, double> my_data;
  my_data[-1.1] = -1.2;
  my_data[-2.3] = -2.1;
  my_data[-2.9] = -3.3;
  my_data[-4.1] = -4.3;
  my_data[-5.0] = -5.3;
  my_data[-6.1] = -5.9;
  my_data[0.] = 0.;
  my_data[1.1] = 1.2;
  my_data[2.3] = 2.1;
  my_data[2.9] = 3.3;
  my_data[4.1] = 4.3;
  my_data[5.0] = 5.3;
  my_data[6.1] = 5.9;

  /*`
  First construct, size and draw a simple plot ready to add some sample lines.
  */
  svg_2d_plot my_plot;
  my_plot.size(400, 400);
  my_plot.plot(my_data, "my_data").fill_color(red);

  /*`
  First draw a line using SVG coordinates (rather than the Cartesian coordinates used for user's data - see below).
  Note that for SVG coordinates, Y increases *down* the page, so Y = 0 is the top and Y = 300 is the bottom.
  Default black is provided for color.
  */
  my_plot.draw_line(10, 390, 390, 10);

/*`This line almost reaches the corners of the SVG image, but for plotting XY graphs,
you probably don't want SVG coordinates, but want to use Cartesian coordinates for user's data.
Now draw a blue line using the Cartesian coordinates for user's data,
from the bottom left through the origin of the plot to the top right of the plot.

*/
  my_plot.draw_plot_line(-10, -10, +10, +10, blue);

/*`If you have calculated a confidence interval, you might want to add curved line(s) showing it (still using the Cartesian coordinates).
For example, you can draw a curve (quadratic) through two X, Y pairs using a Bezier curve with the middle point as control point.
*/
  my_plot.draw_plot_curve(-6, -8, 0, +1, +8, +6, red);

/*`Finally write the SVG image file.
[note At present, notes and lines must come after all plot commands to be put in the correct place.]
*/
  my_plot.write("./demo_2d_lines");
  //show_2d_plot_settings(my_plot);
//] [/demo_2d_lines_2]

  }
  catch (const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*
Output :

demo_2d_lines.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\Debug\demo_2d_lines.exe"

*/
