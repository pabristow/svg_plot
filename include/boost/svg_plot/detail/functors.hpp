/*! \file functors.hpp
  \brief Functors to convert data to doubles.
  \details SVG plot assumes all data are convertible to double or uncertain value type unc before being plotted.
    The functors are used to convert both 1D and 2D (pairs of data values) to be converted.
    Note that uncertain value class unc only holds double precision and long double data
    will therefore lose information.  This seems reasonable design decision as any real data
    to be plotted is unlikely to have more than double precision (about 16 decimal digits).

    "svg_plot\example\convertible_to_double.cpp" demonstrates that built-in types
    @float, @double and @clong double work as expected,
    as well as a sample User Defined Type fixed-point, 
    but @b not a Boost.Multiprecision types like cpp_bin_float_quad.

    Non-convertible-to-double types provoke a  compile-time message:
    "Uncertain types must be convertible to double!"

  \author Jacob Voytko and Paul A. Bristow
  \date Mar 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009, 2012, 2013, 2018

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_DETAIL_FUNCTORS_HPP
#define BOOST_SVG_DETAIL_FUNCTORS_HPP

#if defined (_MSC_VER)
#  pragma warning (push)
#  pragma warning (disable : 4244)
#endif

//#include <boost/svg_plot/uncertain.hpp>
#include <boost/quan/unc.hpp>
#include <boost/quan/meas.hpp>

namespace boost {
namespace svg {
namespace detail
{

class double_1d_convert
{ /*! \class boost::svg::detail::double_1d_convert
      \brief This functor allows any 1D data convertible to doubles to be plotted.
*/
public:
    typedef double result_type; //!< result type is double.

    //! \tparam T Any type convertible to double.
    //! returns A single double data value.
    template <class T>
    double operator()(T val) const //! To convert a single data value to double.
    {
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<T, double>::value, "Uncertain types must be convertible to double!");

       return static_cast<double>(val); //! \return Value that has been converted to double.
    }
}; // class double_1d_convert

template <bool correlated>
class unc_1d_convert
{ /*! \class boost::svg::detail::unc_1d_convert
      \brief This functor allows any 1D data convertible to unc (uncertain doubles) to be plotted.
      \details Defaults provided by the unc class constructor ensure that
        uncertainty, degrees of freedom information, and type are suitably set too.
*/
public:
    typedef unc<correlated> result_type; //!< result type is an uncertain floating-point type.

    //! \tparam T Any data type with a value convertible to double, for example: double, unc, Meas.
    template <class T>
    unc<correlated> operator()(T val) const
    /*!< Convert to uncertain type,
      providing defaults for uncertainty,  degrees of freedom information, and type (meaning undefined).
    \return value including uncertainty information.
    */
    {
      return (unc<correlated>)val;
      /*! \return uncertain type (uncertainty, degrees of freedom information, and type meaning undefined).
       warning C4244: 'argument' : conversion from 'long double' to 'double', possible loss of data.
       because unc only holds values to double precision.
       Suppressed by pragma for MSVC above. Need similar for other compilers.
      */
    }
}; // template <bool correlated> class default_1d_convert

class meas_1d_convert
{ /*! \class boost::svg::detail::meas_1d_convert
      \brief This functor allows any 1D data convertible to measurements
      (with uncertain doubles) to be plotted.
      \details Defaults provided by the meas class constructor ensure that
        uncertainty, degrees of freedom information, type, and order, timestamp and id are suitably set too.
*/
public:
    typedef Meas result_type; //!< result type includes an uncertain floating-point type.

    //! \tparam T Any data type with a value convertible to double, for example: double, unc, Meas.
    template <class T>
    Meas operator()(T val) const
    /*!< Convert to Meas type,
      providing defaults for uncertainty, degrees of freedom information, and type (meaning undefined).
      \return value including uncertainty and other information.
    */
    {
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<T, double>::value, "Uncertain types must be convertible to double!");
      return (Meas)val;
      /*! \return uncertain type (uncertainty, degrees of freedom information, and type meaning undefined).
       Warning C4244: 'argument' : conversion from 'long double' to 'double', possible loss of data.
       because unc only holds values to double precision.
       Suppressed by pragma for MSVC above. Need similar for other compilers.
      */
    }
}; // class default_1d_convert

class pair_double_2d_convert
{ /*! \class boost::svg::detail::pair_double_2d_convert
      \brief This functor allows any 2 D data convertible to type std::pair<double, double> to be plotted.
*/
public:
    typedef std::pair<double, double> result_type; //!< result type is a pair (X and Y) of doubles.

