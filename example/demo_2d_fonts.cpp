/*! \file demo_2d_fonts.cpp
    \brief Example of changing font and sizes.
    \details Creates file demo_2d_fonts.svg
  */

// Copyright Paul A. Bristow 2009, 2021

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

// An example to demonstrate various fonts and font sizes using svg_plot.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_2d_fonts_1

/*`

The conventional wisdom of sticking to one or two fonts are deliberately broken
to show various fonts and sizes that are available for SVG plots.
The result is a graphic designers nightmare!

A font-family may or may not be available for a particular internet browser,
so it is inevitable that the exact appearance of a SVG plot may vary when
viewed with different browsers.  If a font-family is not recognised, then
a default (for that browser) will be used instead.

The font-families available for browsers do not seem to be listed,
but those used below are available on Mozilla Firefox 3.5.
The example below looks very similar with the Adobe SVG Add-in with Microsoft Internet Explorer 8
(which does NOT render SVG natively at all), and with Inkscape and Opera.

Mozilla Firefox 3.5 allows one to add font(s), but how these are rendered with SVG is not so clear.

For most purposes the default Font-family Verdana looks fine.

The following font-families work with Firefox 3.5:

  "arial", "impact", "courier", "lucida console",  "Lucida sans unicode",
  "verdana", "calibri", "century", "lucida calligraphy", "tahoma", "vivaldi"
  "informal roman", "lucida handwriting", "lucida bright", "helvetica"
  "arial narrow" is narrow, so may be useful to fit a long title or label.
  "arial black" is *black*!

These do NOT work and are substituted:

  "comic sans", "sans" "bauhaus" "brush script" "segeo condensed" = Serif

The narrow, wide, bold and italic features produce rather variable and unpredictable results
- the rendering may be 'fuzzy' or ill-formed: so these are not recommended.  For example,

  "Times New Roman Bold" "Times New Roman Italic" are substituted by "Times New Roman"

But to get narrow characters "arial narrow" works well, squeezing in a longer title or label.

[tip It is also possible to [*force] a line of text into a specified width.  Set `text_style.text_length > 0`.
But if used over-enthusiastically, this can cause glyphs to overlap or look oddly spread out.]

The font sizes are also changes from the defaults.  This should change the positioning,
but the calculations are complex and necessarily approximate.
Collisions between labels, other value-labels and axes are not impossible,
especially when the tick value-labels are not horizontal.

[tip If data-point values or tick-value-labels collide, rotating the tex can often improve things.
 For example `.x_major_label_rotation(uphill)` will make the major-tick value-labels slope uphill.
 Using Visual Studio, typing `.y_major_label_rotation( will show a dropdown of the various `rotate_style` values. ]

By default, the `std::precision` is reduced from the default 6 to 3, and unnecessary zeros and signs are stripped.

But it will still often be necessary to change the `std::iosflags` and `std::precision`,
and/or the number of major ticks and/or font size and type to avoid tick value-label collisions.

Unicode symbols can be found at [@http://en.wikipedia.org/wiki/Unicode_symbols Unicode symbols].
The 4 hex digit value needs to be wrapped with prefix &#x and suffix ; like &#xDDDD;
Rendering of Unicode symbols is not entirely predictable, but usually works well
to provide a wide range of greek and mathy symbols.
*/
//] [/demo_2d_fonts_1]

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
#include <map>
  using std::map; // 2D data container.

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
//[demo_2d_fonts_2

