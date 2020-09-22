/*!
  \file 2d_simple.cpp
  \brief Simple 2D plot example.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2007, 2020
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_SVG_LEGEND_DIAGNOSTICS
// I:\Cpp\SVG_plot\svg_plot\2d_simple\x64\Release\2d_simple.log

#include <boost/cstdlib.hpp> // Boost exit_success and failure values.
#include <boost/version.hpp> // Boost version info.

#include <boost/svg_plot/svg_2d_plot.hpp> // svg_plot function
//#include <boost/svg_plot/show_2d_settings.hpp> // boost/libs/svg_plot/doc/html/header/boost/svg_plot/show_2d_settings_hpp.html

#include <iostream>
#include <map> // Container for data.
// using std::map

int main()
{
  std::cout << "svg_plot simple_2d" << std::endl;
  std::cout << "BOOST_VERSION " << BOOST_VERSION << std::endl;

  try
  {
    //! Data to plot stored in container type `std::map`.
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

    using namespace boost::svg;  // Convenient for access to all SVG functions and colors.

    svg_2d_plot my_plot;

    my_plot.title("Race Times")
      .legend_on(true)
      .legend_title("Races")
      .x_range(-1, 11) 
      .y_range(0, 10)
      .x_major_grid_on(true)
      .y_major_grid_on(true)
      ;

    my_plot.plot(map1, "Series 1").stroke_color(green);
    my_plot.plot(map2, "Series 2").stroke_color(orange);
    my_plot.plot(map3, "Series 3").stroke_color(red);

    my_plot.write("./2d_simple.svg");

    //show_2d_plot_settings(my_plot);

    return boost::exit_success;
  }
  catch (std::exception& ex)
  {
    std::cout << "svg_plot exception " << ex.what() << std::endl;
    return boost::exit_exception_failure;
  }
} // int main()
