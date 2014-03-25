/*! \file demo_1d_values.cpp
    \brief Demonstration of showing the 1D values.
    \details  Showing the 1D values of items from the data set.

    Some of the many possible formatting options are demonstrated,
    including controlling the precision and iosflags,
    and prefix and suffix also useful for giving units.

    Quickbook markup to include in documentation.
    \date 19 Jul 2009
    \author Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate labelling data values

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_values_1

/*` Showing the 1D values of items from the data set.
    Some of the many possible formatting options are demonstrated.

    As ever, we need a few includes to use Boost.Plot
*/

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

  #include <boost/svg_plot/show_1d_settings.hpp>
// using boost::svg::show_1d_plot_settings - Only needed for showing which settings in use.

#include <iostream>
  using std::cout;
  using std::endl;
  using std::hex;

#include <vector>
  using std::vector;
//] [demo_1d_values_1]

int main()
{
//[demo_1d_values_2
/*`Some fictional data is pushed into an STL container, here `vector<double>`:*/
  vector<double> my_data;
  my_data.push_back(-1.6);
  my_data.push_back(2.0);
  my_data.push_back(4.2563);
  my_data.push_back(0.00333974);
  my_data.push_back(5.4);
  my_data.push_back(6.556);

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.
    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

    my_1d_plot.title("Default 1D Values Demo") // Add a string title of the plot.
      .x_range(-5, 10) // Add a range for the X-axis.
      .x_label("length (m)"); // Add a label for the X-axis.

/*`Add the one data series, `my_data` and a description, and how the data points are to marked,
here a circle with a diameter of 5 pixels.
*/
    my_1d_plot.plot(my_data, "1D Values").shape(circlet).size(5);

/*`To put a value label against each data point, switch on the option:
*/
    my_1d_plot.x_values_on(true); // Add a label for the X-axis.

/*`If the default size and color are not to your taste, set more options, like:
*/
    my_1d_plot.size(500, 350) // Change from default size
      .x_values_font_size(14) // Change font size for the X-axis value labels.
      .x_values_font_family("Times New Roman") // Change font for the X-axis value labels.
      .x_values_color(red); // Change color from default black to red.

/*`The format of the values may also not be ideal,
so we can use the normal `iostream precision` and `ioflags` to change,
here to reduce the number of digits used from default precision 6 down to a more readable 2,
reducing the risk of collisions between adjacent values.
(Obviously the most suitable precision depends on the range of the data points.
If values are very close to each other, a higher precision wil be needed to differentiate them).
*/
    my_1d_plot.x_values_precision(2); // precision label for the X-axis value label.

/*`We can also prescribe the use of scientific format and force a positive sign:
*/
   my_1d_plot.x_values_ioflags(std::ios::scientific | std::ios::showpos);

/*`By default, any unnecessary spacing-wasting zeros in the exponent field are removed.
(If, perversely, the full 1.123456e+012 format is required, the stripping can be switched off with:
  `my_1d_plot.x_labels_strip_e0s(false);` )

In general, sticking to the defaults usually produces the neatest presentation of the values.
*/

/*`The default value label is horizontal, centered above the data point marker,
but, depending on the type and density of data points, and the length of the values
(controlled in turn by the `precision` and `ioflags` in use),
it is often clearer to use a different orientation.
This can be controlled in steps of 45 degrees, using an 'enum rotate_style`.

* `uphill` - writing up at 45 degree slope is often a good choice,
* `upward` - writing vertically up and
* `backup` are also useful.

(For 1-D plots other directions are less attractive,
placing the values below the horizontal Y-axis line,
but for 2-D plots all writing orientations can be useful).
*/
   my_1d_plot.x_values_rotation(steepup); // Orientation for the X-axis value labels.

   my_1d_plot.x_decor("[ x = ", "", "&#x00A0;sec]"); // Note the need for a Unicode space A0.

/*`To use all these settings, finally write the plot to file.
*/
    my_1d_plot.write("demo_1d_values.svg");

/*`If chosen settings do not have the effect that you expect, it may be helpful to display some of them!
(All the myriad settings can be displayed with `show_1d_plot_settings(my_1d_plot)`.)
*/
    //show_1d_plot_settings(my_1d_plot);
    using boost::svg::detail::operator<<;
    cout << "my_1d_plot.image_size() " << my_1d_plot.size() << endl;
    cout << "my_1d_plot.image x_size() " << my_1d_plot.x_size() << endl;
    cout << "my_1d_plot.image y_size() " << my_1d_plot.y_size() << endl;
    cout << "my_1d_plot.x_values_font_size() " << my_1d_plot.x_values_font_size() << endl;
    cout << "my_1d_plot.x_values_font_family() " << my_1d_plot.x_values_font_family() << endl;
    cout << "my_1d_plot.x_values_color() " << my_1d_plot.x_values_color() << endl;
    cout << "my_1d_plot.x_values_precision() " << my_1d_plot.x_values_precision() << endl;
    cout << "my_1d_plot.x_values_ioflags() " << hex << my_1d_plot.x_values_ioflags() << endl;
//] [demo_1d_values_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

//[demo_1d_values_output

Output:

demo_1d_values.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_values.exe"
 my_1d_plot.x_values_font_size() 14
my_1d_plot.x_values_font_family() Times New Roman
my_1d_plot.x_values_color() RGB(255,0,0)
my_1d_plot.x_values_precision() 2
 my_1d_plot.x_values_ioflags() 1020
Build Time 0:03
//] [demo_1d_values_output]
*/

