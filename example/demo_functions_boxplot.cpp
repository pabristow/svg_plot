/*! \file demo_functions_boxplot.cpp

  \brief Boxplot of two functions.

  \author Jacob Voytko

  \date 2007
*/

//  Copyright Paul A. Bristow 2008, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg_1d_plot.hpp>
#include <boost/svg_plot/svg_boxplot.hpp>

#include <vector>
#include <cmath>
#include <iostream>

using std::multimap;
using std::vector;

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
    std::vector<double> data1, data2;

    svg_boxplot my_plot;
    svg_1d_plot my_1d_plot;

    //my_1d_plot.load_stylesheet(".\style.css");

    my_1d_plot.background_border_color(black)
           .title("Boxplots of Common Functions");

    for(double i=.1; i < 10; i+=.1)
    {
        data1.push_back(h(i));
        data2.push_back(f(i));
    }

    my_1d_plot.plot(data1, "[50 / x]");
    my_1d_plot.plot(data2, "[40 + 25 * sin(50x)]");

    my_1d_plot.write(".\1d_test.svg");

    return 0;
} // int main()
