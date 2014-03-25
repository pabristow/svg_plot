/*! \file demo_2d_histogram.cpp
    \brief Demonstrate drawing histogram as SVG files.
    \details
     Demonstrate drawing histogram as SVG files.
     The *area* under each bin shows the number of cases or value in that bin or class.
     (For bar charts the *height shows the value*).
     Bins can be the same (most common) or different widths.
     Intervals must not overlap and bins must be adjacent.
     Various suggestions for choosing good bins have been made,
     some a common bin width.  The example below has different bin widths
     reflecting very roughly the log of the journey time.

    \author Paul A. Bristow
    */

// Copyright Paul A. Bristow 2008

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#include <iostream>
  using std::cout; using std::endl;
#include <iomanip>
  using std::setw;

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;

#include <map>
  using std::map;

int main()
{
  map<double, double> times; // Example data series.
  // http://en.wikipedia.org/wiki/Histogram
  // Histogram of travel time (min), US 2000 census.

  // Interval(x)   Quantity (y)  Width   Quantity/width bin
  times[0]  =   4180.;   //     5         836     0 to 5 min
  times[5]  =  13687.;   //     5         2737     5 to 10 min
  times[10] =  18618.;   //     5         3723     10 to 15 min
  times[15] =  19634.;   //      5        3926     15 to 20 min
  times[20] =  17981.;   //      5        3596     20 to 25 min
  times[25] =   7190.;   //     5        1438     25 to 30 min
  times[30] =  16369.;   //      5        3273     30 to 35 min
  times[35] =   3212.;   //      5         642     35 to 40 min
  times[40] =   4122.;   //      5         824    40 to 45 min
  times[45] =   9200.;   //      5         613    45 to 60 min
  times[60] =   6461.;   //      5         215    60 to 90 min
  times[90] =   3435.;   //     5          57     90 to 150 min
  times[150] =     0.;   // Final bin end (15) with zero value.

  map<double, double> time2s; // Guess (wild!) at 2050 data series.
  time2s[0]  =   2000.;   //     5         836     0 to 5 min
  time2s[5]  =  5000.;   //     5         2737     5 to 10 min
  time2s[10] =  10000.;   //     5         3723     10 to 15 min
  time2s[15] =  11000.;   //      5        3926     15 to 20 min
  time2s[20] =  12000.;   //      5        3596     20 to 25 min
  time2s[25] =  13000.;   //     5        1438     25 to 30 min
  time2s[30] =  15000.;   //      5        3273     30 to 35 min
  time2s[35] =  16000.;   //      5         642     35 to 40 min
  time2s[40] =  18000.;   //      5         824    40 to 45 min
  time2s[45] =  12000.;   //      5         613    45 to 60 min
  time2s[60] =  11000.;   //      5         215    60 to 90 min
  time2s[90] =   10000.;   //     5          57     90 to 150 min
  time2s[150] =     0.;   // Final bin end (15) with zero value.

  // Check that the last element contains the expected last bin end.
  std::map<double, double>::const_iterator last = times.end();
  last--;
  BOOST_ASSERT(last->first == 150); // expected last bin end,
  BOOST_ASSERT(last->second == 0); // and that the value element is zero.

  if (last->second != 0) 
  {
    cout << "Last bin end " << last->first << " should have zero value! but is "  << last->second << endl;
    // Plot Implementation might advise, ignore or throw.
  }

  cout << "List all input data:" << endl;
  cout << "Time   Number of people" << endl;
  for(std::map<double, double>::const_iterator it = times.begin();
    it != times.end(); ++it)
  {
    cout << it->first << setw(6)   << it->second << endl;
  } // for it
  cout << endl;

  // Calculate and list the bins.
  // (Not needed for the plot).
  map<double, double> bins; // 
  cout << "Interval width  Quantity  quantity/width" << endl;
  for(std::map<double, double>::const_iterator it = times.begin();
    it != times.end(); ++it)
  {
    if (it == last)
    {
      break;
    }
    
    it++;
    double next = it->first;
    it--;
    double width = (next - it->first); // width of bin
    bins[it->first] = it->second/width; // quantity / width of bin
    cout << setw(3) << next  << setw(8) << setw(8) << width << it->second << setw(10) << it->second / width << endl;
  } // for it
  cout << endl;

  cout << "Bin_start height" << endl;
  for(std::map<double, double>::const_iterator it = bins.begin();
    it != bins.end(); ++it)
  {
    cout << it->first << ' ' << it->second << endl;
  } // for it
  cout << endl;
  cout << bins.size() << " bins" << endl;

  cout << svg_color(red) << endl;
  svg_color b(blank);
  cout << b << endl;
  b.write(cout);

  // Plot the histogram.
  svg_2d_plot my_plot1;

  my_plot1.title("US 2000 Census Journey Times")
    .legend_on(true) // Not very useful for a one series histogram.
    .background_border_color(black)
    .y_range(0, 5000.)
    .y_label("thousands of people per one minute interval")
    .x_range(0, 160.)
    .x_label("journey time")
    .x_label_units(" (min)")
    .x_label_units_on(true)

     // Axis settings.
    .x_major_interval(50.)
    .x_num_minor_ticks(4) // plus 1 major = 5 ticks per major step.
    .y_major_interval(1000.)
    .y_value_precision(4) // Without this, get y value labels in 2e3 format!
    .y_num_minor_ticks(4) // plus 1 major = 5 ticks per major step.
    ;

  //my_plot1.plot(times, "Series 1").shape(none).line_on(false).area_fill(red).histogram(column); // Need to supress line and shape plot options.
  // TODO change shape and line to off automatically when histogram is chosen?
  //my_plot1.plot(times, "Journey time").shape(none).line_on(false).line_color(red).area_fill(pink). histogram(column);
  my_plot1.plot(time2s, "2050 times? ").shape(none).line_on(false).line_color(blue).area_fill(lightblue).histogram(column);
  // Overlay hides plot details underneath.
  my_plot1.write("./demo_2d_histogram_x.svg");

  my_plot1.plot(time2s, "2050 times? ").shape(none).line_on(false).line_color(blue).area_fill(blank).histogram(column);
  my_plot1.write("./demo_2d_histogram_x2.svg");

  return 0;
} // int main()


/*

Output:

demo_2d_histogram.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_2d_histogram.exe"
List all input data:
Time   Number of people
0  4180
5 13687
10 18618
15 19634
20 17981
25  7190
30 16369
35  3212
40  4122
45  9200
60  6461
90  3435
150     0
Interval width  Quantity  quantity/width
  5       54180       836
 10       513687    2737.4
 15       518618    3723.6
 20       519634    3926.8
 25       517981    3596.2
 30       57190      1438
 35       516369    3273.8
 40       53212     642.4
 45       54122     824.4
 60      159200   613.333
 90      306461   215.367
150      603435     57.25
Bin_start height
0 836
5 2737.4
10 3723.6
15 3926.8
20 3596.2
25 1438
30 3273.8
35 642.4
40 824.4
45 613.333
60 215.367
90 57.25
12 bins
RGB(255,0,0)
blank
none
Build Time 0:03

*/

