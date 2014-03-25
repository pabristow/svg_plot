/*! \file 1d_full_layout.cpp
    \brief Example of 1 D plot of 3 different STL container types using several layout features.
    \details creates file 1d_complex.svg
    \author Jacob Voytko
*/

// Copyright Jacob Voytko 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_1d_plot.hpp>
using namespace boost::svg;
#include <boost/array.hpp>
  using boost::array;
#include <vector>
  using std::vector;
#include <deque>
  using std::deque;
#include <cmath>
  using std::sqrt;

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
  vector<double> data1;
  deque<double> data2;
  boost::array<double, 10> data3;

  int j=0;
  for(double i=0; i<9.5; i+=1.)
  {
    data1.push_back(f(i));
    data2.push_front(g(i));
    data3[j++] = h(i);
  }

  svg_1d_plot my_plot;

  // Size/scale settings.
  my_plot.size(500, 350)
     .x_range(-3, 10);

  // Text settings.
  my_plot.title("Animal Lives")
    .title_font_size(29)
    .x_label("Time in Months");

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

  // Axis settings.
  my_plot.x_major_interval(2)
         .x_major_tick_length(14)
         .x_major_tick_width(1)
         .x_minor_tick_length(7)
         .x_minor_tick_width(1)
         .x_num_minor_ticks(3);

  // Legend settings.
  my_plot.legend_title("Animals")
    .legend_title_font_size(15)
    ;



  my_plot.plot(data1, "Lions").stroke_color(blue);
  my_plot.plot(data2, "Tigers").stroke_color(orange);
  my_plot.plot(data3, "Bears").stroke_color(red);

  my_plot.write("./1d_full_layout.svg");

  return 0;
} // int main()

// 1d_full_layout.cpp
