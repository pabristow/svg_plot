/*! \file demo_rounds.cpp
  \brief Demonstration of rounding algorithm used for autoscaling.
  \details
  \author Paul A Bristow
*/// demo_rounds.cpp

// Copyright Paul A. Bristow 2006.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/detail/auto_axes.hpp>

namespace boost
{
  namespace svg
  {
   // Demonstrate these functions.
    double roundup10(double value); // 2, 5, 10
    double rounddown10(double value);
    double roundup5(double value); // 5, 10
    double rounddown5(double value);
    double roundup2(double value);
    double rounddown2(double value); // 2, 4, 6, 8
  } // namespace svg
} // namespace boost


#include <iostream>
//  using std::cout;
// using std::endl;

int main()
{

 using namespace boost::svg;
  using std::cout;
  using std::endl;

 double r = roundup10(0.99);

 cout << r << endl;

 cout << "decimal roundup10" << endl;
   cout << roundup10(0.99) << endl; // 1
   cout << roundup10(9.4) << endl; // 10
   cout << roundup10(0.000094) << endl; // 0.0001
   cout << roundup10(5.1) << endl; // 1
   cout << roundup10(4.9) << endl; // 10
   cout << roundup10(0.000049) << endl; // 0.0001
   cout << roundup10(4999.) << endl; // 5000
   cout << roundup10(49.) << endl; // 40
   cout << roundup10(1.2) << endl; // 2
   cout << roundup10(0.0012) << endl; //  0.002
   cout << roundup10(99.) << endl; // 100
   cout << roundup10(101.) << endl; // 200
   cout << roundup10(2.1) << endl; // 5
   cout << roundup10(2.1) << endl; // 5
   cout << rounddown10(1.9) << endl; // 1

   cout << roundup2(1.1) << endl; // 2
   cout << roundup2(0.9) << endl; // 1
   cout << roundup2(2.1) << endl; // 2
   cout << roundup2(3.1) << endl; // 4
   cout << roundup2(4.1) << endl; // 4
   cout << roundup2(5.1) << endl; // 6
   cout << roundup2(7.1) << endl; // 8
   cout << roundup2(1.1e-3) << endl; // 0.0011 - 0.002
   cout << roundup2(0.9e-3) << endl; // 0.001
   cout << roundup2(2.1e-3) << endl; // 0.004
   cout << roundup2(3.1e-3) << endl; //  0.004
   cout << roundup2(4.1e-3) << endl; //
   cout << roundup2(7.1e-3) << endl; //
   cout << roundup2(1.1e+3) << endl; // 2000
   cout << roundup2(0.9e+3) << endl; //
   cout << roundup2(2.1e+3) << endl; //
   cout << roundup2(3.1e+3) << endl; //
   cout << roundup2(4.1e+3) << endl; //
   cout << roundup2(7.1e+3) << endl; //
   cout << rounddown2(2.1) << endl; // 2
   cout << rounddown2(4.1) << endl; // 2

   cout << "5  & 10 roundup10 " << endl;
   cout << roundup5(1.1) << endl; // 5
   cout << roundup5(0.9) << endl; // 1
   cout << roundup5(2.1) << endl; // 5
   cout << roundup5(3.1) << endl; // 5
   cout << roundup5(4.1) << endl; // 5
   cout << roundup5(5.1) << endl; // 10
   cout << roundup5(7.1) << endl; // 10
   cout << roundup5(1.1e-3) << endl; //
   cout << roundup5(0.9e-3) << endl; //
   cout << roundup5(2.1e-3) << endl; //
   cout << roundup5(3.1e-3) << endl; //
   cout << roundup5(4.1e-3) << endl; //
   cout << roundup5(7.1e-3) << endl; // 0.01
   cout << roundup5(1.1e+3) << endl; // 5000
   cout << roundup5(0.9e+3) << endl; // 1000
   cout << roundup5(2.1e+3) << endl; //  5000
   cout << roundup5(3.1e+3) << endl; //  5000
   cout << roundup5(4.1e+3) << endl; // 5000
   cout << roundup5(7.1e+3) << endl; // 10000
   cout << rounddown5(4.1) << endl; // 5
   cout << rounddown5(5.1) << endl; // 5

return 0;

} // int main()

/*

Autorun "j:\Cpp\SVG\debug\demo_rounds.exe"
decimal roundup10
1
10
0.0001
10
5
5e-005
5000
50
2
0.002
100
200
5
binary roundup10
2
1
4
4
6
6
8
0.002
0.001
0.004
0.004
0.006
0.008
2000
1000
4000
4000
6000
8000
5
1
5
5
5
10
10
0.005
0.001
0.005
0.005
0.005
0.01
5000
1000
5000
5000
5000
10000
5
1
2
4
5
5


*/
