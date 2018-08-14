/*! \file 2d_simple.cpp
  \brief Simple 2D plot.
  \author Jacob Voytko
  \date 2007
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2018
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <vector>

using namespace boost::svg;

int main()
{
  using std::map;
  map<double, double> map1;
  map<double, double> map2;
  map<double, double> map3;

  // This is random data used purely for example.
  // Race 1
  map1[1.] = 3.2;
  map1[2.] = 5.4;
  map1[7.3] = 9.1;

  // Race 2
  map2[3.1] = 6.1;
  map2[5.4] = 7.;

  // Race 3
  map3[1.5] = 3.2;
  map3[2.3] = 6.0;
  map3[8.3] = 4.1;

  svg_2d_plot my_plot;

  my_plot.title("Race Times")
         .legend_on(true)
         .legend_title("Races")
         .x_range(-1, 11)
         .y_range(0, 10)
    .x_major_grid_on(true)
    .y_major_grid_on(true)
        ;

  my_plot.plot(map1, "Series 1").stroke_color(blue);
  my_plot.plot(map2, "Series 2").stroke_color(orange);
  my_plot.plot(map3, "Series 3").stroke_color(red);

  my_plot.write("./demo_2d_simple.svg");
  return 0;
} // int main()
