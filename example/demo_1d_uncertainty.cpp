/*! \file
    \brief Simple plot of uncertainty of 1D data.
    \details An example to demonstrate simple 1D plot using two vectors,
     including showing values with uncertainty information as
     "plus minus" and degrees of freedom estimates.
    \author Paul A. Bristow
*/

// demo_1d_uncertainty.cpp

// Copyright Paul A Bristow 2009, 2012

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_1d_uncertainty_1

/*`First we need a few includes to use Boost.Plot:
*/
#include <boost/quan/unc.hpp>  // Handles information uncertainty.

// using boost::quan::unc; // Holds value and uncertainty formation.
#include <boost/svg_plot/detail/functors.hpp>
//  using boost::svg::detail::unc_1d_convert;
#include <boost/svg_plot/svg_1d_plot.hpp>
//using namespace boost::svg;

#include <boost/svg_plot/show_1d_settings.hpp>
//void boost::svg::show_1d_plot_settings(svg_1d_plot&);

#include <iostream>
// using std::cout;
// using std::endl;
#include <vector>
//  using std::vector;
#include <iterator>
//  using std::ostream_iterator;
#include <algorithm>
//  using std::copy;
//] [/demo_1d_uncertainty_1]

int main()
{
  using namespace boost::svg;

//[demo_1d_uncertainty_2

    /*`STL vector is used as the container for our two data series,
    and values are inserted using push_back.  Since this is a 1-D plot
    the order of data values is not important.
    */
    setUncDefaults(std::cout);
    typedef unc<false> uncun; // Uncertain Uncorrelated (the normal case).
    constexpr float NaN = std::numeric_limits<float>::quiet_NaN();
    std::vector<uncun> A_times;
    A_times.push_back(unc<false>(3.1, 0.02F, 8));
    A_times.push_back(uncun(4.2, 0.01F, 14, 0U));

    short unsigned int t = UNC_KNOWN | UNC_EXPLICIT| DEG_FREE_EXACT | DEG_FREE_KNOWN;

    std::vector<unc<false> > B_times;
    B_times.push_back(uncun(2.1, 0.001F, 30, t)); // Value, uncertainty, degrees of freedom and type known.
    // (But use of type is not yet implemented.)
    B_times.push_back(unc<>(5.1, 0.025F, 20, 0U)); // Value, uncertainty, and degrees of freedom known - the usual case.
    B_times.push_back(uncun(7.8, 0.0025F, 1, 0U)); // Value and uncertainty known, but not degrees of freedom.
    B_times.push_back(uncun(3.4, 0.03F, 1, 0U)); // Value and uncertainty known, but not degrees of freedom.
    //B_times.push_back(uncun(6.9, 0.0F, 0, 0U)); // Only value known - no information available about uncertainty but treated as exact.
    B_times.push_back(uncun(5.9, NaN, 1, 0U)); // Only value known - uncertainty explicit NaN meaning no information available about uncertainty.
    // So in both cases show all possibly significant digits (usually 15).
    // This is  ugly on a graph, so best to be explicit about uncertainty.

    std::vector<unc<false> > C_times;
    C_times.push_back(uncun(2.6, 0.1F, 5, 0U));
    C_times.push_back(uncun(5.4, 0.2F, 11, 0U));

    /*`Echo the values input: */
    std::copy(A_times.begin(), A_times.end(), std::ostream_iterator<uncun>(std::cout, " "));
    std::cout << std::endl;
    std::copy(B_times.begin(), B_times.end(), std::ostream_iterator<uncun>(std::cout, " "));
    std::cout << std::endl;
    /*`The constructor initializes a new 1D plot, called `my_plot`,
    and also sets all the very many defaults for axes, width, colors, etc.
    */
    svg_1d_plot my_plot;
    /*`A few (member) functions that are set (using concatenation or chaining) should be fairly self-explanatory:

    * .title() provides a title at the top for the whole plot,
    * .legend_on(true) will mean that titles of data series and markers will display in the legend box.
    * .x_range(-1, 11) sets the axis limits from -1 to +11 (instead of the default -10 to +10).
    * .background_border_color(blue) sets just one of the very many other options.
    */
    my_plot.autoscale_check_limits(false); // Default is true.
    my_plot.autoscale_plusminus(2); // default is 3 standard deviations.
    my_plot.confidence(0.01);  // Change alpha from default 0.05 == 95% to 0.01 == 99%.

    my_plot
    .image_x_size(600)
    .image_y_size(300)
    .plot_window_on(true)
    .background_border_color(blue)
    .plot_border_color(yellow)
    .plot_border_width(1)
    //.x_ticks_on_window_or_axis(0) // now the default.
    .legend_on(false)
    .title("A, B and C Times")
    .x_range(0, 10) // but over-ridden by .x_autoscale(B_times) below:
    .x_autoscale(B_times) // Use the data-point values in series B_times to autoscale the X-axis.
        // Note that this might not be ideal scaling for A_times and/or C_times.
    .x_label("times (sec)")
    .x_values_on(true)
//   .x_values_precision(0) // Automatic number of digits of precision.
    .x_values_precision(2) // User-chosen std::ios precision decimal digits, for example "1.23".
    //.x_values_rotation(steepup) // steeper - but need more image and plot window vertical height for all data point info.
    .x_values_rotation(slopeup) // value at x= 7.8 overflows both plot window and image.
      // so would need to change .x_range 
    .x_plusminus_on(true)
    .x_plusminus_color(blue)
    .x_addlimits_on(true) // Show plus/minus +/- confidence limits for data-point value labels.
    .x_addlimits_color(purple)  // Show +/- in darkgreen, for example: "+/- 0.03".
    .x_df_on(true) // Show degrees of freedom (usually observations -1) for data-points.
    .x_df_color(green)  // Show degrees of freedom in green, for examples: "11").
    ;
    /*`
    Then we add our data-series, and add optional data-series titles, "A_times" "B_times" ...
    (very helpful if we want them to show on the legend).

    All the data points are also labeled with their value,
    and uncertainty (+/-) and degrees of freedom if known. 
    The A_times mark data points with a red border circle with a green fill,
    The B_times use a blue vertical line,
    while C_times use an ellipse whose width (x radius) is from the uncertainty,
    1st standard deviation shows as ellipse in magenta, and 2nd as yellow.

    Or one can explicitly set some (brighter) colors for the uncertainty ellipses, or none to omit.
    */

    //my_plot
    //  .one_sd_color(yellow) // Color of ellipse for one standard deviation (about 66% probability).
    //  .two_sd_color(green) // Color of ellipse for two standard deviation (~95%).
    //  .three_sd_color(red); // Color of ellipse for two standard deviation (~99%).

    my_plot.plot(A_times, "A").shape(circlet).size(10).stroke_color(red).fill_color(green);
    my_plot.plot(B_times, "B").shape(vertical_line).stroke_color(blue);
    my_plot.plot(C_times, "C").shape(unc_ellipse).fill_color(lightyellow).stroke_color(magenta);

    /*`
    Finally, we can write the SVG to a file of our choice.
    */
    std::string svg_file =  (my_plot.legend_on() == true) ?
                            "./demo_1d_uncertainty_legend.svg" : "./demo_1d_uncertainty.svg";

    my_plot.write(svg_file);

    std::cout <<"Plot written to file " << svg_file << std::endl;
    show_1d_plot_settings(my_plot);

//] [/demo_1d_uncertainty_2]

    return 0;
} // int main()

/*

Output:
//[demo_1d_uncertainty_output
demo_1d_uncertainty.cpp
Linking...
Embedding manifest...
Autorun j:\Cpp\SVG\debug\demo_1d_uncertainty.exe
3.1?0.02 (8)  4.2?0.01 (14)
2.1?0.001 (30)  7.8?0.0025 (21)  3.4?0.03 6.9
Build Time 0:03
//] [/demo_1d_uncertainty_output]
*/

/*
Data for another 1D example, also use Boost.Math?

The data is taken from J. C. Banford el al, Analyst (1983) 107, 195
and compares the concentration of thiol of the lysate of the blood of two groups
of volunteers as control and a second suffering from rhematoid arthritis.

  map<pair<unc, unc> > control;
  control.push_back(unc(1.921, 0.076F, 7));
  map<pair<unc, unc> > rheumatoid;
  control.push_back(unc(3.456, 0.44F, 6));


*/

/* Output Netbeans 4.7.2
 *
 3.10 4.200
2.1000 5.10 7.800 3.40 5.900
Plot written to file ./demo_1d_uncertainty.svg


axes_on false
background_border_width 1
background_border_color RGB(0,0,255)
background_color RGB(255,255,255)
...
 *
 *
 */
