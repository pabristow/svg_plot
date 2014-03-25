/*! \file demo_1d_tick_values.cpp
    \brief Demonstration of some simple 1D tick value label formatting.
    \details Quickbook markup to include in documentation.
    \date 19 Jul 2009
    \author Paul A. Bristow
*/

// Copyright Paul A Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate simple 1D settings but tick value formatting.
// See also demo_1d_plot.cpp for a wider range of use.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_tick_values_1

/*`As ever, we need a few includes to use Boost.Plot and an STL container.
*/
//] [demo_1d_tick_values_1]

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

  #include <boost/svg_plot/show_1d_settings.hpp>
// using boost::svg::show_1d_plot_settings - Only needed for showing which settings in use.

#include <iostream>
  using std::cout;
  using std::endl;
  using std::hex;
  using std::dec;
  using std::ios_base;
  using std::fixed;
  using std::scientific;

#include <iomanip>
  using std::setprecision;
  using std::setiosflags;

#include <vector>
  using std::vector;

int main()
{
 //[demo_1d_tick_values_2
/*`Some fictional data is pushed into an STL container, here vector<double>:*/
  vector<double> my_data;
  my_data.push_back(-1.6);
  my_data.push_back(4.2563);
  my_data.push_back(0.00333974);
  my_data.push_back(5.4);
  my_data.push_back(6.556);

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.
    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

    my_1d_plot.title("Demo 1D Tick Values") // Add a string title of the plot.
      .x_range(-5, 10) // Add a range for the X-axis.
      .x_label("temp (&#x00B0;C)"); // Add a label for the X-axis, using Unicode degree symbol.

/*`Add the one data series, `my_data` and a description,
and how the data points are to be marked, a circle with a diameter of 7 pixels.
*/
    my_1d_plot.plot(my_data, "1D Values").shape(circlet).size(7);

/*`If the default size and color are not to your taste, set more options, like:
*/
    my_1d_plot.size(500, 150) // Change plot window from the default image size.
      // Change the X-axis label style:
      .x_axis_label_color(green)
      .x_label_font_family("Arial")
      .x_label_font_size(18)

      // Change the style of the X (major) ticks:
      .x_ticks_values_color(magenta)
      //.x_ticks_values_font_family("Times New Roman")
      .x_ticks_values_font_family("arial")
      .x_ticks_values_font_size(15)

/*`The format of the tick value labels may not suit your data and its range,
so we can use the normal `iostream precision` and `ioflags` to change,
here to reduce the number of digits used from default precision 6 down to a more readable 2,
reducing the risk of collisions between adjacent values.
If values are very close to each other (a small range on the axis),
a higher precision wil be needed to differentiate them).
We could also prescribe the use of scientific format and force a positive sign:
By default, any unnecessary spacing-wasting zeros in the exponent field are removed.

If, perversely, the full 1.123456e+012 format is required, the stripping can be switched off with:
  `my_1d_plot.x_ticks_values_strip_e0s(false);`
*/
      // Change the format from the default "-4", "-2", "0" "2", "4" ...
      // (which makes a 'best guess' at the format)
      // to "-4.00", "-2.00", ..."+2.00", "4.00"
      // showing trailing zeros and a leading positive sign.
      .x_ticks_values_ioflags(ios_base::fixed | std::ios::showpos)
      .x_ticks_values_precision(1) // 

      // One could use ios_base::scientific for e format.
      //.x_ticks_values_ioflags(ios_base::fixed | std::ios::showpos )
      //.x_ticks_values_ioflags(std::ios::showpoint | std::ios::showpos)
      //.x_ticks_values_ioflags(std::ios::scientific)
     ;

/*`To use all these settings, finally write the plot to file.
*/
    my_1d_plot.write("demo_1d_tick_values.svg");

/*`If chosen settings do not have the effect that you expect, it may be helpful to display them.

(All the myriad settings can be displayed with `show_1d_plot_settings(my_1d_plot)`.)
*/
    //show_1d_plot_settings(my_1d_plot);
    using boost::svg::detail::operator<<;
    cout << "my_1d_plot.size() " << my_1d_plot.size() << endl;
    cout << "my_1d_plot.x_size() " << my_1d_plot.x_size() << endl;
    cout << "my_1d_plot.y_size() " << my_1d_plot.y_size() << endl;

    cout << "my_1d_plot.x_axis_label_color() " << my_1d_plot.x_axis_label_color() << endl;
    cout << "my_1d_plot.x_label_font_family() " << my_1d_plot.x_label_font_family() << endl;
    cout << "my_1d_plot.x_label_font_size() " << my_1d_plot.x_label_font_size() << endl;

    cout << "my_1d_plot.x_ticks_values_font_family() " << my_1d_plot.x_ticks_values_font_family() << endl;
    cout << "my_1d_plot.x_ticks_values_font_size() " << my_1d_plot.x_ticks_values_font_size() << endl;
    cout << "my_1d_plot.x_ticks_values_color() " << my_1d_plot.x_ticks_values_color() << endl;
    
    cout << "my_1d_plot.x_ticks_values_precision() " << my_1d_plot.x_ticks_values_precision() << endl;
    cout << "my_1d_plot.x_ticks_values_ioflags() " << hex << my_1d_plot.x_ticks_values_ioflags() << endl;

/*`See demo_1d_ticks_values.cpp for full source code.
*/

//] [demo_1d_tick_values_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

//[demo_1d_tick_values_output

Output:

utorun "j:\Cpp\SVG\Debug\demo_1d_ticks_values.exe
my_1d_plot.image_size() 500, 150
my_1d_plot.image_x_size() 500
my_1d_plot.image_y_size() 150
my_1d_plot.x_axis_label_color() RGB(0,128,0)
my_1d_plot.x_label_font_family() Arial
my_1d_plot.x_label_font_size() 18
my_1d_plot.x_ticks_values_font_family() Verdana
my_1d_plot.x_ticks_values_font_size() 12
my_1d_plot.x_ticks_values_color() RGB(255,0,255)
my_1d_plot.x_ticks_values_precision() 1
my_1d_plot.x_ticks_values_ioflags() 2020

//] [demo_1d_tick_values_output]

*/
