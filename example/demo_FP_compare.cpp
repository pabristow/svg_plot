/*! \file demo_FP_compare.cpp
   \brief Demonstrate features of floating-point comparisons to find if values are close to each other,
    or are too small to be significantly different from zero.

  \author Paul A. Bristow
*/

//  Copyright Paul A. Bristow 2008, 2009, 2020

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
 // using std::cout;
  using std::endl;
  using std::dec;
  using std::hex;
  using std::boolalpha;
#include <iomanip>
 // using std::setprecision;
 // using std::setw;
#include <string>
  //using std::string;
#include <limits>
  //using std::numeric_limits;

#include <boost/svg_plot/detail/fp_compare.hpp>

int main()
{
  std::cout << "Demo FP compare";

#if defined(__FILE__) && defined(__TIMESTAMP__)
  std::cout << "  " << __FILE__ << ' ' << __TIMESTAMP__ ;
#  ifdef _MSC_FULL_VER
  std::cout << ' '<< _MSC_FULL_VER;
#  endif
#endif
  std::cout << boolalpha << endl;

  {
  // Check if a floating-point value is very close to zero, or exactly zero.
  // Sort of operator ~= (if such a thing was possible)

  // Use the default type double and the default small value 2 * min_value.
  smallest<> t_def; // double is default FP type.
  std::cout << t_def.size() << std::endl; // 4.45015e-308
  std::cout << "t(0) " << t_def(0) << std::endl; // true -  is *integer* zero.
  std::cout << "t(0.) " << t_def(0.) << std::endl; // true = really is zero.

  // Specify a default float value.
  smallest<float> tf;
  std::cout << "smallest<float> tf size = " << tf.size() << std::endl; // smallest<float> tf small = 2.35099e-038
  std::cout << " tf(1e-38F) " << tf(1e-38F) << std::endl; // Smaller than float min_value, so expect true ~= zero.
  std::cout << " tf(9.e-38F) " << tf(9.e-38F) << std::endl; // Larger than float min_value, so expect false != zero.

  // Specify a chosen small float value of 1e-10.
  smallest<float> tf10(1e-10F); // Note value must be a float - to match.
  std::cout << "smallest<float> tf10(1e-10); = " << tf10.size() << std::endl; // smallest<float> tf10(1e-10); = 1e-010

  std::cout << " tf10(1e-11F) " << tf10(1e-11F) << std::endl; // Smaller than float 1e-10, so expect true ~= zero.
  std::cout << " tf10(tf10(9.e-9F) " << tf10(9.e-9F) << std::endl; // Larger than float 1e-10, so expect false != zero.

  // Use convenience typdef for double and 2 * (std::numeric_limits<double>::min())
  // typedef smallest<double> tiny;

  tiny tn;
  std::cout  << "tiny tn.size() = " << tn.size() << std::endl; // 4.45015e-308
  std::cout<< "tn(0)  " << tn(0) << std::endl; // true

  smallest<double> z;
  std::cout << z.size() << std::endl;
  std::cout << z(1e-308) << std::endl;

  tiny zz; // typedef smallest<double> tiny;
  std::cout << zz.size() << std::endl;
  std::cout << zz(1e-308) << std::endl;

  double v = (std::numeric_limits<double>::min)();
  if (zz(v))
  {
    std::cout << v << " is tiny." << std::endl;
  }
  tiny z0(0);
  std::cout << zz.size() << std::endl;

  tiny z00(0.);
  std::cout << zz.size() << std::endl;


  close_to<> is_near_100eps(std::numeric_limits<double>::epsilon(), FPC_WEAK);

  std::cout << is_near_100eps(1., 1 + 90 * std::numeric_limits<double>::epsilon() ) << std::endl;
  std::cout << is_near_100eps(1., 1 + 110 * std::numeric_limits<double>::epsilon() ) << std::endl;

  }

  {   // Compare two floating-point values for being close enough to be considered 'equal'.
// Demonstrate use of close-to to check close enough to meet tolerance.

  // Use default tolerance
  // This is twice numeric_limits min,
  // which should allow for a few bits difference from computations.

  // Specific type float, and both tolerance and strength specified.
  close_to<float> t1(1e-15F, FPC_WEAK);
  std::cout << "close_to() t1.size() " << t1.size() << ' ' << (t1.strength() == 0 ? "strong " : "weak" )  << std::endl; //  1e-015 weak
  close_to<float> tdf; // default tolerance = 2 *epsilon and strength = strong
  std::cout << "close_to<float> tdf.size() = "  << tdf.size() << ' ' << (tdf.strength() == 0 ? "strong " : "weak" ) << std::endl; //  2.38419e-007 strong

  // Use the default type double and the default tolerance value 2 * epsilon.
  close_to<double> tds(1e-14, FPC_STRONG); // default strength = strong
  std::cout << "close_to<double> tds.size() = "  << tds.size() << ' ' << (tds.strength() == 0 ? "strong " : "weak" ) << std::endl;
  close_to<double> tdw(1e-14, FPC_WEAK); // default strength = strong
  std::cout << "close_to<double> tdw.size() = "  << tdw.size() << ' ' << (tdw.strength() == 0 ? "strong " : "weak" ) << std::endl;
  close_to<double> tdd; // default tolerance = 2 *epsilon and strength = strong
  std::cout << "close_to<double> tdd.size() = "  << tdd.size() << ' ' << (tdd.strength() == 0 ? "strong " : "weak" ) << std::endl;
  close_to<double> tdds(1e-14); // specific tolerance but use default strength = strong
  std::cout << "close_to<double> tdds.size() = "  << tdds.size() << ' ' << (tdds.strength() == 0 ? "strong " : "weak" ) << std::endl;

  close_to<> t; //
  std::cout << "close_to<double> t.size() = "  << t.size() << ' ' << (t.strength() == 0 ? "strong " : "weak" ) << std::endl;
  std::cout << "close_to<double> tdd.size() = "  << tdd.size() << ' ' << (tdd.strength() == 0 ? "strong " : "weak" ) << std::endl;

  // neareq
  // Use nearby the convenience typedef for close_to<double>
  neareq neq;
  std::cout << "neq(0) " << neq(0, FPC_STRONG) << std::endl; // true -  is *integer* zero.
  std::cout << "neq(0.) " << neq(0., FPC_STRONG) << std::endl; // true = really is zero.
  std::cout << "neq(1 * (std::numeric_limits<double>::min)()) " << neq(1 * (std::numeric_limits<double>::min)(), 2 * (std::numeric_limits<double>::min)()) << std::endl; // true

  neareq neqd(1 * (std::numeric_limits<double>::min)()); // Specify tolerance & rely on default strong requirement.
  std::cout << neqd.size()  << neqd.size() << ' ' << (neqd.strength() == 0 ? "strong " : "weak" ) << std::endl;
  neareq neqdw(1 * (std::numeric_limits<double>::min)(), FPC_WEAK); // Both tolerance strength specified.
  std::cout << neqdw.size()  << neqdw.size() << ' ' << (neqdw.strength() == 0 ? "strong " : "weak" ) << std::endl;

  std::cout << neq(1e-308, 1.1e-308) << std::endl;
  std::cout << neq(1e-308, 1.0000000000000001e-308) << std::endl;

  close_to<> is_near_100eps(100 * std::numeric_limits<double>::epsilon(), FPC_WEAK);
  // Set a tolerance of 100 epsilon ~= 1e-14

  std::cout << is_near_100eps(1., 1. + 90 * std::numeric_limits<double>::epsilon() ) << std::endl; // true
  std::cout << is_near_100eps(1., 1. + 110 * std::numeric_limits<double>::epsilon() ) << std::endl; // false
 }
  return 0;
}  // int main()


