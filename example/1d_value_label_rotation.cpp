/*! \file
    \brief Demonstration of showing the 1D data point values rotated at various angles.
    \details  Showing the 1D values of items from the data set.

    Some of the many possible formatting options are demonstrated,
    including controlling the precision and iosflags,
    and prefix and suffix also useful for giving units.

    Quickbook markup to include in documentation.
*/

// 1d_value_label_rotation.cpp
// 
// Copyright Paul A Bristow 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
//   or copy at http://www.boost.org/LICENSE_1_0.txt)

// An example to demonstrate labelling data-points with their values and rotation.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[1d_value_label_rotation

/*` Showing the 1-D values of items from the data set.
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
//] [1d_value_label_rotation]

int main()
{
  //[demo_1d_values_2
  /*`Some fictional data is pushed into an STL container, here `vector<double>`:*/
  vector<double> my_data;
  my_data.push_back(+1.1);
  my_data.push_back(2.2);
  my_data.push_back(3.3);
  my_data.push_back(4.4);
  my_data.push_back(5.5);

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.
    svg_1d_plot my_1d_plot; // Construct a plot with all the default constructor values.

    my_1d_plot.title("Rotating 1D value-labelling") // Add a string title of the plot.
      .x_range(-0, 4) // Add a range for the X-axis.
      .x_major_interval(1.)
      .x_num_minor_ticks(4)
      .x_label("length (m)"); // Add a label for the X-axis.

/*`Add the one data series, `my_data` and a description, and how the data points are to marked,
here a circle with a diameter of 5 pixels.
*/
    my_1d_plot.plot(my_data, "1D Values").shape(circlet).size(10).stroke_color(red).fill_color(blue);

    /*`To put a value-label against each data point, switch on the option:
    */
    my_1d_plot.x_values_on(true); // Add a label for the X-axis.

/*`If the default size and color are not to your taste, set more options, like:
*/
    my_1d_plot.size(500, 250) // Change from default size.
      .x_values_font_size(14) // Change font size for the X-axis value-labels.
      .x_values_font_family("Times New Roman") // Change font for the X-axis value-labels.
      .x_values_color(red); // Change color of value-label text from default black to red.
      

/*`The format of the values may also not be ideal,
so we can use the normal `iostream precision` and `ioflags` to change,
here to reduce the number of digits used from default precision 6 down to a more readable 2,
reducing the risk of collisions between adjacent values.
(Obviously the most suitable precision depends on the range of the data points.
If values are very close to each other, a higher precision wil be needed to differentiate them).
*/
    my_1d_plot.x_values_precision(2); // precision label for the X-axis value-label.

