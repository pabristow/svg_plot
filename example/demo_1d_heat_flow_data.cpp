/*! \file demo_1d_heat_flow_data.cpp
  \brief Demonstration of 1D plotting with some real-life data.
  \details
  An example to show Heat Flow data from
  http://www.itl.nist.gov/div898/handbook/eda/section4/eda4281.htm
  This data set was collected by Bob Zarr of NIST in January, 1990
  from a heat flow meter calibration and stability analysis.

  This data was used in an example in the Boost.Math Library.

  \author Paul A Bristow
*/

// demo_1d_heat_flow_data.cpp

// Copyright Paul A Bristow 2008, 2012

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[heat_flow_1d_plot_1
// An example to show Heat Flow data from
// http://www.itl.nist.gov/div898/handbook/eda/section4/eda4281.htm
// This data set was collected by Bob Zarr of NIST in January, 1990
// from a heat flow meter calibration and stability analysis.

//#ifdef _MSC_VER
// Path submitted to cure this.
//#  pragma warning (disable: 4510) // boost::array<T,N>' : default constructor could not be generated
//#  pragma warning (disable: 4610) // warning C4610: class 'boost::array<T,N>' can never be instantiated - user defined constructor required
//#endif

#include <boost/svg_plot/svg_1d_plot.hpp>
  using namespace boost::svg;
  using boost::svg::svg_1d_plot;
#include <boost/svg_plot/svg_boxplot.hpp>
  using boost::svg::svg_boxplot;

#include <boost/svg_plot/show_1d_settings.hpp>
// void boost::svg::show_1d_plot_settings(svg_1d_plot&);

#include <iostream> // for debugging.
  using std::cout;
  using std::endl;

#include <vector>
  using std::vector;

#include <limits>
  using std::numeric_limits;

int main()
{
  boost::array<const double, 195> heat_flows = {
  9.206343,
  9.299992,
  9.277895,
  9.305795,
  9.275351,
  9.288729,
  9.287239,
  9.260973,
  9.303111,
  9.275674,
  9.272561,
  9.288454,
  9.255672,
  9.252141,
  9.297670,
  9.266534,
  9.256689,
  9.277542,
  9.248205,
  9.252107,
  9.276345,
  9.278694,
  9.267144,
  9.246132,
  9.238479,
  9.269058,
  9.248239,
  9.257439,
  9.268481,
  9.288454,
  9.258452,
  9.286130,
  9.251479,
  9.257405,
  9.268343,
  9.291302,
  9.219460,
  9.270386,
  9.218808,
  9.241185,
  9.269989,
  9.226585,
  9.258556,
  9.286184,
  9.320067,
  9.327973,
  9.262963,
  9.248181,
  9.238644,
  9.225073,
  9.220878,
  9.271318,
  9.252072,
  9.281186,
  9.270624,
  9.294771,
  9.301821,
  9.278849,
  9.236680,
  9.233988,
  9.244687,
  9.221601,
  9.207325,
  9.258776,
  9.275708,
  9.268955,
  9.257269,
  9.264979,
  9.295500,
  9.292883,
  9.264188,
  9.280731,
  9.267336,
  9.300566,
  9.253089,
  9.261376,
  9.238409,
  9.225073,
  9.235526,
  9.239510,
  9.264487,
  9.244242,
  9.277542,
  9.310506,
  9.261594,
  9.259791,
  9.253089,
  9.245735,
  9.284058,
  9.251122,
  9.275385,
  9.254619,
  9.279526,
  9.275065,
  9.261952,
  9.275351,
  9.252433,
  9.230263,
  9.255150,
  9.268780,
  9.290389,
  9.274161,
  9.255707,
  9.261663,
  9.250455,
  9.261952,
  9.264041,
  9.264509,
  9.242114,
  9.239674,
  9.221553,
  9.241935,
  9.215265,
  9.285930,
  9.271559,
  9.266046,
  9.285299,
  9.268989,
  9.267987,
  9.246166,
  9.231304,
  9.240768,
  9.260506,
  9.274355,
  9.292376,
  9.271170,
  9.267018,
  9.308838,
  9.264153,
  9.278822,
  9.255244,
  9.229221,
  9.253158,
  9.256292,
  9.262602,
  9.219793,
  9.258452,
  9.267987,
  9.267987,
  9.248903,
  9.235153,
  9.242933,
  9.253453,
  9.262671,
  9.242536,
  9.260803,
  9.259825,
  9.253123,
  9.240803,
  9.238712,
  9.263676,
  9.243002,
  9.246826,
  9.252107,
  9.261663,
  9.247311,
  9.306055,
  9.237646,
  9.248937,
  9.256689,
  9.265777,
  9.299047,
  9.244814,
  9.287205,
  9.300566,
  9.256621,
  9.271318,
  9.275154,
  9.281834,
  9.253158,
  9.269024,
  9.282077,
  9.277507,
  9.284910,
  9.239840,
  9.268344,
  9.247778,
  9.225039,
  9.230750,
  9.270024,
  9.265095,
  9.284308,
  9.280697,
  9.263032,
  9.291851,
  9.252072,
  9.244031,
  9.283269,
  9.196848,
  9.231372,
  9.232963,
  9.234956,
  9.216746,
  9.274107,
  9.273776
};

  try
  { /*`try'n'catch block Very useful to ensure you see any error messages! */

    vector<double> heat_flow_data(heat_flows.begin(), heat_flows.end()); // Copy from Boost.array to vector.
    // This might be useful if data is in a C array.

    svg_1d_plot heat_flow_1d_plot; // Construct with all the default constructor values.
    heat_flow_1d_plot.document_title("NIST Heat_Flow Data") // This text shows on the browser tab.
      .description("NIST Heat_Flow Data")
      .copyright_date("2008-06-19")
      .copyright_holder("Paul A. Bristow, Bob Zarr, NIST")
      .license("permits", "permits", "requires", "permits", "permits"); // Require notice only.
      //see  http://creativecommons.org/licenses/ for details.
    // This will generate an XML comment and an author and rights entries thus:
    // <!-- SVG Plot Copyright Paul A. Bristow, Bob Zarr, NIST 2008-06-19 -->
    // <dc:author><cc:Agent><dc:title>Paul A. Bristow, Bob Zarr, NIST </dc:title> </cc:Agent> </dc:author>
    // <dc:rights><cc:Agent><dc:title>Paul A. Bristow, Bob Zarr, NIST</dc:title></cc:Agent></dc:rights>

    heat_flow_1d_plot.coord_precision(6); // Some rather precise data real-life,
    // so use high precision (even at the expense of slightly longer svg files).

    heat_flow_1d_plot
      .title("NIST Heat flow data") // Add title for the plot.
      .x_label("heat flow") // Add a label for the X-axis.
      .x_autoscale(heat_flow_data); // Use autoscale from this data.

    heat_flow_1d_plot.plot(heat_flows, "NIST heat flows"); // Add a dataset as an array.
    //heat_flow_1d_plot.plot(heat_flow_data, "NIST heat flows"); // Add a dataset.
    heat_flow_1d_plot.write("./heat_flow_data.svg");

    /*`Now we use a boxplot to show the same data, including its quartiles and any outliers. */

    svg_boxplot heat_flow_boxplot; // Construct with all the default constructor values.
    heat_flow_boxplot.y_autoscale(heat_flow_data); // Autoscale using this heat_flow data.
    heat_flow_boxplot.title("NIST Heat_Flow Data"); // Give a title.
    heat_flow_boxplot.plot(heat_flow_data, "NIST heat flows"); // Add a dataset.
    heat_flow_boxplot.write("./heat_flow_data_boxplot.svg");
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }
//] [/heat_flow_1d_plot_1]

  return 0;
} // int main()

/*

Compiling...
demo_1d_heat_flow_data.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\Debug\demo_heatflow_examples.exe


*/

