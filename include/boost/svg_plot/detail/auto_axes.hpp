/*!
  \file auto_axes.hpp
  \brief Scalable Vector Graphic (SVG) autoscaling of axes.
  \details Inspect container or data values to find minimum and maximum,
    avoiding values that are NaN and/or 'at limit'.
    Scale axis using max and min values (calculated or user provided),
    optionally to include the orgin, and to set the ticks.
    Provide fine control over any overlap at the edges of the axes to avoid a tiny
    amount over the limit resulting in an ugly extra major tick.
    Also allow optional forcing of the ticks to be multiples of 1, 2, 5, 10.
  \author
*/

// Copyright Paul A. Bristow 2006 - 2013, 2021.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_AUTO_AXES_HPP
#define BOOST_SVG_AUTO_AXES_HPP

#include <boost/svg_plot/detail/numeric_limits_handling.hpp>
#include <boost/svg_plot/detail/fp_compare.hpp> // is_small & is_close
#include <boost/quan/meas.hpp> // for boost::quan::value_of.

//#include <boost/math/special_functions/fpclassify.hpp>
// for template <class FPT> bool boost::math::isfinite(FPT t);
#include <boost/algorithm/minmax_element.hpp>
 using boost::minmax_element;
 // minmax_element finds both min and max elements more efficiently than separately.

#include <cmath> // using std::fabs, std::pow, std::ceil, std::log10
#include <limits> // using std::numeric_limits;
#include <stdexcept> // using std::domain_error; sdtd::runtime_error;
#include <iterator> // using std::iterator_traits;
#include <utility> // using std::pair; using std::make_pair;

// Autoscaling algorithm derived from:
// Michael P.D. Bramley. CUJ July 2000, p 20 - 26.
// Antonio Gomiz Bas, CUJ march 2000, p 42 - 45
// J. A. Nelder and W. Douglas Stirling, FORTRAN program SCALE.
//  // Algorithm AS 168: Scale Selection and Formatting  W.Douglas Stirling
// Journal of the Royal Statistical Society.Series C(Applied Statistics)
// Vol. 30, No. 3 (1981), pp. 339-344 (6 pages)
// Published By: Wiley
// Journal of the Royal Statistical Society.Series C(Applied Statistics)
// https://doi.org/10.2307/2346366
// https://www.jstor.org/stable/2346366
// Algorithm AS 96 https://doi.org/10.2307/2346537 J. A. Nelder, Simple Algorithm for Scaling Graphs
// http://lib.stat.cmu.edu/apstat/96   FORTRAN code