    double i; //!< Current value, 1st set by start(double i0).
    void start(double i0)
    { //! Set a start value.
      i = i0;
    }

     //! Convert a pair of X and Y (whose types can be converted to double values) to a pair of doubles.
     //! \tparam T type whose value can be converted to double.
     //! \tparam U type whose value can be converted to double.
    //! \returns @c std::pair of double data point values.
    template <typename T, typename U>
    std::pair<double, double> operator()(const std::pair<T, U>& a) const
    { //! Assumes that a conversion from double yields just the value component of the uncertain value.
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<T, double>::value, "Uncertain types must be convertible to double!");
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<U, double>::value, "Uncertain types must be convertible to double!");
      return std::pair<double, double>(static_cast<double>(a.first), static_cast<double>(a.second));
    }

    template <typename T>
    std::pair<double, double> operator()(T a)
    {  //! Convert a pair of X and Y values to a pair of doubles.
       //! \return pair of doubles.
        return std::pair<double, double>(i++, static_cast<double>(a));
    }
}; // class pair_double_2d_convert

template <bool correlated>
class pair_unc_2d_convert
{ /*! \class boost::svg::detail::pair_unc_2d_convert
      \brief This functor allows any 2D data convertible to type std::pair<unc, unc> to be plotted.
*/
public:
    typedef std::pair<unc<correlated>, unc<correlated> > result_type; //!< result type is pair of uncertain values.

    unc<correlated> i;  //!< Current uncertain value, 1st set by start(double i0).

    void start(unc<correlated> i0)
    { //!< Set a start value.
       i = i0;
    }

    //!< \tparam T type convertible to double.
    //!< \tparam U type convertible to double.
    //! \returns A @c std::pair of double data point values.
    template <class T, class U>
    std::pair<unc<correlated>, unc<correlated> > operator()(const std::pair<T, U>& a) const
    {  //!< Convert a pair of X and Y uncertain type values to a pair of doubles.
       //! \return pair of uncs.
      // Cast to double so that can use with float, long double and UDTs.
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<T, double>::value, "Uncertain types must be convertible to double!");
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<U, double>::value, "Uncertain types must be convertible to double!");
       return std::pair<unc<correlated>, unc<correlated> >(
         (unc<correlated>)(a.first), (unc<correlated>)(static_cast<double>(a.second))
         );
    }

    template <typename T>    //!< \tparam T Any type convertible to double.
    std::pair<unc<correlated>, unc<correlated> > operator()(T a)
    {  //!< Convert a pair of X and Y uncertain type values to a @c std::pair of @c unc.
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<T, double>::value, "Uncertain types must be convertible to double!");
      return std::pair<unc <correlated>, unc<correlated> >(i++, (unc<correlated>)a); //! \return pair of unc.
    }
}; // class pair_unc_2d_convert

 /*! This functor allows any 2D data convertible to type @c std::pair<Meas, unc> to be plotted.
   \tparam correlated @c true if the uncertainties are correlated (for example, adding to a constant value).
   */
template <bool correlated>
class pair_Meas_2d_convert
{
public:
    typedef std::pair<Meas, unc<correlated> > result_type;
    //!< result type is pair of uncertain values.

    Meas i; //!< Current Meas (uncun + datetime etc) value.

    void start(Meas i0)
    { //!< Set a start value.
       i = i0;
    }

    //!< \tparam T type convertible to double.
    //!< \tparam U type convertible to double.
    //! \returns A @c std::pair of double precision data point values.
    template <typename T, typename U>
    std::pair<Meas, unc<correlated> > operator()(const std::pair<T, U>& a) const
    {  //!< Convert a pair of X and Y uncertain type values to a pair of doubles.
       //! \return pair of Meas & uncs.
       // Cast to double so that can use with float, long double.
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<T, double>::value, "Uncertain types must be convertible to double!");
      BOOST_STATIC_ASSERT_MSG(std::is_convertible<U, double>::value, "Uncertain types must be convertible to double!");
      return std::pair<Meas, unc<correlated> >((Meas)(a.first), (unc<correlated>)(static_cast<double>(a.second)));
    }

    template <typename T>    //!< \tparam T Any type convertible to double.
    std::pair<Meas, unc<correlated> > operator()(T a)
    {  //!< Convert a pair of X and Y uncertain type values to a pair of Meas & unc.
        return std::pair<Meas, unc<correlated> >(i++, (unc<correlated>)a);
        //! \return pair of Meas & unc.
    }
}; // class pair_unc_2d_convert

} // namespace detail
} // namespace svg
} // namespace boost

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif // BOOST_SVG_DETAIL_FUNCTORS_HPP
