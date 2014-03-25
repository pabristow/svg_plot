/*! \file demo_point_markers.cpp
  \brief Demonstration of some marking data point options.
  \details Includes Quickbook markup.
  \author Paul A Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate some of the point plot markers available.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_point_markers_1

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

//] [demo_point_markers_1]

int main()
{
  vector<double> my_data;
  my_data.push_back(-9.);
  my_data.push_back(1.23);
  my_data.push_back(numeric_limits<double>::infinity());
  my_data.push_back(numeric_limits<double>::quiet_NaN());

//[demo_point_markers_2

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.
    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

    my_1d_plot.title("Demo point markers Demo") // Add a string title of the plot.
      .x_label("length (m)"); // Add a label for the X-axis.

/*`Add the one data series, `my_data` and a description, and how the data points are to marked,
here a circle with a diameter of 5 pixels.
*/
    my_1d_plot.plot(my_data, "1D Values").shape(diamond).size(10);

    // TODO want to display ALL the possible markers - to check they all work too.

/*`To put a value label against each data point, switch on the option:
*/
    my_1d_plot.x_values_on(true); // Add a label for the X-axis.

/*`If the default size and color are not to your taste, set more options, like:
*/
    my_1d_plot.x_values_font_size(14) // Change font size for the X-axis value labels.
      .x_values_font_family("Times New Roman") // Change font for the X-axis value labels.
      .x_values_color(red); // Change color from default black to red.

 /*` The 'at limit' values (infinity or NaN) markers can be customised, for example:
 */
    my_1d_plot.limit_color(red).limit_fill_color(green);

    /*`To use all these settings, finally write the plot to file.
*/
    my_1d_plot.write("demo_point_markers.svg");

/*`If chosen settings do not have the expected effect, is may be helpful to display them.

(All settings can be displayed with `show_1d_plot_settings(my_1d_plot)`.)
*/
    cout << "my_1d_plot.x_values_font_size() " << my_1d_plot.x_values_font_size() << endl;
    cout << "my_1d_plot.x_values_font_family() " << my_1d_plot.x_values_font_family() << endl;
    cout << "my_1d_plot.x_values_color() " << my_1d_plot.x_values_color() << endl;
    cout << "my_1d_plot.x_values_precision() " << my_1d_plot.x_values_precision() << endl;
    cout << "my_1d_plot.x_values_ioflags() " << hex << my_1d_plot.x_values_ioflags() << endl;

    cout << "limit points stroke color " << my_1d_plot.limit_color() << endl;
    cout << "limit points fill color " << my_1d_plot.limit_fill_color() << endl;

//] [demo_point_markers_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

//[demo_point_markers_output

Output:

Compiling...
demo_point_markers.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_point_markers.exe"
my_1d_plot.x_values_font_size() 14
my_1d_plot.x_values_font_family() Times New Roman
my_1d_plot.x_values_color() RGB(255,0,0)
my_1d_plot.x_values_precision() 3
my_1d_plot.x_values_ioflags() 200
Build Time 0:02
*/

