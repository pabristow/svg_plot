/*! \file demo_2d_area_fill.cpp
  \brief Demonstration of area fill below curves.
  \details Using some trig functions to show how to area fill.

  \date 2009
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
  using std::map; // STL container for data.
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
  map<double, double> data_sin; // Containers for some trig data.
  map<double, double> data_cos;
  map<double, double> data_tan;

  double inter = 3.14159265358979 / 8.; // 16 points per cycle of 2 pi.
  // Problem here if pi is more accurate (adding 3 at end) = 3.141592653589793
  // tan line - going to infinity and back does not show.
  // This is because tan is at +infinity or -infinity.
  // tan is very badly behaved and floating-point is evil!

  for(double i = 0; i <= 10.; i += inter)
  { //
    data_sin[i] = my_sin(i);
    data_cos[i] = my_cos(i);
    data_tan[i] = my_tan(i);

    std::cout << i << ' '<< data_tan[i] << std::endl;

  } // for

  svg_2d_plot my_plot;

  // Size/scale settings.
  my_plot.size(700, 500)
         .x_range(-1, 10)
         .y_range(-75, 75);

  // Text settings.
  my_plot.title("Plot of 50 * sin(x), cos(x) and tan(x)")
         .title_font_size(20)
         .title_color(red)
         .x_label("x")
         .y_label("50 * f(x)")
         .x_major_labels_side(bottom) // X axis label below bottom of plot window (default).
         .y_major_labels_side(left) // Y axis label to left of plot window (default).
         .x_major_grid_on(true) // Use grids.
         .y_major_grid_on(true)
         .x_major_grid_color(cyan)
         .y_major_grid_color(cyan)
         ;

  // Color settings.
  my_plot.background_color(whitesmoke)
         .legend_background_color(lightyellow)
         .legend_border_color(yellow)
         .plot_background_color(ghostwhite)
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
         .y_num_minor_ticks(4); // 4 minor ticks between 0 to 25, so mark major 0, minor 5, 10, 15, 20, major 25 ...

  svg_2d_plot_series& s_sin = my_plot.plot(data_sin, "sin(x)").line_on(true).area_fill(red);
  std::cout << "s_sin.area_fill() " << s_sin.area_fill() << std::endl; // s_sin.area_fill() RGB(255,0,0)

  svg_2d_plot_series& s_cos = my_plot.plot(data_cos, "cos(x)").line_on(true).area_fill(blue).shape(square);
  std::cout << "s_cos.area_fill() " << s_cos.area_fill() << std::endl; // s_cos.area_fill() RGB(0,0,255)

  svg_2d_plot_series& s_tan = my_plot.plot(data_tan, "tan(x)").shape(cone).line_on(true).area_fill(blank);
  // Note that svg_color(blank) or svg_color(false) returns a non-color blank, so no fill.
  std::cout << "s_tan.area_fill() " << s_tan.area_fill() << std::endl; // s_tan.area_fill() blank

  std::cout << my_plot.title() << std::endl; // "Plot of 50 * sin(x), cos(x) and tan(x)"

  my_plot.write("./demo_2d_area_fill_1.svg");

  my_plot.plot(data_sin, "sin(x)").line_on(true).area_fill(green).shape(square).fill_color(red);
  // Note how this overwrites the previously cos fill and tan line.
  // (It also needs a new title).

  my_plot.title("sin overwriting cos and tan");
  std::cout << my_plot.title() << std::endl; // "sin overwriting cos and tan"

  my_plot.write("./demo_2d_area_fill_2.svg");

   return 0;
} // int main()

/*

Output:

Autorun "j:\Cpp\SVG\Debug\demo_2d_area_fill.exe
0 0
0.392699 20.7107
0.785398 50
1.1781 120.711
1.5708 3.09493e+016
1.9635 -120.711
2.35619 -50
2.74889 -20.7107
3.14159 -1.61554e-013
3.53429 20.7107
3.92699 50
4.31969 120.711
4.71239 1.08118e+016
5.10509 -120.711
5.49779 -50
5.89049 -20.7107
6.28319 -3.23109e-013
6.67588 20.7107
7.06858 50
7.46128 120.711
7.85398 6.02427e+015
8.24668 -120.711
8.63938 -50
9.03208 -20.7107
9.42478 -3.73641e-013
9.81748 20.7107
s_sin.area_fill() RGB(255,0,0)
s_cos.area_fill() RGB(0,0,255)
s_tan.area_fill() blank
Plot of 50 * sin(x), cos(x) and tan(x)
sin overwriting cos and tan


*/
