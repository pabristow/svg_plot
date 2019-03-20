#          Copyright Michael Tesch 2019.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          https://www.boost.org/LICENSE_1_0.txt)

find_package (Boost REQUIRED COMPONENTS unit_test_framework )

foreach (BASE 
    1d_color_consistency
    1d_tests
    2d_color_consistency
    svg_test_2d_boxplot
    test_svg
    test_svg_xml
    )
  add_executable (${BASE} ${BASE}.cpp)
  target_link_libraries (${BASE} ${Boost_LIBRARIES})
  target_compile_definitions (${BASE} PUBLIC -DBOOST_TEST_MAIN)
  add_test (NAME ${BASE} COMMAND ${BASE})
endforeach()