/*`The code below shows plotting the sqrt function selecting the range of the axis by a user choice.
*/
    {
      svg_2d_plot my_plot; // Construct a 2D plot.

      my_plot.legend_on(true)  // Note chaining.
           .title("&#x221A; Function ") // Unicode sqrt symbol.
           .title_font_size(35)
           .title_font_family("arial black")
           .legend_title("Legend title")
           .legend_title_font_size(15)
       //  was  .legend_header_font_size(15)
           .legend_font_family("lucida calligraphy")
           .legend_color(cyan)

           .x_range(0, +20.)
           .x_major_interval(2.)
           .x_num_minor_ticks(4) // For MAJOR, minor, minor, minor, minor, MAJOR ticks.
           .x_label("x abcd1234")
           .x_axis_label_color(green)
           .x_label_font_family("helvetica")
           .x_label_font_size(40)
           .x_ticks_values_color(red) //
           .x_ticks_values_font_family("Times New Roman")
           .x_ticks_values_font_size(14)
           .x_ticks_values_precision(0)
           .x_ticks_values_ioflags(ios_base::fixed)

           .y_label("sqrt(x) or (&#x221A;x)")
           .y_range(0., 5.)
           .y_ticks_values_color(magenta)
           .y_ticks_values_precision(1)
           .y_ticks_values_ioflags(ios_base::scientific | ios_base::showpos)
           .y_ticks_values_font_family("Lucida sans unicode")
           .y_ticks_values_font_size(20)
           //.y_label_font_family("informal roman")
           .y_label_font_family("Times New roman")
           .y_label_font_size(40)
           .y_axis_label_color(blue)
         ;

      // Add a container of data to the plot, choosing a color.
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(3).bezier_on(true).line_color(pink);

      my_plot.write("./demo_2d_fonts_1.svg"); // Write 1st plot to file.
      // Show some styling, for example for X ticks.
      cout << "my_plot.x_ticks_values_color() " << my_plot.x_ticks_values_color() << endl;
      cout << "my_plot.x_ticks_values_font_family() " << my_plot.x_ticks_values_font_family() << endl;
      cout << "my_plot.x_ticks_values_font_size() " << my_plot.x_ticks_values_font_size() << endl;
      cout << "my_plot.x_ticks_values_precision() " << my_plot.x_ticks_values_precision() << endl;
      cout << "my_plot.x_ticks_values_ioflags() 0x" << hex << my_plot.x_ticks_values_ioflags() << dec << endl;
    }
    // Axis label rotation default is horizontal.
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("X axis label font default size 14")
             .y_label("Y axis label font default size 14")
             //.x_label_font_size(10)
             //.y_label_font_size(10)
             ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_1.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("x (small X axis label font size 10)")
             .y_label("y (small X axis label font size 10)")
             .x_label_font_size(10)
             .y_label_font_size(10);
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_2.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("X axis label 30")
             .y_label("Y axis label 30")
            .x_label_font_size(30)
            .y_label_font_size(30);
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_3.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("x (large tick font size 20)")
             .y_label("y (large tick font size 20)")
            .x_label_font_size(10)
            .y_label_font_size(10)
            .x_ticks_values_font_size(20)
            .y_ticks_values_font_size(20)

            ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_4.svg"); // Write another plot to file.
    }
// Now alter the rotation of the axis labels.
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("X axis label font default size 14")
             .y_label("Y axis label font default size 14")
             .x_major_label_rotation(uphill)
             .y_major_label_rotation(uphill)
             //.x_label_font_size(10)
             //.y_label_font_size(10)
             ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_5.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("x (small X axis label font size 10)")
             .y_label("y (small X axis label font size 10)")
             .x_label_font_size(10)
             .y_label_font_size(10)
             .x_major_label_rotation(uphill)
             .y_major_label_rotation(uphill)
             ;

      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_6.svg"); // Write another plot to file.
    }

    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("X axis label 30")
             .y_label("Y axis label 30")
             .x_label_font_size(30)
             .y_label_font_size(30)
             .x_major_label_rotation(uphill)
             .y_major_label_rotation(uphill)
             ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_7.svg"); // Write another plot to file.
    }

    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("x tick size 12, label 14")
             .y_label("y tick size 12, label 14")
            .x_label_font_size(14)
            .y_label_font_size(14)
            .x_ticks_values_font_size(12)
            .y_ticks_values_font_size(12)
             .x_major_label_rotation(uphill)
             .y_major_label_rotation(uphill)
            ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_8.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("X axis label font default size 14")
             .y_label("Y axis label font default size 14")
             .x_major_label_rotation(downward)
             .y_major_label_rotation(upward)
             ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_9.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("x (small X axis label font size 10)")
             .y_label("y (small X axis label font size 10)")
             .x_label_font_size(10)
             .y_label_font_size(10)
             .x_major_label_rotation(steepdown)
             .y_major_label_rotation(steepup)
             ;

      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_10.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("X axis label 30")
             .y_label("Y axis label 30")
             .x_label_font_size(30)
             .y_label_font_size(30)
             .x_major_label_rotation(downhill)
             .y_major_label_rotation(uphill)
             ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_11.svg"); // Write another plot to file.
    }
    {
      svg_2d_plot my_plot; // Construct another 2D plot to try other variations of font sizes.
      my_plot.x_range(0, +20.)
             .y_range(0., 5.)
             .x_label("x tick size 12, label 14")
             .y_label("y tick size 12, label 14")
             .x_label_font_size(14)
             .y_label_font_size(14)
             .x_ticks_values_font_size(12)
             .y_ticks_values_font_size(12)
             .x_major_label_rotation(slopedownhill)
             .y_major_label_rotation(slopeup)
            ;
      my_plot.plot(data1, "Function (&#x221A;)").stroke_color(red).shape(circlet).size(10).line_on(false).line_color(green);
      my_plot.write("./demo_2d_fonts_12.svg"); // Write another plot to file.
    }
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()
//] [/demo_2d_fonts_2]

//[demo_2d_fonts_output
/*`
Output:

Autorun "I:\Cpp\SVG_plot\svg_plot\x64\Release\demo_2d_fonts.exe"
my_plot.x_ticks_values_color() RGB(255,0,0)
my_plot.x_ticks_values_font_family() Lucida Sans Unicode
my_plot.x_ticks_values_font_size() 14
my_plot.x_ticks_values_precision() 0
my_plot.x_ticks_values_ioflags() 0x2000
*/
//] [/demo_2d_fonts_output]
