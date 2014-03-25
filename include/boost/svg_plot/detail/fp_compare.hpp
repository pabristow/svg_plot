/*! \file
  \brief Class for comparing floating point values to see if nearly equal.

  \details
    Two types of comparison are provided:
    FPC_STRONG, "Very close"   - Knuth equation 1', the default.
    FPC_WEAK   "Close enough" - equation 2'.
    equations in Dougles E. Knuth, Seminumerical algorithms (3rd Ed) section 4.2.4, Vol II,
    pp 213-225, Addison-Wesley, 1997, ISBN: 0201896842.

    Strong requires closeness relative to BOTH values being compared,
    Weak only requires only closeness to EITHER ONE value.

    This permits one to avoid some of the problems that can arise from comparing floating-point values
    by circumnavigating the assumption that floating point operations always give exactly the same result.

    See http://hal.archives-ouvertes.fr/docs/00/28/14/29/PDF/floating-point-article.pdf
    for more about the pitfalls.


  \author Paul A. Bristow
  \date Aug 2009
*/
//  fp_compare.hpp
//  Copyright Paul A. Bristow 2008
//  derived from Copyright Gennadiy Rozental 2001-2007.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the Boost.Test library home page.
//  Deliberately removed any treatment of percent!

#ifndef BOOST_TEST_FLOATING_POINT_COMPARISON_HPP
#define BOOST_TEST_FLOATING_POINT_COMPARISON_HPP

#include <boost/limits.hpp>  // for std::numeric_limits
#include <boost/math/tools/precision.hpp> // for max_value, min_value & epsilon for floating_point type;

// Check if two floating-point values are close within a chosen tolerance.
//template<typename FPT> class close_to;

// Check if floating-point value is smaller than a chosen small value.
//template<typename FPT> class smallest;

//! \enum floating_point_comparison_type Two types of comparison of two floating point values.
enum floating_point_comparison_type
{ //!< Two types of comparison of two floating point values.
  FPC_STRONG, //!< "Very close"   - Knuth equation 1',  the default.
              //!< Strong requires closeness relative to BOTH values being compared,

  FPC_WEAK    //!< "Close enough" - equation 2'.
              //!< Weak only requires only closeness to EITHER ONE value.
  // equations in Dougles E. Knuth, Seminumerical algorithms (3rd Ed) section 4.2.4, Vol II,
  // pp 213-225, Addison-Wesley, 1997, ISBN: 0201896842.
};

// GNU int gsl_fcmp (double x, double y, double epsilon) provides similar function.
// fcmp also provides a C implementation at https://sourceforge.net/projects/fcmp/
// For IEEE floating-point types, some speedups are possible, for example see:
// Taming the Floating-point Beast, Chris Lomont
// www.lomont.org/Math/Papers/2005/CompareFloat.pdf
// Alberto Squassabia, Comparing Floats:
// How to determine if Floating-point quantities are close enough
// once a tolerance has been reached:  C++ report March 2000.
// Gennadiy Rozental, Floating_point comparison algorithms,
// www.boost.org/libs/test/doc/components/test_tools/floating_point_comparison.html
// Comparison of Floating Point Numbers, Matthias Ruppwww.mrupp.info/Data/2007floatingcomp.pdf, July 2007.
// The pitfalls of verifying floating-point computations, David Monniaux
// CNRS Ecole normale superieure, 1 Feb 2008, http://arxiv.org/abs/cs/0701192v4
// submitted to ACM TOPLAS.

// \tparam FPT Floating-Point Type: float, double, long double, or User-Defined like NTL quad_float or RR.
// from boost/math/tools/precision.hpp
template <class T> T max_value(T); //!< std::numeric_limits<>::max() or similar.
template <class T> T min_value(T); //!< std::numeric_limits<>::min() or similar.
template <class T> T epsilon(T); //!< std::numeric_limits<>::epsilon() or similar.

template<typename FPT> FPT
fpt_abs(FPT arg)
{ //! abs function (just in case abs is not defined for FPT).
  return arg <static_cast<FPT>(0) ? -arg : arg;
}

template<typename FPT> FPT
safe_fpt_division(FPT f1, FPT f2)
{ //! Safe from under and overflow.
  //! Both f1 and f2 must be unsigned here.

  if( (f2 < static_cast<FPT>(1))  && (f1 > f2 * boost::math::tools::max_value<FPT>()) )
  { // Avoid overflow.
    return boost::math::tools::max_value<FPT>();
  }

  if( (f1 == static_cast<FPT>(0))
    || ((f2 > static_cast<FPT>(1)) && (f1 < f2 * boost::math::tools::min_value<FPT>()) )
    )
  {  // Avoid underflow.
    return static_cast<FPT>(0);
  }
  return f1 / f2;
} // safe_fpt_division(FPT f1, FPT f2)

//! Check two floating-point values are close within a chosen tolerance.
template<typename FPT = double>
class close_to
{
public:

  // One  only.
  template<typename T>
  explicit close_to(T tolerance,
    floating_point_comparison_type fpc_type = FPC_STRONG)
  :
    fraction_tolerance_(tolerance),
      strong_or_weak_(fpc_type)
  { //! Constructor for fraction tolerance and strength of comparison.
    //! Checks that tolerance isn't negative - which does not make sense,
    //! and can be assumed to be a programmer error?
    BOOST_ASSERT(tolerance >= static_cast<T>(0));
  }

