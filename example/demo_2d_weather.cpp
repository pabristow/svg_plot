/*! \file demo_2d_weather.cpp
    \brief Example of a histogram of weather data.
    \details Creates file weather6 to 8.svg
    \author Paul A. Bristow
    \date Jul 2009
  */

// Copyright Paul A. Bristow 2009

// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org

#ifdef _MSC_VER
#  pragma warning (disable : 4127) // conditional expression is constant
#endif

// An example to demonstrate plotting some weather data from a CSV file (output from Excel)
// The data was collected from a weather station that stored months of data in its RAM.
// This data was then downloaded using the EasyWeather 1 software provided and saved in its .csv format.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.

// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

//[demo_2d_weather_1

/*`
An example to demonstrate plotting some weather data from a CSV file (output from Excel)
The data was collected from a weather station that stored months of data in its RAM.
This data was then downloaded using the EasyWeather 1 software provided and saved in its .csv format.

See the source code for various includes to use Boost.Plot, and STL 1D container vector etc.

This data is read and parsed into STL container(s) with some messy code to deal with missing values,
indicated by the string "---" in the .csv file, which are replaced by quiet_NaN because the svg_plot
program assumes that these are used for 'missing' values.

Missing values are not used for autoscaling, and are shown by a different symbol at the edges of the plot.

*/
//] [/demo_2d_weather_1]

#include <boost/svg_plot/svg_2d_plot.hpp>
using namespace boost::svg;
#include <boost/svg_plot/detail/pair.hpp>
//  using boost::svg::detail::operator<<; // Output pair as, for example: 1.23, 4.56

#include <iostream>
 using std::cout;
  using std::endl;
  using std::ios_base;
  using std::dec;
  using std::hex;
#include <vector>
  using std::vector; // 1D data container.
#include <map>
  using std::map; // 2D data container.
#include <fstream> // for file streaming, ifstream & ofstream.
  using std::ifstream;
#include <string>
  using std::string;
  using std::getline;
#include <sstream>
  using std::istringstream;
#include <limits>
  using std::numeric_limits;
#include <algorithm>
  using std::min;
  using std::max;
#include <utility>
  using std::pair;
#include <ios>
  using std::noskipws;

  float getvalue(string s)
  {
    if (s == "---")
    { // missing value, so return NaN
      return numeric_limits<float>::quiet_NaN();
    }
    istringstream is(s); // Read float from string.
    float v;
    is >> v;
    if (is.fail() == true)
    {
      return numeric_limits<float>::quiet_NaN();
    }
    return v;
  } // float getvalue(string s)

  int getintvalue(string s)
  {
    if (s == "---")
    { // missing value, so return NaN
      return numeric_limits<int>::denorm_min();
    }
    istringstream is(s); // Read float from string.
    int i;
    is >> i;
    if (is.fail() == true)
    {
      return numeric_limits<int>::denorm_min();
    }
    return i;
  } // int getintvalue(string s)

