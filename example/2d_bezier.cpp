/*! \file 2d_bezier.cpp
  \brief Simple 2D plot using bezier curves.
  \author Jacob Voytko and Paul A. Bristow
  \date 2007
*/

// Copyright (C) Jacob Voytko 2007
// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Example of using SVG plot bezier option showing smoothing effect when using a limited number of data points.

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <map>
 // using std::map;
#include <cmath>
  // using std::sin;
  // using std::cos;

// sin and cos functions.
double f(double x)
{
  return std::sin(x);
}

double g(double x)
{
  return std::cos(x);
}

int main()
{
  std::map<double, double> sin_data;
  std::map<double, double> cos_data;

  const double interval = 3.14159265 / 8.; // pi/8 - between values, too few for a smooth plot.

  for(double i = 0; i <= 10.; i += interval)
  {
    sin_data[i] = f(i);
    cos_data[i] = g(i);
  }

  svg_2d_plot my_plot;

  // Size/scale settings.
  my_plot.size(700, 500)
    .x_range(-.5, 10.5)
    .y_range(-1.1, 1.1);  // Ensure that 1.0 and -1.0 are visible.

  // Text settings.
  my_plot.title("Plot of sin and cos functions")
    .title_font_size(29)
    .x_label("X Axis Units")
    .y_major_labels_side(-1)
    .y_major_grid_on(true);

  // Commands.
  my_plot.legend_on(true) // To show a legend box.
    .plot_window_on(true)
    .x_label_on(true);

  // Color settings.
  my_plot.background_color(svg_color(67, 111, 69))  // Color specified using RGB values.
    .legend_background_color(svg_color(207, 202,167))
    .legend_border_color(svg_color(102, 102, 84))
    .plot_background_color(svg_color(136, 188, 126))
    .title_color(white) // Color specified using SVG named colors.
    .y_major_grid_color(grey);

  // X axis settings.
  my_plot.x_major_interval(2)
    .x_major_tick_length(14)
    .x_major_tick_width(1)
    .x_minor_tick_length(7)
    .x_minor_tick_width(1)
    .x_num_minor_ticks(3)

  // Y axis settings.
    .y_major_interval(25)
    .y_num_minor_ticks(5);

  // legend settings.
  my_plot.legend_title_font_size(15);

  my_plot.plot(sin_data, "Sin(x)").line_on(true)
    .shape(circlet).size(5).fill_color(yellow);

  my_plot.plot(cos_data, "Cos(x)")
    .line_color(blue)
    .bezier_on(true)
    .shape(square).size(5).fill_color(red);

  // Note the slightly smoother curve for the cosine curve using bezier_on, compared to the line_on option.

  my_plot.write("./2d_bezier.svg");

  return 0;
} // int main()


