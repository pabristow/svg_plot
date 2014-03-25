/*! \file boxplot_full.cpp

  \brief More complex boxplot plot.

  \author Jacob Voytko

  \date 2007
*/

// Copyright Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <vector>
#include <cmath>

#include <boost/svg_plot/svg_boxplot.hpp>

using std::vector;

// Functions we are using to simulate distributions.
double f(double x)
{
    return 50 / x;
}

double g(double x)
{
    return 40 + 25 * sin(x * 50);
}

int main()
{
    using namespace boost::svg;
    std::vector<double> data1, data2;

    // Fill our vectors;
    for(double i = .1; i < 10; i+=.1)
    {
        data1.push_back(f(i));
        data2.push_back(g(i));
    }

    // Initialize a new plot.
    svg_boxplot my_plot;

    // Color information.
    my_plot.background_border_color(black)
    .background_color(svg_color(67, 111, 69))
    .plot_background_color(svg_color(136, 188, 126))
    .title_color(white)
    .x_label_color(white)
    .y_label_color(white);

    // String information.
    my_plot.title("Boxplots of Common Functions")
           .x_label("Functions")
           .y_label("Population Size");

    // Axis information.
    my_plot.y_range(0, 100)
           .y_minor_tick_length(20)
           .y_major_interval(20);

    // Write data.
    my_plot.plot(data1, "[50 / x]");
    my_plot.plot(data2, "[40 + 25 * sin(x * 50)]");

    my_plot.write("boxplot_full.svg");
    return 0;
} // int main()