/*`We can also prescribe the use of scientific format and force a positive sign:
*/
    //my_1d_plot.x_values_ioflags(std::ios::scientific | std::ios::showpos);

    /*`By default, any unnecessary spacing-wasting zeros in the exponent field are removed.
    (If, perversely, the full 1.123456e+012 format is required, the stripping can be switched off with:
      `my_1d_plot.x_labels_strip_e0s(false);` )

    In general, sticking to the defaults usually produces the neatest presentation of the values.
    */
     my_1d_plot.x_decor("[ x=", "", "&#x00A0;s]"); 
   // my_1d_plot.x_decor("&#x00A0; [x=", "", " s]"); 
    // Note Leading Normal spaces are ignored!  
    // To get a real space you may need one or more of the several Unicode spaces, for example: A0 as &#x00A0; .

     /*`[note Code is shared between 1D and 2D variants, so arrangement is never perfect. [br]
     1D data-point markers are usually above the X-axis line, so many will overwrite the line.
     Prefix Unicode space(s) can avoid this, but the marker and value-label are then further apart.
     */

    /*`The default value-label is horizontal, centered above the data point marker,
    but, depending on the type and density of data points, and the length of the values
    (controlled in turn by the `precision` and `ioflags` in use),
    it is often clearer to use a different orientation.
    This can be controlled in steps of 45 degrees, using an 'enum rotate_style` whose possible values are:
   ``
    enum rotate_style
    {
      // Also need a no_rotate, = -1;
      horizontal = 0, //!< normal horizontal left to right, centered.
      slopeup = -30, //!< slope up.
      uphill = -45, //!< 45 steep up.
      steepup = -60, //!< up near vertical.
      upward = -90, //!< vertical writing up.
      backup = -135, //!< slope up backwards - upside down!
      leftward= -180, //!< horizontal to left.
      rightward = 360, //!< horizontal to right.
      slopedownhill = 30, //!< 30 gentle slope down.
      downhill = 45, //!< 45 down.
      steepdown = 60, //!<  60 steeply down.
      downward = 90,  //!< vertical writing down.
      backdown = 135, //!< slope down backwards.
      upsidedown = 180 //!< upside down!  (== -180)
    };
``
    * `uphill` - writing up at 45 degree slope is often a good choice,
    * `upward` - writing vertically up and
    * `backup` are also useful.

    (For 1-D plots other directions are less attractive,
    placing the values below the horizontal Y-axis line,
    but for 2-D plots all writing orientations can be useful).
    */

    // Orientation for the X-axis value-labels,
   //  my_1d_plot.x_values_rotation(steepup);  //  Nearly vertically upwards. OK Best compromise?
    // my_1d_plot.x_values_rotation(slopeup); // OK but is a bit high.
    // my_1d_plot.x_values_rotation(uphill); // OK but is a bit high.
    // my_1d_plot.x_values_rotation(upward); //  Vertically upwards. OK, but may need height for long labels. 
    // my_1d_plot.x_values_rotation(horizontal);  // Default Centered above, OK, but nearby values can collide, especially long labels. 
    // my_1d_plot.x_values_rotation(backup); // OK 
    // my_1d_plot.x_values_rotation(steepdown); // For 1D, Clashes with the y = 0 axis line, so need a few leading spaces, for example
    // my_1d_plot.x_decor("&#x00A0; [x=", "", " s]"); 
    // Or put the X-axis tick value-labels above the y= 0 axis line, and the markers below the axis line.
    // my_1d_plot.x_values_rotation(slopedownhill); //  Clashes with X axis line.
    //  my_1d_plot.x_values_rotation(downhill); //  Clashes with X axis line.
    //   my_1d_plot.x_values_rotation(downward); // Straight down - Clashes with X axis line.
    // my_1d_plot.x_values_rotation(backdown); // Clashes with X axis line.
    // my_1d_plot.x_values_rotation(upsidedown); // Upside down and Clashes with X axis line.

  //  my_1d_plot.x_values_rotation(leftward); // OK, but can clash with nearby points. 
  //  my_1d_plot.x_values_rotation(rightward); // OK, but can clash with nearby points. 
     // default orientation write value-labels horizontally above the markers.

 /*`To use all these settings, finally write the plot to file.
 */
  //  my_1d_plot.write("1d_value_label_rotation.svg");
    
    // Repeat for testing labelling rotation.

    //my_1d_plot.x_values_rotation(rightward); // OK, but can clash with nearby points. 
    //my_1d_plot.write("1d_value_label_rotation_all.svg");
    //my_1d_plot.x_values_rotation(leftward); // 
    //my_1d_plot.write("1d_value_label_rotation_all.svg");
    my_1d_plot.x_values_rotation(upward); // 
    my_1d_plot.write("1d_value_label_rotation_all.svg");


    //my_1d_plot.x_values_rotation(uphill); // 
    //my_1d_plot.write("1d_value_label_rotation_all.svg");
    //my_1d_plot.x_values_rotation(upward); // 
    //my_1d_plot.write("1d_value_label_rotation_all.svg");
    //my_1d_plot.x_values_rotation(backup); // 
    //my_1d_plot.write("1d_value_label_rotation_all.svg");

    /*`If chosen settings do not have the effect that you expect, it may be helpful to display some of them!
    (All the myriad settings can be displayed with `show_1d_plot_settings(my_1d_plot)`.)
    */
    //show_1d_plot_settings(my_1d_plot);
    using boost::svg::detail::operator<<;
    //] [demo_1d_values_2]
  }
  catch (const std::exception& e)
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
Plot written to file demo_1d_values.svg.
my_1d_plot.image_size() 500, 350
my_1d_plot.image x_size() 500
my_1d_plot.image y_size() 350
my_1d_plot.x_values_font_size() 14
my_1d_plot.x_values_font_family() Times New Roman
my_1d_plot.x_values_color() RGB(255,0,0)
my_1d_plot.x_values_precision() 2
my_1d_plot.x_values_ioflags() 1020

//] [demo_1d_values_output]
*/

