/*! \file pair.hpp
    \brief Provides a private implementation of @c operator<< for @c std::pair that
      outputs pairs with a comma-separated format, for example: "1.2, 3.4".
    \details Hidden in namespace @c detail to avoid clashes with other potential implementations of @c std::pair @c operator<<.
    \author Paul A. Bristow
*/

// Copyright Paul A. Bristow 2006 - 2013.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_PAIR
#define BOOST_SVG_PAIR

#ifdef _MSC_VER
#  pragma once
#endif

#include<iostream>
 // using std::ostream;

#include <utility>
  //using std::pair;
  //using std::make_pair;

namespace boost
{
namespace svg
{
namespace detail
{
  std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p); //! Output pair of doubles to ostream.

  // Hidden in namespace `detail` to avoid potential clashes with other implementations of @c std::pair @c operator<<.
  template<class T1, class T2>
  std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p); //! Output pair of type T1 and T2 to ostream.
  
 // std::ostream& operator<< (std::ostream& os, const std::pair<boost::svg::uncun, boost::svg::uncun>& p);

  /*! Output a @c std::pair of @c const @c T1 and @c T2 values to @c std::ostream, for example:
      \code
         int precision = os.precision(5); // Save & use 5 rather than default precision(6).
      \endcode
      \tparam T1 Type of @c std::pair.first.
      \tparam T2 Type of @c std::pair.second.
      \param os @c std::ostream for output.
      \param p @c std::pair to be output.

  */ 
  template<class T1, class T2>
  std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
  { //! Output pair of type T1 and T2 to @c std::ostream.
         os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      return os;
  } // std::ostream& operator<<

  /*! Output a @c std::pair of @c const @c double values to @c std::ostream, for example:
      \code
         int precision = os.precision(5); // Save & use 5 rather than default precision(6).
      \endcode
      \param os std::ostream for output.
      \param p @c std::pair of @c double to be output.

  */
  std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p)
  {
      os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      //os.precision(precision); // Restore.
      return os;
  } // std::ostream& operator<<

  //std::pair<class T1, class T2>& operator=(const pair& rhs)
  //{
  //  first = rhs.first;
  //  second = rhs.second;
  //  return *this; //! to make chainable.
  //}

  // Maybe better as:
  //template<typename charT, typename traits, typename T1, typename T2>
  //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<T1, T2>& p)
  //{
  //  return os << p.first << ", " << p.second;
  //}
  //

    /*! Output a @c std::pair of @c const @c double values to @c std::ostream, for example:
      \code
         int precision = os.precision(5); // Save & use 5 rather than default precision(6).
      \endcode
      \param p c std::pair of @c double to be output.
       \param os @c std::ostream for output.

  */
  // Explicit double, double.
  template<typename charT, typename traits>
  inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<double, double>& p)
  {
    return os << p.first << ", " << p.second;
  }
  // but OK for this purpose.
} // namespace detail

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_PAIR_HPP



