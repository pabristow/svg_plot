/*! \file
  \brief Functors to convert data to doubles.
  \details SVG plot assumes all data are convertible to double or uncertain value type unc before being plotted.
    The functors are used to convert both 1D and 2D (pairs of data values) to be converted.
    Note that uncertain value class unc only holds double precision so higher precision data type
    will therefore lose information.  This seems a reasonable design decision as any real data
    to be plotted is unlikely to have more than double precision (about 16 decimal digits).

    "svg_plot\example\convertible_to_double.cpp" demonstrates that built-in types
    @c float, @c double, @c long double @ Boost.Multiprecision @c cpp_bin_float_quad 
    work as expected, as well as a sample User Defined Type (UDT) a fixed-point type.

    Types that cannot be converted to double nor constructible from double provoke a compile-time message:
    "Uncertain types must be convertible to double!"

    (This uses checks using http://www.cplusplus.com/reference/type_traits/is_constructible/
      @c BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be convertible to double!");
     ).
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009, 2012, 2013, 2018, 2021

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_DETAIL_FUNCTORS_HPP
#define BOOST_SVG_DETAIL_FUNCTORS_HPP

#include <boost/quan/unc.hpp>
#include <boost/quan/meas.hpp>

namespace boost {
namespace svg {
namespace detail
{
  using namespace boost::quan;

  /*! \class boost::svg::detail::double_1d_convert
     \brief This functor allows any 1D data that can be converted to @c double to be plotted.
 */
class double_1d_convert
{
public:
    typedef double result_type; //!< result type is @c double.

    //! To convert a single data value to double.
    //! \tparam T Any type that can be converted to double.
    //! \returns A single @c double data value.
    template <class T>
    double operator()(T val) const 
    {
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be convertible to double!");

       return static_cast<double>(val); //! \return Value that has been converted to double.
    }
}; // class double_1d_convert

 /*! \class boost::svg::detail::unc_1d_convert
      \brief This functor allows any 1D data that can be converted to @c boost::quan::unc (uncertain double) to be plotted.
      \details Defaults provided by the @c unc class constructor ensure that
        uncertainty, degrees of freedom information, and uncertain type are suitably set too.
*/
template <bool correlated>
class unc_1d_convert
{
public:
    typedef unc<correlated> result_type; //!< @c result_type is an uncertain floating-point type.

    /*!< Convert to uncertain type,
      providing defaults for uncertainty,  degrees of freedom information, and type (meaning undefined).
      \return value including uncertainty information.
       \tparam T Any data type with a value that can be converted to double, for example: @c double, @c uncun, @c Meas.
      */
    template <class T>
    unc<correlated> operator()(T val) const
    {
      return (result_type)val;
      /*! \return uncertain type (uncertainty, degrees of freedom information, and type meaning undefined). */
    }
}; // template <bool correlated> class default_1d_convert

/*! \class boost::svg::detail::meas_1d_convert
      \brief This functor allows any 1D data that can be converted to measurements
      (with uncertain doubles) to be plotted.
      \details Defaults provided by the meas class constructor ensure that
        uncertainty, degrees of freedom information, type, and order, timestamp and id are suitably set too.
    \tparam T Any data type with a value that can be converted to double, for example: double, unc, Meas.
    \return uncertain type (uncertainty, degrees of freedom information, and type meaning undefined).
*/
class meas_1d_convert
{ 
public:
    typedef Meas result_type; //!< result type includes an uncertain floating-point type.

    template <class T>
    Meas operator()(T val) const
    /*!< Convert to Meas type,
      providing defaults for uncertainty, degrees of freedom information, and uncertain type.
      \return value including uncertainty and other information.
    */
    {
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be that can be converted to double!");
      return static_cast<result_type>(val);
    }
}; // class default_1d_convert

class pair_double_2d_convert
{ /*! \class boost::svg::detail::pair_double_2d_convert
      \brief This functor allows any 2-D data that can be converted to type @c std::pair<double, double> to be plotted.
     Convert a pair of X and Y (whose types can be converted to double values) to a pair of @c doubles.
     \tparam T type whose value can be converted to @c double.
     \tparam U type whose value can be converted to @c  double.
     \returns @c std::pair of @c double data point values.
  */
public:
    typedef std::pair<double, double> result_type; //!< result type is a pair (X and Y) of doubles.

    double i; //!< Current value, 1st set by start(double i0).
    void start(double i0)
    { //! Set a start value.
      i = i0;
    }

    template <typename T, typename U>
    std::pair<double, double> operator()(const std::pair<T, U>& a) const
    { //! Assumes that a conversion from double yields just the value component of the uncertain value.
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be convertible to double!");
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<U, double>::value, "Uncertain types must be convertible to double!");
      return std::pair<double, double>(static_cast<double>(a.first), static_cast<double>(a.second));
    }

    template <typename T>
    std::pair<double, double> operator()(T a)
    {  //! Convert a pair of X and Y values to a @c std::pair of @c doubles.
       //! \return @c std::pair of doubles.
        return std::pair<double, double>(i++, static_cast<double>(a));
    }
}; // class pair_double_2d_convert

template <bool correlated>
class pair_unc_2d_convert
{ /*! \class boost::svg::detail::pair_unc_2d_convert
      \brief This functor allows any 2D data that can be converted to type @c std::pair<unc, unc> to be plotted.
*/
public:
    typedef std::pair<unc<correlated>, unc<correlated> > result_type; //!< result type is pair of uncertain values.

    unc<correlated> i;  //!< Current uncertain value, 1st set by start(double i0).

