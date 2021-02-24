/*! \file numeric_limits_handling.hpp
    \brief Functions to check if data values are NaN or infinity or denormalised.
    \details
      Since only double is used, template versions are not needed,
      and TR1 should provide max, min, denorm_min, infinity and isnan,
      but older compilers and libraries may not provide all these.

      Better to use boost::math throughout?  Done?

    \author Jacob Voytko and Paul A. Bristow
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007, 2013

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP
#define BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP

#include <boost/math/special_functions/fpclassify.hpp>

#include <boost/quan/unc.hpp>
// using boost::quan::uncun ...
#include <boost/quan/meas.hpp>
// using boost::quan Meas;

#include <limits>
  // using std::numeric_limits;
#include <cmath> // Why?

namespace boost
{
namespace svg
{
namespace detail
{
  // Provide checks on data values to be plotted.
  // Test if at max or +infinity, or -max or - infinity, or NaN.

// std::numeric_limits<double>::min() or denorm min() are just ignored as almost zero (which is an OK value).

constexpr double margin = 4.;  //!< Consider values close to std::numeric_limits<double>::max() as maximum to try to take account of computation errors.

using boost::quan::unc;
using boost::quan::Meas;

inline bool limit_max(double a)
{ //! At (or near) max value or +infinity, most positive values.
    return ((a > (std::numeric_limits<double>::max)() / margin) // Avoid macro max trap!
         || (std::isinf(a)));
        // || (a == std::numeric_limits<double>::infinity()));
}

inline bool limit_min(double a)
{ //! At (or near) -max or -infinity, most negative values.
  return (
    (a < -(std::numeric_limits<double>::max)() /margin) // Avoid macro max trap!
    || (a == -std::numeric_limits<double>::infinity())
    );
}

// Allow NaNs to be displayed differently from just too big or too small values.
inline bool limit_NaN(double a)
{ //! Separate test for NaNs.
  using std::isnan;
  return isnan(a) ? true : false;
  // Ternary operator used to remove warning about casting int to bool.
}

inline bool is_limit(double a)
{ //! Is at some limit.
  return limit_max(a) || limit_min(a) || limit_NaN(a);
}

inline bool pair_is_limit(std::pair<const double, double> a)
{ //! Check on both x and y double data points. Return false if either or both are at limit.
  return limit_max(a.first) || limit_min(a.first) || limit_NaN(a.first)
    || limit_max(a.second) || limit_min(a.second) || limit_NaN(a.second);
}

inline bool pair_is_limit(std::pair<double, double> a)
{ //! Check on both x and y double data points. Return false if either or both are at limit.
  return limit_max(a.first) || limit_min(a.first) || limit_NaN(a.first)
    || limit_max(a.second) || limit_min(a.second) || limit_NaN(a.second);
}

inline bool pair_is_limit(std::pair<const int, double> a)
{ //! Check on both x int and y double data points. Return false if either or both are at limit.
  return limit_max(a.first) || limit_min(a.first) // || limit_NaN(a.first) - can't be NaN if int.
    || limit_max(a.second) || limit_min(a.second) || limit_NaN(a.second);
}

inline bool pair_is_limit(std::pair<int, double> a)
{ //! Check on both x int and y double data points. Return false if either or both are at limit.
  return limit_max(a.first) || limit_min(a.first) // || limit_NaN(a.first) - can't be NaN if int.
    || limit_max(a.second) || limit_min(a.second) || limit_NaN(a.second);
}

template <bool correlated>
inline bool pair_is_limit(std::pair<const unc<correlated>, unc<correlated> > a)
{ //! Check on values of both x and y unc data points.
  // \return false if either or both are at limit.
  return limit_max(value_of(a.first)) || limit_min(value_of(a.first)) || limit_NaN(value_of(a.first))
    || limit_max(value_of(a.second)) || limit_min(value_of(a.second)) || limit_NaN(value_of(a.second));
}

template <bool correlated>
inline bool pair_is_limit(std::pair<Meas, unc<correlated> > a) // const version
{ //! Check on values of both x Meas and y unc data points.
  // \return false if either or both are at limit.
  return limit_max(value_of(a.first)) || limit_min(value_of(a.first)) || limit_NaN(value_of(a.first))
    || limit_max(value_of(a.second)) || limit_min(value_of(a.second)) || limit_NaN(value_of(a.second));
}

template <bool correlated>
inline bool pair_is_limit(std::pair<const Meas, unc<correlated> > a)
{ //! Check on values of both x Meas and y unc data points.
  // \return false if either or both are at limit.
    double rounddown2(double value); // 2, 4,
  return limit_max(value_of(a.first)) || limit_min(value_of(a.first)) || limit_NaN(value_of(a.first))
    || limit_max(value_of(a.second)) || limit_min(value_of(a.second)) || limit_NaN(value_of(a.second));
}

} // namespace detail
} // namespace svg
} // namespace boost

// Defines :
/*
bool boost::svg::detail::limit_max(double); // true if max or +infinity.
bool boost::svg::detail::limit_min(double); // true if min, denorm_min or -infinity.
bool boost::svg::detail::limit_NaN(double); // true if NaN.
bool boost::svg::detail::is_limit(double); // max, min, infinity or NaN - not a 'proper' data value.
bool boost::svg::detail::pair_is_limit(std::pair<double, double>); // x and/or y  not a proper data value.
bool boost::svg::detail::pair_is_limit(std::pair<const double, double>); // x and/or y  not a proper data value.

template <bool correlated>
bool boost::svg::detail::pair_is_limit(std::pair<const unc<correlated>, unc<correlated> >); // x and/or y not a proper data value!
*/
#endif // BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP
