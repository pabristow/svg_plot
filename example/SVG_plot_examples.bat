rem svg_plot_examples.bat
echo off
rem Do not forget to SVN update jamroot which contains the (updated) Boost version number.
rem This changes the version number suffix!
rem cd boost-trunk/tools/build/v2/
rem boostrap.bat
rem Build all SVG_plot examples
rem including the date and time, so that each log file has a unique name,
rem for example: math_test_23-Mar-2010_1751_html.log
rem The DOS time format is assumed 12:34 and the : separator is not used.
set t=%time% /T
set tim=%t:~0,2%%t:~3,2%
rem pick just hours and minutes.
rem time may include leading space, like " 915", so remove space.
set tim=%tim: =%
rem needed for Clang 3.1
set PATH=c:/mingw32-dw2/bin;%PATH%
echo on
b2 -a -j4 toolset=msvc > svg_plot_examples_%date%_%tim%.log 2>&1
b2 -a -j4 toolset=gcc --build-type=complete  >> svg_plot_examples_%date%_%tim%.log 2>&1
b2 -a -j4 toolset=clang-3.1 --build-type=complete  >> svg_plot_examples_%date%_%tim%.log 2>&1

if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
pause
rem Copyright Paul A. Bristow 2014
rem Distributed under the Boost Software License, Version 1.0. 
rem See accompanying file LICENSE_1_0.txt
rem or copy at http://www.boost.org/LICENSE_1_0.txt)
