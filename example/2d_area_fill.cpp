/*! \file 2d_area_fill.cpp
  \brief Demonstration of area fill below curves.
  \date 2007
  \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_2d_plot.hpp>
  using namespace boost::svg; // Needed to get svg colors and others.
  using boost::svg::svg_2d_plot;

#include <map> 
  using std::map;
#include <cmath>
  using std::sin;
  using std::cos;
  using std::tan;

double my_sin(double x)
{
  return 50. * sin(x);
}

double my_cos(double x)
{
  return 50. * cos(x);
}

double my_tan(double x)
{
  return 50. * tan(x);
}

int main()
{
  map<double, double> data_sin;
  map<double, double> data_cos;
  map<double, double> data_tan;
  
  double inter = 3.14159265 / 8.; // 16 points per cycle of 2 pi.

  for(double i = 0; i <= 10.; i+=inter)
  { // Just 10 data points for each function.
    data_sin[i] = my_sin(i);
    data_cos[i] = my_cos(i);
    data_tan[i] = my_tan(i);
  } // for

  svg_2d_plot my_plot;

  // Size/scale settings.
  my_plot.size(700, 500)
         .x_range(-1, 10)
         .y_range(-75, 75);

  // Text settings.
  my_plot.title("Plot of 50 * sin(x), cos(x) and tan(x)")
         .title_font_size(20)
         .x_label("x")
         .y_label("50 * f(x)")
         .x_major_labels_side(bottom)
         .y_major_labels_side(left)
         .x_major_grid_on(true)
         .y_major_grid_on(true)
         .x_major_grid_color(cyan)
         .y_major_grid_color(cyan)
         ;
  // Commands.
  my_plot.plot_window_on(true)
         .x_label_on(true)
         ;
  
  // Color settings.
  my_plot.background_color(whitesmoke)
         .legend_background_color(lightyellow)
         .legend_border_color(yellow)
         .plot_background_color(ghostwhite)
         .title_color(red)
         ;
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
  
  svg_2d_plot_series& s_sin = my_plot.plot(data_sin, "sin(x)").line_on(true).area_fill(red);
  std::cout << "s_sin.area_fill() " << s_sin.area_fill() << std::endl; // s_sin.area_fill() RGB(255,0,0)

    svg_2d_plot_series& s_cos = my_plot.plot(data_cos, "cos(x)").line_on(true).area_fill(blue).shape(square);
  std::cout << "s_cos.area_fill() " << s_cos.area_fill() << std::endl; // s_cos.area_fill() RGB(0,0,255)

  svg_2d_plot_series& s_tan = my_plot.plot(data_tan, "tan(x)").shape(cone).line_on(true).area_fill(blank);
  // Note that svg_color(blank) or svg_color(false) returns a non-color blank, so no fill.
  std::cout << "s_tan.area_fill() " << s_tan.area_fill() << std::endl; // s_tan.area_fill() blank

  std::cout << my_plot.title() << std::endl; // "Plot of 50 * sin(x), cos(x) and tan(x)"

  my_plot.write("./2d_area_fill_1.svg");


  my_plot.plot(data_sin, "sin(x)").line_on(true).area_fill(green).shape(square).fill_color(red);
  // Note how this overwrites the previously cos fill and tan line.
  // (It also needs a new title).

  my_plot.title("sin overwriting cos and tan");
  std::cout << my_plot.title() << std::endl; // "sin overwriting cos and tan"

  my_plot.write("./2d_area_fill_2.svg");

   return 0;
} // int main()

/*

Output:

2d_area_fill.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\2d_area_fill.exe"
s_sin.area_fill() RGB(255,0,0)
s_cos.area_fill() RGB(0,0,255)
s_tan.area_fill() blank
Plot of 50 * sin(x), cos(x) and tan(x)

*/
