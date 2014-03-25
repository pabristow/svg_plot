/*! \file 2d_full.cpp
  \brief 2D plot of functions from map of doubles showing use of more options.
  \author Jacob Voytko
  \date 2007
*/

// Copyright Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <map>
using std::map;
#include <cmath>
using ::sqrt;

// Some functions to plot.
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
  return -1 + 2*x;
}

int main()
{
  map<double, double> data1, data2, data3;

  for(double i=0; i<=10.; i+=1.)
  {
    data1[i] = f(i);
    data2[i] = g(i);
    data3[i] = h(i);
  }

  svg_2d_plot my_plot;

  // Size/scale settings.
  my_plot.size(700, 500)
         .x_range(-1, 10)
         .y_range(-5, 100);

  // Text settings.
  my_plot.title("Plot of Mathematical Functions")
         .title_font_size(29)
         .x_label("X Axis Units");

  // Commands.
  my_plot.legend_on(true)
         .plot_window_on(true)
         .x_label_on(true)
         .x_major_labels_side(true);

  // Color settings.
  my_plot.background_color(svg_color(67, 111, 69))
         .legend_background_color(svg_color(207, 202,167))
         .legend_border_color(svg_color(102, 102, 84))
         .plot_background_color(svg_color(136, 188, 126))
         .title_color(white);

  //X axis settings.
  my_plot.x_major_interval(2)
         .x_major_tick_length(14)
         .x_major_tick_width(1)
         .x_minor_tick_length(7)
         .x_minor_tick_width(1)
         .x_num_minor_ticks(3)

  //Y axis settings.
         .y_major_interval(10)
         .y_num_minor_ticks(3);

  //legend settings
  my_plot.legend_title_font_size(15);

  my_plot.plot(data1, "Sqrt(x)").stroke_color(red);
  my_plot.plot(data2, "-2 + x^2").stroke_color(orange);
  my_plot.plot(data3, "-1 + 2x").stroke_color(yellow).shape(square).size(5);

  my_plot.write("./2d_full.svg");

  return 0;
} // int main()
