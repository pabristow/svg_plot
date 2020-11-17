/*! \file test_svg.cpp
   \brief Tests for svg class.
   \details Tests some of the fundamental SVG class functions used by SVG plot functions.
   (See also other test modules for 1d_plot, 2d_plot and boxplot).
   \date 10 Feb 2009
   \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MAIN
// NB define BOOST_TEST_MAIN must come BEFORE this include.
#include <boost/test/unit_test.hpp>

#include <boost/svg_plot/svg.hpp>
  using boost::svg::svg;
  using boost::svg::g_element;
  using boost::svg::rect_element;
  using boost::svg::svg_style;

#include <boost/svg_plot/svg_color.hpp>
  using namespace boost::svg;

#include <boost/svg_plot/svg_fwd.hpp> // Forward declarations.
  // Included here as a cross check.

#include <iostream>
  using std::cout;
  using std::endl;
#include <string>
  using std::string;
#include <sstream>
  using std::ostringstream;

BOOST_AUTO_TEST_CASE(test_styles)
{
  // Check on default values and all set and get functions.
  // (Note: changes to default will mean tests must change too!)
  // Note that constants for comparison must be exactly the right type
  // to avoid a warning from within Boost.Test.
  // Use of unsigned int means that test value must also be unsigned,
  // for example, "12U" rather "12", and
  // double must be "1." rather than integer "1".

  // Test svg_color
  BOOST_CHECK_EQUAL(aliceblue, 0); // Is 1st array element, index zero.
  BOOST_CHECK_EQUAL(black, 7); // Is 8th array element, index 7.
  BOOST_CHECK_EQUAL((size_t)blank, sizeof(color_array)/sizeof(svg_color)-1); // Is last array element, index 147.
  BOOST_CHECK_EQUAL(svg_color(aliceblue), svg_color(240, 248, 255));
  BOOST_CHECK_EQUAL(svg_color(red), svg_color(255, 0, 0));
  BOOST_CHECK_EQUAL(svg_color(black), svg_color(0, 0, 0)); // default black.
  BOOST_CHECK_EQUAL(svg_color(blank), svg_color(255, 255, 255)); // blank is now white
  BOOST_CHECK_NE(black, white);
  BOOST_CHECK_NE(red, green);

  BOOST_CHECK_EQUAL(black, black); //
  BOOST_CHECK_EQUAL(white, white); //
  BOOST_CHECK_EQUAL(svg_color(blank), svg_color(blank)); //

  // Test operator==
  svg_color my_red(255, 0, 0); //
  BOOST_CHECK_EQUAL(my_red, svg_color(red));
  BOOST_CHECK_EQUAL(my_red, red);
  // Test operator!=
  BOOST_CHECK_NE(my_red, black);

  // Test svg_color operator<<
  ostringstream oss;
  oss << svg_color(red);
  BOOST_CHECK_EQUAL(oss.str().c_str(), "RGB(255,0,0)");
  //oss.str().erase(); // doesn't seem to erase the string - only a copy!
  //BOOST_CHECK_EQUAL(oss.str().size(), 0U); // fails.
  //BOOST_CHECK_EQUAL(oss.str().c_str(), ""); // fails.
  oss.str(""); // So use this null string to erase the string.
  BOOST_CHECK_EQUAL(oss.str().c_str(), "");
  oss << svg_color(lime);
  BOOST_CHECK_EQUAL(oss.str().c_str(), "RGB(0,255,0)");
  oss.str("");
  oss << svg_color(blue);
  BOOST_CHECK_EQUAL(oss.str().c_str(), "RGB(0,0,255)");

  BOOST_CHECK_EQUAL(blank, blank);
  BOOST_CHECK_NE(blank, red);
  BOOST_CHECK_EQUAL(blank, svg_color(blank));

// Test style in svg_style (fill stroke & width)

  BOOST_CHECK_EQUAL(detail::PLOT_BACKGROUND, 0);
  // 1st MUST be zero because indexing an array.
  BOOST_CHECK_EQUAL(detail::document_ids_[0], "imageBackground");
  BOOST_CHECK_EQUAL(detail::document_ids_[detail::SVG_PLOT_DOC_CHILDREN], "plotDocChildren");
  // This will change if any new document elements are added.

  // svg_style.hpp
  // Test the point and line style structures.

  plot_point_style my_point(svg_color(pink), svg_color(orange), 10, circlet);
  BOOST_CHECK_EQUAL(my_point.shape(), circlet);
  BOOST_CHECK_EQUAL(my_point.stroke_color(), svg_color(pink));
  BOOST_CHECK_EQUAL(my_point.fill_color(), svg_color(orange));
  BOOST_CHECK_EQUAL(my_point.size(), 10);

  plot_line_style my_plot_line(svg_color(black), false, false);
  BOOST_CHECK_EQUAL(my_plot_line.color(), svg_color(black));
  BOOST_CHECK_EQUAL(my_plot_line.area_fill(), svg_color(blank));
  BOOST_CHECK_EQUAL(my_plot_line.line_on(), true);
  BOOST_CHECK_EQUAL(my_plot_line.bezier_on(), false);
  plot_line_style my_plot_line2(svg_color(red), true, true);
  my_plot_line2.area_fill(green);
  // Note 1: green is a color constant, nor a svg_color.
  BOOST_CHECK_EQUAL(my_plot_line2.color(), svg_color(red));
  BOOST_CHECK_EQUAL(my_plot_line2.area_fill(), green);
  BOOST_CHECK_EQUAL(my_plot_line2.line_on(), true);
  BOOST_CHECK_EQUAL(my_plot_line2.bezier_on(), false);

  // Test the class svg_style.

  svg_style my_style; // Default constructor.
  BOOST_CHECK_EQUAL(my_style.stroke_color(), svg_color(0, 0, 0)); // default black.
  BOOST_CHECK_EQUAL(my_style.stroke_color(), black); // default black.
  BOOST_CHECK_EQUAL(svg_color(black), svg_color(0, 0, 0)); // default black.

  my_style.stroke_color(red); // change
  BOOST_CHECK_EQUAL(my_style.stroke_color(), svg_color(red)); // & check.
  my_style.stroke_color(white); // change
  BOOST_CHECK_EQUAL(my_style.stroke_color(), svg_color(white)); // & check.

  BOOST_CHECK_EQUAL(my_style.fill_color(), svg_color(blank)); // default black.
  my_style.fill_color(white); // change
  BOOST_CHECK_EQUAL(my_style.fill_color(), svg_color(white)); // & check.

  BOOST_CHECK_EQUAL(my_style.stroke_width(), 0U); // default black.
  my_style.stroke_width(10U); // change
  BOOST_CHECK_EQUAL(my_style.stroke_width(), 10U); // & check.

  // Test the text_element class.
  // text_element::text_element(double x, double y,
  //  const std::string&,
  // text_style&,

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

  // Test the text_style class.
  //  int,
  //  const std::string& font,
  //  const std::string& style, const std::string& weight,
  //  const std::string& stretch, const std::string& decoration,
  //int font_size;
  //std::string font_family;
  //std::string weight;
  //std::string style;
  //std::string stretch;
  //std::string decoration;

  text_style def_style; // All defaults.

  BOOST_CHECK_EQUAL(def_style.font_size(), 12);
  BOOST_CHECK_EQUAL(def_style.font_family(), "Lucida Sans Unicode");
  BOOST_CHECK_EQUAL(def_style, no_style);

  text_style test_style(12, "Arial", "bold", "italic", "wider", "underline");
  cout << test_style << endl;
  BOOST_CHECK_NE(def_style, test_style);
  text_style copy_style(test_style);
  BOOST_CHECK_EQUAL(copy_style, test_style);
  copy_style = def_style;
  BOOST_CHECK_NE(copy_style, test_style);
  BOOST_CHECK_EQUAL(copy_style, def_style);

  BOOST_CHECK_EQUAL(test_style.font_size(), 12);
  BOOST_CHECK_EQUAL(test_style.font_family(), "Arial");
  BOOST_CHECK_EQUAL(test_style.font_weight(), "bold");
  BOOST_CHECK_EQUAL(test_style.font_style(), "italic");
  BOOST_CHECK_EQUAL(test_style.font_stretch(), "wider");
  BOOST_CHECK_EQUAL(test_style.font_decoration(), "underline");

  BOOST_CHECK_EQUAL(test_style.font_family(), "Arial");
  test_style.font_family("X");
  BOOST_CHECK_EQUAL(test_style.font_family(), "X");

  BOOST_CHECK_EQUAL(test_style.font_style(), "italic");
  test_style.font_style("X");
  BOOST_CHECK_EQUAL(test_style.font_style(), "X");

  BOOST_CHECK_EQUAL(test_style.font_weight(), "bold");
  test_style.font_weight("X");
  BOOST_CHECK_EQUAL(test_style.font_weight(), "X");

  BOOST_CHECK_EQUAL(test_style.font_stretch(), "wider");
  test_style.font_stretch("X");
  BOOST_CHECK_EQUAL(test_style.font_stretch(), "X");

  BOOST_CHECK_EQUAL(test_style.font_decoration(), "underline");
  test_style.font_decoration("X");
  BOOST_CHECK_EQUAL(test_style.font_decoration(), "X");

  BOOST_CHECK_EQUAL(text.rotation(), horizontal);
  text.rotation(upward);
  BOOST_CHECK_EQUAL(text.rotation(), upward);

  BOOST_CHECK_EQUAL(text.alignment(), left_align);
  text.alignment(right_align);
  BOOST_CHECK_EQUAL(text.alignment(), right_align);

  // Test the svg_element class.

  svg my_svg;
  // Check the default svg image sizes:
  BOOST_CHECK_EQUAL(my_svg.x_size(), 400U);
  BOOST_CHECK_EQUAL(my_svg.y_size(), 400U);

  my_svg.size(200U, 100U);
  BOOST_CHECK_EQUAL(my_svg.x_size(), 200U);
  my_svg.size(500U, 100U); // update x
  BOOST_CHECK_EQUAL(my_svg.x_size(), 500U); // & check
  my_svg.size(500U, 300U); // update y
  BOOST_CHECK_EQUAL(my_svg.y_size(), 300U); // & check
  BOOST_CHECK_EQUAL(my_svg.x_size(), 500U);

  BOOST_CHECK_EQUAL(my_svg.document_title(), ""); // Default no document title
  my_svg.document_title("Document Title");
  BOOST_CHECK_EQUAL(my_svg.document_title(), "Document Title");

  BOOST_CHECK_EQUAL(my_svg.description(), ""); // Default no document description.
  my_svg.description("Document Description");
  BOOST_CHECK_EQUAL(my_svg.description(), "Document Description");

  BOOST_CHECK_EQUAL(my_svg.copyright_holder(), ""); // Default no copyright_holder.
  my_svg.copyright_holder("Jake Voytko");
  BOOST_CHECK_EQUAL(my_svg.copyright_holder(), "Jake Voytko");

  BOOST_CHECK_EQUAL(my_svg.copyright_date(), ""); // Default no copyright_date.
  my_svg.copyright_date("2007");
  BOOST_CHECK_EQUAL(my_svg.copyright_date(), "2007");

  ostringstream ossvg;
  my_svg.write(ossvg);

  cout << ossvg.str().c_str() << endl;
  // Might use Boost.Test to compare string with 'expected' output
  // but it has lots of " so can't be used as a C string,
  // so would need to compare with a file of 'expected' svg stuff.

  // Test rect_element

  rect_element my_rect(1, 2, 3, 4);
  BOOST_CHECK_EQUAL(my_rect.x(), 1);
  BOOST_CHECK_EQUAL(my_rect.y(), 2);
  BOOST_CHECK_EQUAL(my_rect.width(), 3);
  BOOST_CHECK_EQUAL(my_rect.height(), 4);
  BOOST_CHECK(my_rect.height() == 4);

  rect_element my_rect2(1, 2, 3, 4);
  cout << "rect_element my_rect2(1, 2, 3, 4);  = " << my_rect2 << endl;
  BOOST_CHECK_EQUAL(my_rect, my_rect2);
  rect_element my_rect3(9, 2, 3, 4);
  BOOST_CHECK_NE(my_rect, my_rect3);

  poly_path_point ppp(1, 2);
  cout << "poly_path_point ppp(1, 2) " << ppp << endl;
  oss.str(""); // erase
  oss << ppp;
  BOOST_CHECK_EQUAL(oss.str().c_str(), "(1, 2)");

  polygon_element my_polygon(0, 0);
  my_polygon.P(50, 50);
  my_polygon.P(0, 100);
  my_polygon.P(100, 100);
  // BOOST_CHECK_EQUAL(my_polygon, ); what tests are possible?
  // my_polygon.operator<<(cout);
  cout << "my_polygon " << my_polygon << endl;
  oss.str(""); // erase
  oss << my_polygon;
  BOOST_CHECK_EQUAL(oss.str().c_str(), "(0, 0)(50, 50)(0, 100)(100, 100)");

  polygon_element my_polyline(1, 2);
  my_polyline.P(3, 4);
  my_polyline.P(5, 6);
  cout << my_polyline << endl; // (1, 2)(3, 4)(5, 6)
  oss.str(""); // erase
  oss << my_polyline;
  BOOST_CHECK_EQUAL(oss.str().c_str(), "(1, 2)(3, 4)(5, 6)");

} // BOOST_AUTO_TEST_CASE(test_styles)

/*

Output:

------ Build started: Project: test_svg, Configuration: Debug Win32 ------
Compiling...
test_svg.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\test_svg.exe"
Running 1 test case...
text_style(12, Arial, italic, bold)
<?xml version="1.0" encoding="UTF-8" standalone="no"?><!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/graphics/svg/1.1/dtd/svg11.dtd">
<svg width="500" height ="300" version="1.1" xmlns="http://www.w3.org/2000/svg"
 xmlns:xlink="http://www.w3.org/1999/xlink"
 xmlns:ev="http://www.w3.org/2001/xml-events">
<!-- SVG plot written using Boost.Plot program (Creator Jacob Voytko) -->
<!-- Use, modification and distribution of Boost.Plot subject to the -->
<!-- Boost Software License, Version 1.0.-->
<!-- (See accompanying file LICENSE_1_0.txt -->
<!-- or copy at http://www.boost.org/LICENSE_1_0.txt) -->
<!-- SVG Plot Copyright Jake Voytko 2007 -->
<meta name="copyright" content="Jake Voytko" />
<meta name="date" content="2007" />
<!-- Document Description -->
<desc>Document Description</desc>
<title>Document Title</title>
</svg>
rect_element my_rect2(1, 2, 3, 4);  = rect(1, 2, 3, 4)
poly_path_point ppp(1, 2) (1, 2)
my_polygon (0, 0)(50, 50)(0, 100)(100, 100)
(1, 2)(3, 4)(5, 6)
*** No errors detected
Build Time 0:04
Build log was saved at "file://j:\Cpp\SVG\test_svg\Debug\BuildLog.htm"
test_svg - 0 error(s), 0 warning(s)
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========



*/