int main()
{ // Construct two STL containers for the two data series to plot.

  const char comma_or_tab = ','; // separator - assumed comma for .csv file.

  //bool diag = false;

  map<double, double> in_temps; // record number and value.
  map<double, double> out_temps;
  map<double, double> rain_hours;

  const char* weather("EasyWeather7mar09.csv"); // The weather data, comma separated, with one column title line.
  ifstream fin(weather, ios_base::in);
  if (fin.is_open())
  {
    cout << "Reading weather from " << weather << endl;
  }
  else
  {
    cout << "Could not open file " <<  weather << " to read weather data!" << endl;
    return 0;
  }

  int readings = 0; // Count of weather data records processed.
  int plotted = 0; // Count of weather data records plotted.
  string line; // A single complete weather data record as a string.
  getline(fin, line);
  cout << line << endl; // Header line with Column titles.
  // No,Time,Interval(mi), Indoor Humidity(%), Indoor Temperature(C), Outdoor Humidity(%), Outdoor Temperature(C), Absolute Pressure(hPa),
  // Wind(m/s), Gust(m/s), Direction, Relative Pressure(hPa), Dewpoint(C), Windchill(C),
  // Hour Rainfall(mm), 24 hour Rainfall(mm), Week Rainfall(mm), Month Rainfall(mm), Total Rainfall(mm), Wind Level(bft), Gust Level(bft)

  // EastWeather software allows this as an option, convenient when working with a spreadsheet.
  // Here it is just ignored, assuming that all items are chosen for including in the .csv file.

  do
  { // Get all the lines of weather data records.
    getline(fin, line);
    // cout << line << endl; // Show a sample line of weather readings.
    // no   time          int   in_h
    // 1, 28-12-2008 14:10, 30, 57, 17, 68, 4.8, 1025.9, 3.4, 4.8, N, 1006.1, -0.6, 0.2, 0, 2.1, 2.1, 2.1, 2.1, 3, 3

    if (line.size() == 0)
    {
      break;
    }

    readings++;

    char sep; // Expect comma or tab as separator.
    // Fields in weather data record:
    int j; // Sequential record number in the weather station data.
    int day; // Timestamp of the record.
    int month;
    int year;
    int hour;
    int min;
    int interval;  // between readings (min).
    float in_humidity;
    float in_temp;
    float out_humidity;
    float out_temp;
    float pressure;
    float wind;
    float gust;
    char dir; // Single letter, N, S, E, or W.
    string direction; // May be up to 3 letters, like NNW.
    float rel_pressure;
    float dewpoint;
    float windchill;
    float rain_hour;
    float rain_day;
    float rain_week;
    float rain_month;
    float rain_all;
    int beaufort;
    int gusty;

    istringstream is(line); // Single comma separated line of data, terminated by newline.
    is >> noskipws; // Noskipws to allow test of space as separator.
    is >> j;
    if (is.fail() == true)
    {
      cout << "Failed to read data record number from file " <<  weather << "!" << endl;
      break; // To try next line?
    }
    is >> sep; // comma separator.
    if (sep != comma_or_tab)
    {
      cout << "Expected " << comma_or_tab << " as separator, but got " << sep << endl;
      cout << " whole line is " << line << endl;
    }
    is >> day; // dd
    is >> sep; // -
    if (sep != '-')
    {
      cout << "Expected - as separator, but got " << sep << endl;
    }
    is >> month; // mm
    is >> sep; // -
    if (sep != '-')
    {
      cout << "Expected - as separator, but got " << sep << endl;
    }
    is >> year; // yyyy
    is >> sep; // space
    if (sep != ' ')
    {
      cout << "Expected space as separator, but got " << sep << endl;
    }
    is >> hour; // hh
    is >> sep; // :
    if (sep != ':')
    {
      cout << "Expected : as separator, but got " << sep << endl;
    }
    is >> min; // mm
    is >> sep; // ,
    if (sep != ',')
    {
      cout << "Expected , as separator, but got " << sep << endl;
    }

    is >> interval; // usually 30 minutes
    is >> sep;
    if (sep != ',')
    {
      cout << "Expected , as separator, but got " << sep << endl;
    }

    string s;
    getline(is, s, ',');
    in_humidity = getvalue(s); // Indoor Humidity(%)

    getline(is, s, ',');
    in_temp = getvalue(s); // Indoor Temperature (C)

    getline(is, s, ',');
    out_humidity = getvalue(s); // Indoor Humidity (%)

    getline(is, s, ',');
    out_temp = getvalue(s); // Outdoor Temperature (C)

    getline(is, s, ',');
    pressure = getvalue(s); // Absolute Pressure (hPa)

    getline(is, s, ',');
    wind = getvalue(s); // Wind (m/s)
    getline(is, s, ',');
    gust = getvalue(s); // Gust (m/s)
    do
    { // Direction may be encoded as N, NW or NNW
      is >> dir;
      if ((dir == 'N') || (dir == 'S') || (dir == 'E') || (dir == 'W')
        || (dir == '-') // "---" used to show no value available.
        )
      {
        direction += dir;
      }
      else if (dir == comma_or_tab)
      {
        break;
      }
      else
      {
        cout << "Unexpected (not N, S, E or W) wind direction character " << dir << "!" << endl;
        break;
      }
    } while(true);
    if (direction == "---")
    {
      direction = "?"; // Indicate a missing value (or "" perhaps).
    }

    getline(is, s, ',');
    rel_pressure = getvalue(s); // Relative Pressure (hPa)
    getline(is, s, ',');
    dewpoint = getvalue(s); // Dewpoint(C)

    getline(is, s, ',');
    windchill = getvalue(s); // Windchill (C)
    getline(is, s, ',');
    rain_hour = getvalue(s); // Hour Rainfall (mm)
    getline(is, s, ',');
    rain_day = getvalue(s); // Day Rainfall (mm)
    getline(is, s, ',');
    rain_week = getvalue(s); // Week Rainfall (mm)
    getline(is, s, ',');
    rain_month = getvalue(s); // Month Rainfall(mm)
    getline(is, s, ',');
    rain_all = getvalue(s); // Total Rainfall (mm)
    getline(is, s, ',');
    beaufort = getintvalue(s); // Wind Level (Beaufort scale 0 - 12)
    getline(is, s, ',');
    gusty = getintvalue(s); // Gust Level(Beaufort scale 0 - 12)

    // line ends with newline, but this is consumed by getline, so no separator.

    //if (diag == true)
    if (is.fail() == true)
    {
      cout << j << endl;
      cout << day << endl;
      cout << month << endl;
      cout << "year " << year << endl;
      cout << hour << endl;
      cout << min << endl;
      cout << interval << endl;
      cout << in_humidity << endl;
      cout << in_temp << endl;
      cout << out_humidity << endl;
      cout << out_temp << endl;
      cout << pressure << endl;
      cout << wind << endl;
      cout << gust << endl;
      cout << direction << endl;
      cout << rel_pressure << endl;
      cout << dewpoint << endl;
      cout << windchill << endl;
      cout << rain_hour << endl;
      cout << rain_day << endl;
      cout << rain_week << endl;
      cout << rain_month << endl;
      cout << rain_all << endl;
      cout << "Beaufort scale " << beaufort << endl;
      cout << "gustiness " << gusty << endl;
      is.clear();
    }

    if ((j % 1) == 0)
    { // Only chose a sub-set of readings (to avoid too many points merging).
      // Fill the two containers with temperature data:
      plotted++;
      in_temps[j] = in_temp;
      out_temps[j] = out_temp;
      rain_hours[j] = rain_hour;
    }
  }
  while (line.size() > 0);
  // Or test is.eof() == true?

  cout << readings << " readings read, of which only " << plotted << " were plotted." << endl;

  try
  { // try'n'catch blocks are needed to ensure error messages from any exceptions are shown.


  //[demo_2d_weather_2

/*` The code below shows plotting just the inside and outside temperatures,
selecting the range of the axis by a user choice or automatically.
*/

/*` SVG_plot range_all is another mechanism for handling multiple containers
 providing a more convenient way to find the minimum of minimums and maximum of maximums.
It is especially convenient when there are many containers (of the same type),
and there may be 'missing' data items.
*/
    {
      svg_2d_plot my_plot; // Construct a 2D plot.

      my_plot.x_size(2000)
             .image_y_size(400)
             .legend_on(true) // Set title and legend, and X axis range.
             .title("Temperatures at Long: 2:45:16.2W, Lat:54:17:47.1N")
             .x_range(0., 2000.)
             .x_major_interval(500.)
             .x_ticks_values_color(red)
             .x_num_minor_ticks(4)
             .x_axis_label_color(green)
             .x_ticks_values_precision(0)
             .x_ticks_values_ioflags(ios_base::fixed)
             .x_ticks_values_font_family("arial")
             .x_ticks_values_font_size(20)
             //.autoscale_check_limits(true) // Is default, but check for NaN, infinity etc.
             //.xy_autoscale(in_temps) // Autoscale BOTH axes.
             // Implementation does not (yet) permit use of the container for .x_autoscale(in_temps),
             // but can ignore the autoscale range thus:
             .y_range(0., 20.) // User chosen range, over-riding the autoscale.
             .y_ticks_values_color(magenta)
             .y_ticks_values_precision(1)
             .y_ticks_values_ioflags(ios_base::scientific)
             .y_axis_label_color(blue)
             ;

      my_plot.x_label("time (hr)").y_label("temp (&#x00B0;C)"); // Note chaining.

      // Add a container of data to the plot, choosing a color.
      my_plot.plot(in_temps, "Inside (&#x00B0;C)").stroke_color(red).shape(none).size(1).bezier_on(true).line_color(red);
      //my_plot.plot(out_temps, "Outside (&#x00B0;C)"); // default is black circle, 5 pixel size, with no fill.
      my_plot.plot(out_temps, "Outside (&#x00B0;C)").stroke_color(blue).shape(point);

/*`Note how the point markers are switched off for the inside temperatures,
and a bezier line chosen
but the point markers are set to `round` for the outside temperatures,
giving an  less attractive spotty appearance, but showing the actual data points.
The default round size is 5 which is a bit big when there are so many points to plot.
The size also needs to be > 1 to be displayed.

There may be also, by default, too many axis ticks and labels on the X-axis,
so one can either use autoscale or explicitly set the number of X ticks.

*/
      my_plot.write("./demo_2d_weather_1.svg"); // Write the plot to file.

            // Show the ticks styling
      cout << "my_plot.x_ticks_values_color() " << my_plot.x_ticks_values_color() << endl;
      cout << "my_plot.x_ticks_values_font_family() " << my_plot.x_ticks_values_font_family() << endl;
      cout << "my_plot.x_ticks_values_color() " << my_plot.x_ticks_values_color() << endl;
      cout << "my_plot.x_ticks_values_precision() " << my_plot.x_ticks_values_precision() << endl;
      cout << "my_plot.x_ticks_values_ioflags() " << hex << my_plot.x_ticks_values_ioflags() << dec << endl;

      cout << "my_plot.y_ticks_values_color() " << my_plot.y_ticks_values_color() << endl;
      //cout << "my_plot.y_ticks_values_font_family() " << my_plot.y_ticks_values_font_family() << endl;
      cout << "my_plot.y_ticks_values_color() " << my_plot.y_ticks_values_color() << endl;
      cout << "my_plot.y_ticks_values_precision() " << my_plot.y_ticks_values_precision() << endl;
      cout << "my_plot.y_ticks_values_ioflags() " << hex << my_plot.y_ticks_values_ioflags() << dec << endl;

      // Show the X, Y styling.
      cout << "my_plot.x_values_color() " << my_plot.x_values_color() << endl;
      cout << "my_plot.x_values_font_family() " << my_plot.x_values_font_family() << endl;
      cout << "my_plot.x_values_color() " << my_plot.x_values_color() << endl;
      cout << "my_plot.x_values_precision() " << my_plot.x_values_precision() << endl;
      cout << "my_plot.x_values_ioflags() " << hex << my_plot.x_values_ioflags() << dec << endl;
    }
    { // Plot another graph.
      svg_2d_plot my_plot; // Construct a 2D plot.

      my_plot.x_size(2000)
             .image_y_size(400)
             .legend_on(true) // Set title and legend, and X axis range.
             .title("Rainfall at Long: 2:45:16.2W, Lat:54:17:47.1N")
             .x_range(0., 2000.)
             .x_major_interval(500.)
             .x_ticks_values_color(red)
             .x_num_minor_ticks(4)
             .x_axis_label_color(green)
             .x_ticks_values_precision(0)
             .x_ticks_values_ioflags(ios_base::fixed)
             .x_ticks_values_font_family("arial")
             .x_ticks_values_font_size(20)
             //.autoscale_check_limits(true) // Is default, but check for NaN, infinity etc.
             //.xy_autoscale(in_temps) // Autoscale BOTH axes.
             // Implementation does not (yet) permit use of the container for .x_autoscale(in_temps),
             // but can ignore the autoscale range thus:
             .y_range(0., 10.) // User chosen range, over-riding the autoscale.
             .y_ticks_values_color(magenta)
             .y_ticks_values_precision(1)
             //.y_ticks_values_ioflags(ios_base::scientific)
             .y_axis_label_color(blue)
             ;

      my_plot.x_label("time (hr)").y_label("rain (mm/hr)"); // Note chaining.
      my_plot.plot(rain_hours, "rain (mm/hr)").stroke_color(red).line_on(true).line_color(red);

      my_plot.write("./demo_2d_weather_2.svg"); // Write the plot to file.


    }
  }
  catch(const std::exception& e)
  {
    std::cout <<
      "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  }

  return 0;
} // int main()

//] [/demo_2d_weather_2]


