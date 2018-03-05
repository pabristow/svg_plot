/*! \file svg_test_2d_boxplot.cpp
   \brief Tests for svg 2d boxplot.
   \details
   \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_2d_plot.hpp>
#include <boost/svg_plot/svg_boxplot.hpp>

#include <map>
#include <cmath>

using std::multimap;

double h(double x)
{
    return 50 / (x);
}

double f(double x)
{
    return 40 + 25 * sin(x * 50);
}

using std::cout;
using std::cin;

int main()
{
    using namespace boost::svg;
    std::multimap<double, double> data1;

    svg_boxplot my_plot;
    svg_2d_plot my_2d_plot;

    my_2d_plot.background_border_color(black)
           .title("Y axis label experiment")
           .y_major_labels_on(true)
           .y_label_on(true)
           .x_label_on(true)
           .x_major_labels_on(true)
           .plot_window_on(true)
           .y_external_style_on(true);

    for(double i=.1; i < 10; i+=.1)
    {
        data1.insert(std::pair<double, double>(i, h(i)));
    }

    my_2d_plot.plot(data1, "[50 / x]");

    my_2d_plot.write("D:\\axislabel_test.svg");

    return 0;
}