namespace boost
{
namespace svg
{

// Forward Declarations:

// Show and show_all to display size and contents of STL containers.
// range_mx and range_all to find the min and max of STL containers.
// _all versions deal with a container of containers.

// Round value up to nearest multiple of 10. Round up and round down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems:
//
double roundup10(double value);
 // Round value down to nearest multiple of 10.
double rounddown10(double value);
// Round value up to nearest multiple of 5.
double roundup5(double value);
 // Round value down to nearest multiple of 5.
double rounddown5(double value);
 // Round value up to nearest multiple of 2.
double roundup2(double value);
// Round value down to nearest multiple of 2.
double rounddown2(double value);

// Four versions of scale_axis with different parameter combinations.

// Scale axis and update min and max axis values, and tick increment and number of ticks.
void scale_axis(
   double min_value, //!< Scale axis from explicit input minimum.
   double max_value, //!< Scale axis from explicit input maximum.
   double* axis_min_value, //!< Computed minimum value for the axis, updated by scale_axis.
   double* axis_max_value, //!<  Computed maximum value for the axis, updated by scale_axis.
   double* axis_tick_increment, //!<  Computed tick increment for the axis, updated by scale_axis.
   int* auto_ticks, //!< Computed number of ticks, updated by scale_axis.
   //  NO check_limits parameter.
   bool origin = false, //!< Do not include the origin unless the range min_value <= 0 <= max_value.
   double tight = 0., //!< Tightness - fraction of overrun allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
   int min_ticks = 6, //!< Minimum number of major ticks.
   int steps = 0 //!< Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
);

/* Scale axis function to define axis marker ticks based on min & max parameters values (handling uncertainty).

  \param min_value Scale axis from explicit input minimum.
  \param max_value Scale axis from explicit input maximum.

  \param axis_min_value Computed minimum value for the axis, updated by scale_axis.
  \param axis_max_value Computed maximum value for the axis, updated by scale_axis.
  \param axis_tick_increment Computed tick increment for the axis, updated by scale_axis.
  \param auto_ticks  Computed number of ticks, updated by scale_axis.
  \param check_limits  If true then check all values for infinity, NaN etc.
  \param autoscale_plusminus Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  \param origin If true, ensures that zero is a tick value.
  \param tight  Allows user to avoid a small fraction over a tick using another tick.
  \param min_ticks  Minimum number of major ticks.
  \param steps Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
 */
void scale_axis(
  double min_value, // Scale axis from explicit input minimum.
  double max_value, // Scale axis from explicit input maximum.
  double* axis_min_value, // Minimum value for the axis, updated by scale_axis.
  double* axis_max_value, //  Maximum value for the axis, updated by scale_axis.
  double* axis_tick_increment, //  Tick increment for the axis, updated by scale_axis.
  int* auto_ticks,  // Updated with number of ticks.
  bool check_limits = true, // Whether to check all values for infinity, NaN etc.
  double autoscale_plusminus = 2., // Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  bool origin = false, // If true, ensures that zero is a tick value.
  double tight = 0., // Allows user to avoid a small fraction over a tick using another tick.
  int min_ticks = 6, // Minimum number of ticks.
  int steps = 0 // Round up and down to 2, 4, 6, 8, 10, or 5, 10, or 2, 5, 10 major ticks multiples.
);

/* Scale axis using an iterator into an STL container.

 \details Scale axis from data-series values (usually to then plot), perhaps using only part of container.

 \tparam iter Iterator into an STL container: array, vector, set ...
 \param begin Iterator into @c begin in STL container.
 \param end iterators into @c end in STL container.
 \param axis_min_value Computed minimum value for the axis, updated by scale_axis.
 \param axis_max_value Computed maximum value for the axis, updated by scale_axis.
 \param axis_tick_increment Computed tick increment for the axis, updated by scale_axis.
 \param auto_ticks  Computed number of ticks, updated by scale_axis.
 \param check_limits  If true then check all values for infinity, NaN etc.
 \param autoscale_plusminus Mutiplier of uncertainty or standard deviations to allow for confidence ellipses.
 \param origin If true, ensures that zero is a tick value.
 \param tight  Allows user to avoid a small fraction over a tick using another tick.
 \param min_ticks  Minimum number of major ticks.
 \param steps Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
*/

template <typename Iter>
void scale_axis(
  Iter begin, // Iterator into begin in STL container.
  Iter end, // Iterator into end in STL container.
  // (not necessarily ordered by size, so will find min and max).
  double* axis_min_value, // Computed minimum value for the axis, updated by scale_axis.
  double* axis_max_value, //  Computed maximum value for the axis, updated by scale_axis.
  double* axis_tick_increment, //  Computed tick increment for the axis, updated by scale_axis.
  int* auto_ticks, // Computed number of ticks, updated by scale_axis.
  bool check_limits, // Whether to check all values for infinity, NaN etc.
  double autoscale_plusminus, // Mutiplier of uncertainty or standard deviations to allow for confidence ellipses.
  bool origin = false, // Do not include the origin unless the range min_value <= 0 <= max_value.
  double tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
  // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
  // allowing values just 1 pixel over the tick to be shown.
  int min_ticks = 6, // Minimum number of major ticks.
  int steps = 0 // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
);

/* Scale axis using all the 1D data in an STL container.
  Scale axis using an \b entire Container data-series, usually to plot.
  (not necessarily ordered, so will find min and max).
  \tparam C an STL container: array, vector ...
  \param container STL container, usually of a data-series.
  \param axis_min_value Computed minimum value for the X-axis, updated by scale_axis.
  \param axis_max_value Computed minimum value for the X-axis, updated by scale_axis.
  \param axis_tick_increment Computed tick increment for the axis, updated by scale_axis.
  \param auto_ticks Computed number of ticks, updated by scale_axis.
  \param check_limits Whether to check all values for infinity, NaN etc.
  \param autoscale_plusminus Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  \param origin Do not include the origin unless the range min_value <= 0 <= max_value.
  \param tight fraction of 'overrun' allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
  \param min_ticks Minimum number of major ticks.
  \param steps  0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
*/
 template <typename C>
 void scale_axis(
   const C& container, // STL container, usually of a data-series.
   double* axis_min_value, // Computed minimum value for the axis, updated by @c scale_axis.
   double* axis_max_value,  // Computed maximum value for the axis, updated by @c scale_axis.
   double* axis_tick_increment, //  Computed tick increment for the axis, updated by @c scale_axis.
   int* auto_ticks, // Computed number of ticks, updated by @c scale_axis.
   bool check_limits, // Whether to check all values for infinity, NaN etc.
   double autoscale_plusminus = 3., // Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
   bool origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
   double tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
   //! for visual effect up to about 0.001 might suit a 1000 pixel wide image,
   //! allowing values just 1 pixel over the tick to be shown.
   int min_ticks = 6, // Minimum number of major ticks.
   int steps = 0 // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
);

/* Scale X and Y axis using T a 2D STL container: array of pairs, vector of pairs, list of pairs, map ...
  \tparam C STL container of 2D pairs of X and Y.

  \param container data-series to plot - entire 2D container (not necessarily ordered, so will find min and max)..
  \param x_axis_min_value Computed minimum value for the X-axis, updated by @c scale_axis.
  \param x_axis_max_value Computed minimum value for the X-axis, updated by @c scale_axis.
  \param x_axis_tick_increment Computed tick increment for the axis, updated @c by scale_axis.
  \param x_auto_ticks Computed number of ticks, updated by scale_axis.
  \param y_axis_min_value  Computed minimum value for the Y-axis, updated by @c scale_axis.
  \param y_axis_max_value  Computed maximum value for the Y-axis, updated by @c scale_axis.
  \param y_axis_tick_increment Updated with Y axis tick increment.
  \param y_auto_ticks Computed number of Y-axis ticks, updated by scale_axis.
  \param check_limits Whether to check all values for infinity, NaN etc.
  \param autoscale_plusminus Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  \param x_origin Do not include the origin unless the range min_value <= 0 <= max_value.
  \param x_tight fraction of 'overrun' allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
  \param x_min_ticks Minimum number of X-axis major ticks.
  \param x_steps  0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
  \param y_origin  Do not include the origin unless the range min_value <= 0 <= max_value.
  \param y_tight fraction of 'overrun' allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
  \param y_steps  0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
  \param y_min_ticks Minimum number of Y axis major ticks.
*/
template <typename C>
void scale_axis(
  const C& container, // data-series to plot - \b entire 2D container (not necessarily ordered, so will find min and max).
  double* x_axis_min_value, //  \param x_axis_min_value Computed minimum value for the X-axis, updated by scale_axis.
  double* x_axis_max_value,  //   \param x_axis_max_value Computed minimum value for the X-axis, updated by scale_axis.
  double* x_axis_tick_increment, // Updated with X axis tick increment.
  int* x_auto_ticks,   // Computed number of X axis ticks, updated by @c scale_axis.
  double* y_axis_min_value, // Computed minimum value for the Y-axis, updated by @c scale_axis.
  double* y_axis_max_value,  // Computed maximum value for the Y-axis, updated by @c scale_axis.
  double* y_axis_tick_increment, // Computed Y axis tick increment. updated by @c scale_axis.
  int* y_auto_ticks,  // Computed number of Y-axis ticks, updated by @c scale_axis.
  bool check_limits = true, // Whether to check all values for infinity, NaN etc.
  double autoscale_plusminus = 3., // Mutiplier of uncertainty or standard deviations to allow fo confidence ellipses.
  bool x_origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
  double x_tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
  //! for visual effect up to about 0.001 might suit a 1000 pixel wide image,
  //! allowing values just 1 pixel over the tick to be shown.
  int x_min_ticks = 6, // Minimum number of major ticks.
  int x_steps = 0, // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
  bool y_origin = false, // do not include the origin unless the range min_value <= 0 <= max_value.
  double y_tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
  // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
  // allowing values just 1 pixel over the tick to be shown.
  int y_min_ticks = 6, // Minimum number of major ticks.
  int y_steps = 0 // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
);

//  End forward scale_axis and roundup and round down declarations.

// All use:
namespace detail
{ // Declaration of implementation with default parameters.
//* \cond DETAIL
void scale_axis_impl(double min_value, double max_value, // Scale axis from input range min & max.
               double* axis_min_value,  double* axis_max_value, double* axis_tick_increment,
               int* auto_ticks, // All 4 updated.
               // NO check_limits parameter in this version.
               bool origin = false, // Do not include the origin unless the range min_value <= 0 <= max_value.
               double tight = 0., // tightest - fraction of 'overrun' allowed before another tick used.
               // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
               // allowing values just 1 pixel over the tick to be shown.
               int min_ticks = 6, // Minimum number of major ticks.
               int steps = 0); // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
//! \endcond // DETAIL
} // namespace detail

// End of declarations.

// Definitions of scale_axis and roundup and round down.

