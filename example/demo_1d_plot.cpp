/*! \file demo_1d_plot.cpp
    \brief Demonstration of many features for 1D plots.
    \details Contains Quickbook markup.

    \author Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2008, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_plot_1
// An entirely contrived example designed to demonstration
// as many features as possible in a single example.
// The results is not intended to be useful,
// nor is it intended to be pretty, but solely to highlight features used!
// See other examples for more practical (and tasteful) examples using typical features.

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;

#include <boost/svg_plot/show_1d_settings.hpp>
// void boost::svg::show_1d_plot_settings(svg_1d_plot&);

#include <boost/svg_plot/svg_fwd.hpp> // forward declarations.
// for testing its correctness.

#include <iostream> // for debugging.
 // using std::cout;
 // using std::endl;

#include <vector>
 // using std::vector;

#include <limits>
//  using std::numeric_limits;

int main()
{
  std::vector<double> my_data1;
  // Initialize my_data here with some entirely fictional data.
  my_data1.push_back(0.1);
  my_data1.push_back(1.1);
  my_data1.push_back(4.2);
  my_data1.push_back(3.3);
  my_data1.push_back(5.4);
  my_data1.push_back(6.5);

  std::vector<double> my_data2;
  // Initialize my_data here with some more entirely fictional data.
  my_data2.push_back(0.5);
  my_data2.push_back(1.5);
  my_data2.push_back(4.7);

  std::vector<double> my_data3;
  // Initialize my_data here with some more entirely fictional data.
  my_data3.push_back(0.7);
  my_data3.push_back(2.5);
  my_data3.push_back(5.8);

  std::vector<double> my_data4;
  // Initialize my_data with some integral values so can check points are marked exactly right.
  my_data4.push_back(1.);
  my_data4.push_back(2.);
  my_data4.push_back(3.);
  my_data4.push_back(6.);
  my_data4.push_back(7.);
  my_data4.push_back(8.);

  std::vector<double> my_data5;
  my_data5.push_back(0.);
  my_data5.push_back(-1.);
  my_data5.push_back(+1.);
  // Include an out-of-axis range value:
  my_data5.push_back(999.9);
  // Max and min values.
  my_data5.push_back((std::numeric_limits<double>::max)());
  my_data5.push_back((std::numeric_limits<double>::min)());
  // and non-finite values.
  my_data5.push_back((std::numeric_limits<double>::quiet_NaN)());
  my_data5.push_back((std::numeric_limits<double>::infinity)());

  svg_1d_plot my_1d_plot; // Construct with all the default constructor values.
  std::cout << "Image x & y " << my_1d_plot.x_size() << " by " << my_1d_plot.y_size() << std::endl;
  //my_1d_plot.size(100,100); // Alter both together.
  //cout << "Image x & y " << my_1d_plot.x_size() << " by " << my_1d_plot.y_size() << endl;
  //// And alter both separately.
  //my_1d_plot.x_size(200);
  //my_1d_plot.y_size(600);
  //cout << "Image x & y " << my_1d_plot.x_size() << " by " << my_1d_plot.y_size() << endl;

  my_1d_plot.document_title("Document title demo_1d_plot"); // This text shows on the browser tab.
  my_1d_plot.description("My demo_1d_plot description");
  my_1d_plot.copyright_date("2008-03-29");
  my_1d_plot.copyright_holder("Paul A. Bristow");
  my_1d_plot.license("permits", "permits", "requires", "permits", "permits"); // Require notice only.
  //see  http://creativecommons.org/licenses/ for details.
  my_1d_plot.coord_precision(4);

 std::cout << "font-family was " << my_1d_plot.title_font_family() << std::endl;

  my_1d_plot
  .y_size(250)
  .background_color(ghostwhite) // whole image.
  .background_border_color(aqua) //
  .background_border_width(10.) //
  .plot_window_on(true) //
  .plot_background_color(aliceblue) // just the plot area.
  .plot_border_color(pink)
  .plot_border_width(5.)
  .title("Demo 1D plot <sup>-&#945; </sup> &#x3A9; &#x3A6; &#x221A; &#x221E; &#x3B6; &#x00B1;")
    // domain of the random variable is [0, &#8734;]") //  Capital Omega <superscript> &#x3A9; </superscript>") doesn't work yet.
  .title_font_size(20)
  .title_font_family("Times New Roman")
  .title_color(magenta)
  .legend_on(true)
  .legend_background_color(beige)
  .legend_border_color(chocolate)
  .legend_title("My Legend &#956;") // generates <em>&#956;</em>  greek mu
  .legend_title_font_size(12)
  .legend_font_family("arial") // "arial", "impact", "courier", "lucida console",  "Lucida sans unicode", "verdana"
  .legend_font_weight("bold")
  .legend_color(darkgreen)
  .legend_lines(false) // Horizontal sample color line not useful if already showing values with colored shapes.
  .x_label_on(true) // show x-axis text label.
  .x_label("volume") // Care: this doesn't show unless .x_label_on() == true!
  .x_axis_color(blue)
  .x_label_color(blue)
  //.x_label_font_family("Verdana")
  .x_label_font_family("Lucida sans unicode")
//  .x_label_font_family("Times New Roman")
  .x_label_units_on(true)
  .x_label_font_size(12)
  .x_label_units(" (meter&#179; or m&#179;)") // super 2 = &#xB2; super 3 = &#179;
  // Note you must provide any space and any brackets if required.
  // Care: this doesn't show unless .x_label_units_on() == true!
  // Ticks
  .x_ticks_up_on(true) //
  .x_ticks_down_on(true) // So have Up and downward ticks.
  // Add grid - not very useful for 1D.
  .x_major_tick_width(3)
  .x_major_tick_length(10)
  .x_minor_tick_length(7)
  .x_major_grid_on(true)
  .x_major_grid_width(2)
  .x_major_grid_color(lightblue)
  .x_minor_grid_on(true)
  .x_minor_grid_width(1)
  .x_minor_grid_color(pink)
  .nan_limit_color(red)
  .nan_limit_fill_color(green)
  .x_ticks_on_window_or_axis(0) // -1 bottom, 0 on axis, +1 top

  .x_range(-1., 7.); // Display range.

  //my_1d_plot.plot(my_data1, "my values 1");

  //my_1d_plot.plot(my_data2, "my values round").shape(round).size(10).fill_color(pink);

  //my_1d_plot.plot(my_data1, "my values 1").shape(vertical_tick);

  //my_1d_plot.plot(my_data2, "my red values ").stroke_color(red).fill_color(blue);

  my_1d_plot.plot(my_data3, "data3").shape(diamond).size(10).stroke_color(red).fill_color(blue);
  my_1d_plot.plot(my_data4, "data4").shape(symbol).size(20);
  my_1d_plot.plot(my_data5, "data5").shape(cone).symbols("&#x3A9;").stroke_color(magenta).fill_color(aqua);
  //my_1d_plot.plot(my_data4, "data4").shape(symbol).symbols("&#x2721;").stroke_color(magenta).fill_color(aqua);
 // // U+2721 is Star of David or hexagram http://en.wikipedia.org/wiki/Hexagram

  my_1d_plot.write("demo_1d_plot.svg");

  show_1d_plot_settings(my_1d_plot);

//] [/demo_1d_plot_1]

  return 0;
} // int main()

/*

Compiling...
demo_1d_plot.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_plot.exe"
Build Time 0:11

*/