    void start(unc<correlated> i0)
    { //!< Set a start value.
       i = i0;
    }

    //!< \tparam T type that can be converted to double.
    //!< \tparam U type that can be converted to double.
    //! \returns A @c std::pair of double data point values.
    template <class T, class U>
    std::pair<unc<correlated>, unc<correlated> > operator()(const std::pair<T, U>& a) const
    { //!< Convert a pair of X and Y uncertain type values to a pair of @c doubles.
      //! Cast to double so that can use with float, long double and UDTs.
      //! \return @c std::pair of uncs.
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be convertible to double!");
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<U, double>::value, "Uncertain types must be convertible to double!");
       return std::pair<unc<correlated>, unc<correlated> >(
         (unc<correlated>)(a.first), (unc<correlated>)(static_cast<double>(a.second))
         );
    }

    template <typename T>    //!< \tparam T Any type that can be converted to double.
    std::pair<unc<correlated>, unc<correlated> > operator()(T a)
    {  //!< Convert a pair of X and Y uncertain type values to a @c std::pair of @c unc.
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be convertible to double!");
      return std::pair<unc <correlated>, unc<correlated> >(i++, (unc<correlated>)a); //! \return pair of unc.
    }
}; // class pair_unc_2d_convert

 /*! This functor allows any 2D data that can be converted to type @c std::pair<Meas, unc> to be plotted.
   \tparam correlated @c true if the uncertainties are correlated (for example, adding to a constant value).
   */
template <bool correlated>
class pair_Meas_2d_convert
{
public:
    typedef std::pair<Meas, unc<correlated> > result_type;
    //!< result type is pair of Meas (uncertain including datetime etc) and an uncertain value.

    Meas i; //!< Current Meas (uncun + datetime etc) value.

    void start(Meas i0)
    { //!< Set a start value.
       i = i0;
    }
    //!< Convert a pair of X and Y uncertain type values to a pair of uncertain types uncun.
    //! \return pair of Meas (an unc including time, ID and order info) & an unc.
    //! Cast to double so that can potentially use with @c float, @c long double.
    //!< \tparam T type that can be converted to double.
    //!< \tparam U type that can be converted to uncertain type @c unc.
    //! \returns A @c std::pair of double precision data-point values.
    template <typename T, typename U>
    std::pair<Meas, unc<correlated> > operator()(const std::pair<T, U>& a) const
    {  
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be convertible to uncun!");
      BOOST_STATIC_ASSERT_MSG(std::is_constructible<U, double>::value, "Uncertain types must be convertible to uncun!");
      return std::pair<Meas, unc<correlated> >((Meas)(static_cast<Meas>(a.first)), (unc<correlated>)(static_cast<unc<correlated>>(a.second)));
    }

    template <typename T>  //!< \tparam T Any type that can be converted to uncun. 
    std::pair<Meas, unc<correlated> > operator()(T a)
    {  //!< Convert a pair of X and Y uncertain type values to a pair of Meas & unc.
        return std::pair<Meas, unc<correlated> >(i++, (unc<correlated>)(static_cast<double>(a)));
        //! \return pair of Meas & unc.
    }
}; // class pair_Meas_2d_convert

 /*! This functor allows any 2D data that can be converted to type double to be plotted.
   \tparam correlated @c true if the uncertainties are correlated (for example, adding up to a constant value).
   */
template <bool correlated>
class pair_Meas_2d_double_convert
{
public:
 //typedef std::pair<Meas, unc<correlated> > result_type;
  typedef std::pair<double, double > result_type;
  //!< result type is pair of double values.

  Meas i; //!< Current Meas (uncun + datetime etc) value.

  void start(Meas i0)
  { //!< Set a start value.
    i = i0;
  }
  //!< Convert a pair of X and Y uncertain type values to a pair of doubles.
  //! \return pair of Meas (unc including time, ID and order info) & an unc.
  //! Cast to double so that can potentially use with float, long double.
  //!< \tparam T type that can be converted to double.
  //!< \tparam U type that can be converted to double.
  //! \returns A @c std::pair of @c double precision data-point values.
  template <typename T, typename U>
  std::pair<Meas, unc<correlated> > operator()(const std::pair<T, U>& a) const
  {  
    BOOST_STATIC_ASSERT_MSG(std::is_constructible<T, double>::value, "Uncertain types must be convertible to double!");
    BOOST_STATIC_ASSERT_MSG(std::is_constructible<U, double>::value, "Uncertain types must be convertible to double!");
     return std::pair<Meas, unc<correlated> >((Meas)(static_cast<Meas>(a.first)), (unc<correlated>)(static_cast<uncun>(a.second))); // OK uncertain values.
     // but fails for \boost\libs\svg_plot\example\convertible_to_double.cpp and this compiles
     //return std::pair<Meas, unc<correlated> >((Meas)(static_cast<double>(a.first)), (unc<correlated>)(static_cast<double>(a.second)));
     // but does not work for uncertainty displays.  Not fully understood.
 }

  //! Convert a pair of X and Y uncertain type values to a pair of @c Meas & @c unc.
  //! \tparam T Any type that can be converted to @c double.
  template <typename T>    
  std::pair<Meas, unc<correlated> > operator()(T a)
  {  
    return std::pair<Meas, unc<correlated> >(i++, (unc<correlated>)(static_cast<double>(a)));
    //! \return pair of Meas & unc.
  }
}; // class pair_Meas_2d_double_convert

} // namespace detail
} // namespace svg
} // namespace boost

#endif // BOOST_SVG_DETAIL_FUNCTORS_HPP
