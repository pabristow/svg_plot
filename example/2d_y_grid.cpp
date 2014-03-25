/*! \file 2d_y_grid.cpp
  \brief 2D plot with Y axis grids.
  \details Shows an example of adding a Y horizontal grid to a 2D plot.
  \author Jacob Voytko and Paul A. Bristow 
  \date 2007
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <vector>

using std::map;
using namespace boost::svg;

int main()
{
  map<double, double> map1;
  map<double, double> map2;

  // This is random data used purely for example.
  map1[1.] = 3.2;
  map1[2.] = 5.4;
  map1[7.3] = 9.1;

  map2[3.1] = 6.1;
  map2[5.4] = 7.;

  svg_2d_plot my_plot;

  my_plot.title("Race Times")
         .legend_on(true)
         .x_range(-1, 11)
         .background_border_color(black)
         .plot_window_on(true);

    my_plot.y_major_grid_on(true)
       .y_minor_grid_on(true)
       .y_major_grid_color(blue)
       .y_minor_grid_color(lightgray);

    my_plot.plot(map1, "Series 1").stroke_color(blue).fill_color(red);
    my_plot.plot(map2, "Series 2").stroke_color(orange); // Fill white by default.

    my_plot.write("./2d_y_grid.svg");
  return 0;
} // int main()
