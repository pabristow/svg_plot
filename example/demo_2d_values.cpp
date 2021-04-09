/*! \file demo_2d_values.cpp
    \brief Demonstration of marking data-point values and uncertainty information in 2D plots.
    \details Contains Quickbook Markup to be included in documentation.

    \author Jacob Voytko and Paul A. Bristow
*/
// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate marking data-point values with uncertainty information
// like uncertainty (nominally standard deviation),
// degrees of freedom (nominally observations - 1),
// and/or computed confidence limits (confidence intervals).

// See also demo_2d_plot.cpp for a wider range of use.
// See demo_2d_uncertainty.cpp to show confidence intervals as ellipse around the data_points.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_2d_values_1

/*`As always, we need a few includes to use Boost.Plot
*/

#include <boost/svg_plot/svg_2d_plot.hpp>
  //using namespace boost::svg;
  //using boost::svg::svg_2d_plot;

#include <iostream>
  //using std::cout;
  //using std::endl;
  //using std::dec;
  //using std::hex;

#include <map>
//  using std::map;

//] [demo_2d_values_1]

int main()
{

  using namespace boost::svg;

//[demo_2d_values_2
/*`Some fictional data is pushed into an STL container, here map:*/

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.

/*`This example uses a single map to demonstrate autoscaling. We create a map to hold our first data-series using type double.
*/
    std::map<double, double> my_data;
    /*`
    Inserting some fictional values also sorts the data.
    The index value in [ ] is the x value.
    */
    my_data[1.1] = 3.2;  // X = 1.1 and Y = 3.2.
    my_data[7.3] = 9.1; // 
    my_data[2.12] = 2.4394;
    my_data[5.47] = 5.3861;

    using boost::svg::svg_2d_plot;  // This is needed to construct, setup and output the 2D plot in SVG format.

    svg_2d_plot my_2d_plot; // Construct a plot with all the default constructor values.

    my_2d_plot.title("Demo 2d Values") // Add a string title of the plot.
      .x_range(-5, 10) // Add a range for the X-axis.
      .x_label("length (m)"); // Add a label for the X-axis.


/*`Add the one data-point series, `my_data` and a description, and how the data-points are to be marked,
here a circle with a diameter of 5 pixels.
*/
    my_2d_plot.plot(my_data, "2d Values").shape(circlet).size(5).line_on(false);

/*`To put a value-label against each data_point, switch on the option:
*/
    //my_2d_plot.x_values_on(true); // Add a label for the X-axis.
    //my_2d_plot.y_values_on(true); // Add a label for the X-axis.
    my_2d_plot.xy_values_on(true); // Add a label for both the X and Y-axis.

/*`If the default size and color are not to your taste, set more options, like:
*/
    my_2d_plot.x_values_font_size(16) // Change font size for the X-axis value-labels.
      .x_values_font_family("Times New Roman") // Change font for the X-axis value-labels.
      .x_values_color(red); // Change X values color from default black to red.

    my_2d_plot.y_values_font_size(14) // Change font size for the Y-axis value-labels.
      .y_values_font_family("Arial") // Change font for the Y-axis value-labels.
      .y_values_color(blue); // Change Y color from default black to blue.

/*`The format of the values may also not be ideal,
so we can use the normal `iostream precision` and `ioflags` to change,
here to reduce the number of digits used from default precision 6 down to a more readable 2,
reducing the risk of collisions between adjacent values.
(Obviously the most suitable precision depends on the range of the data_points.
If values are very close to each other, a higher precision will be needed to differentiate them).
For measurement of typical precision, 2 or 3 decimal places will suffice.
*/
    my_2d_plot.x_values_precision(3); // Typical precision (3) for the X-axis value-label.
    my_2d_plot.y_values_precision(5); // Higher precision (5) for the Y-axis value-label.

/*`We can also prescribe the use of scientific, fixed format and/or force a positive sign:
*/
   //my_2d_plot.x_values_ioflags(std::ios::scientific | std::ios::showpos);
   //my_2d_plot.x_values_ioflags(std::ios::scientific);
   //my_2d_plot.y_values_ioflags(std::ios::fixed);

/*`By default, any unnecessary spacing-wasting zeros in the exponent field are removed.
Stripping "e+000" may appear to mean that the normal effect of `scientific` is not working.
(If, probably perversely, the full 1.123456e+012 format is required,
the stripping can be switched off with:
  `my_2d_plot.x_labels_strip_e0s(false);` )

In general, sticking to the default ioflags usually produces the neatest presentation of values.
*/
    my_2d_plot.x_plusminus_on(true); // unc label for the X-axis value-label.
    my_2d_plot.x_df_on(true); // Degrees of freedom label for the X-axis value-label.

    my_2d_plot.y_plusminus_on(true); // unc label for the X-axis value-label.
    my_2d_plot.y_df_on(true); // Degrees of freedom label for the X-axis value-label.  

/*`The default value-label is horizontal, centered above the data_point marker,
but, depending on the type and density of data_points, and the length of the values
(controlled in turn by choice of options, the `precision` and `ioflags` in use),
it is often clearer to use a different orientation.
This can be controlled in steps using an 'enum rotate_style` for convenience (or in degrees).

* `leftward` - writing level with the data_point but to its left.
* `rightward` - writing level with the data-point but to its right.
* `uphill` - writing up at 45 degree slope is often a good choice,
* `upward` - writing vertically up and
* `backup` - writing to the left are also useful.

(For 1-D plots other directions are less attractive,
placing the values below the horizontal Y-axis line,
but for 2-D plots all writing orientations can be useful).
*/
    my_2d_plot.x_values_rotation(rightward); // Orientation for the Y-axis value-labels.
    //my_2d_plot.x_values_rotation(horizontal); // Orientation for the X-axis value-labels.
    //my_2d_plot.x_values_rotation(uphill); // Orientation for the X-axis value-labels.
   // my_2d_plot.y_values_rotation(downhill); // Orientation for the Y-axis value-labels.
    //my_2d_plot.x_values_rotation(leftward); // Orientation for the X-axis value-labels.
    //my_2d_plot.y_values_rotation(rightward); // Orientation for the Y-axis value-labels.

   // my_2d_plot.x_plusminus_on(true); // Show Uncertainty for X-axis value-labels.
   // my_2d_plot.x_df_on(true); // Show Degrees of freedom (n-1) for X-axis value-labels.
    //my_2d_plot.y_plusminus_on(true); // Uncertainty for X-axis value-labels.

/*`To use all these settings, finally write the SVG plot to file.
*/
    my_2d_plot.write("demo_2d_values.svg");

/*`If chosen settings do not have the expected effect, is may be helpful to display them.

(All settings can be displayed with `show_2d_plot_settings(my_2d_plot)`.)
*/
    //std::cout << "my_2d_plot.x_values_font_size() " << my_2d_plot.x_values_font_size() << std::endl;
    //std::cout << "my_2d_plot.x_values_font_family() " << my_2d_plot.x_values_font_family() << std::endl;
    //std::cout << "my_2d_plot.x_values_color() " << my_2d_plot.x_values_color() << std::endl;
    //std::cout << "my_2d_plot.x_values_precision() " << my_2d_plot.x_values_precision() << std::endl;
    //std::cout << "my_2d_plot.x_values_ioflags() " << std::hex << my_2d_plot.x_values_ioflags() << std::dec << std::endl;

    //std::cout << "my_2d_plot.y_values_font_size() " << my_2d_plot.y_values_font_size() << std::endl;
    //std::cout << "my_2d_plot.y_values_font_family() " << my_2d_plot.y_values_font_family() << std::endl;
    //std::cout << "my_2d_plot.y_values_color() " << my_2d_plot.y_values_color() << std::endl;
    //std::cout << "my_2d_plot.y_values_precision() " << my_2d_plot.y_values_precision() << std::endl;
    //std::cout << " my_2d_plot.y_values_ioflags() " << std::hex << my_2d_plot.y_values_ioflags() << std::dec << std::endl;
//] [demo_2d_values_2]
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

//[demo_2d_values_output

Output:

demo_2d_values.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_2d_values.exe"
my_2d_plot.x_values_font_size() 16
my_2d_plot.x_values_font_family() Times New Roman
my_2d_plot.x_values_color() RGB(255,0,0)
my_2d_plot.x_values_precision() 3
my_2d_plot.x_values_ioflags() 200
my_2d_plot.y_values_font_size() 14
my_2d_plot.y_values_font_family() Arial
my_2d_plot.y_values_color() RGB(0,0,255)
my_2d_plot.y_values_precision() 5
 my_2d_plot.y_values_ioflags() 200

//] [demo_2d_values_output]

    //my_2d_plot.x_values_rotation(uphill); // Orientation for the X-axis value-labels.
    //my_2d_plot.x_values_rotation(horizontal); // Orientation for the X-axis value-labels.
    //my_2d_plot.x_values_rotation(backup); // Orientation for the X-axis value-labels.
    my_2d_plot.x_values_rotation(downhill); // Orientation for the X-axis value-labels.

*/

