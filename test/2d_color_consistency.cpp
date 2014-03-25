// 2d_color_consistency.cpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
#  pragma warning(disable : 4996) // Deprecated.
#  pragma warning(disable : 4267) //  '=' : conversion from 'size_t' to 'unsigned int' in spirit
#  pragma warning(disable : 4180) // warning C4180: qualifier applied to function type has no meaning; ignored
#  pragma warning(disable : 4172) // warning C4180: qualifier applied to function type has no meaning; ignored
#  pragma warning(disable : 4224) // nonstandard extension used : formal parameter 'arg' was previously defined as a type
// added to boost.test/detail supress_warnings
//#  pragma warning(disable : 4310) //  cast truncates constant value
//#  pragma warning(disable : 4512) //  assignment operator could not be generated
//#  pragma warning(disable : 4702) //  unreachable code
#endif

#define BOOST_TEST_MAIN

// Must come BEFORE this include.
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <iostream>
using std::cout;
using std::endl;

BOOST_AUTO_TEST_CASE( test1 )
{
  svg_2d_plot my_plot;

  BOOST_CHECK_EQUAL(my_plot.title(), ""); // Default now null string.
  my_plot.title("Plot of my data");
  cout << my_plot.title() << endl;
  BOOST_CHECK_EQUAL(my_plot.title(), "Plot of my data"); 

  my_plot.x_range(-1., 1.);
  cout << "x max " << my_plot.x_min() << endl;
  cout << "x min " << my_plot.x_max() << endl;

  // Tests that do NOT apply to 1D.

  BOOST_CHECK_EQUAL(my_plot.y_label(), ""); //  Check default label is null.
  my_plot.y_label("Y"); // 
   BOOST_CHECK_EQUAL(my_plot.y_label(), "Y"); // & check.

  BOOST_CHECK_EQUAL(my_plot.x_ticks_up_on(), false); // Check default. 
  my_plot.x_ticks_up_on(false); // Set
   BOOST_CHECK_EQUAL(my_plot.x_ticks_up_on(), false); // & check.
   BOOST_CHECK_EQUAL(my_plot.x_ticks_up_on(), false); // & check.

  BOOST_CHECK_EQUAL(my_plot.x_ticks_down_on(), true); // Check default. 
  my_plot.x_ticks_down_on(false); // Set
   BOOST_CHECK_EQUAL(my_plot.x_ticks_down_on(), false); // & check.
   BOOST_CHECK_EQUAL(my_plot.x_ticks_down_on(), false); // & check.

  BOOST_CHECK_EQUAL(my_plot.y_ticks_right_on(), false); // Check default. 
  my_plot.y_ticks_right_on(true); // Set
   BOOST_CHECK_EQUAL(my_plot.y_ticks_right_on(), true); // & check.
   BOOST_CHECK_EQUAL(my_plot.y_ticks_right_on(), true); // & check.

  BOOST_CHECK_EQUAL(my_plot.y_ticks_left_on(), true); // Check default. 
  my_plot.y_ticks_left_on(false); // Set
   BOOST_CHECK_EQUAL(my_plot.y_ticks_left_on(), false); // & check.
   BOOST_CHECK_EQUAL(my_plot.y_ticks_left_on(), false); // & check.

  my_plot
  .background_border_color(svg_color(0, 0, 1))
  .background_color(svg_color(0, 0, 2))
  .legend_background_color(svg_color(0, 0, 3))
  .legend_border_color(svg_color(0, 0, 4))
  .plot_background_color(svg_color(0, 0, 5))
  .title_color(svg_color(0, 0, 6))
  .x_axis_color(svg_color(0, 0, 7))
  .x_label_color(svg_color(0, 0, 8))
  .x_major_grid_color(svg_color(0, 0, 9))
  .x_major_tick_color(svg_color(0, 0, 10))
  .x_minor_grid_color(svg_color(0, 0, 11))
  .x_minor_tick_color(svg_color(0, 0, 12))
  .y_axis_color(svg_color(0, 0, 13))
  .y_label_color(svg_color(0, 0, 14))
  .y_major_grid_color(svg_color(0, 0, 15))
  .y_major_tick_color(svg_color(0, 0, 16))
  .y_minor_grid_color(svg_color(0, 0, 17))
  .y_minor_tick_color(svg_color(0, 0, 18));

  BOOST_CHECK(my_plot.background_border_color() == svg_color(0, 0, 1));
  BOOST_CHECK(my_plot.background_color() == svg_color(0, 0, 2));
  BOOST_CHECK(my_plot.legend_background_color() == svg_color(0, 0, 3));
  BOOST_CHECK(my_plot.legend_border_color() == svg_color(0, 0, 4));
  BOOST_CHECK(my_plot.plot_background_color() == svg_color(0, 0, 5));
  BOOST_CHECK(my_plot.title_color() == svg_color(0, 0, 6));
  BOOST_CHECK(my_plot.x_axis_color() == svg_color(0, 0, 7));
  BOOST_CHECK(my_plot.x_label_color() == svg_color(0, 0, 8));
  BOOST_CHECK(my_plot.x_major_grid_color() == svg_color(0, 0, 9));
  BOOST_CHECK(my_plot.x_major_tick_color() == svg_color(0, 0, 10));
  BOOST_CHECK(my_plot.x_minor_grid_color() == svg_color(0, 0, 11));
  BOOST_CHECK(my_plot.x_minor_tick_color() == svg_color(0, 0, 12));
  //BOOST_CHECK(my_plot.x_axis_color() == svg_color(0, 0, 7)); // Seems to work???
  cout << svg_color(red) << endl;
  cout << my_plot.y_axis_color() << endl;
  BOOST_CHECK(my_plot.y_axis_color() == svg_color(0, 0, 13));
  //cout << my_plot.y_axis_color() << endl;
  //BOOST_CHECK(my_plot.y_axis_color() == svg_color(0, 0, 13)); // fails " does not expect 0 arguments"
  BOOST_CHECK(my_plot.y_label_color()  ==  svg_color(0, 0, 14));
  BOOST_CHECK_EQUAL(my_plot.y_label_color(), svg_color(0, 0, 14));
  //BOOST_CHECK(my_plot.y_major_grid_color() == svg_color(0, 0, 15));
  //BOOST_CHECK(my_plot.y_major_tick_color() == svg_color(0, 0, 16));
  //BOOST_CHECK(my_plot.y_minor_grid_color() == svg_color(0, 0, 17));
  //BOOST_CHECK(my_plot.y_minor_tick_color() == svg_color(0, 0, 18));

  my_plot
  .y_minor_tick_color(svg_color(0, 0, 18))
  .y_minor_grid_color(svg_color(0, 0, 17))
  .y_major_tick_color(svg_color(0, 0, 16))
  .y_major_grid_color(svg_color(0, 0, 15))
//  .y_label_color(svg_color(0, 0, 14))
  .y_axis_color(svg_color(0, 0, 13))
  .x_minor_tick_color(svg_color(0, 0, 12))
  .x_minor_grid_color(svg_color(0, 0, 11))
  .x_major_tick_color(svg_color(0, 0, 10))
  .x_major_grid_color(svg_color(0, 0, 9))
  .x_label_color(svg_color(0, 0, 8))
  .x_axis_color(svg_color(0, 0, 7))
  .title_color(svg_color(0, 0, 6))
  .plot_background_color(svg_color(0, 0, 5))
  .legend_border_color(svg_color(0, 0, 4))
  .legend_background_color(svg_color(0, 0, 3))
  .background_color(svg_color(0, 0, 2))
  .background_border_color(svg_color(0, 0, 1));

//  BOOST_CHECK(my_plot.y_minor_tick_color() == svg_color(0, 0, 18));
//  BOOST_CHECK(my_plot.y_minor_grid_color() == svg_color(0, 0, 17));
//  BOOST_CHECK(my_plot.y_major_tick_color() == svg_color(0, 0, 16));
//  BOOST_CHECK(my_plot.y_major_grid_color() == svg_color(0, 0, 15));
////  BOOST_CHECK(my_plot.y_label_color() == svg_color(0, 0, 14));
//  BOOST_CHECK(my_plot.y_axis_color() == svg_color(0, 0, 13));
  BOOST_CHECK(my_plot.x_minor_tick_color() == svg_color(0, 0, 12));
  BOOST_CHECK(my_plot.x_minor_grid_color() == svg_color(0, 0, 11));
  BOOST_CHECK(my_plot.x_major_tick_color() == svg_color(0, 0, 10));
  BOOST_CHECK(my_plot.x_major_grid_color() == svg_color(0, 0, 9));
  BOOST_CHECK(my_plot.x_label_color() == svg_color(0, 0, 8));
  BOOST_CHECK(my_plot.x_axis_color() == svg_color(0, 0, 7));
  BOOST_CHECK(my_plot.title_color() == svg_color(0, 0, 6));
  BOOST_CHECK(my_plot.plot_background_color() == svg_color(0, 0, 5));
  BOOST_CHECK(my_plot.legend_border_color() == svg_color(0, 0, 4));
  BOOST_CHECK(my_plot.legend_background_color() == svg_color(0, 0, 3));
  BOOST_CHECK(my_plot.background_color() == svg_color(0, 0, 2));
  BOOST_CHECK(my_plot.background_border_color() == svg_color(0, 0, 1));

} // BOOST_AUTO_TEST_CASE( test1 )


/*

Autorun "j:\Cpp\SVG\debug\test_2d_color_persistence.exe
Running 1 test case...
*** No errors detected


*/
