/*! \file demo_2d_tick_values.cpp
    \brief Example of changing tick values color, etc
    \details Creates file demo_2d_tick_values.svg
    \author Paul A. Bristow
    \date Jul 2009
  */

// Copyright Paul A. Bristow 2009

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// An example to demonstrate options to change the way tick values are labelled.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_2d_tick_values_1
/*`
An example to demonstrate some options for controlling the layout and color of tick values.
*/
//] [/demo_2d_tick_values_1]

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;
#include <boost/svg_plot/detail/pair.hpp>
//  using boost::svg::detail::operator<<; // Output pair as, for example: 1.23, 4.56

#include <iostream>
 using std::cout;
  using std::endl;
  using std::ios_base;
  using std::dec;
  using std::hex;
#include <vector>
  using std::vector; // 1D data container.
#include <map>
  using std::map; // 2D data container.
#include <fstream> // for file streaming, ifstream & ofstream.
  using std::ifstream;
#include <string>
  using std::string;
  using std::getline;
#include <sstream>
  using std::istringstream;
#include <limits>
  using std::numeric_limits;
#include <algorithm>
  using std::min;
  using std::max;
#include <utility>
  using std::pair;
#include <ios>
  using std::noskipws;

double f(double x)
{ // A sample function to generate some X, Y pairs.
  return sqrt(x);
}

int main()
{ // Construct one STL containers for the data series to plot.

  map<double, double> data1; // record number and value.

  for(double i = 0.; i <= 20.; i += 1.)
  {
    data1[i] = f(i);
  }

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.
//[demo_2d_tick_values_2

/*`
The code below shows plotting the sqrt function
selecting the range of the axis by a user choice.

[note Unicode symbols can be found at http://en.wikipedia.org/wiki/Unicode_symbols.
The 4 hex digit value needs to be wrapped with prefix &#x and suffix ; like &#xDDDD;]
*/
    svg_2d_plot my_plot; // Construct a 2D plot.

    my_plot.legend_on(true) // Set title and legend, and X and Y axis range.
           .title("&#x221A; Function ") // Unicode sqrt symbol.
           .x_range(0, +20.)
           .x_major_interval(2.)

           .x_axis_label_color(green)
           .x_label_font_family("helvetica")
           .x_label_font_size(30)

           .x_num_minor_ticks(4) // MAJOR, minor, minor, minor, minor, MAJOR
           .x_ticks_values_color(red) //
           .x_ticks_values_font_family("Times New Roman")
           .x_ticks_values_font_size(20)
           .x_ticks_values_precision(0)
           .x_ticks_values_ioflags(ios_base::fixed)

           .y_range(0., 5.)
           .y_ticks_values_color(magenta)
           .y_ticks_values_precision(1)
           .y_ticks_values_ioflags(ios_base::scientific | ios_base::showpos)

            // "arial", "impact", "courier", "lucida console",  "Lucida sans unicode", "verdana", "calibri", "century", "lucida calligraphy", "tahoma", "vivaldi"
            // "informal roman", "lucida handwriting", "lucida bright", "helvetica"
            // "arial narrow" is narrow, so may be useful.
            // "arial black" is black!
            // "Times New Roman Bold" "Times New Roman italic" = Times New Roman
            // "comic sans", "sans" "bauhaus" "brush script" "segeo condensed" = Serif

            .y_ticks_values_font_family("lucida console")
            .y_ticks_values_font_size(10)

            .y_label_font_family("Times New Roman")
            .y_label_font_size(30)
            .y_axis_label_color(blue)
          ;

    my_plot.x_label("x abcd1234(&#x221A;)").y_label("sqrt(x) "); // Note chaining.

    // Add a container of data to the plot, choosing a color.
    my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(3).bezier_on(true).line_color(pink);

    my_plot.write("./demo_2d_tick_values.svg"); // Write the plot to another file.

    // Show the ticks styling:
    // X ticks.
    cout << "my_plot.x_ticks_values_color() " << my_plot.x_ticks_values_color() << endl;
    cout << "my_plot.x_ticks_values_font_family() " << my_plot.x_ticks_values_font_family() << endl;
    cout << "my_plot.x_ticks_values_font_size() " << my_plot.x_ticks_values_font_size() << endl;
    cout << "my_plot.x_ticks_values_precision() " << my_plot.x_ticks_values_precision() << endl;
    cout << "my_plot.x_ticks_values_ioflags() 0x" << hex << my_plot.x_ticks_values_ioflags() << dec << endl;
    // Y ticks.
    cout << "my_plot.y_ticks_values_color() " << my_plot.y_ticks_values_color() << endl;
    cout << "my_plot.y_ticks_values_font_family() " << my_plot.y_ticks_values_font_family() << endl;
    cout << "my_plot.y_ticks_values_font_size() " << my_plot.y_ticks_values_font_size() << endl;
    cout << "my_plot.y_ticks_values_color() " << my_plot.y_ticks_values_color() << endl;
    cout << "my_plot.y_ticks_values_precision() " << my_plot.y_ticks_values_precision() << endl;
    cout << "my_plot.y_ticks_values_ioflags() 0x" << hex << my_plot.y_ticks_values_ioflags() << dec << endl;
  }
//] [/demo_2d_tick_values_2]
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }

  return 0;
} // int main()



