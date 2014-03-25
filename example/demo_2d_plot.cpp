/*! \file demo_2d_plot.cpp
    \brief Demonstration of some 2D plot features.
    \details Uses some simple math functions to generate curves.
    The detailed output shows the plot settings for each plot.

    \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef _MSC_VER
#  pragma warning (disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#  pragma warning (disable : 4180) // qualifier applied to function type has no meaning; ignored
#  pragma warning (disable : 4503) // decorated name length exceeded, name was truncated
#  pragma warning (disable : 4512) // assignment operator could not be generated
#  pragma warning (disable : 4100) // unreferenced formal parameter
#  pragma warning (disable : 4224) // formal parameter 'hash' was previously defined as a type
#endif

#include <boost/svg_plot/svg_2d_plot.hpp>

#include <boost/svg_plot/show_2d_settings.hpp>
// for show_2d_plot_settings - Only needed for showing which settings in use.

//#include ".\axis_scaling.hpp"

#include <boost/array.hpp>
  using boost::array;

#include <map>
  using std::map;
#include <vector>
   using std::vector;

#include <cmath>
   using std::sqrt;
   using std::pair;
#include <iostream>
   using std::cout;
   using std::endl;
   using std::scientific;
   using std::hex;
   using std::ios;
   using std::boolalpha;

using namespace boost::svg;
// Convenient to use namespace because there are so many functions,
// and very many named colors in this namespace.

  // Use map with both x and y double.
  map<double, double> data1;
  map<double, double> data2;
  map<double, double> data3;

 // Several simple math functions to demonstrate:
double f(double x)
{
  return sqrt(x);
}

double g(double x)
{
  return -2 + x * x;
}

double h(double x)
{
  return -1 + 2 * x;
}

double sq(double x)
{
  return x * x;
}

double recip(double x)
{
  return 1. / x;
}

void plot(const std::string& title, const std::string& file,
    const std::string& x_label = std::string(), // default is ""
    double xmin = 0., double xmax = 0.,
    const std::string& y_label = std::string(),
    double ymin = 0., double ymax = 0.,
    int x_major_label = -1, int y_major_label = -1, // None
    int x_rotation = horizontal, int y_rotation = upward) //
{
  using namespace boost::svg;

  svg_2d_plot my_plot;
  // Size/scale settings.
  my_plot.size(700, 500);
  //my_plot.legend_top_left(175, 100);
  my_plot.title(title)
          .title_font_size(20)
          .x_label(x_label)
          .y_label(y_label)
          ;
  // Legend settings.
  my_plot.legend_on(true)
          //.legend_place(outside_bottom)
          //.legend_place(outside_right)
          .title_on(true)
          .plot_window_on(true)
          .plot_border_color(magenta)
          .x_label_on(true)
          .y_label_on(true)
          .y_major_labels_side(left_side)
          .x_major_labels_side(left_side)
          //.legend_title("Function")
          .legend_title("Unicode &#x3A9;&#x3A6;") // Omega Phi
          .legend_title_font_size(16);

  // Color settings chosen for visibility rather than taste!

  // Color settings.
  my_plot.background_color(ghostwhite) // image
          .legend_background_color(lightyellow)
          .legend_border_color(yellow)
          .plot_background_color(svg_color(white))
          .plot_border_color(svg_color(green))
          .plot_border_width(1)
          .title_color(red)
          ;
  // Options for x and/or y num_minor_ticks.
  // .y_num_minor_ticks(4)  // 0 major, 2,4,6,7,8 minor, 10 major ...
  // .y_num_minor_ticks(1) // 0 major, 5, minor, 10 major ...
  // .y_num_minor_ticks(2) // show half points.
  // .y_num_minor_ticks(3) // show half and quarter points.
  // .y_num_minor_ticks(4) // 0 major, 2,4,6,7,8 minor, 10 major ...
  // .y_num_minor_ticks(9) // 0 major, 1,2,3,4,5,6,7,8,9 minor, 10 major ...

  // X axis settings.
  my_plot.x_range(xmin, xmax)
          .x_major_interval(2.)
          .x_major_tick_length(10) // pixels
          .x_major_tick_width(2) // pixels
          .x_minor_tick_length(5) // pixels
          .x_minor_tick_width(1) // pixels
          .x_num_minor_ticks(2) // plus 1 major = 5 ticks per major step.
          ;

  // Y-axis settings.
 my_plot
    .y_range(ymin, ymax)
    .y_major_interval(2.)
    .y_num_minor_ticks(2) // plus 1 major = 5 ticks per major step.
    .y_major_tick_length(10) // pixels
    .y_major_tick_width(2) // pixels
    .y_minor_tick_length(5) // pixels
    .y_minor_tick_width(1) // pixels
    ;

  // Very pale blue grid - like old fashioned graph paper.
  my_plot.x_major_grid_color(svg_color(200, 220, 255))
         .x_minor_grid_color(svg_color(240, 240, 255))
         .y_major_grid_color(svg_color(200, 220, 255))
         .y_minor_grid_color(svg_color(240, 240, 255))
         .y_major_grid_width(2)
         .y_minor_grid_width(1)
     // But nothing shows - until you make .major_grid_on(true)!
         .x_major_grid_on(true)
         .x_minor_grid_on(true)
         .y_major_grid_on(true)
         .y_minor_grid_on(true);

  my_plot.x_ticks_down_on(true); // X-axis.
  my_plot.y_ticks_left_on(true); // Y-axis.

  // Where the ticks (and labels if any) go, left/right, on axis, or bottom/top.
  // Default x_ticks_on_window_or_axis == -1 left or bottom, +1 right to top, 0 = on axis.
  // my_plot.x_ticks_on_window_or_axis(+1); //
  // my_plot.y_ticks_on_window_or_axis(+1);
  // my_plot.x_ticks_on_window_or_axis(-1); // right or top.
  // my_plot.y_ticks_on_window_or_axis(-1);
  // x_ticks_on_window_or_axis == 0 : on axes, if possible.
  my_plot.x_ticks_on_window_or_axis(0); // ticks on axes.
  my_plot.y_ticks_on_window_or_axis(0); // ticks on axes.

  // Which side of axis line or plot window the value labels go.
  //my_plot.x_major_labels_side(0); // NO value labels.
  //my_plot.y_major_labels_side(0); // NO value labels.

  //my_plot.x_major_labels_side(top_side); // Top side value labels.
  //my_plot.x_major_labels_side(bottom_side); // Bottom side value labels (default).
  //my_plot.y_major_labels_side(no_labels); // NO value labels.
  //my_plot.y_major_labels_side(right_side); // Right side of axis value labels.
  //my_plot.y_major_labels_side(left_side); // Left side value labels (default).

  //my_plot.x_major_label_rotation(rotate_style(x_rotation));// Use this plot function's defaults.
  //my_plot.x_major_label_rotation(horizontal); // svg_2d_plot default.
  //my_plot.x_major_label_rotation(upward);
  //my_plot.x_major_label_rotation(downward);
  //my_plot.x_major_label_rotation(uphill);
  //my_plot.x_major_label_rotation(downhill);

  //my_plot.y_major_label_rotation(rotate_style(y_rotation)); // Use this plot function's defaults.
  //my_plot.y_major_label_rotation(upward); // svg_2d_plot default.
  //my_plot.y_major_label_rotation(downhill); // sloping.
  //my_plot.y_major_label_rotation(uphill);
  //my_plot.y_major_label_rotation(horizontal);.
  //my_plot.y_major_label_rotation(downward);

  my_plot.y_value_ioflags(ios::dec | ios::fixed).y_value_precision(1);
  my_plot.x_value_ioflags(ios::dec | ios::scientific).x_value_precision(2);
  //  my_plot.x_value_ioflags(ios::dec).x_value_precision(2);

  my_plot.plot(data1, "Sqrt(x)").fill_color(red);
  my_plot.plot(data2, "-2 + x^2").fill_color(orange).size(5).bezier_on(true);
  my_plot.plot(data3, "-1 + 2x").fill_color(yellow).line_color(blue).line_width(3).shape(square);

  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << file << endl;
  my_plot.write(file);
  show_2d_plot_settings(my_plot);

  } // plot

int main()
{
  try
  {
  // boost::array or anything in boost such that pair_type has
  // std and boost as associated namespaces.
  //typedef ::std::pair< ::boost::array<int, 1>, int> pair_type;
  //pair_type p1;
  //pair_type p2;

  //p1.first[0] = p1.second = 0;
  //p2.first[0] = p2.second = 1;

  //array<pair<double, double>, 10> ddpairs;
  //ddpairs[0].first = 1.; ddpairs[0].second = 2.;

  for(double i = -10; i <= 10.; i += 1.)
  {
    data1[i] = f(i);
    data2[i] = g(i);
    data3[i] = h(i);
  }

   // Demonstrate/test plots with various range of x and y, some *not* including zero.

   plot("Plot of Mathematical Functions XYPM", "./demo_2d_plot_XYPM.svg", "X-axis", -10., +10., "Y-axis", -10., +10.); // Both X & Y include zero.
   plot("Plot of Mathematical Functions XP", "./demo_2d_plot_XP.svg", "X-axis", +1., +10., "Y-axis", -10., 10.); // X all > 0
   plot("Plot of Mathematical Functions XN", "./demo_2d_plot_XN.svg", "X-axis", -10., -1., "Y-axis", -10., 10.); // x all < 0
   plot("Plot of Mathematical Functions YP", "./demo_2d_plot_YP.svg", "X-axis", -1., +10., "Y-axis", +1., +10.); // Y all > 0
   plot("Plot of Mathematical Functions YN", "./demo_2d_plot_YN.svg", "X-axis", -1., +10., "Y-axis", -10., -1.); // y all < 0
   plot("Plot of Mathematical Functions XYP", "./demo_2d_plot_XYP.svg", "X-axis", +1., +10., "Y-axis", +1., +10.); // x & y all > 0
   plot("Plot of Mathematical Functions XYN", "./demo_2d_plot_XYN.svg", "X-axis", -10., -1., "Y-axis", -10., -1.); // x & y all < 0
   plot("Title with Unicode <sup>-&#945; </sup> &#x3A9; &#x3A6; &#x221A; &#x221E; &#x3B6; &#x00B1; &#x2080; &#x2081; &#x2082;&#x2083;", "./demo_2d_plot_XYPM.svg", "X-axis &#x00B1;&#x3A9;", -10., +10., "Y-axis &#x221E;&#x221A;", -10., +10.); // Both X & Y include zero.
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


Autorun "j:\Cpp\SVG\Debug\demo_2d_plot.exe"
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_XYPM.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 46, 524.1
plot_window_x_left 46
plot_window_x_right 524.1
plot_window_y 58, 454
plot_window_y_top 58
plot_window_y_bottom 454
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max 10
x_min -10
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() -10, 10
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis none
y_axis_position y_axis_position intersects X axis (X range includes zero)
x_axis_position x_axis_position intersects Y axis (Y range includes zero)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() -10, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis none
y_max 10
y_min -10
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_XP.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 88, 524.1
plot_window_x_left 88
plot_window_x_right 524.1
plot_window_y 58, 454
plot_window_y_top 58
plot_window_y_bottom 454
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max 10
x_min 1
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() 1, 10
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis none
y_axis_position y_axis_position left (all X values < 0)
x_axis_position x_axis_position intersects Y axis (Y range includes zero)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() -10, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis left
y_max 10
y_min -10
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_XN.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 46, 524.1
plot_window_x_left 46
plot_window_x_right 524.1
plot_window_y 58, 454
plot_window_y_top 58
plot_window_y_bottom 454
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max -1
x_min -10
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() -10, -1
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis none
y_axis_position y_axis_position right (all X values > 0)
x_axis_position x_axis_position intersects Y axis (Y range includes zero)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() -10, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis right
y_max 10
y_min -10
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_YP.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 46, 524.1
plot_window_x_left 46
plot_window_x_right 524.1
plot_window_y 58, 436
plot_window_y_top 58
plot_window_y_bottom 436
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max 10
x_min -1
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() -1, 10
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis bottom
y_axis_position y_axis_position intersects X axis (X range includes zero)
x_axis_position x_axis_position bottom (all Y values > 0)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() 1, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis none
y_max 10
y_min 1
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_YN.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 46, 524.1
plot_window_x_left 46
plot_window_x_right 524.1
plot_window_y 58, 454
plot_window_y_top 58
plot_window_y_bottom 454
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max 10
x_min -1
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() -1, 10
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis top
y_axis_position y_axis_position intersects X axis (X range includes zero)
x_axis_position x_axis_position top (all Y values < 0)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() -10, -1
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis none
y_max -1
y_min -10
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_XYP.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 79.6, 524.1
plot_window_x_left 79.6
plot_window_x_right 524.1
plot_window_y 58, 436
plot_window_y_top 58
plot_window_y_bottom 436
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max 10
x_min 1
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() 1, 10
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis bottom
y_axis_position y_axis_position left (all X values < 0)
x_axis_position x_axis_position bottom (all Y values > 0)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() 1, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis left
y_max 10
y_min 1
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_XYN.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 46, 524.1
plot_window_x_left 46
plot_window_x_right 524.1
plot_window_y 58, 454
plot_window_y_top 58
plot_window_y_bottom 454
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max -1
x_min -10
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() -10, -1
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis top
y_axis_position y_axis_position right (all X values > 0)
x_axis_position x_axis_position top (all Y values < 0)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() -10, -1
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis right
y_max -1
y_min -10
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ./demo_2d_plot_XYPM.svg
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(248,248,255)
image_border_margin() 3
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
x_size 700
image_y_size 500
image_size 700, 500
image_filename
legend_on true
legend_place 2
legend_top_left 538.1, 58, legend_bottom_right 692, 202
legend_background_color RGB(255,255,224)
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 16
legend_font_weight
legend_width 153.9
legend_lines true
limit points stroke color RGB(128,128,128)
limit points fill color blank
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(0,128,0)
plot_border_width 1
plot_window_on true
plot_window_x 46, 524.1
plot_window_x_left 46
plot_window_x_right 524.1
plot_window_y 58, 454
plot_window_y_top 58
plot_window_y_bottom 454
title_on true
title ""
title_color RGB(255,0,0)
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 20
title_font_stretch
title_font_style
title_font_weight
x_values_on false
x_values_font_size 12
x_values_font_family
x_values_precision 3
x_values_ioflags 200 IOS format flags (0x1200) dec, scientific.
y_values_precision 3
y_values_font_size() 1
y_values_ioflags 200 IOS format flags (0x200) dec.
y_values_color blank
y_values_font_family()
y_values_font_size() 12
x_max 10
x_min -10
x_autoscale false
y_autoscale false
xy_autoscale false
x_autoscale_check_limits true
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color blank
x_values_color blank
x_axis_width 1
x_label_on true
x_label X-axis &#x00B1;&#x3A9;
x_label_color blank
x_label_font_family Verdana
x_label_font_size 14
x_label_units
x_label_units_on false
x_major_labels_side left
x_major_label_rotation 0
x_major_grid_color RGB(200,220,255)
x_major_grid_on true
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 10
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 5
x_minor_tick_width 1
x_minor_grid_on true
x_minor_grid_color RGB(240,240,255)
x_minor_grid_width 0.5
x_range() -10, 10
x_num_minor_ticks 2
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis none
y_axis_position y_axis_position intersects X axis (X range includes zero)
x_axis_position x_axis_position intersects Y axis (Y range includes zero)
x_plusminus_on false
x_plusminus_color RGB(0,0,255)
x_df_on false
x_df_color RGB(0,0,255)
x_prefix
x_separator
x_suffix
xy_values_on false
y_label_on true
y_label_axis Y-axis &#x221E;&#x221A;
y_axis_color RGB(0,0,0)
y_axis_label_color blank
y_axis_on true
axes_on true
y_axis_value_color RGB(0,0,0)
y_axis_width 1
y_label Y-axis &#x221E;&#x221A;
y_label_color blank
y_label_font_family Verdana
y_label_font_size 14
y_label_on true
y_label_units
y_label_units_on false
y_label_width 0
y_major_grid_on true
y_major_grid_color RGB(200,220,255)
y_major_grid_width 2
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  10
y_major_tick_width  2
y_minor_grid_on true
y_minor_grid_color  RGB(240,240,255)
y_minor_grid_width 1
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 5
y_minor_tick_width 1
y_range() -10, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis none
y_max 10
y_min -10
y_values_on false
y_plusminus_on false
y_plusminus_color RGB(0,0,255)
y_df_on false
y_df_color RGB(0,0,255)
y_prefix
y_separator
y_suffix
data lines width 2
Build Time 0:05

*/