 /*! \brief Inspect values to find min and max.
     \details Inspect all values between begin and (one before) end to work out and update min and max.
       Similar to boost::minmax_element, but ignoring at 'limit': non-finite, +-infinity, max & min, & NaN).
       If can't find a max and a min, then throw a runtime_error exception.
    \tparam iter InputIterator into STL container.
    \param begin Iterator to chosen first item in container.
    \param end Iterator to chosen last item in container.
    \param min Updated with Minimum value found (not 'at limit').
    \param max Updated with Maximum value found (not 'at limit').
    \return number of normal values (not 'at limit' neither too big, NaN nor infinite).
  */
template <typename Iter>
int mnmx(
  Iter begin, // iterator to chosen first item in container.
  Iter end,  // iterator to chosen last item in container.
  double* min, // Updated with Minimum value found (not 'at limit').
  double* max) // Updated with Maximum value found (not 'at limit').
{
  *max = std::numeric_limits<double>::quiet_NaN();
  *min = std::numeric_limits<double>::quiet_NaN();
  using boost::svg::detail::is_limit; // Either x and/or y not a proper data value.
  int goods = 0; // Count of values within limits.
  int limits = 0;
  Iter pos = begin;
  using boost::quan::value_of;
  while(pos != end && is_limit(value_of(*pos)))
  { // Count any limits before the first 'good' (FP normal) value.
    limits++;
    pos++;
  }
  if (pos == end)
  { // ALL values are at limit!
      throw std::runtime_error("Autoscale could not find any useful values to scale axis!");
    // std::cout << "all values at limit (NaN or infinity)!" << std::endl;
    // min and max are both == NaN
  }
  else
  {
    using boost::quan::value_of;
    double x = value_of(*pos);
    *max = x;
    *min = x;
    //std::cout << "Initial min & max " << x << std::endl;
    pos++;
    goods++;
    while(pos != end)
    {
      if (!is_limit(value_of(*pos)))
      { // x is finite.
        x = value_of(*pos);
        if (x > *max)
        {
          *max = x;
        }
        if (x < *min)
        {
          *min = x;
        }
        goods++;
        //std::cout << goods << " goods, " << x << std::endl;
     } // if finite
      else
      { // If x not finite, then the y value won't be plotted.
        std::cout << "limit value: " << *pos  << std::endl;
        limits++;
      }
      ++pos;
    } // while
    //std::cout << "min " << *min << ", max " << *max << std::endl; //
    //std::cout << "limits " << limits << std::endl;
  }
  if (goods < 2)
  {
    throw std::runtime_error("Autoscale could not find useful min & max to scale axis!");
  }
  return goods; // If goods < 2,
} // template <typename iter>int mnmx((iter begin, iter end, double* min, double* max)

/*! Scale axis and update min and max axis values, and tick increment and number of ticks.

   \param min_value Scale axis from explicit input range minimum.
   \param max_value Scale axis from explicit input range maximum.

   \param axis_min_value Computed minimum value for the axis, updated by scale_axis.
   \param axis_max_value Computed maximum value for the axis, updated by scale_axis.
   \param axis_tick_increment  Computed tick increment for the axis, updated by scale_axis.
   \param auto_ticks Computed number of ticks, updated by scale_axis.
   \param origin If false, do not include the origin unless the range @c min_value to @c max_value includes zero.
   \param tight Fraction of overrun allowed before another tick used. For a good visual effect, up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
   \param min_ticks Minimum number of major ticks.
   \param steps 0, or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
*/
void scale_axis(
   double min_value, // Scale axis from explicit input range minimum.
   double max_value, // Scale axis from input range maximum.
   double* axis_min_value, // Computed minimum value for the axis, updated by @c scale_axis.
   double* axis_max_value, //  Computed maximum value for the axis, updated by @c scale_axis.
   double* axis_tick_increment, //  Computed tick increment for the axis, updated by scale_axis.
   int* auto_ticks, // Computed number of ticks, updated by scale_axis.
   //  NO check_limits parameter.
   bool origin, // = false, // Do not include the origin unless the range min_value <= 0 <= max_value.
   double tight, // = 0., // Tightness - fraction of 'overrun' allowed before another tick used.
   // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
   // allowing values just 1 pixel over the tick to be shown.
   int min_ticks, //= 6, // Minimum number of major ticks.
   int steps // = 0 // Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
)
{
  detail::scale_axis_impl(min_value, max_value,
    axis_min_value, axis_max_value, axis_tick_increment, auto_ticks, // All 4 updated.
    origin, tight, min_ticks, steps); // Display range.
} //

/*! Scale axis function to define axis marker ticks based on min & max parameters values (handling uncertainty).

  \param min_value Scale axis from explicit input minimum.
  \param max_value Scale axis from explicit input maximum.

  \param axis_min_value Computed minimum value for the axis, updated by scale_axis.
  \param axis_max_value Computed maximum value for the axis, updated by scale_axis.
  \param axis_tick_increment Computed tick increment for the axis, updated by scale_axis.
  \param auto_ticks  Computed number of ticks, updated by scale_axis.
  \param check_limits  If true then check all values for infinity, NaN etc.
  \param autoscale_plusminus Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  \param origin If true, ensures that zero is a tick value.
  \param tight  Allows user to avoid a small fraction over a tick using another tick.
  \param min_ticks  Minimum number of major ticks.
  \param steps Round up and down to 2, 4, 6, 8, 10, or 5, 10 or 2, 5, 10 systems.
 */
void scale_axis(
   double min_value, // Updated with Minimum value found.
   double max_value, // Updated with Maximum value found.
   double* axis_min_value, // Minimum value for the axis, updated by scale_axis.
   double* axis_max_value, //  Maximum value for the axis, updated by scale_axis.
   double* axis_tick_increment, //  Tick increment for the axis, updated by scale_axis.
   int* auto_ticks,  // Updated with number of ticks.
   bool check_limits, // Whether to check all values for infinity, NaN etc.
   double autoscale_plusminus, // Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
   bool origin, // If true, ensures that zero is a tick value.
   double tight, // Allows user to avoid a small fraction over a tick using another tick.
   int min_ticks, // Minimum number of ticks.
   int steps) // Round up and down to 2, 4, 6, 8, 10, or 5, 10, or 2, 5, 10 major ticks multiples.
{
  // Must assume max and min are OK (can't ignore limit values).
  // If either at limit then will be caught and exception thrown later by x_range.
  // So deliberately ignore check_limits parameter & and autoscale_plusminus & suppress any warnings.
  detail::scale_axis_impl(min_value, max_value,
    axis_min_value, axis_max_value, axis_tick_increment, auto_ticks, // All 4 updated.
    origin, tight, min_ticks, steps); // Display range.
}

