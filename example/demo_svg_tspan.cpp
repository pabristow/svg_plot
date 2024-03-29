/*! \file demo_svg_tspan.cpp
  \brief demonstrate use of SVG tspan command.
  \details
    Shows the use of SVG tspan command to control text layout.
    tspen is needed to continue text
    changing color, position(next line effect), superscript or subscript.
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2008, 2020

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/svg_plot/svg.hpp>
//using namespace boost::svg;
#include <boost/cstdlib.hpp> // 
#include <boost/lexical_cast.hpp>

#include <iostream>
//using std::cout;
//using std::endl;
//using std::boolalpha;
//
int main()
{
  using namespace boost::svg;
  try
  {
    text_style default_style;  // Using constructor with all default values.

    svg doc;  // Construct a new (empty or default) SVG document.
    doc.size(500, 400); // Change its (non-default) size.
    std::cout << "doc.document_size() = " <<  doc.document_size() << std::endl;

    g_element& g = doc.add_g_element(); // Add first (zeroth) new element to the document array of g_elements gs,
    g_element& g0 = doc.gs(0); // so index of g_elements is zero.
    BOOST_ASSERT_MSG((g == g0), "first added g_element must be [0]!");
    std::cout << "my_svg.document_size() = number of g_elements = " << doc.document_size() << std::endl; // == 1
    
    g0.id("group element 0"); // Add an ID to this group.
    std::cout << "g0.id() " << g0.id() << std::endl; // and echo ID assigned.
    
 //   doc.gs(0).push_back(new rect_element(0, 0, doc.x_size(), doc.y_size())); // border to image.
    g0.push_back(new rect_element(0, 0, doc.x_size(), doc.y_size()));

    // Push causes output of a group and style info
    // <g id="group element 0" stroke="rgb(255,255,0)" fill="rgb(240,255,255)" stroke-width="5">
    //   <rect x="0" y="0" width="400" height="400" />  // Border in yellow.
    // </ g>

    g0.rect(100, 200, 300, 350); // is yellow bottom right outputs only rect: info.
    // <rect x="100" y="200" width="300" height="350"/>  (NO style info so uses the group style)

    // Not using push_back outputs just the rect coordinates and size.
    rect_element& r = doc.rect(10, 20, 30, 40); // 	<rect x="10" y="20" width="30" height="40"/>  is black - default color.
    // and NOT in group g0 "group element 0"

    // Set some SVG style color and width info in "group_element 0", g0.
    g0.style().stroke_on(true); // stroke on true
    g0.style().stroke_color(yellow); // 
    g0.style().stroke_width(5);
    std::cout << "width = " << g0.style().stroke_width() << std::endl;
    g0.style().fill_on(true); // stroke on true
    g0.style().fill_color(azure); // 

    const text_style serif_style(10, "serif");
    // Simple text_element.
  //  text_element& t = doc.text(100, 100, "This ", no_text_style, align_style::center_align, uphill);
    // Default font style, but uphill.
    text_element& t = doc.text(100, 100, "Text_0", serif_style, align_style::center_align, horizontal);
    // Add more text using tspan, hoping to avoid output of font etc info, but still output.
   // tspan_element ts;
     //ts = t.tspan("text_1").x(100.).dy(20.);
     //ts = t.tspan("text_2").x(100.).dy(40.);
     //ts = t.tspan("text_3").x(100.).dy(60.);

     for (int i = 1; i <= 4; i++)
     {
       std::string no = boost::lexical_cast<std::string>(i); // Convert 1 to "1" ...
       std::string s = "text_" + no; // "Text_1, ...
       tspan_element ts = t.tspan(s, not_a_text_style).x(100.).dy(20.);
     }

     g0.text(300, 300, "Text g0", no_text_style); // Uses g0 style (image border) - so yellow and wide and almost illegible!

  //  // Now output a tspan element moved down over a bit.
    tspan_element ts = t.tspan("text_n").dx(100.).dy(20.)
      .font_size(40).font_family("Arial").font_weight("bold")
      .stroke_color(purple).fill_color(pink).stroke_width(2); // tspan_element should set fill_on and stroke_on == true.
    // Shows in expected place and colors and width too.
   // Outputs:
   // <	<tspan stroke="rgb(128,0,128)" fill="rgb(255,192,203)" stroke-width="2" 
    //   dx="100" dy="20" font-size="40" font-family="Arial" font-weight="bold">text_n
    // </tspan>

    std::cout<< "dx " << ts.dx() << std::endl; // 100
    std::cout<< "dy " << ts.dy() << std::endl; // 20
    std::cout<< "tspan text is " << ts.text() << std::endl; // text_n
    std::cout << "ts.text_style() is " << ts.text_style() << std::endl; // ts.text_style() is text_style(40, "Arial", "", "bold", "", "")
    std::cout<< "font size is " << ts.font_size() << std::endl; // 40
    std::cout<< "font family " << ts.font_family() << std::endl; // Arial
    std::cout<< "font weight is " << ts.font_weight() << std::endl; // bold
    std::cout<< "font_decoration is " << ts.text_style_.font_decoration() << std::endl; // ""
    std::cout<< "font_decoration is " << ts.text_style().font_decoration() << std::endl; // ""
    // and inherits from svg_element so can read
    std::cout<< "style is " << ts.style() << std::endl; 
    //  style is svg_style(RGB(0,0,0), blank, 0, no stroke, no fill, no width) 
    std::cout<< "stroke color " << ts.stroke_color() << std::endl; // stroke color RGB(0,0,0) == black
    std::cout<< "stroke on " << std::boolalpha << ts.stroke_on() << std::endl; // false but expect true???
    std::cout<< "fill color " << ts.fill_color() << std::endl; // blank but expect pink == rgb(255,192,203)
    std::cout<< "fill on " << std::boolalpha << ts.fill_on() << std::endl; // false but expect true from
    std::cout<< "width_on is  " << ts.width_on() << std::endl; // false
    std::cout<< "stroke_width is " << ts.stroke_width() << std::endl; // 0
    std::cout<< "ts.style().width_on() " << ts.style().width_on() << std::endl; // false
    std::cout<< "ts.style().stroke_width() " << ts.style().stroke_width() << std::endl; // 0
    std::cout<< "ts.style().stroke_color() " << ts.style().stroke_color() << std::endl; // ts.style().stroke_color() RGB(0,0,0) 
    std::cout<< "ts.style().fill_color() " << ts.style().fill_color() << std::endl; // ts.style().fill_color() blank

  //    .fill_color(pink).fill_on(true).stroke_color(purple).stroke_on(true);

    std::cout << "ts = " << ts << std::endl; // Show the full state including inherited svg style.
  // ts = tspan(0, 0, 100, 20, 0, 0, relative, relative, text_style(40, "Arial", "", "bold", "", ""), 
    //             svg_style(RGB(0,0,0), blank, 0, no stroke, no fill, no width))



  //  //<!--File demo_svg_tspan.svg-->
  //  //  <text x = "100" y = "100" text-anchor = "middle" transform = "rotate(-45 100 100)"
  //  //    font-size = "12" font-family = "Lucida Sans Unicode">This 	<tspan fill = "rgb(255,192,203)" dx = "10" dy = "20" font - size = "40" font - family = "Arial" font - weight = "bold">text< / tspan>
  //  //   <tspan dx = "50" dy = "50" font-size = "20" font-family = "serif" font-style = "bold">text2</tspan>
  //  //  < / text>
  //  //  < / svg>

  //  // Shows pink fill and stroke purple as expected.

  //  // These lines below have no effect on the output svg tspan!
  //  //ts.text("green 30 Arial");
  //  //ts.dx(1).dy(5.);
  //  //ts.style().fill_color(green);
  //  //ts.font_size(30);
  //  //ts.font_family("Arial");
  //  //ts.font_style().font_size(30);
  //  //ts.font_style().font_family("Arial");

  //  // Data is OK in debug here.
  //  // So the info is stored OK, but not used on the write.

  //  // <text x="100" y="100" text-anchor="middle" transform = "rotate(-45 100 100 )" font-size="20">
  //  //   This
  //  //     <tspan font-size="20">
  //  //       text
  //  //     </tspan>
  //  // </text>

  //  // Only shows "this text"
  //  // NO offset and style info added to ts
  //  // NO tspan text for  "green 30 Arial"

  //  text_style my_ts;
  //  my_ts.font_family("serif").font_style("bold").font_size(20); // Set some font styling.
  //  std::cout << "text_style my_ts = " << my_ts << std::endl;
  //  // text_style my_ts = text_style(20, "serif", "bold", "", "", "")

  //  tspan_element ts2 = t.tspan("text_2", my_ts).dx(50).dy(50); // Use constructor to use my_ts text_style.
  //  std::cout << "ts2 = " << ts2 << std::endl; 
  //  // ts2 = tspan(0, 0, 50, 50, 0, 0, relative, relative, text_style(20, "serif", "bold", "", "", ""))
  //  ts2.textstyle(my_ts); // Use set function to set text_style.
  //  std::cout << "ts2 = " << ts2 << std::endl;
  //  // ts2 = tspan(0, 0, 50, 50, 0, 0, relative, relative, text_style(20, "serif", "bold", "", "", ""))
  //  text_style tst2 = ts2.textstyle(); // Use get function to read the tspan text_style.
  //  std::cout << "tst2 = " << tst2 << std::endl; // text_style(20, "serif", "bold", "", "", "")
  //  // 	<tspan dx="50" dy="50" font-size="20" font-family="serif" font-style="bold">text_2</tspan>

  //  tspan_element ts3 = t.tspan("text_3", my_ts).dx(70).dy(70).fill_color(pink).stroke_color(purple); ; // Use constructor to use my_ts text_style.
  //  std::cout << "ts3 = " << ts3 << std::endl;
  //  // ts3 = tspan(0, 0, 70, 70, 0, 0, relative, relative, text_style(20, "serif", "bold", "", "", ""))
  //  // <tspan stroke="rgb(128,0,128)" fill="rgb(255,192,203)" dx="70" dy="70" font-size="20" font-family="serif" font-style="bold">text_3</tspan>


    doc.write("demo_svg_tspan.svg");

    return boost::exit_success;
  }
catch (const std::exception& e)
{
  std::cout<<
    "\n""Message from thrown exception was:\n   " << e.what() << std::endl;
  return boost::exit_failure;
}

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
