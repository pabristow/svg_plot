/*! \file demo_svg_tspan.cpp
  \brief demonstrate use of SVG tspan command.
  \details
    Shows the use of SVG tspan command to control text layout.
    tspen is needed to continue text
    changing color, position(next line effect), superscript or subscript.
  \author Jacob Voytko \& Paul A. Bristow

  \date 2007
*/

// Copyright Jacob Voytko 2008
// Copyright Paul A. Bristow 2008

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifdef _MSC_VER
#  pragma warning (disable : 4512) // assignment operator could not be generated.
#endif

#include <boost/svg_plot/svg.hpp>
using namespace boost::svg;

#include <iostream>
using std::cout;
using std::endl;
using std::boolalpha;

int main()
{
    svg doc;

    doc.size(400, 400);

    text_element& t = doc.text(100, 100, "This ", no_style, center_align, uphill);
    tspan_element ts = t.tspan("text").dx(10.).dy(20.).font_size(40).font_family("Arial").font_weight("bold").fill_color(pink);
    cout << "dx " << ts.dx() << endl;
    cout << "dy " << ts.dy() << endl;
    cout << "tspan text is " << ts.text() << endl;
    cout << "font is " << ts.font_size() << endl;
    cout << "font family " << ts.font_family() << endl;
    cout << "font weight is " << ts.font_weight() << endl;
    cout << "fill color " << ts.fill_color() << endl; // expect pink == rgb(255,192,203) but get blank!
    cout << "fill on " << boolalpha << ts.fill_on() << endl;

    //<text x="100" y="100" text-anchor="middle" transform = "rotate(-45 100 100 )" font-size="20">
    //  This
    //  <tspan fill="rgb(255,192,203)" dx="10" dy="20" font-size="40" font-family="Arial" font-weight="bold">
    //    text
    //  </tspan>
    //</text>

    // shows expected This text with position and color as expected,
    // BUT doesn't echo fill color????

    // These lines below have no effect on the output svg tspan!
    //ts.text("green 30 Arial");
    //ts.dx(1).dy(5.);
    //ts.style().fill_color(green);
    //ts.font_size(30);
    //ts.font_family("Arial");
    //ts.font_style().font_size(30);
    //ts.font_style().font_family("Arial");

    // Data is OK in debug here.
    // So the info is stored OK, but not used on the write.

    // <text x="100" y="100" text-anchor="middle" transform = "rotate(-45 100 100 )" font-size="20">
    //   This
    //     <tspan font-size="20">
    //       text
    //     </tspan>
    // </text>

    // Only shows "this text"
    // NO offset and style info added to ts
    // NO tspan text for  "green 30 Arial"

    text_style my_ts;
    my_ts.font_family("serif").font_style("bold").font_size(20);
    tspan_element ts2 = t.tspan("text2", my_ts).dx(50).dy(50); // Use constructor to set text_style.
    ts2.textstyle(my_ts); // Use set function to set text_style.
    text_style tst2 = ts2.textstyle(); // Use get function.
    doc.write("demo_svg_tspan.svg");

    return 0;

} // int main()

/*

demo_svg_tspan.cpp
Linking...
Embedding manifest...
Autorun "j:\Cpp\SVG\debug\demo_tspan.exe"
dx 10
dy 20
tspan text is text
font is 40
font family Arial
font weight is bold
fill color blank
fill on false
Build Time 0:02


*/
