rem SVGplot_doc_html_index.bat
echo off
rem regeneration of SVG plot html with index docs.
rem Runs bjam jamfile.v2 and pipes the output to named log file,
rem including the date and time, so that each log file has a unique name,
rem Assumes Windows data format is dd-MMM-yy and HH::mm
rem for example: quickdoc_23-Mar-2010_1751_html.log
rem The DOS time format is assumed 12:34 and the : separator is not used.
set t=%time% /T
set tim=%t:~0,2%%t:~3,2%
rem pick just hours and minutes.
rem time may include leading space, like " 915", so remove space.
set tim=%tim: =%
b2 html -a --enable-index > svgplot_doc_html_index_%date%_%tim%.log
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
if ERRORLEVEL 1 goto :exit
rem echo "boost-no-inspect" >  inspect_html_index_%date%_%tim%.html
rem cd ./html
rem inspect.exe >> ../inspect_html_index_%date%_%tim%.html 2>&1
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
:exit
pause
rem Copyright Paul A. Bristow 2014
rem Distributed under the Boost Software License, Version 1.0. 
rem See accompanying file LICENSE_1_0.txt
rem or copy at http://www.boost.org/LICENSE_1_0.txt)


 