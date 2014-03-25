/*! \file demo_2d_simple.cpp
    \brief Demonstration of some simple 2D plot features.
    \details Uses some simple math functions to generate curves.
    This demonstrates plotting some simple math functions with most of the 2-D defaults,
    just changing a few typical details.
    The detailed output shows the plot settings for each plot.
    See default_2d_plot.cpp for using \b all defaults.
    See also demo_2d_plot.cpp for use of some of the very many options.
    \date Feb 2009
    \author Jacob Voytko and Paul A. Bristow 
*/

// demo_2d_simple.cpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2008, 2012

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_2d_plot.hpp>
  using boost::svg::svg_2d_plot;

#include <boost/svg_plot/show_2d_settings.hpp>
// Only needed for showing which settings in use.

#include <boost/quan/unc_init.hpp>

#include <iostream>
  using std::cout;
  using std::endl;
  using std::boolalpha;
#include <map>
  using std::map;
#include <cmath>
  using std::sqrt;
#include <string>
  using std::string;

// using namespace boost::svg;
// may be *very convenient* if using any SVG named colors,
// to avoid writing
  using boost::svg::red;
  using boost::svg::yellow;
  using boost::svg::orange;
  using boost::svg::blue;
  // and other enum options used:
  using boost::svg::square;
  // for every color used.

double f(double x)
{
  return sqrt(x);
}

double g(double x)
{
  return -2 + x*x;
}

double h(double x)
{
  return -1 + 2 * x;
}

int main()
{

  try
  {
    // Some containers for (sorted) sample data.
    map<double, double> data1;
    map<double, double> data2;
    map<double, double> data3;

    for(double i = -5; i <= 10.; i += 1.)
    { // Several data points for each function.
      data1[i] = f(i);
      data2[i] = g(i);
      data3[i] = h(i);
      // List if desired:
      // cout << i << ' '<< data1[i] << ' ' << data2[i] << ' '<< data3[i] << endl;
    }

    setUncDefaults(std::cout);
    svg_2d_plot my_plot;
    // Uses most defaults, but scale settings are usually sensible.

    // Add the data series to the plot:
    my_plot.title("demo_2d_simple");
    cout << " my_plot.title() " << my_plot.title() << endl;
    my_plot.x_label("X-axis").y_label("Y-axis"); // Note chaining.

    std::string s = my_plot.title();

    my_plot.plot(data1, "Sqrt(x)").fill_color(red);
    my_plot.plot(data2, "-2 + x^2").fill_color(orange).size(5);
    my_plot.plot(data3, "-1 + 2x").fill_color(yellow).bezier_on(true).line_color(blue).shape(square);
    cout << " my_plot.title() " << my_plot.title() << endl;

    my_plot.write("./demo_2d_simple.svg");
    cout << " my_plot.title() " << my_plot.title() << endl;

    show_2d_plot_settings(my_plot);
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n  " << e.what() << std::endl;
  }
  return 0;
} // int main()


/*

Output:

Compiling...
demo_2d_simple.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_2d_simple.exe"
 my_plot.title() demo_2d_simple title
 my_plot.title() demo_2d_simple title
 my_plot.title()
axes_on true
background_border_width 2
background_border_color RGB(255,255,0)
background_color RGB(255,255,255)
image_border_margin() 10
image_border_width() 2
coord_precision 3
copyright_date
copyright_holder
description
document_title
image_x_size 500
image_y_size 400
legend_on false
legend_place 2
legend_top_left -1, -1, legend_bottom_right -1, -1
legend_background_color blank
legend_border_color RGB(255,255,0)
legend_color blank
legend_title
legend_title_font_size 14
legend_font_weight
legend_width 0
legend_lines true
license_on false
license_reproduction permits
license_distribution permits
license_attribution requires
license_commercialuse permits
plot_background_color RGB(255,255,255)
plot_border_color RGB(119,136,153)
plot_border_width 2
plot_window_on true
plot_window_x 70.2, 488
plot_window_x_left 70.2
plot_window_x_right 488
plot_window_y 57, 338
plot_window_y_top 57
plot_window_y_bottom 338.2
title_on true
title ""
title_color blank
title_font_alignment 2
title_font_decoration
title_font_family Verdana
title_font_rotation 0
title_font_size 18
title_font_stretch
title_font_style
title_font_weight
x_value_precision 3
x_value_ioflags 200 IOS format flags (0x200) dec.
y_value_precision 3
y_value_ioflags 200 IOS format flags (0x200) dec.
x_max 10
x_min -10
x_axis_on true
x_axis_color() RGB(0,0,0)
x_axis_label_color RGB(0,0,0)
x_axis_value_color RGB(0,0,0)
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
x_major_grid_on false
x_major_grid_width 1
x_major_interval 2
x_major_tick 2
x_major_tick_color RGB(0,0,0)
x_major_tick_length 5
x_major_tick_width 2
x_minor_interval 0
x_minor_tick_color RGB(0,0,0)
x_minor_tick_length 2
x_minor_tick_width 1
x_minor_grid_on false
x_minor_grid_color RGB(200,220,255)
x_minor_grid_width 0.5
x_range() -10, 10
x_num_minor_ticks 4
x_ticks_down_on true
x_ticks_up_on false
x_ticks_on_window_or_axis bottom
y_axis_position y_axis_position intersects X axis (X range includes zero)
x_axis_position x_axis_position intersects Y axis (Y range includes zero)
y_label_on true
y_label_axis Y-axis
y_axis_color RGB(0,0,0)
y_axis_label_color RGB(0,0,0)
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
y_major_grid_on false
y_major_grid_color RGB(200,220,255)
y_major_grid_width 1
y_major_interval 2
y_major_labels_side bottom
y_major_label_rotation 0
y_major_tick_color RGB(0,0,0)
y_major_tick_length  5
y_major_tick_width  2
y_minor_grid_on false
y_minor_grid_color  RGB(200,220,255)
y_minor_grid_width 0.5
y_minor_interval 0
y_minor_tick_color RGB(0,0,0)
y_minor_tick_length 2
y_minor_tick_width 1
y_range() -10, 10
y_num_minor_ticks
y_ticks_left_on true
y_ticks_right_on false
y_ticks_on_window_or_axis left
y_max 10
y_min -10
data lines width 2




*/