  close_to()
  :
  fraction_tolerance_(2 * boost::math::tools::epsilon<FPT>()),
    strong_or_weak_(FPC_STRONG)
  { //! Default constructor is strong comparison to twice numeric_limits<double>::epsilon().
  }

  bool operator()(FPT left, FPT right) const
  { //! Compare two floating point values
    //! \return true if they are effectively equal (approximately) within tolerance & comparison strength.
    FPT diff = fpt_abs(left - right);
    FPT d1   = safe_fpt_division(diff, fpt_abs(right));
    FPT d2   = safe_fpt_division(diff, fpt_abs(left));

    return strong_or_weak_
      ? ((d1 <= fraction_tolerance_) && (d2 <= fraction_tolerance_)) // Strong.
      : ((d1 <= fraction_tolerance_) || (d2 <= fraction_tolerance_)); // Weak.
  }

  FPT size()
  { //! \return fraction tolerance.
    return fraction_tolerance_;
  }

  floating_point_comparison_type strength()
  { //! \return floating_point comparison type strength, FPC_STRONG or FPC_WEAK
    return strong_or_weak_;
  }

private:
    FPT fraction_tolerance_; //! Tolerance expressed as a fraction, 1% == 0.01
    floating_point_comparison_type strong_or_weak_; //! floating_point comparison type strength, FPC_STRONG or FPC_WEAK

}; // class close_to


// David Monniaux, http://arxiv.org/abs/cs/0701192v4,
// It is somewhat common for beginners to add a comparison check to 0 before
// computing a division, in order to avoid possible division-by-zero exceptions or
// the generation of infinite results. A first objection to this practise is that, anyway,
// computing 1/x for x very close to zero will generate very large numbers
// that will most probably result in overflows later.
// Another objection, which few programmers know about and that we wish to draw attention
// to, is that it may actually fail to work, depending on what the compiler
// does - that is, the program may actually test that x 6= 0, then, further down,
// find that x = 0 without any apparent change to x!

//! Check floating-point value is smaller than a chosen small value.
template<typename FPT = double>
class smallest
{ /*!< \details
    David Monniaux, http://arxiv.org/abs/cs/0701192v4,
    It is somewhat common for beginners to add a comparison check to 0 before
    computing a division, in order to avoid possible division-by-zero exceptions or
    the generation of infinite results. A first objection to this practise is that, anyway,
    computing 1/x for x very close to zero will generate very large numbers
    that will most probably result in overflows later.
    Another objection, which few programmers know about and that we wish to draw attention
    to, is that it may actually fail to work, depending on what the compiler
    does - that is, the program may actually test that x 6= 0, then, further down,
    find that x = 0 without any apparent change to x!
*/
public:
  template<typename T>
  explicit smallest(T s)
  :
  smallest_(s)
  { // Constructor.
  }

  smallest()
  :
  smallest_(2 * boost::math::tools::min_value<FPT>())
  { /*!< Default Constructor.
      Default smallest_ =  2. * boost::math::tools::min_value<double>();
      multiplier m = 2 (must be integer or static_cast<FPT>())
      is chosen to allow for a few bits of computation error.
      Pessimistic multiplier is the number of arithmetic operations,
      assuming every operation causes a 1 least significant bit error,
      but a more realistic average would be half this.
    */
  }

  template<typename T>
  bool operator()(T fp_value, T s)
  { //!< \return true if smaller than the given small value.
    if (fpt_abs(fp_value) == static_cast<T>(0))
    { // Test for zero first in case T is actually an integer type zero,
      // when the comparison < below would fail because
      // smallest_ could become zero when min_value converts to integer.
      return true;
    }
    return fpt_abs(fp_value) < fpt_abs(s);
  } // bool operator()

  template<typename T>
  bool operator()(T fp_value)
  { //!< \return true if smaller than the defined smallest effectively-zero value.
    if (fpt_abs(fp_value) == static_cast<FPT>(0))
    { // Test for zero first in case FPT is actually an integer type,
      // when the comparison < below would fail because
      // smallest could become zero.
      return true;
    }
    return fpt_abs(fp_value) < fpt_abs(smallest_);
  } // bool operator()

  FPT size()
  { //!< \return smallest value that will be counted as effectively zero.
    return smallest_;
  }

private:
  //!< Smallest value that will be counted as effectively zero.
  FPT smallest_;
}; // class smallest

// Since double and the default smallest value 2 * min_value = 4.45015e-308
// is a very common requirement, provide an convenience alias for this:
typedef smallest<double> tiny; //!< A shorthand for twice std::numeric_limits<double>::min_value(), often 4.4e-308.

// Since double and the default twice std::numeric_limits<double>::epsilon() = 2.220446e-016
// is a very common requirement, provide an convenience alias for this:
typedef close_to<double> neareq; //!< A shorthand for twice std::numeric_limits<double>::epsilon(), often 2e-16.

#endif // BOOST_FLOATING_POINT_COMPARISON_HPP