 /*! Scale axis from data-series (usually to plot), perhaps only part of container.

   \tparam Iter Type of interator into STL container type: @c array, @c vector ...

   \param begin First item in container to use to calculate autoscale mimimum or maximum.
   \param end Last item in container to use to calculate autoscale mimimum or maximum.
   \param axis_min_value Computed minimum value for the axis, updated by scale_axis.
   \param axis_max_value Computed maximum value for the axis, updated by scale_axis.
   \param axis_tick_increment  Computed tick increment for the axis, updated by scale_axis.
   \param auto_ticks Computed number of ticks, updated by scale_axis.
   \param check_limits Whether to check all values for infinity, NaN etc.
   \param autoscale_plusminus Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
   \param origin If false, do not include the origin unless the range min_value <= 0 <= max_value.
   \param tight fraction of 'overrun' allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
   \param min_ticks Minimum number of major ticks.
   \param steps 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
 */
template <typename Iter>
void scale_axis(
   Iter begin, // iterators into begin in STL container.
   Iter end, // iterators into end in STL container.
   // (not necessarily ordered by size, so will find min and max).
   double* axis_min_value, // Computed minimum value for the axis, updated by scale_axis.
   double* axis_max_value, //  Computed maximum value for the axis, updated by scale_axis.
   double* axis_tick_increment, //  Computed tick increment for the axis, updated by scale_axis.
   int* auto_ticks, // Computed number of ticks, updated by scale_axis.
   bool check_limits, // Whether to check all values for infinity, NaN etc.
   double autoscale_plusminus, // Mutiplier of uncertainty or standard deviations to allow fo confidence ellipses.
   bool origin, // = false, // Do not include the origin unless the range min_value <= 0 <= max_value.
   double tight, // = 0., // tightest - fraction of 'overrun' allowed before another tick used.
   // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
   // allowing values just 1 pixel over the tick to be shown.
   int min_ticks, // = 6, // Minimum number of major ticks.
   int steps) // = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{
  double x_min;
  double x_max;
  if (!check_limits)
  { // minmax_element is efficient for maps because can use knowledge of being sorted,
    // BUT only if it can be assumed that no values are 'at limits',
    // infinity, NaN, max_value, min_value, denorm_min.
    // Otherwise it is necessary to inspect all values individually.
    std::pair<Iter, Iter> result = boost::minmax_element(begin, end); // min & max
    // scale_axis (not check_limits version) forward declaration to ensure compiler finds right version.
     x_min = *(result.first);
     x_max = *(result.second);
  }
  else
  { // Must check limits.
    int good = mnmx(begin, end, &x_min, &x_max);
    if (good < 2)
    {
      throw std::runtime_error("Autoscale could not find useful min & max to scale axis!");
    }
    detail::scale_axis_impl(x_min, x_max,
    axis_min_value, axis_max_value, axis_tick_increment, auto_ticks, // All 4 updated.
      origin, tight, min_ticks, steps); // Display range.
  }
} // template <typename iter> void scale_axis(iter begin, iter end, ...

/*!
  \brief Scale axis using an \b entire Container of a data-series, usually to plot (not necessarily ordered, so will find minimum and maximum).

  \tparam C STL container type: @c array, @c vector ...

  \param container STL container, usually of a data-series.
  \param axis_min_value Computed minimum value for the axis, updated by scale_axis.
  \param axis_max_value Computed maximum value for the axis, updated by scale_axis.
  \param axis_tick_increment  Computed tick increment for the axis, updated by scale_axis.
  \param auto_ticks Computed number of ticks, updated by scale_axis.
  \param check_limits Whether to check all values for infinity, NaN etc.
  \param autoscale_plusminus Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  \param origin If false, do not include the origin unless the range min_value \<= 0 \<= max_value.
  \param tight fraction of overrun allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
  \param min_ticks Minimum number of major ticks.
  \param steps 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
*/
template <class C>
void scale_axis(
  const C& container, // STL container, usually of a data-series.
  double* axis_min_value, // Computed minimum value for the axis, updated by scale_axis.
  double* axis_max_value,  // Computed maximum value for the axis, updated by scale_axis.
  double* axis_tick_increment, //  Computed tick increment for the axis, updated by scale_axis.
  int* auto_ticks, // Computed number of ticks, updated by scale_axis.
  bool check_limits, // Whether to check all values for infinity, NaN etc.
  double autoscale_plusminus, // = 3., Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  bool origin, // = false, // do not include the origin unless the range min_value to max_value includes zero.
  double tight, // = 0., // tightest - fraction of overrun allowed before another tick used.
  // For a good visual effect, up to about 0.001 might suit a 1000 pixel wide image,
  // allowing values just 1 pixel over the tick to be shown.
  int min_ticks, // = 6, // Minimum number of major ticks.
  int steps) // = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{ // s
  double x_min;
  double x_max;
  if (!check_limits)
  {
    //std::pair<T::iterator, T::iterator> result = boost::minmax_element(container.begin(), container.end());
    std::pair<typename C::const_iterator, typename C::const_iterator> result = boost::minmax_element(container.begin(), container.end());
    // minmax_element is efficient because can use knowledge of being sorted,
    // BUT only if it can be assumed that no values are 'at limits',
    // infinity, NaN, max_value, min_value, denorm_min.
    using boost::quan::value_of;
    x_min = value_of(*(result.first));
    x_max = value_of(*(result.second));
  }
  else
  { // It is necessary to inspect all values individually.
    // std::cout << container.size() << " values." << std::endl;
    // Work out min and max, ignoring non-finite, +-infinity, max & min, & NaN).
    // If can't find a max and a min, then will throw exception.
    int good = mnmx(container.begin(), container.end(), &x_min, &x_max);
    if (good < 2)
    {
      throw std::runtime_error("Autoscale could not find useful min & max values to scale the X axis!");
    }
    // std::cout << "x_min " << x_min << ", x_max " << x_max << std::endl; //
  }