/*

Output:

Autorun "j:\Cpp\SVG\Debug\demo_fp_compare.exe
Demo FP compare  i:\boost-sandbox\SOC\2007\visualization\libs\svg_plot\example\demo_FP_compare.cpp Wed Mar 25 12:57:38 2009 150021022
4.45015e-308
t(0) true
t(0.) true
smallest<float> tf size = 2.35099e-038
 tf(1e-38F) true
 tf(9.e-38F) false
smallest<float> tf10(1e-10); = 1e-010
 tf10(1e-11F) true
 tf10(tf10(9.e-9F) false
tiny tn.size() = 4.45015e-308
tn(0)  true
4.45015e-308
true
4.45015e-308
true
2.22507e-308 is tiny.
4.45015e-308
4.45015e-308
false
false
close_to() t1.size() 1e-015 weak
close_to<float> tdf.size() = 2.38419e-007 strong
close_to<double> tds.size() = 1e-014 strong
close_to<double> tdw.size() = 1e-014 weak
close_to<double> tdd.size() = 4.44089e-016 strong
close_to<double> tdds.size() = 1e-014 strong
close_to<double> t.size() = 4.44089e-016 strong
close_to<double> tdd.size() = 4.44089e-016 strong
neq(0) true
neq(0.) true
neq(1 * (numeric_limits<double>::min)()) false
2.22507e-3082.22507e-308 strong
2.22507e-3082.22507e-308 weak
false
true
true
false

*/



