/*!
  \file 2d_uncertain_values.cpp
  \brief Simple 2D plot example showing how uncertain values can also be labelled with their uncertainty, confidence interval and degrees of freedom.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A Bristow 2007, 2020
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/cstdlib.hpp> // Boost exit_success and failure values.

#include <boost/svg_plot/svg_2d_plot.hpp> // svg_plot function
#include <boost/svg_plot/show_2d_settings.hpp> // boost/libs/svg_plot/doc/html/header/boost/svg_plot/show_2d_settings_hpp.html

#include <iostream>
#include <map> // Container for data.
// using std::map

#include <vector>
// using std::vector
#include <utility>
// using std::pair
// using std::make_pair
#include <algorithm>
// using std::foreach
#include <boost/quan/unc.hpp>
//#include <boost/quan/unc_init.hpp>

int main()
{
  std::cout << "svg_plot 2D Uncertain value-labels." << std::endl;

  using boost::quan::unc;
  using boost::quan::uncun;
  using boost::quan::setUncDefaults;
  using boost::quan::adddegfree;
  using boost::quan::addlimits;
  using boost::quan::plusminus;

  typedef unc<false> uncun; // Uncertain type to use (Uncorrelated the normal case).
  setUncDefaults(std::cout);  // Initialisation of uncertain type.

  try
  {
    //! Data to plot stored in container type `std::map`.

   uncun ux1(2.23, 0.056F, 7); // For an X-value. 
   // Using the  uncertain class unc `operator<<` provided we can output all the details of the uncertain values.
    std::cout << std::scientific << plusminus << addlimits << adddegfree << std::setw(20) << std::left 
      << "ux = " << ux1 << std::endl; // ux =             2.23 +/-0.056 <2.19, 2.27> (7)
    uncun uy1(3.45, 0.67F, 9); // For a Y-value. 
    std::cout << "uy = " << uy1 << std::endl; // uy = 3.5 +/-0.67 <3.01, 3.89> (9)
    std::pair<uncun, uncun > up1 = make_pair(ux1, uy1); // Make an X & Y pair of uncertain values.
    std::cout << "up1 = " << up1 << std::endl; 
    // up1 = 2.23 +/-0.056 <2.19, 2.27> (7), 3.5 +/-0.67 <3.01, 3.89> (9)

    uncun ux2(5.45, 0.45F, 8); // For 2nd X-value. 
    uncun uy2(6.08, 0.52F, 5); // For 2nd Y-value.
    std::pair<uncun, uncun > up2 = make_pair(ux2, uy2); // Make a second X & Y pair of uncertain values.
    std::cout << "up2 = " << up2 << std::endl; 
    // up2 = 5.5 +/-0.45 <5.14, 5.76> (8), 6.1 +/-0.52 <5.62, 6.54> (5)

    std::vector<std::pair<uncun, uncun> > uncertains;
    std::cout << uncertains.size() << std::endl; // zero.
    uncertains.reserve(10);
    uncertains.push_back(up1);
    uncertains.push_back(up2);
    // Display the uncertain values packaged in a vector using conventional for loop.
    for (size_t i = 0; i < uncertains.size(); i++)
    {
      std::cout << "#" << i << " " << uncertains[i] << std::endl;
    }

   //// Display the uncertain values packaged in a vector using a C++ lambda with functor.
   // auto show = [](const std::pair<uncun, uncun>& u) { std::cout << " " << u << std::endl; };
   // std::for_each(uncertains.begin(), uncertains.end(), show); std::cout << std::endl;

   // // Display using std::copy.
   // std::copy(uncertains.begin(), uncertains.end(), std::ostream_iterator<std::pair<uncun, uncun>>(std::cout, "\n"));
   // std::cout << std::endl;

    using namespace boost::svg;  // Convenient for access to all SVG functions and colors.

    svg_2d_plot my_2d_plot;

    my_2d_plot
      .title("Uncertains - labelled with X and Y values and uncertainty info.")
      .title_font_size(10)
      // No legend needed as only one data-series?
      //.legend_on(true)
      //.legend_title("Knowns")
      .x_range(0, 10)
      .y_range(0, 10)
      .x_major_grid_on(true)
      .y_major_grid_on(true)
      ;

    /*`To put a value-label against each data-point, switch on or other, or both the options:
*/
//my_2d_plot.x_values_on(true); // Add a label for the X-axis.
//my_2d_plot.y_values_on(true); // Add a label for the X-axis.
    my_2d_plot.xy_values_on(true); // Add a label for both the X and Y-axis.

    /*`If the default size and color are not to your taste, set more options, like:
*/
    my_2d_plot.x_values_font_size(8) // Change font size for the X-axis value-labels.
      .x_values_font_family("Times New Roman") // Change font for the X-axis value-labels.
      .x_values_color(red); // Change X-values color from default black to red.

    my_2d_plot.y_values_font_size(10) // Change font size for the Y-axis value-labels.
      .y_values_font_family("Arial") // Change font for the Y-axis value-labels.
      .y_values_color(blue); // Change Y-values color from default black to blue.