/*

Parked stuff:

  // TODO autoscale
  // int scale_axis(double min_value, double max_value, // input
  //             double* axis_min_value,  double* axis_max_value, double* axis_tick_increment, // updated.
  //             bool origin, double tight);

  // Use map with x int and y double.
  map<int, double> datai1;
  map<int, double> datai2;


  other functions

    datai1[i] = sq(i);
    datai2[i] = recip(i);


  unsigned int legend_title_font_size = my_plot.legend_title_font_size();
  cout << "Legend title font size is " << legend_title_font_size << endl;
   unsigned int title_font_size = my_plot.title_font_size();
  cout << "Title font size is " << title_font_size << endl;
  cout << "Title color is " << my_plot.title_color() << endl;

  //text_style label_style(40, "sans"); // Or use all defaults.
  //cout << label_style.font_size() << endl;
  //cout << label_style.font_family() << endl;
  //label_style.font_weight("bold");
 // cout << label_style.font_weight() << endl;
  //label_style.font_size(30);
//  cout << label_style.font_size() << endl;

 // text_style& x_label_style(); // returns ref to the style.

//Then the user could write:
//
//x_label_style().font_size(20);
//x_label_style().font_size(20).font_family("Arial");
//
//x_label_style().font_family("Arial");
//
//which is fairly readable and logical, and doesn't clutter the plot classes
//with so many methods or data members for that matter.  One could even create
//a text_style object and reuse it for multiple graphs or multiple items
//within one graph:
//
//my_plot1.x_label_style = my_plot1.y_label_style = myplot2.x_label_style =
//mystyle_object; // etc

  */
