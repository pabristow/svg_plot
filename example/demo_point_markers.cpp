/*! \file demo_point_markers.cpp
  \brief Demonstration of some marking data-point options.
  \details Includes Quickbook markup.
  \author Paul A Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2020

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
  //using namespace boost::svg;
  //using boost::svg::svg_1d_plot;

#include <iostream>
  //using std::cout;
  //using std::endl;

#include <vector>
 // using std::vector;

#include <limits>
 // using std::numeric_limits;

#include <boost/svg_plot/show_1d_settings.hpp>  // Only required for diagnostics display of all settings.

//] [demo_point_markers_1]

int main()
{
  // Construct two data-sets as std::vectors.
  std::vector<double> my_data_1;
  my_data_1.push_back(-10.0);
  my_data_1.push_back(-9.0);
  my_data_1.push_back(-1.0);
  my_data_1.push_back(1.23456); // Data value-label will be rounded to 1.23 with my_1d_plot.x_values_precision(3); 
  my_data_1.push_back(2.0);
  my_data_1.push_back(8.0987); // Rounded to 8.1 
  my_data_1.push_back(99.0); // Finite value that is too big to fit into the plot window (shows as a point-right triangle in green).

  // Add some not-normal values to show how they are displayed.
  my_data_1.push_back(-std::numeric_limits<double>::infinity()); // Minus infinity shows as a point-left triangle or cone.
  my_data_1.push_back(+std::numeric_limits<double>::infinity()); // Plus infinity shows as a point-right triangle or cone.
  my_data_1.push_back(std::numeric_limits<double>::quiet_NaN()); // NaN (NotANumber) shows as a point-down triangle, at the origin (0, 0).

  // Second data-set.
  std::vector<double> my_data_2;
  my_data_2.push_back(-6.0);
  my_data_2.push_back(-4.0);
  my_data_2.push_back(+4.0);
  my_data_2.push_back(+6.0);

  using namespace boost::svg; // Convenient to ensure all color are available as CSS words, for example: red, blue, green, pink, purple...
  using boost::svg::svg_1d_plot;  

//[demo_point_markers_2

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.

    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

    my_1d_plot.title("Demo point markers 1D") // Add a string title of the plot.
      .x_label("length (m)"); // Add a label for the X-axis, including a unit.

/*`Add the one data-series, `my_data` and a description, and how the data-points are to be marked,
here a blue diamond shape with a size of 10 pixels.
*/
    my_1d_plot.plot(my_data_1, "1D Values").shape(diamond).size(10).stroke_color(red).fill_color(blue);

    my_1d_plot.plot(my_data_2, "More 1D Values").shape(circlet).size(10).stroke_color(blue).fill_color(red);

/*`To put a decimal digit value-label against each data point, switch on the option:
*/
    my_1d_plot.x_values_on(true); // Add data decimal digit values as labels above the X-axis.

    my_1d_plot.x_values_precision(3); // Decimal digits precision for the X-axis value-label, for example "1.23".
    // if data-point value = 1.23456, then Data value-label will be 1.23 with my_1d_plot.x_values_precision(3); 
    my_1d_plot.x_values_rotation(steepup); // Orientation for the X-axis value-labels.
    my_1d_plot.x_values_font_size(7); // Font Size for the X-axis value-labels.

    my_1d_plot.x_values_alignment(align_style::right_align); // has no effect
    // And we can show these settings:
    std::cout << "alignment is " << my_1d_plot.x_values_alignment() << std::endl; // alignment is left
    std::cout << "rotation is " << show_rotation(my_1d_plot.x_values_rotation()) << std::endl; // rotation is steepup (-60)

/*`If the default size and color are not to your taste, set more options, like:
*/
    my_1d_plot.x_values_font_size(14) // Change font size for the X-axis value-labels.
      .x_values_font_family("Times New Roman") // Change font for the X-axis value-labels.
      .x_values_color(red); // Change x-values font-color from default black to red.

 /*` The 'at limit' values (+/- infinity or NaN) markers can be customised, for example:
    my_1d_plot.nan_limit_color(purple);
    my_1d_plot.nan_limit_fill_color(green); // No effect on fill color?
    my_1d_plot.nan_limit_size(20); 
    // But this currently makes + and - infinity and NaN all solid purple.
  NaN limit points stroke color RGB(128,0,128)  purple
  NaN limit points fill color RGB(0,128,0) green
  NaN limit points size 20

  +infinity limit points stroke color RGB(255,0,0)    red
  +infinity limit points fill color RGB(255,255,255)  white
  +infinity limit points size 10

  -infinity limit points stroke color RGB(0,0,255)  blue
  -infinity limit points fill color  RGB(255,255,255)  white
 */

    /*`To use all these settings, finally write the plot to file.
*/
    my_1d_plot.write("demo_point_markers.svg");

//`If chosen settings do not have the expected effect, is may be helpful to show them.

    std::cout << "my_1d_plot.x_values_font_size() " << my_1d_plot.x_values_font_size() << std::endl;
    std::cout << "my_1d_plot.x_values_font_family() " << my_1d_plot.x_values_font_family() << std::endl;
    std::cout << "my_1d_plot.x_values_color() " << my_1d_plot.x_values_color() << std::endl;
    std::cout << "my_1d_plot.x_values_precision() " << my_1d_plot.x_values_precision() << std::endl;
    std::cout << "my_1d_plot.x_values_ioflags() " << std::hex << my_1d_plot.x_values_ioflags() << std::dec << std::endl;

    std::cout << "NaN limit points stroke color " << my_1d_plot.nan_limit_color() << std::endl;
    std::cout << "NaN limit points fill color " << my_1d_plot.nan_limit_fill_color() << std::endl;
    std::cout << "NaN limit points size " << my_1d_plot.nan_limit_size() << std::endl;
    std::cout << "+infinity limit points stroke color " << my_1d_plot.plus_inf_limit_color() << std::endl;
    std::cout << "+infinity limit points fill color " << my_1d_plot.plus_inf_limit_fill_color() << std::endl;
    std::cout << "+infinity limit points size " << my_1d_plot.plus_inf_limit_size() << std::endl;
    std::cout << "-infinity limit points stroke color " << my_1d_plot.minus_inf_limit_color() << std::endl;
    std::cout << "-infinity limit points fill color " << my_1d_plot.minus_inf_limit_fill_color() << std::endl;
    std::cout << "-infinity limit points size " << my_1d_plot.minus_inf_limit_size() << std::endl;

//] [demo_point_markers_2]

// (Or all (over one hundred) settings can be displayed with `show_1d_plot_settings(my_1d_plot)`, commented out below.
   // show_1d_plot_settings(my_1d_plot);

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

