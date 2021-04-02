/*! \file svg_colors.cpp

  \brief Example to display all the SVG 1.1 named colors as rectangles.

  \details
    Deals with colors that have special names. The reason that the
    underscore separator convention does not match the normal Boost format
    is that these names that are specified by the SVG standard.
    http://www.w3.org/TR/SVG/types.html#ColorKeywords

  \author Paul A. Bristow
*/

// Copyright Paul A. Bristow 2007
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Need completion to all colors, not just one!

#include <boost/svg_plot/svg.hpp>
// using boost::svg::rect;

#include <boost/svg_plot/svg_1d_plot.hpp>
using namespace boost::svg;

#include <boost/svg_plot/svg_color.hpp>

#include <boost/svg_plot/svg_fwd.hpp>

 using namespace boost::svg;
  // Is most convenient because otherwise all the colors used must be specified thus
  using boost::svg::blue;
  using boost::svg::white;
  using boost::svg::red;
  using boost::svg::yellow;
  // ... which may get tedious!

#include <iostream>
  //using std::cout;
  //using std::endl;
  //using std::boolalpha;

int main()
{


  svg my_svg;
  my_svg.x_size(300);
  my_svg.y_size(200);

  svg& rect(double x1, double y1, double width, double height);

  my_svg.add_g_element(); // Add first (zeroth) new element,
  g_element& g0 = my_svg.gs(0); // so index is zero.
  std::cout << "my_svg.document_size() = number of g_elements = " << my_svg.document_size() << std::endl;
  g0.id("group element 0");
  std::cout << "group element " << g0.id() << std::endl;

  g0.push_back(new rect_element(0, 0, my_svg.x_size(),  my_svg.y_size() ) ); // border to all image.

  // Create a small test rectangle.
  g0.style().fill_color(azure); 
  g0.style().fill_on(true); 
  g0.style().stroke_color(blue);
  g0.push_back(new rect_element(20, 20, 10, 10) ); // border to rectangle.
  //g0.text(50, 50, "my color", no_text_style, right_align); // SVG name of color to the right.

  my_svg.add_g_element(); 
  g_element& g1 = my_svg.gs(1); // so index is now one.

  g1.id("group element 1");
  std::cout << "my_svg.document_size() = number of g_elements = " << my_svg.document_size() << std::endl;
  g1.id("group element 0");
  std::cout << "group element " << g0.id() << std::endl;

  g1.style().fill_color(black); 
  g1.style().fill_on(false); 
  g1.style().stroke_color(red);
  g1.style().fill_color(color_array[90]); 


  text_style big_font;
  big_font.font_size(30);
  //g1.push_back(new text_element(100, 50, "my color", big_font)); // SVG name of color to the default alignment center.
  g1.push_back(new text_element(100, 50, "my color", big_font, align_style::center_align)); // SVG name of color to the right.

  using boost::svg::color_array;

  std::cout << "Standard colors (" << color_array.size() << ")" << std::endl;
  for (int i = 0; i < color_array.size(); i++)
  {
    g1.style().fill_color(black);
    g1.style().fill_on(false);
    g1.style().stroke_color(red);
    g1.style().stroke_on(true);
    // Need to move to location and the style includihn color.
    // Use tspan?

    g1.push_back(new text_element(100, 50, "my color", big_font, align_style::right_align)); // SVG name of color to the right.
  }


  std::cout << "my_svg.document_size() = number of g_elements = " << my_svg.document_size() << std::endl;


  // The need to repeat down the page for all the colors.  TODO
  // Need a 'reverse lookup' of the enum colors as "azure" :-((

  my_svg.write("./svg_colors.svg");
  return 0;
} // int main()