  detail::scale_axis_impl(x_min, x_max,
    axis_min_value, axis_max_value, axis_tick_increment, auto_ticks,
    origin, tight, min_ticks, steps);

} // template <class C> int scale_axis  C an STL container: array, vector ...

/*! Scale X and Y axis using a 2D STL container: @c std::array of @c std::pairs, @c std::vector of @c std::pairs, ...

  \tparam C STL container holding 2D pairs of X and Y.

  \param container data-series to plot - entire 2D container.
  \param x_axis_min_value Computed minimum value for the X-axis, updated by scale_axis.
  \param x_axis_max_value Computed minimum value for the X-axis, updated by scale_axis.
  \param x_axis_tick_increment Computed tick increment for the axis, updated by scale_axis.
  \param x_auto_ticks Computed number of ticks, updated by scale_axis.
  \param y_axis_min_value  Computed minimum value for the Y-axis, updated by scale_axis.
  \param y_axis_max_value  Computed maximum value for the Y-axis, updated by scale_axis.
  \param y_axis_tick_increment Updated with Y-axis tick increment.
  \param y_auto_ticks Computed number of Y-axis ticks, updated by scale_axis.
  \param check_limits Whether to check all values for infinity, NaN etc.
  \param autoscale_plusminus Multiplier of uncertainty or standard deviations to allow for confidence ellipses.
  \param x_origin Do not include the origin unless the range min_value <= 0 <= max_value.
  \param x_tight Fraction of 'overrun' allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
  \param x_min_ticks Minimum number of X-axis major ticks.
  \param x_steps  0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
  \param y_origin Do not include the origin unless the range min_value to max_value contains zero.
  \param y_tight Fraction of 'overrun' allowed before another tick used. For visual effect up to about 0.001 might suit a 1000 pixel wide image, allowing values just 1 pixel over the tick to be shown.
  \param y_min_ticks Minimum number of Y axis major ticks.
  \param y_steps 0, or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
*/
template <class C>
void scale_axis(
  const C& container, // data-series to plot - \b entire 2D container (not necessarily ordered, so will find min and max).
  double* x_axis_min_value, // Computed minimum value for the X-axis, updated by scale_axis.
  double* x_axis_max_value,  // Computed minimum value for the X-axis, updated by scale_axis.
  double* x_axis_tick_increment, // Updated with X axis tick increment.
  int* x_auto_ticks,   // Computed number of X axis ticks, updated by scale_axis.
  double* y_axis_min_value, // Computed minimum value for the Y-axis, updated by scale_axis.
  double* y_axis_max_value,  // Computed maximum value for the Y-axis, updated by scale_axis.
  double* y_axis_tick_increment, // Updated with Y axis tick increment.
  int* y_auto_ticks,  // Computed number of Y-axis ticks, updated by scale_axis.
  bool check_limits, // = true, // Whether to check all values for infinity, NaN etc.
  double autoscale_plusminus, // = 3., // Mutiplier of uncertainty or standard deviations to allow fo confidence ellipses.
  bool x_origin, // = false, // do not include the origin unless the range min_value <= 0 <= max_value.
  double x_tight, // = 0., // tightest - fraction of 'overrun' allowed before another tick used.
  // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
  // allowing values just 1 pixel over the tick to be shown.
  int x_min_ticks, // = 6, // Minimum number of major ticks.
  int x_steps, // = 0, // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
  bool y_origin, // = false, // do not include the origin unless the range min_value <= 0 <= max_value.
  double y_tight, // = 0., // tightest - fraction of 'overrun' allowed before another tick used.
  // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
  // allowing values just 1 pixel over the tick to be shown.
  int y_min_ticks, // = 6, // Minimum number of major ticks.
  int y_steps) // = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{ /* Scale X and Y axis using T a 2D STL container: array of pairs, vector of pairs, list of pairs, map ...
      container data-series to plot - entire 2D container.
    */
  double x_max = std::numeric_limits<double>::quiet_NaN();
  double x_min = std::numeric_limits<double>::quiet_NaN();
  double y_max = std::numeric_limits<double>::quiet_NaN();
  double y_min = std::numeric_limits<double>::quiet_NaN();

  if (!check_limits)  // TODO my_plot.autoscale_check_limits(false);
  { // BUT only if it can be assumed that no values are 'at limits',
    // infinity, NaN, max_value, min_value, denorm_min.
    // minmax_element is efficient for maps because it can use knowledge of all maps being sorted,
    // And also sadly it doesn't work right - Y minimum is wrong!  // TODO ??

    std::pair<typename C::const_iterator, typename C::const_iterator> result
      = boost::minmax_element(container.begin(), container.end());
    using boost::quan::values_of;
    std::pair<double, double> px = values_of(*result.first); // X min & X max.
    std::pair<double, double> py = values_of(*result.second); // Y min & Y max.
    x_min = px.first;
    x_max = py.first;
    // x are OK, but Y are only those corresponding to those X, not min and max.,
    // so still need to iterate through the Y to find y min and y max.
    //y_min = px.second;
    //y_max = py.second;
    typename C::const_iterator pos = container.begin();
    if (pos == container.end())
    {
      throw std::runtime_error("SVG_plot Autoscale could not find any values to scale axes!");
    }
    using boost::quan::value_of;
    double y = value_of(pos->second);
    using boost::quan::unc_of;
    double yu = unc_of(pos->second) * autoscale_plusminus;
    y_max = y + yu;
    y_min = y - yu;
    while(pos != container.end())
    {
      y = value_of(pos->second);
      yu = unc_of(pos->second) * autoscale_plusminus;
      if (y + yu > y_max)
      {
        y_max = y + yu;
      }
      if (y - yu < y_min)
      {
        y_min = y - yu;
      }
      pos++;
    }
    std::cout << "SVG_plot Autoscale warning (No limits checks):"
                 "x_min = " << x_min
            << ", x_max = " << x_max
            << ", y_min = " << y_min
            << ", y_max = " << y_max << std::endl;
  }
  else
  { // Otherwise it is necessary to inspect all values individually.
    // It seems that X and Y need to be examined in pairs, so sadly, we can't use:
    // int good_x = mnmx(container.begin(), container.end(), &x_min, &x_max);
    // or
    // int good_y = mnmx(container.begin(), container.end(), &y_min, &y_max);

    // Work out min and max, ignoring non-finite (+-infinity & NaNs).
    using boost::svg::detail::pair_is_limit; // Either x and/or y is not a proper data value.
    int goods = 0; // Count of values where both X and Y are normal (within limits).
    int limits = 0;// Count of values where both X and Y are at limits (not normal).
    typename C::const_iterator pos = container.begin();
    while(pos != container.end() && pair_is_limit(*pos))
    { // Count any limits before the first good.
      limits++;
      pos++;
    }
    if (pos == container.end())
    { // ALL values are at limit!
      //std::cout << "all values at limit" << std::endl;
      throw std::runtime_error("Autoscale could not find any useful values to scale axes!");
    }
    else
    {
      using boost::quan::value_of;
      double x = value_of(pos->first);
      using boost::quan::unc_of;
      double xu = unc_of(pos->first) * autoscale_plusminus;
      x_max = x + xu;
      x_min = x - xu;
      double y = value_of(pos->second);
      double yu = unc_of(pos->second) * autoscale_plusminus;
      y_max = y + yu;
      y_min = y - yu;
      //std::cout << "Initial min & max " << x << "+-" << xu << " = " << x_min << " to " << x_max << ", " << y << "+-" << yu << "=" <<y_min << " to " << y_max << std::endl;
      pos++;
      goods++;
      while(pos != container.end())
      {
        if (!pair_is_limit(*pos))
        { // Either x and/or y are finite.
          x = value_of(pos->first);
          xu = unc_of(pos->first) * autoscale_plusminus;
          if (x + xu > x_max)
          {
            x_max = x + xu;
          }
          if (x - xu < x_min)
          {
            x_min = x - xu;
          }
          y = value_of(pos->second);
          yu = unc_of(pos->second) * autoscale_plusminus;
          if (y + yu > y_max)
          {
            y_max = y + yu;
          }
          if (y - yu < y_min)
          {
            y_min = y - yu;
          }
          //std::cout << "min & max " << x << "+-" << xu << " = " << x_min << " to " << x_max << ", " << y << "+-" << yu << "=" <<y_min << " to " << y_max << std::endl;
          goods++;
          // std::cout << goods << " goods, " << x << ' ' << y << std::endl;
       } // if finite
        else
        { // If either are not finite, then neither useful for autoscaling.
          // If x not finite, then the y value won't be plotted.
          // If y value not finite, then it will be 'off limits'.
          // std::cout << "limit value: " << pos->first << ' ' << pos->second  << std::endl;
          limits++;
        }
        ++pos;
      } // while
      std::cout << "Checked: x_min " << x_min << ", x_max " << x_max << ", y_min " << y_min << ", y_max " << y_max << ", " << goods << " 'good' values, " << limits << " values at limits."<< std::endl;
    }
  }
  detail::scale_axis_impl(x_min, x_max,
    x_axis_min_value, x_axis_max_value, x_axis_tick_increment, x_auto_ticks,
    x_origin, x_tight, x_min_ticks, x_steps);

  detail::scale_axis_impl(y_min, y_max,
    y_axis_min_value, y_axis_max_value, y_axis_tick_increment, y_auto_ticks,
    y_origin, y_tight, y_min_ticks, y_steps);

} // template <class T> int scale_axis  T an STL container: array, vector ...

