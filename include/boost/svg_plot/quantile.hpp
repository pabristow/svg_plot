/*!  \file

  \brief Estimate p th quantile of data.

  \details Estimate p th quantile of data using one of 5 definitions.
  Default is the recommendation of Hyndman and Fan = definition #8.

  Hyndman and Fan recommend their definition 8 (Maple's default definition),
  which gives quartiles between those reported by Minitab and Excel. This
  approach is approximately median unbiased for continuous distributions.

  Hyndman and Fan, 1996, "Sample Quantiles in Statistical Packages", The American Statistician 50(4):361-365,1996
  http://www.pcreview.co.uk/forums/thread-3494699.php // Excel - Interquartile Range Miscalculation

  The interquartile range is calculated using the 1st & 3rd sample quartiles,
  but there are various ways to calculate those quartiles.
  Excel, S-Plus, etc use H&F definition 7, which returns SMALL(data,i) as
  quantile(data,(i-1)/(n-1)) and interpolates in between. For a continuous
  distribution, this will tend to give too narrow an interquartile range, since
  there will tend to be a small fraction of the population beyond the extreme
  sample observations. In particular, for odd n (=2*k+1), Excel calculates the
  1st (3rd) quartile as the median of the lower (upper) "half" of the sample
  including the sample median (k+1 observations).

  Minitab, etc use H&F definition 6, which calculates the 1st (3rd) quartile
  as the median of the lower (upper) "half" of the sample. This "half" sample
  excludes the sample median (k observations) for odd n (=2*k+1). This will
  tend to be a better estimate for the population quartiles, but will tend to
  give quartile estimates that are a bit too far from the center of the whole
  sample (too wide an interquartile range).
*/
// quantile.hpp
// Copyright Paul A. Bristow 2008, 2013

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_QUANTILE_HPP
#define BOOST_SVG_QUANTILE_HPP

#include <vector>
  using std::vector;

