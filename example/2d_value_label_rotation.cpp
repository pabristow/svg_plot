/*! \file 
    \brief Demonstration of labelling data-point values with various rotations on 2D plots.

*/

// 2d_value_label_rotation.cpp

// Copyright Paul A Bristow 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
//   or copy at http://www.boost.org/LICENSE_1_0.txt)

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

//[2d_value_label_rotation_1

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

//] [2d_value_label_rotation_1]

int main()
{
  using namespace boost::svg;

  //[2d_value_label_rotation_2
  /*`Some fictional data is pushed into an STL container, here std::map:*/

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.

/*` We create a map to hold several data-series using type `double`.
*/
    std::map<double, double> my_data_0, my_data_1, my_data_2, my_data_3, my_data_4;
    /*`
    Inserting some fictional values also sorts the data.
    The index value in [ ] is the x value.
    */
    my_data_0[0.] = 0.;
    my_data_1[1.1] = 3.3;  // X = 1.1 and Y = 3.2.
    my_data_2[5.5] = 5.5;
    my_data_3[-2.2] = -2.2;
   // my_data_4[7.7] = 8.8; // 

    using boost::svg::svg_2d_plot;  // This is needed to construct, setup and output the 2D plot in SVG format.

    svg_2d_plot my_2d_plot; // Construct a plot with all the default constructor values.

    my_2d_plot
     // .title("2d_value_label_rotation") // Add a string title of the plot.
      .title_on(false)  // Avoid a title if repeating write("svg_files") as issue in title may mean title is ommitted causing a y offset.
      .x_range(-10, 10) // Add a range for the X-axis.
      .x_label("length (m)"); // Add a label for the X-axis.

    /*`
    Add a prefix, separator and suffix for the data-point value-labels.*/
    my_2d_plot.x_decor("[ x=", "", "&#x2009;s]");
    my_2d_plot.y_decor("{ y=", "", "&#x2009;g}");

/*`Add the one data-point series, `my_data` and a description, and how the data-points are to be marked,
here a circle with a diameter of 5 pixels.
*/
  //  my_2d_plot.plot(my_data_1, "2d Values").shape(circlet).size(5); // Small circle.
  //  my_2d_plot.plot(my_data_2, "2d Values").shape(cone_point_down).size(10); // Medium black downward cone.
   // my_2d_plot.plot(my_data_0, "2d Values").shape(square).size(20); // Larger square.
    my_2d_plot.plot(my_data_0, "2d Values").shape(circlet).size(20); // Larger circle
 //   my_2d_plot.plot(my_data_4, "2d Values").shape(star).size(10); // Medium star.

    /*`To put a value-label against each data_point, switch on the option:
    */
  //  my_2d_plot.x_values_on(true); // Add a label for the X-axis.
    my_2d_plot.y_values_on(true); // Add a label for the Y-axis.
   // my_2d_plot.xy_values_on(true); // Add a label for both the X and Y-axis on same line.

/*`If the default size and color are not to your taste, set more options, like font family:
*/
    my_2d_plot.x_values_font_size(20) // Change font size for the X-axis value-labels.
      .x_values_font_family("Times New Roman") // Change font for the X-axis value-labels.
      .x_values_color(red); // Change X value-label color from default black to red.

    my_2d_plot.y_values_font_size(14) // Change font size for the Y-axis value-labels.
      .y_values_font_family("Arial") // Change font for the Y-axis value-labels.
      .y_values_color(blue); // Change Y value-label color from default black to blue.

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

In general, sticking to the default `ioflags` usually produces the neatest presentation of values.

The uncertainty information about the values can also be shown,
but none is available for our demonstration values, so would show just implicit zero degrees of freedom.

*/
//my_2d_plot.x_plusminus_on(true); // Uncertainty label for the X-axis value-label.
//my_2d_plot.x_df_on(true); // Degrees of freedom label for the X-axis value-label.

//my_2d_plot.y_plusminus_on(true); // Uncertainty label for the Y-axis value-label.
//my_2d_plot.y_df_on(true); // Degrees of freedom label for the Y-axis value-label.  

/*`The default value-label is horizontal, centered above the data_point marker,
but, depending on the type and density of data_points, and the length of the values
(controlled in turn by choice of options, the `precision` and `ioflags` in use),
it is often clearer to use a different orientation.
This can be controlled in steps using an 'enum rotate_style` for convenience (or in degrees).

* `leftward` - writing level with the data_point but to its left.
* `rightward` - writing level with the data-point but to its right.
* `uphill` - writing up at 45 degree slope is often a good choice,
* `upward` - writing vertically up (and `downward` less so)
* `backup` - writing to the left are also useful.
* 
*    ``
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
      downward = 90,  //!< vertical writing downwards from marker.
      backdown = 135, //!< slope down backwards.
      upsidedown = 180 //!< upside down!  (== -180)
    };
``

(For 1-D plots other directions are less attractive,
placing the values below the horizontal Y-axis line,
but for 2-D plots all writing orientations can be useful).
*/
    my_2d_plot.x_values_rotation(upward);
// my_2d_plot.x_values_rotation(rightward); // Orientation for the X-axis value-labels.
//my_2d_plot.x_values_rotation(leftward); 
// 
 //   my_2d_plot.x_values_rotation(horizontal); 
//my_2d_plot.x_values_rotation(uphill); // Orientation for the X-axis value-labels./my_2d_plot.x_values_rotation(upward); // Vertical Orientation for the X-axis value-labels.

//my_2d_plot.x_values_rotation(downward); // Orientation for the X-axis value-labels. OK but close to marker.
   // my_2d_plot.y_values_rotation(downhill); // Orientation for the Y-axis value-labels.
//    my_2d_plot.x_values_rotation(backup); // Orientation for the X-axis value-labels.  to left is OK
  //  my_2d_plot.y_values_rotation(upward); // Orientation for the Y-axis value-labels.

   // my_2d_plot.x_plusminus_on(true); // Show Uncertainty for X-axis value-labels.
   // my_2d_plot.x_df_on(true); // Show Degrees of freedom (n-1) for X-axis value-labels.
    //my_2d_plot.y_plusminus_on(true); // Uncertainty for X-axis value-labels.

/*`To use all these settings, finally write the SVG plot to file.
*/
    const char* svg_filename = "2d_value_label_rotation.svg";
    my_2d_plot.write(svg_filename);
    //] [2d_value_label_rotation_2]

    const char* svg_x_allways_filename = "2d_value_label_rotation_allways_x.svg";

    my_2d_plot.x_values_rotation(rightward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(leftward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(upward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(downward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);

    my_2d_plot.x_values_rotation(slopeup); // 30 degrees Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(uphill); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(steepup); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);

    my_2d_plot.x_values_rotation(slopedownhill); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(downhill); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(steepdown); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_x_allways_filename);

    my_2d_plot.x_values_rotation(backup); // Orientation for the X-axis value-labels. left of marker.
    my_2d_plot.write(svg_x_allways_filename);
    my_2d_plot.x_values_rotation(backdown); // Orientation for the X-axis value-labels. Left of marker.
    my_2d_plot.write(svg_x_allways_filename);


    const char* svg_y_allways_filename = "2d_value_label_rotation_allways_y.svg";

    my_2d_plot.y_values_rotation(rightward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(leftward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(upward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(downward); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);

    my_2d_plot.y_values_rotation(slopeup); // 30 degrees Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(uphill); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(steepup); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);

    my_2d_plot.y_values_rotation(slopedownhill); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(downhill); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(steepdown); // Orientation for the X-axis value-labels. Right of marker.
    my_2d_plot.write(svg_y_allways_filename);

    my_2d_plot.y_values_rotation(backup); // Orientation for the X-axis value-labels. left of marker.
    my_2d_plot.write(svg_y_allways_filename);
    my_2d_plot.y_values_rotation(backdown); // Orientation for the X-axis value-labels. Left of marker.
    my_2d_plot.write(svg_y_allways_filename);





    std::cout << "Wrote plot to file " << "\"" << svg_filename << "\"" << std::endl;
  }
  catch (const std::exception& e)
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


*/