// Above versions all use the scale_axis_impl implementation below that does the real scaling work.
namespace detail
{ // Definition of implementation with default parameters.
//! @cond DETAIL

void scale_axis_impl(
  double min_value,
  double max_value, // Scale axis from Input range min & max.
  double* axis_min_value,
  double* axis_max_value,
  double* axis_tick_increment,
  int* auto_ticks, // All 4 updated.
               // NO check_limits parameter in this version.
  bool origin, // = false, // Do not include the origin unless the range min_value <= 0 <= max_value.
  double tight, // = 0., // tightest - fraction of 'overrun' allowed before another tick used.
               // for visual effect up to about 0.001 might suit a 1000 pixel wide image,
               // allowing values just 1 pixel over the tick to be shown.
  int min_ticks, // = 6, // Minimum number of major ticks.
  int steps) // = 0) // 0,  or 2 for 2, 4, 6, 8, 10, 5 for 1, 5, 10, or 10 (2, 5, 10).
{
  int ticks = -1; // Negative to warn of 'bad' value.
  double test_max;
  double test_min;
  double test_increment;
  switch (steps)
  { // Optionally expand the range by rounding actual max and min values up and down.
  case 0 :
    break; // No steps.
  case 10 :
    max_value = roundup10(max_value);
    min_value = rounddown10(min_value);
    break;
  case 5 :
    max_value = roundup5(max_value);
    min_value = rounddown5(min_value);
    break;
  case 2 :
    max_value = roundup2(max_value);
    min_value = rounddown2(min_value);
    break;
  default:
    throw std::domain_error("Unimplemented steps!");
  } // switch
  double range = max_value - min_value;  // range of data.

  smallest<> is_small(1000. * (std::numeric_limits<double>::min)()); // 1000 * min value
  close_to<> is_near_100eps(100. * (std::numeric_limits<double>::epsilon)()); // 100 * epsilon

  if ((tight < 0.) || tight > 1.)
  { // Tight can't be negative and > 1 is very likely a mistake, 0.01 = 1% more reasonable.
    throw std::domain_error("tight not in range 0 to 1 !");
  }

  using std::isfinite;
  if(!(isfinite)(min_value))
  {
    throw std::domain_error("min_value not finite!");
  }
  if(!(isfinite)(max_value))
  {
    throw std::domain_error("max_value not finite!");
  }
  if (origin == true)
  { // Ensure the axis includes zero.
    if (min_value > 0.)
    { // All positive case.
      min_value = 0.;
    }
    else if(max_value < 0.)
    { // All negative case.
      max_value = 0.;
    }
  } // origin

  if (min_value > max_value)
  { // max and min are transposed!
    throw std::domain_error("min > max!");
  }
  else if (is_small(range)
    // range <= 1000. * std::numeric_limits<double>::min()) // Absolute range > ~1e-308 * 1000 ~= 1e-305
    // Range has already been checked to be > 0.
    // This checks for range too near to zero to be useful.
    // is_small is similar to Boost.Test check_is_small

    || is_near_100eps(max_value, min_value)

    //|| (range <= 100. * std::numeric_limits<double>::epsilon() * abs(min_value))
    // This checks the relative range is not too near to epsilon to be useful.
    // Knuth Vol II, avoiding over and underflow, as used in Boost.Test close_at_tolerance.
    )
  { // Factor of 1000 is to ensure range is more than a few epsilon relative wide.
    // Special cases of max ~== min values *and* exactly max == min (including == 0).
    // This could be two or more duplicate (repeat) measurements on x or y axis,
    // or only a modest number of epsilons apart even,
    // so it not necessarily an error, but some special handling is required.
    // At what point does range become big enough to be plausible to provide axis max & min tick values?
    // A few numeric_limits<double>::epsilon() only covers smallish compute errors.
    // at least 1000 epsilon absolute seems more plausible, and relative to biggest?
    // Return 3 ticks: -1, max==min==mid, and +1 ticks.
    // But uncertain if this is best solution?

    double mean = (min_value + max_value) /2;
    test_increment = 1;
    test_min = mean - test_increment;
    test_max = mean + test_increment;

    ticks = 3; // ticks - OK, but ticks == 3 warns that max_value ~== min_value.
  }
  else
  { // Range is reasonably large, so
    // compute candidate for increment - must be smaller than range, so divide by 10.
    test_increment = std::pow(10., std::ceil(std::log10(std::abs<double>(range)/10.)));
    // Must be a decimal multiple or decimal fraction,
    // but is not necessarily exactly representable in floating-point format.
    // Establish maximum axis scale value, using this increment.

    test_max = (static_cast<long>(max_value / test_increment)) * test_increment;

    if(test_max < max_value)
    {
      test_max += test_increment;
    }
    ticks = 1; // Must be 1 'extra' tick at the end.
    // Establish minimum axis tick value by decrementing from test_max.
    test_min = test_max;
    do
    {
      ticks++;
      test_min -= test_increment;
    }
    while (test_min > min_value); // min_value);

    // Subtracting small values can screw up the scale limits,
    // eg: if scale_axis is called with (min, max)=(0.01, 0.1),
    // then the calculated scale is 1.0408E17 TO 0.05 BY 0.01,
    // rather than 0, 0.05, 0.01.
    // I suspect 1.e-10 is bigger than necessary?  related to std::numeric_limits<>::epsilon?
    if(std::abs(test_min) < 1.E-14)
    { // test_min is very near zero,
      test_min = 0.; // so treat as exact zero to avoid risk of a switch to e format.
    }
    while(ticks < min_ticks)
    {  // Adjust for too few tick marks by
      test_increment /= 2.; // halving the increment.
      // (divide by two should not cause trouble by being inexact).
      ticks = static_cast<int>((test_max - test_min) / test_increment) +1;
      if (steps == 0)
      { // Remove any superfluous ticks above max and below min.
        while((test_min + test_increment) <= min_value)
        { // min_value is > 2nd from bottom tick,
          test_min += test_increment;
          ticks--; // so we can scrap the 1st bottom tick.
        }
        while((test_max - test_increment) >= max_value)
        { // max_value is > top_but_one tick,
          ticks--; // so ditch the top tick.
          test_max -= test_increment;
        }
      }
    } // while

    if (tight > 0.)
    { // Check that can't use a tick less at top or bottom.
      double max_plus_margin = test_max - test_increment + test_increment * tight;
      if (max_value < max_plus_margin)
      { // max is too big, so remove top tick.
        ticks -= 1;
        test_max -= test_increment;
      }

      double min_plus_margin = test_min + test_increment - test_increment * tight;
      if (min_value > min_plus_margin)
      { // min is too small, so remove the bottom tick.
        ticks -= 1;
        test_min += test_increment;
      }
      // Check again to make quite sure can't reduce again.
      max_plus_margin = test_max - test_increment + test_increment * tight;
      if (max_value < max_plus_margin)
      { // max is too big
        ticks -= 1;
        test_max -= test_increment;
      }
      min_plus_margin = test_min + test_increment - test_increment * tight;
      if (min_value > min_plus_margin)
      { // min is too small, so remove the bottom tick.
        ticks -= 1;
        test_min += test_increment;
      }
    } // if (tight != 0.)
  } // range reasonable

  // Pass computed min & max axis tick values back to caller.
  *axis_min_value = test_min; //
  *axis_max_value = test_max;
  *axis_tick_increment = test_increment; // major_tick_interval.
  *auto_ticks = ticks; // major ticks.
} // scale_axis_impl

//! @endcond // DETAIL

} // namespace detail
// Utility functions to display containers
// and to find min and max values in containers.

