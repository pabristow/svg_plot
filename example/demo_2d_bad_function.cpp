/*! \file demo_2d_bad_function.cpp
  \brief Demonstration of badly behaved function like tan
  \details Problemn with line not being draw because of singularity.

  \date 2009
  \author Paul A. Bristow
*/

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
  using std::tan;

double my_tan(double x)
{
  return 50. * tan(x);
}

int main()
{
  // Container for some trig data.
  map<double, double> data_tan;
  
  double inter = 3.141592653589793 / 8.; // 16 points per cycle of 2 pi.
  // Problem here if pi is more accurate (adding 3 at end) = 3.141592653589793
  // tan line - going to infinity and back does not show.
  // This is because tan is at +infinity or -infinity.
  // tan is very badly behaved and floating-point is evil!

  // Problem seems to be caused by the line going to large minus instead of large plus
  // But changing anything seems to make things come right again.
  // TODO perhaps resolve this.  May not be a common problem.

  double x_min = 0.;
  double x_max = 10.;

  for(double i = 0.; i <= x_max; i += inter)
  { // 
    data_tan[i] = my_tan(i);
    std::cout << i << ' '<< data_tan[i] << std::endl;
  } // for

  svg_2d_plot my_plot;

  // Size & scale settings.
  my_plot.size(700, 500)
         .x_range(x_min -1, x_max) // Leave blank space below x_min.
         .y_range(-75, 75);

  // Text settings.
  my_plot.title("Plot of 50 * tan(x)")
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
  

  // svg_2d_plot_series& s_tan = 
    my_plot.plot(data_tan, "tan(x)").shape(cone).line_on(true).line_color(red);

  std::cout << my_plot.title() << std::endl; // "Plot of 50 * (x)"

  my_plot.write("./demo_2d_bad_function_1.svg");

  

   return 0;
} // int main()

/*

Output:

Autorun "j:\Cpp\SVG\Debug\demo_2d_bad_function.exe
0 0
0.392699 20.7107
0.785398 50
1.1781 120.711
1.5708 8.16562e+017
1.9635 -120.711
2.35619 -50
2.74889 -20.7107
3.14159 -6.12323e-015
3.53429 20.7107
3.92699 50
4.31969 120.711
4.71239 2.72187e+017
5.10509 -120.711
5.49779 -50
5.89049 -20.7107
6.28319 7.65714e-014
6.67588 20.7107
7.06858 50
7.46128 120.711
7.85398 -1.5401e+016
8.24668 -120.711
8.63938 -50
9.03208 -20.7107
9.42478 2.48084e-013
9.81748 20.7107
Plot of 50 * tan(x)
Build Time 0:05


*/
