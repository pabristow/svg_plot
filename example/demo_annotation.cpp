/*! \file demo_annotation.cpp
    \brief Demonstration of 2D 'note' annotation.
    \details Adding a text annotation to a plot, changing color, font and orientation.
    \author Paul A. Bristow
    \date Mar 2009
*/

// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

#ifdef _MSC_VER
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4512) // assignment operator could not be generated
#  pragma warning (disable : 4100) // unreferenced formal parameter
#endif

//[demo_2d_annotation_1

/*`First we need some includes to use Boost.Plot and C++ Standard Library:
*/#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <boost/svg_plot/show_2d_settings.hpp>
// using boost::svg::show_2d_plot_settings - Only needed for showing which settings in use.

#include <map>
  using std::map;
  using std::multimap;

#include <iostream>
   using std::cout;
   using std::endl;
   using std::scientific;
   using std::hex;
   using std::ios;
   using std::boolalpha;

int main()
{
  try
  {
//[demo_2d_annotation_2

/*`
This shows how to add notes to a plot, for example to identify a particular area or point.
*/

  map<double, double> my_data;

  my_data[1.1] = 3.2;
  my_data[4.3] = 3.1;
  my_data[0.25] = 1.4;

  
  /*`
  First construct, size and draw a simple plot ready to add some sample annotation.
  */
  svg_2d_plot my_plot;
  my_plot.size(400, 300);
  my_plot.plot(my_data, "my_data").fill_color(red);
  /*`Now add a string note at the SVG coordinates X = 100 and Y =200.
  */
  my_plot.draw_note(150, 200, "My 1st Note");
  /*`
  Note that for SVG coordinates, Y increases *down* the page, so Y = 0 is the top and Y = 300 is the bottom.
  Defaults are provided for size, text style = no_style, center alignment and rotation horizontal.
  */
  my_plot.draw_note(40, 10, "top left (0, 0)");
  my_plot.draw_note(my_plot.image_x_size()-10, my_plot.image_y_size()- 10, "bottom right(380, 280)");

/*`Using center_align is strongly recommended because it will ensure that note will center correctly
 (even if note string is made much longer because it contains Unicode,
 for example Greek, taking about 6 characters per symbol)
because the render engine does the centering.
You can use either plain char space or Unicode space \&#x00a0;
*/
  my_plot.draw_note(150, 100, "Greek Unicode &#x3A9;&#x3A6;&#x221A;&#x00a0;&#x221E;&#x3B6; &#x00B1;");
/*`You can change the font, but defining a new text_style, for example: */
  
  text_style my_note_style(16, "verdana", "italic", "bold"); // Used for note below.
  
/*`and you can change the alignment and rotation using enums align_style and rotate_style.
*/
  my_plot.draw_note(350, 70, "My 2nd Note", slopeup, right_align, black, my_note_style);

/*`To change the color to red (and text style,  alignment and rotation too, just for fun)
*/
  text_style my_red_note_style(16, "arial", "italic", "bold");
  my_plot.draw_note(350, 170, "Red Note",  upsidedown,left_align, red, my_red_note_style);
  my_plot.draw_note(300, 210, "Blue Note", steepup, center_align, blue);

  my_plot.write("./demo_2d_annotation");

  //show_2d_plot_settings(my_plot);

  //] [/demo_2d_annotation_2]
  
  }
  catch (const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
  return 0;
} // int main()

/*

Output :

*/
/*

*/