template <typename T> // T an STL container: array, vector ...
size_t show(const T& container)
{ //! Utility functions to display STL containers.
  std::cout << container.size() << " values in container: ";
  for (typename T::const_iterator it = container.begin(); it != container.end(); it++)
  {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;
  return container.size();
}// Container data-series to plot.

// Pointer version is not needed - iterator version is used instead.

template <typename iter> // T an STL container: array, vector ...
size_t show(iter begin, iter end) // Iterators
{ //! Utility function to display STL containers.
  size_t count = 0;
  while (begin != end)
  {
    count++;
    std::cout << *begin << ' ';
    ++begin;
  }
  std::cout << ": " << count << " values used.";
  std::cout << std::endl;
  return count;
}// Container data-series to plot.

template <typename T>
size_t show_all(const T& containers)
{ //! Show all the containers values.
  // \tparam T an STL container: container of containers.
  for (typename T::const_iterator it = containers.begin(); it != containers.end(); it++)
  {
    show(*it);
  }
  return containers.size();
} // Container data-series to plot.

  /*! Calculate minimum and maximum from data in a container.
    \param container Container data-series.
    \return minimum and maximum of an STL container as a @c std::pair.
    \tparam  T an STL container: array, vector, set, map ...
  */
template <class T>
std::pair<double, double> range_mx(const T& container)
{
  std::pair<typename T::const_iterator, typename T::const_iterator> result
    = boost::minmax_element(container.begin(), container.end());
  std::pair<double, double> minmax;
  minmax.first = *result.first;
  minmax.second = *result.second;
  return minmax;
} // template <class T> range_mx

template <typename T> // T an STL container: array, vector, set, map ...
std::pair<double, double> range_all(const T& containers) // Container of STL containers of data-series.
{ /*! \return minimum and maximum of a container containing STL containers.
      \tparam T an STL container: array, vector, set, map ...
  */
  std::pair<double, double> minmax(
    (std::numeric_limits<double>::max)(), (std::numeric_limits<double>::min)()
  );
  for (typename T::const_iterator it = containers.begin(); it != containers.end(); it++)
  {
    std::pair<double, double> mm = range_mx(*it); // Scale of this container.
    minmax.first = (std::min)(mm.first, minmax.first); //
    minmax.second = (std::max)(mm.second, minmax.second);
  }
  return minmax;
} // template <class T> scale_all

/*! Round up to nearest multiple of 10.
  Decimal scaling steps, so value is 0.1, 0.2, 0.5, 1., 2., 5. or 1., 10., 20., 100. ...
  \return Rounded up value.
*/
double roundup10(double value)
{
  BOOST_MATH_STD_USING
  smallest<> is_small(100. * (std::numeric_limits<double>::min)()); // 100 * min value.
  if (is_small(value) )
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = std::abs(value);

  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled_value = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.
  if(scaled_value > 5.)
  {
    return 10. * pow10order;
  }
  else if(scaled_value > 2.)
  {
    return 5. * pow10order;
  }
  else if(scaled_value > 1.)
  {
    return  2. * pow10order;
  }
  else
  {
    return  1. * pow10order;
  }
} // double roundup10(double value)

/*! Round down to nearest multiple of 10.
  Decimal scaling steps, so value is 0.1, 0.2, 0.5, 1., 2., 5. or 1., 10., 20., 100. ...
  \return Rounded down value.
*/
double rounddown10(double value)
{
  BOOST_MATH_STD_USING

  smallest<> is_small(100. * (std::numeric_limits<double>::min)()); // 100 * min value
  if (is_small(value))
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = std::abs(value);
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled_value = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled_value <= 2.)
  {
    return 1. * pow10order;
  }
  else if(scaled_value <= 5.)
  {
    return 2. * pow10order;
  }
  else if(scaled_value <= 10.)
  {
    return 5. * pow10order;
  }
  else
  {
    return 10. * pow10order;
  }
} // double rounddown10(double value)