/*`The default value-label position is horizontal, centered slightly above the data_point marker,
but, depending on the type and density of data_points, and the length of the values
(controlled in turn by choice of options, the `precision` and `ioflags` in use),
it is often clearer to use a different orientation.
This can be controlled in steps using an 'enum rotate_style` for convenience (or in degrees).

* `leftward` - writing level with the data_point but to its left.
* `rightward` - writing level with the data-point but to its right.
* `uphill` - writing up at 45 degree slope is often a good choice,
* `upward` - writing vertically up and
* `backup` - writing to the left are also useful.
``
    enum rotate_style
    {
      // Also need a no_rotate, = -1;
      horizontal = 0, //!< normal horizontal left to right, centered.
      slopeup = -30, //!< slope up.
      uphill = -45, //!< 45 steep up.
      steepup = -60, //!< up near vertical.
      upward = -90, //!< vertical writing up.
      backup = -135, //!< slope up backwards - upside down!
      leftward= -180, //!< horizontal to left.
      rightward = 360, //!< horizontal to right.
      slopedownhill = 30, //!< 30 gentle slope down.
      downhill = 45, //!< 45 down.
      steepdown = 60, //!<  60 steeply down.
      downward = 90,  //!< vertical writing downwards from marker.
      backdown = 135, //!< slope down backwards.
      upsidedown = 180 //!< upside down!  (== -180)
    };


``

(For 1-D plots other directions are less attractive,
placing the values below the horizontal Y-axis line,
but for 2-D plots all writing orientations can be useful).
*/
    my_2d_plot.x_values_rotation(rightward); // Orientation for the Y-axis value-labels, placing information to the right of the data-point marker.
   // my_2d_plot.x_values_rotation(horizontal); // Orientation for the Y-axis value-labels, placing information above the data-point marker.
   // my_2d_plot.x_values_rotation(uphill); // Orientation for the Y-axis value-labels, placing information to the right of the data-point marker.
   // my_2d_plot.x_values_rotation(upward); // Orientation for the Y-axis value-labels, placing information to the right of the data-point marker.
  //  my_2d_plot.x_values_rotation(leftward); // Orientation for the Y-axis value-labels, placing information to the right of the data-point marker.
    // This is only useful for x prefix and value, like "X = 1.23" not any following information.

  /*`Add some information about uncertainty to both the X and Y-values:*/
    my_2d_plot.x_plusminus_on(true); // Uncertainty (standard deviation) +/- value-label for the X-axis value.
    my_2d_plot.x_plusminus_color(blue); // Change from default color black to color blue.
    my_2d_plot.x_df_on(true); // Degrees of freedom (observations-1) value-label for the X-axis value.
    my_2d_plot.x_addlimits_on(true); // Confidence limit value-label for the X-axis value.
    my_2d_plot.x_datetime_on(true);
    my_2d_plot.x_order_on(true);
    my_2d_plot.x_decor("X=&#x200A;", "",""); // Suffix value-label for the X-axis value.
    // https://jkorpela.fi/chars/spaces.html describes Unicode spaces that must be explicit, not just spaces in the string.
    // Normal space is  "&#x00A0;", but hair space "&#x200A;" is most useful between digits and characters,
    // for example: "X=&#x200A;" and "X=&#x2001;" is em quad, 1 em (nominally, the height of the font).
    // and about Y-values:
    my_2d_plot.y_decor(", Y=", ""," g"); // Suffix value-label for the Y-axis value.

    my_2d_plot.y_plusminus_on(true); // Uncertainty (standard deviation) value-label for the X-axis value.
    my_2d_plot.y_plusminus_color(green);
    // Degrees of freedom for the Y-axis value is rather redundant if already shown for X-axis values.
    my_2d_plot.y_df_on(true); // Degrees of freedom for the Y-axis value. 
    my_2d_plot.y_addlimits_on(true); // Confidence limit or interval for the X-axis value.

    std::cout << "x_values_color() = " << my_2d_plot.x_values_color() << std::endl; // y_values_color() = RGB(0,0,255) == blue for fill and stroke
    std::cout << "y_values_color() = " << my_2d_plot.y_values_color() << std::endl; // y_values_color() = RGB(0,0,255) == blue for fill and stroke

  //  my_2d_plot.plot(doubles, "data-series 1 - doubles").stroke_color(blue).fill_color(red);

    // These affect all the marker points.
    //my_2d_plot.y_values_font_size(16) // Change font size for the Y-axis value-labels.
    //  .y_values_font_family("bold") // Change font for the Y-axis value-labels.
    //  .y_values_color(green); // Change Y color from default black to blue.