namespace boost
{
namespace svg
{

// http://en.wikipedia.org/wiki/Quartile
// http://mathforum.org/library/drmath/view/60969.html // as Dr Math
// http://support.wolfram.com/archive/mathematica/quartilesnotes.html
// Hyndman and Fan, 1996, "Sample Quantiles in Statistical Packages", The American Statistician 50(4):361-365,1996
// http://www.pcreview.co.uk/forums/thread-3494699.php // Excel - Interquartile Range Miscalculation

// The interquartile range is calculated using the 1st & 3rd sample quartiles,
// but there are various ways to calculate those quartiles.
//
// Five of Hyndman and Fan's sample quantile definitions have a particularly
// simple common form given by the following VBA code. You select among the
// methods according to which definition of m you uncomment.

//Function quantile(data, p)
//' p=fraction of population, e.g. p=0.25 for 1st quartile
//n = WorksheetFunction.Count(data)
//' m = 0 ' H&F 4: SAS (PCTLDEF=1), R (type=4), Maple (method=3)
//' m = 0.5 ' H&F 5: R (type=5), Maple (method=4)
//' m = p ' H&F 6: Minitab, SPSS, BMDP, JMP, SAS (PCTLDEF=4), R
//(type=6), Maple (method=5)
//' m = 1 - p ' H&F 7: Excel, S-Plus, R (type=7[default]), Maxima, Maple
//(method=6)
//' m = (p+1)/3 ' H&F 8: R (type=8), Maple (method=7[default])
//' m=(p+1.5)/4 ' H&F 9: R (type=9), Maple (method=8)
//npm = n * p + m
//j = Fix(npm): If j = 0 Then j = 1
//If j > n Then j = n
//g = npm - j
//quantile = WorksheetFunction.Small(data, j)
//If g >= 0 And j < n Then
//quantile = (1 - g) * quantile + g * WorksheetFunction.Small(data, j
//+ 1)
//End If
//End Function

// Excel, S-Plus, etc use H&F definition 7, which returns SMALL(data,i) as
// quantile(data,(i-1)/(n-1)) and interpolates in between. For a continuous
// distribution, this will tend to give too narrow an interquartile range, since
// there will tend to be a small fraction of the population beyond the extreme
// sample observations. In particular, for odd n (=2*k+1), Excel calculates the
// 1st (3rd) quartile as the median of the lower (upper) "half" of the sample
// including the sample median (k+1 observations).
//
// Minitab, etc use H&F definition 6, which calculates the 1st (3rd) quartile
// as the median of the lower (upper) "half" of the sample. This "half" sample
// excludes the sample median (k observations) for odd n (=2*k+1). This will
// tend to be a better estimate for the population quartiles, but will tend to
// give quartile estimates that are a bit too far from the center of the whole
// sample (too wide an interquartile range).
//
// Hyndman and Fan recommend their definition 8 (Maple's default definition),
// which gives quartiles between those reported by Minitab and Excel. This
// approach is approximately median unbiased for continuous distributions.


double quantile(vector<double>& data, double p, int HF_definition = 8)
{ /*! Estimate quantile from values in vector data.
   \note Assumes values are pre-sorted from min to max.

  \param[in] data Population for which to estimate quantile (percentile). Data must be ordered minimum to maximum.

  \param[in] p Fraction of population, for example, p = 0.25 for 1st quartile. (Usually p = 0.25, or p = 0.75 for boxplots).

  \param[in] HF_definition Algorithm to use for the estimation.

  \return Estimated quantile from the population.

  */

  size_t n = data.size();
  // The interquartile range is calculated using the 1st & 3rd sample quartiles,
  // but there are various ways to calculate those quartiles.
  // Five of Hyndman and Fan's sample quantile definitions
  // Rob J. Hyndman and Yanan Fan, 1996, "Sample Quantiles in Statistical Packages",
  // The American Statistician 50(4):361-365, (1996).
  // have a particularly simple common form given by the VBA code given.
  // You select among the methods according to which definition of m is chosen.
  // from Function quantile(data, p) in Visual Basic from
  // http://www.pcreview.co.uk/forums/thread-3494699.php
  // Jerry W. Lewis
  double m;
  switch (HF_definition)
  {  // Hyndman and Fan definitions:
  case 4: // H&F 4 SAS (PCTLDEF=1), R (type=4), Maple (method=3)
    m = 0; // Largest IQR
    break;
    case 5: // H&F 5: R (type=5), Maple (method=4), Wolfram Mathematica quartiles
      // http://support.wolfram.com/archive/mathematica/quartilesnotes.html
      // "Symmetric linear interpolation a common choice when the data represent a sample
      // from a continuous distribution and
      // you want an unbiased estimate of the quartiles of that distribution."
    m = 0.5; // moderate IRQ
    break;
    case 6: // H&F 6: Minitab, SPSS, BMDP, JMP, SAS (PCTLDEF=4), R(type=6), Maple (method=5).
      // calculates the 1st (3rd) quartile
      // as the median of the lower (upper) "half" of the sample.
      // This "half" sample excludes the sample median (k observations) for odd n (=2*k+1).
      // This will tend to be a better estimate for the population quartiles,
      // but will tend to give quartile estimates that are a bit too far
      // from the center of the whole sample (too wide an interquartile range).
      m = p; // Biggest IQR
      break;
    case 7:// H&F 7: Excel, S-Plus, R (type=7[default]), Maxima, Maple (method=6).
      m = 1 - p; // Smallest IQR
      // For a continuous distribution,
      // this will tend to give too narrow an interquartile range,
      // since there will tend to be a small fraction of the population beyond the extreme
      // sample observations. In particular, for odd n (=2*k+1), Excel calculates the
      // 1st (3rd) quartile as the median of the lower (upper) "half" of the sample
      // including the sample median (k+1 observations).
      break;
    case 8: // H&F 8: R (type=8), Maple (method=7[default]).
      m = (p + 1) / 3;
      break; // Middling IQR
      // Definition 8 recommended by H&F because
      // "it is approximately median-unbaised estimate regardless of distribution"
      // and thus suitable for continuous and discrete distributions.
      // (Maple's default definition),
      // which gives quartiles between those reported by Minitab and Excel. This
      // approach is approximately median unbiased for continuous distributions.
    case 9: m = (p + 1.5) / 4; // H&F 9: R (type=9), Maple (method=8).
      break;
    default:
      m = p; // == case H&F 6: Minitab, SPSS, BMDP, JMP, SAS (PCTLDEF=4), R(type=6), Maple (method=5).
   }
  double npm = n * p + m; // 9 * 0.25 + 0.25
  size_t j = static_cast<size_t>(npm); // j = Fix(npm): fix returns integer part.
  // Index for data array.
  //if (j == 0)
  //{ // If j = 0 Then j = 1 // assumes 1 based ...
  //  j = 1;
  //}
  // but J can't be < 0 so check not needed.
  if (j >= n)
  { // If j > n Then j = n
    j = n -1; // Limit to last element of data.
  }

  double g = npm - j; // fractional part of npm = 0.5
  ////  quantile = WorksheetFunction.Small(data, j) Returns the k-th smallest value in a data set.
  //if ((g >= 0) && (j <= n))
  //{ // Need to interpolate.
  //  quantile = (1 - g) * quantile + g * series[j]; // WorksheetFunction.Small(data, j + 1)
  //}
  double quantile = data[j - 1];  // VBA example appears to be 1-based (not default 0)! So j-1.
  if ((g >= 0) && ((j-1) < n))
  { // Need (and can) to interpolate.
    quantile = (1 - g) * quantile + g * data[j]; // VBA example appears to be 1-based (not default 0)! So NOT j+1.
  }
  //cout << "Quantile(" << p << ") = " << quantile << endl;
  return quantile;
} // double quantile(vector<double>& data, double p)

 /*! Median of values in vector @c data.
   \note Assumes pre-sorted from min to max.
 */
 double median(vector<double>& data)
{
  size_t data_size = data.size();
  double median_;
  if ((data_size % 2) == 0)
  { // Even, so take the mean of middle two values.
    median_ = (data[data_size / 2 - 1] // is even so divides exactly without rounding!
    + data[data_size / 2]) // plus next above the middle.
    / 2;
    //median_ = (series[(unsigned int)((data_size) / 2) -1]
    //+ series[(unsigned int)((data_size) / 2)])
  }
  else
  { // Odd, so just take the middle value.
    //median_ = series[(unsigned int)((data_size + 1) / 2 - 1)];
    median_ = data[(data_size + 1) / 2 - 1];
  }
 return median_;
} // double median(vector<double>& data)

  } // namespace boost
} //  namespace svg

#endif // BOOST_SVG_QUANTILE_HPP