/*! Semi-decimal scaling, so return 0.1, 0.5, 1, 5, 10, 50, 100 ...
  \return rounded up value.
*/
double roundup5(double value)
{
  BOOST_MATH_STD_USING

  smallest<> is_small(100. * (std::numeric_limits<double>::min)()); // 100 * min value.
  if (is_small(value) )
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = std::abs(value);
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled_value = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled_value > 5.)
  { // Scale down to 1.
    return 10. * pow10order;
  }
  else if(scaled_value > 1.)
  { //
    return 5. * pow10order;
  }
  else
  { // is < 1
  }
  return 1. * pow10order;
} // double roundup2(double value)

/*! Semi-decimal scaling, so return 0.1, 0.5, 1, 5, 10, 50, 100 ...
  \return Rounded down value.
*/
double rounddown5(double value)
{
  BOOST_MATH_STD_USING

  smallest<> is_small(100. * (std::numeric_limits<double>::min)()); // 100 * min value
  if (is_small(value))
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = std::abs(value);
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled_value = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled_value < 2.)
  { //
    return 1. * pow10order;
  }
  else if(scaled_value < 10.)
  { //
    return 5. * pow10order;
  }
  else
  { //
    return 10. * pow10order;
  }
} // double rounddow5(double value)


/*! Binary scaling steps, so return 0.1, 0.2, 0.4, 0.6, 0.8, 1.0, 2, 4, 6, 8, 10, 20, 40 60, 80, 100..
  \return Rounded up value.
*/
double roundup2(double value)
{
  BOOST_MATH_STD_USING

  smallest<> is_small(100. * (std::numeric_limits<double>::min)()); // 100 * min value.
  if (is_small(value) )
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = std::abs(value);
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled_value = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled_value > 8.)
  {
    return 10. * pow10order;
  }
  else if(scaled_value > 6.)
  {
    return 8. * pow10order;
  }
  else if(scaled_value > 4.)
  {
    return 6. * pow10order;
  }
  else if(scaled_value > 2.)
  {
    return 4. * pow10order;
  }
  else
  {
    return 2. * pow10order;
  }
} // double roundup2(double value)

/*! Binary scaling steps, so return 0.1, 0.2, 0.4, 0.6, 0.8, 1.0, 2, 4, 6, 8, 10, 20, 40 60, 80, 100..
  \return Rounded down value.
*/
double rounddown2(double value)
{
  BOOST_MATH_STD_USING
  smallest<> is_small(100. * (std::numeric_limits<double>::min)()); // 100 * min value
  if (is_small(value))
  { // Value very close to zero.
    return 0.; // Just return zero.
  }
  bool is_neg = (value >= 0) ? false : true;
  value = std::abs(value);
  int order = int(floor(log10(value))); // 0 to 9.999, gives 0, 10 to 99.9 gives 2 ...
  double scaled_value = value * pow(10., -order); // 0 to 9.99 is unchanged, 10 to 9.99 scaled down to 1. to 9.99
  double pow10order = is_neg ? -pow(10., order) : pow(10., order); //  power of ten, signed.

  if(scaled_value < 2.)
  { // Not scaled.
    return 1. * pow10order;
  }
  else if(scaled_value < 4.)
  {
    return 2. * pow10order;
  }
  else if(scaled_value < 6.)
  {
    return 4. * pow10order;
  }
  else if(scaled_value < 8.)
  {
    return 6. * pow10order;
  }
  else
  { // > 8
    return 8. * pow10order;
  }
} // double rounddown2(double value)

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_AUTO_AXES_HPP
