/*! \file svg_test_boxplot.cpp
   \brief Tests for svg boxplot.
   \details

   \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_1d_plot.hpp>
#include <boost/svg_plot/svg_boxplot.hpp>

#include <vector>
using std::vector;
#include <map>
using std::multimap;
#include <cmath>
using ::sin;
#include <iostream>
using std::cout;
using std::endl;

double h(double x)
{
    return 50 / (x);
}

double f(double x)
{ 
    return 40 + 25 * sin(x * 50);
}

int main()
{
    using namespace boost::svg;
    std::vector<double> data1, data2;

    for(double i=.1; i < 10; i+=.1)
    { // Fill the vectors with some data.
        data1.push_back(h(i));
        data2.push_back(f(i));
    }

    svg_1d_plot my_1d_plot;
    my_1d_plot.background_border_color(black)
           .title("1D plots of Common Functions");
    my_1d_plot.plot(data1, "[50 / x]");
    my_1d_plot.plot(data2, "[40 + 25 * sin(50x)]");

    my_1d_plot.write("./svg_test_1d.svg");

    // Now plot the same data using boxplot.
    svg_boxplot my_box_plot;
    my_box_plot.background_border_color(black)
           .title("Boxplots of Common Functions");

    my_box_plot.plot(data1, "[50 / x]");
    my_box_plot.plot(data2, "[40 + 25 * sin(50x)]");
    my_box_plot.y_autoscale(data1);  // Compute autoscale values for the plot.


    my_box_plot.write("./svg_test_boxplot.svg");
   return 0;
} // int main()

