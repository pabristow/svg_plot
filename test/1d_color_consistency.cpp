/*! \file  1d_color_consistency.cpp
  \brief Test of SVG 1D plot
  \author Jacob Voytko & Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

//#if defined (BOOST_MSVC) // requires a prior Boost include, so use MSC_VER instead.

#if defined (_MSC_VER)
#  pragma warning(disable : 4267) //  '=' : conversion from 'size_t' to 'unsigned int'
// in spirit
#  pragma warning(disable : 4310) //  Cast truncates constant value.
#  pragma warning(disable : 4512) //  Assignment operator could not be generated.
#  pragma warning(disable : 4702) //  Unreachable code.
#  pragma warning(disable : 4701) //  Potentially uninitialized local variable 'old_iph' used.
#  pragma warning(disable : 4996) //  Deprecated.
#endif

#define BOOST_TEST_MAIN
// Must come BEFORE this include.
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
#include <boost/svg_plot/svg_fwd.hpp> // forward declarations.
// Included to check its validity.

#include <iostream>
  using std::cout;
  using std::endl;
#include <string>
  using std::string;

BOOST_AUTO_TEST_CASE( test1 )
{
  // Check on default values and all set and get functions.
  // (Note: changes to default will mean tests must change too!)
  // Note that constants for comparison must be exactly the right type.
  // to avoid a warning from within Boost.Test.
  // use of unsigned int means that test value must also be unsigned,
  // for example 12U rather 12,
  // double must be 1. rather than 1...

  // Test the text_element class.
  // text_element::text_element(double x, double y,
  //  const std::string&,
  //  int,
  //  const std::string& font,
  //  const std::string& style, const std::string& weight,
  //  const std::string& stretch, const std::string& decoration,
  //  int align, int rotate);


  text_element text;
  BOOST_CHECK_EQUAL(text.x(), 0);
  text.x(10);
  BOOST_CHECK_EQUAL(text.x(), 10);
  BOOST_CHECK_EQUAL(text.y(), 0);
  text.y(20);
  BOOST_CHECK_EQUAL(text.y(), 20);

  BOOST_CHECK_EQUAL(text.text(), "");
  text.text("X");
  BOOST_CHECK_EQUAL(text.text(), "X");

  BOOST_CHECK_EQUAL(text.textstyle().font_size(), 12);
  text.textstyle().font_size(10);
  BOOST_CHECK_EQUAL(text.textstyle().font_size(), 10);

  BOOST_CHECK_EQUAL(text.textstyle().font_family(), "Lucida Sans Unicode");
  text.textstyle().font_family("Verdana");
  BOOST_CHECK_EQUAL(text.textstyle().font_family(), "Verdana");

  BOOST_CHECK_EQUAL(text.textstyle().font_style(), "");
  text.textstyle().font_style("X");
  BOOST_CHECK_EQUAL(text.textstyle().font_style(), "X");

  BOOST_CHECK_EQUAL(text.textstyle().font_weight(), "");
  text.textstyle().font_weight("X");
  BOOST_CHECK_EQUAL(text.textstyle().font_weight(), "X");

  BOOST_CHECK_EQUAL(text.textstyle().font_stretch(), "");
  text.textstyle().font_stretch("X");
  BOOST_CHECK_EQUAL(text.textstyle().font_stretch(), "X");

  BOOST_CHECK_EQUAL(text.textstyle().font_decoration(), "");
  text.textstyle().font_decoration("X");
  BOOST_CHECK_EQUAL(text.textstyle().font_decoration(), "X");

  double string_svg_length(const std::string& s, const text_style& style);

  text_style t(1); // font size / wh = unity for tests.
  // double wh = 0.7; declared in svg_style.hpp

  const string s0("");
  double l = string_svg_length(s0, t);
  BOOST_CHECK_EQUAL(l, 0);
  const string s1("1");
  BOOST_CHECK_EQUAL(string_svg_length(s1, t) /wh, 1);

  string s10("1234567890");
  l = string_svg_length(s10, t);
   BOOST_CHECK_EQUAL(l / wh, 10); // Normal chars
  const string g1("&#x00B1;"); // One greek.
  BOOST_CHECK_EQUAL( string_svg_length(g1, t)/ wh, 1);
   const string g2("&#x221A;&#x00B1;"); // 2 greek only.
  l = string_svg_length(g2, t);
  BOOST_CHECK_EQUAL( l / wh, 2);
  const string g3("&#x221A; &#x00B1;"); // 2 greek plus a space.
  const string x0("<>");
  l = string_svg_length(x0, t);
  l = l/wh;
  BOOST_CHECK_EQUAL(l, 0);
 // BOOST_CHECK_EQUAL( string_svg_length(x0, t) / wh, 0);
  const string xn("<    >"); // Should ignore all inside.
  BOOST_CHECK_EQUAL( string_svg_length(xn, t)/ wh, 0);

  const std::string s = "Demo 1D plot <sup>-&#945; </sup> &#x3A9; &#x3A6; &#x221A; &#x221E; &#x3B6; &#x00B1;";
  BOOST_CHECK_EQUAL( string_svg_length(s, t)/ wh, 28);

  // Test the svg_element class.

  //svg_1d_plot my_plot;
  //// Check the default image sizes:
  //BOOST_CHECK_EQUAL(my_plot.x_size(), 500U);
  //BOOST_CHECK_EQUAL(my_plot.y_size(), 200U);

  //my_plot.size(200U, 100U);
  //BOOST_CHECK_EQUAL(my_plot.x_size(), 200U);
  //my_plot.size(500U, 100U); // update x
  //BOOST_CHECK_EQUAL(my_plot.x_size(), 500U); // & check
  //my_plot.size(500U, 300U); // update y
  //BOOST_CHECK_EQUAL(my_plot.y_size(), 300U); // & check

 // // cout <<  my_plot.x_size() << endl; // 500
  //BOOST_CHECK_EQUAL(my_plot.x_size(), 500U);
 // // Check that test really works by checking the wrong answer!
  ////BOOST_CHECK_EQUAL(my_plot.x_size(), 400U);
 // // check my_plot.x_size() == 400U failed [500 != 400]

 // BOOST_CHECK_EQUAL(my_plot.legend_top_left(), std::pair(-1., -1.); // default position unassigned = -1..
 // my_plot.legend_top_left(10., 20.);
 // BOOST_CHECK_EQUAL(my_plot.legend_top_left(), std::pair(10., 20.); // new assigned position.

 // BOOST_CHECK_EQUAL(my_plot.title(), "Plot of data"); // Title of plot.
 // my_plot.title("test");
 // BOOST_CHECK_EQUAL(my_plot.title(), "test");

 // BOOST_CHECK_EQUAL(my_plot.legend_title(), "Legend"); // Default no legend title.
 // my_plot.legend_title("");
 // BOOST_CHECK_EQUAL(my_plot.legend_title(), "");

 // BOOST_CHECK_EQUAL(my_plot.document_title(), ""); // Default no document title
 // my_plot.document_title("Document Title");
 // BOOST_CHECK_EQUAL(my_plot.document_title(), "Document Title");

 // BOOST_CHECK_EQUAL(my_plot.description(), ""); // Default no document description.
 // my_plot.description("Document Description");
 // BOOST_CHECK_EQUAL(my_plot.description(), "Document Description");

 // BOOST_CHECK_EQUAL(my_plot.copyright_holder(), ""); // Default no copyright_holder.
 // my_plot.copyright_holder("Jake Voytko");
 // BOOST_CHECK_EQUAL(my_plot.copyright_holder(), "Jake Voytko");

 // BOOST_CHECK_EQUAL(my_plot.copyright_date(), ""); // Default no copyright_date.
 // my_plot.copyright_date("2007");
 // BOOST_CHECK_EQUAL(my_plot.copyright_date(), "2007");

 // // Plot Title & font.

 //  BOOST_CHECK_EQUAL(my_plot.title_font_family(), "Lucida Sans Console");// Check default.
 // my_plot.title_font_family("Times New Roman"); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_font_family(),"Times New Roman"); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.title_font_style(), ""); // Check default.
 // my_plot.title_font_style("bold"); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_font_style(), "bold"); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.title_font_weight(), ""); // Check default.
 // my_plot.title_font_weight("400"); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_font_weight(), "400"); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.title_font_stretch(), ""); // Check default.
 // my_plot.title_font_stretch("wider"); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_font_stretch(), "wider"); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.title_font_decoration(), ""); // Check default.
 // my_plot.title_font_decoration("underline"); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_font_decoration(), "underline"); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.title_font_rotation(), 0);// Check default.
 // my_plot.title_font_rotation(180); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_font_rotation(), 180); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.title_font_alignment(), center_align);// Check default.
 // my_plot.title_font_alignment(left_align); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_font_alignment(), left_align); // & check.

 // // legend title & font

 //  BOOST_CHECK_EQUAL(my_plot.legend_on(), false); // Check default.
 // my_plot.legend_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.legend_on(), true); // & check.

 // BOOST_CHECK_EQUAL(my_plot.legend_title_font_size(), 16U);// Check default.
 // my_plot.legend_title_font_size(15); // Set
 //  BOOST_CHECK_EQUAL(my_plot.legend_title_font_size(), 15U); // & check.


 //  BOOST_CHECK_EQUAL(my_plot.plot_window_on(), false); // Check default.
 // my_plot.plot_window_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.plot_window_on(), true); // & check.
 //  BOOST_CHECK_EQUAL(my_plot.plot_window_on(), true); // & check.

 // BOOST_CHECK_EQUAL(my_plot.x_ticks_up_on(), false); // Check default.
 // my_plot.x_ticks_up_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_ticks_up_on(), true); // & check.

 // BOOST_CHECK_EQUAL(my_plot.x_ticks_down_on(), true); // Check default.
 // my_plot.x_ticks_down_on(false); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_ticks_down_on(), false); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_label_on(), false); // Check default.
 // my_plot.x_label_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_label_on(), true); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_label_units_on(), false); // Check default.
 // my_plot.x_label_units_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_label_units_on(), true); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_major_labels_side(), true); // Check default.
 // my_plot.x_major_labels_side(false); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_major_labels_side(), false); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.title_on(), true); // Check default.
 // my_plot.title_on(false); // Set
 //  BOOST_CHECK_EQUAL(my_plot.title_on(), false); // & check.
 //  BOOST_CHECK_EQUAL(my_plot.title_on(), false); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_major_grid_on(), false); // Check default.
 // my_plot.x_major_grid_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_major_grid_on(), true); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_minor_grid_on(), false); // Check default.
 // my_plot.x_minor_grid_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_minor_grid_on(), true); // & check.

 // BOOST_CHECK_EQUAL(my_plot.axes_on(), false); // Check default (both axes on).
 // my_plot.axes_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.axes_on(), true); // & check.
 // my_plot.axes_on(false); // Set false revert to default (both axes on).

 //  BOOST_CHECK_EQUAL(my_plot.x_axis_on(), false); // & check.
 // my_plot.x_axis_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.x_axis_on(), true); // & check.

 // BOOST_CHECK_EQUAL(my_plot.y_axis_on(), true); // Check default.
 // my_plot.y_axis_on(false); // But leave false - as always should be for 1D?
 //  BOOST_CHECK_EQUAL(my_plot.y_axis_on(), false); // & check.
 // my_plot.y_axis_on(true); // Set
 //  BOOST_CHECK_EQUAL(my_plot.y_axis_on(), true); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.axes_on(), false); //
 // my_plot.axes_on(true); //
 //  BOOST_CHECK_EQUAL(my_plot.axes_on(), true); // Only x axis on
 // my_plot.axes_on(true); // Both on
 //  BOOST_CHECK_EQUAL(my_plot.axes_on(), true); //

 // // ticks left and right only for 2-D


 // my_plot.y_axis_on(false); // But leave false - as always should be for 1D?

 //  BOOST_CHECK_EQUAL(my_plot.title_color(), svg_color(0, 0, 0)); //  Check default color.
 // my_plot.title_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.title_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.background_color(), svg_color(255, 255, 255)); //  Check default color.
 // my_plot.background_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.background_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.legend_background_color(), svg_color(255, 255, 255)); //  Check default color.
 // my_plot.legend_background_color( svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.legend_background_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.legend_border_color(), svg_color(0, 0, 0)); //  Check default color.
 // my_plot.legend_border_color( svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.legend_border_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.background_border_color(), svg_color(0, 0, 0)); //  Check default color.
 // my_plot.background_border_color( svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.background_border_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.plot_background_color(), svg_color(255, 255, 255)); //  Check default color.
 // my_plot.plot_background_color( svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.plot_background_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_axis_color(), svg_color(black)); //  Check default color.
 // my_plot.x_axis_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.x_axis_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.y_axis_color(), svg_color(black)); //  Check default color.
 // my_plot.y_axis_color( svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.y_axis_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_label_color(), svg_color(black)); //  Check default color.
 // my_plot.x_label_color( svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.x_label_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.y_label_color(), svg_color(black)); //  Check default color.
 // my_plot.y_label_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.y_label_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_major_tick_color(), svg_color(black)); //  Check default color.
 // my_plot.x_major_tick_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.x_major_tick_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_minor_tick_color(), svg_color(black)); //  Check default color.
 // my_plot.x_minor_tick_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.x_minor_tick_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_major_grid_color(), svg_color(blue)); //  Check default color.
 // my_plot.x_major_grid_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.x_major_grid_color(), svg_color(red)); // & check.
 //  BOOST_CHECK_EQUAL(my_plot.x_major_grid_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_major_grid_width(), 3U); //  Check default width.
 // my_plot.x_major_grid_width(10U); //
 //  BOOST_CHECK_EQUAL(my_plot.x_major_grid_width(), 10U); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_minor_grid_width(), 1U); //  Check default width.
 // my_plot.x_minor_grid_width(10U); //
 //  BOOST_CHECK_EQUAL(my_plot.x_minor_grid_width(), 10U); // & check.

 // my_plot.x_minor_grid_color(svg_color(red)); //
 //  BOOST_CHECK_EQUAL(my_plot.x_minor_grid_color(), svg_color(red)); // & check.
 //  BOOST_CHECK_EQUAL(my_plot.x_minor_grid_color(), svg_color(red)); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_axis_width(), 0U); //  Check default width.
 // my_plot.x_axis_width(0); //
 //  BOOST_CHECK_EQUAL(my_plot.x_axis_width(), 0U); // & check.
 //  BOOST_CHECK_EQUAL(my_plot.x_axis_width(), 0U); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_label(), "X Axis"); //  Check default label.
 // my_plot.x_label("X"); //
 //  BOOST_CHECK_EQUAL(my_plot.x_label(), "X"); // & check.
 //  BOOST_CHECK_EQUAL(my_plot.x_label(), "X"); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_label_units(), "(units)"); //  Check default label.
 // my_plot.x_label_units("(meter)"); //
 //  BOOST_CHECK_EQUAL(my_plot.x_label_units(),"(meter)"); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_major_interval(), 2.); //  Check default.
 // my_plot.x_major_interval(1.); //
 //  BOOST_CHECK_EQUAL(my_plot.x_major_interval(), 1.); // & check.

 //  BOOST_CHECK_EQUAL(my_plot.x_major_tick_length(), 10U);
 // my_plot.x_major_tick_length(5U); //
 //  BOOST_CHECK_EQUAL(my_plot.x_major_tick_length(), 5U);

 //  BOOST_CHECK_EQUAL(my_plot.x_major_tick_width(), 3U);
 // my_plot.x_major_tick_width(5U); //
 //  BOOST_CHECK_EQUAL(my_plot.x_major_tick_width(), 5U);

 //  BOOST_CHECK_EQUAL(my_plot.x_minor_tick_width(), 1U);
 // my_plot.x_minor_tick_width(5U); //
 //  BOOST_CHECK_EQUAL(my_plot.x_minor_tick_width(), 5U);

 //  BOOST_CHECK_EQUAL(my_plot.x_num_minor_ticks(), 2U);
 // my_plot.x_num_minor_ticks(9U); //
 //  BOOST_CHECK_EQUAL(my_plot.x_num_minor_ticks(), 9U);

 //  BOOST_CHECK_EQUAL(my_plot.x_major_interval(), 1.);
 // my_plot.x_major_interval(2.); //
 //  BOOST_CHECK_EQUAL(my_plot.x_major_interval(), 2.);

 //  BOOST_CHECK_EQUAL(my_plot.x_minimum(), -10.); //  Check default xmin.
 //  BOOST_CHECK_EQUAL(my_plot.x_maximum(), +10.); //  Check default xmax.
 // my_plot.x_range(-1., +1.); //
 //  BOOST_CHECK_EQUAL(my_plot.x_minimum(), -1.); // & check.
 //  BOOST_CHECK_EQUAL(my_plot.x_maximum(), +1.); // & check.

 // std::pair<double, double> r = my_plot.x_range();
 //  BOOST_CHECK_EQUAL(r.first, -1.); // & check.
 //  BOOST_CHECK_EQUAL(r.second, +1.); // & check.

 // std::pair<double, double> xr = my_plot.x_range();
 //  BOOST_CHECK_EQUAL(xr.first, -1.); // & check.
 //  BOOST_CHECK_EQUAL(xr.second, +1.); // & check.

 // const string style = "test_style.css";
 // //my_plot.load_stylesheet(style); // TODO -  Error opening file test_style.css
 // //BOOST_CHECK(validate_stylesheet(style));
 //

 // my_plot
  //.background_border_color(svg_color(0, 0, 1))
  //.background_color(svg_color(0, 0, 2))
  //.legend_background_color(svg_color(0, 0, 3))
  //.legend_border_color(svg_color(0, 0, 4))
  //.plot_background_color(svg_color(0, 0, 5))
  //.title_color(svg_color(0, 0, 6))
  //.x_axis_color(svg_color(0, 0, 7))
  //.x_label_color(svg_color(0, 0, 8))
  //.x_major_grid_color(svg_color(0, 0, 9))
  //.x_major_tick_color(svg_color(0, 0, 10))
  //.x_minor_grid_color(svg_color(0, 0, 11))
  //.x_minor_tick_color(svg_color(0, 0, 12));

  //BOOST_CHECK(my_plot.background_border_color() == svg_color(0, 0, 1));
  //BOOST_CHECK(my_plot.background_color() == svg_color(0, 0, 2));
  //BOOST_CHECK(my_plot.legend_background_color() == svg_color(0, 0, 3));
  //BOOST_CHECK(my_plot.legend_border_color() == svg_color(0, 0, 4));
  //BOOST_CHECK(my_plot.plot_background_color() == svg_color(0, 0, 5));
  //BOOST_CHECK(my_plot.title_color() == svg_color(0, 0, 6));
  //BOOST_CHECK(my_plot.x_axis_color() == svg_color(0, 0, 7));
  //BOOST_CHECK(my_plot.x_label_color() == svg_color(0, 0, 8));
  ////BOOST_CHECK_EQUAL(svg_color(1, 1, 1), svg_color(0, 0, 0));
 // // check svg_color(1, 1, 1) == svg_color(0, 0, 0) failed [RGB(1,1,1) != RGB(0,0,0)]

  //BOOST_CHECK_EQUAL(red, red);  // OK
  //// BOOST_CHECK_EQUAL(white, black);
 // // check white == black failed [143 != 7]
 // // Note that named colors are enums!

  //BOOST_CHECK_EQUAL(svg_color(0, 0, 8), svg_color(0, 0, 8));
  //BOOST_CHECK_EQUAL(my_plot.x_label_color(), svg_color(0, 0, 8));
 // // using BOOST_CHECK_EQUAL is more convenient because it shows the difference!
 // // provided operator== and operator >> is provided.

  //BOOST_CHECK(my_plot.x_major_grid_color() == svg_color(0, 0, 9));
  //BOOST_CHECK(my_plot.x_major_tick_color() == svg_color(0, 0, 10));
  //BOOST_CHECK(my_plot.x_minor_grid_color() == svg_color(0, 0, 11));
  //BOOST_CHECK(my_plot.x_minor_tick_color() == svg_color(0, 0, 12));

 // // Repeat - intentional? - would different values be a better test?
  //my_plot
  //.x_minor_tick_color(svg_color(0, 0, 12))
  //.x_minor_grid_color(svg_color(0, 0, 11))
  //.x_major_tick_color(svg_color(0, 0, 10))
  //.x_major_grid_color(svg_color(0, 0, 9))
  //.x_label_color(svg_color(0, 0, 8))
  //.x_axis_color(svg_color(0, 0, 7))
  //.title_color(svg_color(0, 0, 6))
  //.plot_background_color(svg_color(0, 0, 5))
  //.legend_border_color(svg_color(0, 0, 4))
  //.legend_background_color(svg_color(0, 0, 3))
  //.background_color(svg_color(0, 0, 2))
  //.background_border_color(svg_color(0, 0, 1));

  //BOOST_CHECK(my_plot.x_minor_tick_color() == svg_color(0, 0, 12));
  //BOOST_CHECK(my_plot.x_minor_grid_color() == svg_color(0, 0, 11));
  //BOOST_CHECK(my_plot.x_major_tick_color() == svg_color(0, 0, 10));
  //BOOST_CHECK(my_plot.x_major_grid_color() == svg_color(0, 0, 9));
  //BOOST_CHECK(my_plot.x_label_color() == svg_color(0, 0, 8));
  //BOOST_CHECK(my_plot.x_axis_color() == svg_color(0, 0, 7));
  //BOOST_CHECK(my_plot.title_color() == svg_color(0, 0, 6));
  //BOOST_CHECK(my_plot.plot_background_color() == svg_color(0, 0, 5));
  //BOOST_CHECK(my_plot.legend_border_color() == svg_color(0, 0, 4));
  //BOOST_CHECK(my_plot.legend_background_color() == svg_color(0, 0, 3));
  //BOOST_CHECK(my_plot.background_color() == svg_color(0, 0, 2));
  //BOOST_CHECK(my_plot.background_border_color() == svg_color(0, 0, 1));

} // BOOST_AUTO_TEST_CASE( test1 )


/*
demo_1d_plot.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_1d_plot.exe"
font-family was Lucida Sans Console
font-family is Times New Roman
Plot window x 5 to 340, y 45 to 315
Build Time 0:11
Build log was saved at "file://j:\Cpp\SVG\demo_1d_plot\Debug\BuildLog.htm"
demo_1d_plot - 0 error(s), 0 warning(s)


*/