///   Change the data-point markers colors to green circle filled with a yellow center.
    my_2d_plot.plot(uncertains, "1 data-series - 2 uncertains").stroke_color(green).fill_color(yellow);

    my_2d_plot.write("./demo_2d_uncertain_values_1.svg");

  //  show_2d_plot_settings(my_2d_plot, std::cout); // Needs include <boost/svg_plot/show_2d_settings.hpp>

    return boost::exit_success;
  }
  catch (std::exception& ex)
  {
    std::cout << "svg_plot exception " << ex.what() << std::endl;
    return boost::exit_exception_failure;
  }
} // int main()

/*

Build started...
1>------ Build started: Project: demo_2d_uncertain_values, Configuration: Debug x64 ------
1>demo_2d_uncertain_values.cpp
1>demo_2d_uncertain_values.vcxproj -> I:\Cpp\SVG_plot\svg_plot\x64\Debug\demo_2d_uncertain_values.exe
1>Autorun "I:\Cpp\SVG_plot\svg_plot\x64\Debug\demo_2d_uncertain_values.exe"
1>svg_plot 2D Uncertain values
1>ux =                5.2 +/-0.56 <4.82, 5.64> (7)
1>uy = 8.4 +/-0.67 <8.01, 8.89> (9)
1>0
1> 5.2 +/-0.56 <4.82, 5.64> (7), 8.4 +/-0.67 <8.01, 8.89> (9)
1> 7.5 +/-0.45 <7.14, 7.76> (8), 6.1 +/-0.52 <5.62, 6.54> (5)
1>
1>Done building project "demo_2d_uncertain_values.vcxproj".
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

Projects build report:
  Status    | Project [Config|platform]
 -----------|---------------------------------------------------------------------------------------------------
  Succeeded | demo_2d_uncertain_values\demo_2d_uncertain_values.vcxproj [Debug|x64]

Build time 00:00:04.211
Build ended at 22Jan2021 17:36:03

1>#0 5.2 +/-0.56 <4.82, 5.64> (7), 8.4 +/-0.67 <8.01, 8.89> (9)
1>#1 7.5 +/-0.45 <7.14, 7.76> (8), 6.1 +/-0.52 <5.62, 6.54> (5)



*/
