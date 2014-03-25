rem SVGplot_doc_pdf.bat
echo off
rem regeneration of SVG plot only html docs.
rem Runs b2 jamfile.v2 (sans -a) and pipes the output to named log file,
rem including the date and time, so that each log file has a unique name,
rem for example: svgplot_23-Mar-2010_1751_html.log
rem The DOS time format is assumed 12:34 and the : separator is not used.
set t=%time% /T
set tim=%t:~0,2%%t:~3,2%
rem pick just hours and minutes.
rem time may include leading space, like " 915", so remove space.
set tim=%tim: =%
b2 pdf -a > svgplot_doc_pdf_%date%_%tim%.log
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
if ERRORLEVEL 1 goto :exit
echo "boost-no-inspect" >  inspect_pdf_%date%_%tim%.html
cd ./html
inspect.exe >> ../inspect_pdf_%date%_%tim%.html 2>&1
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
:exit
pause
rem Copyright Paul A. Bristow 2013
rem
rem Distributed under the Boost Software License, Version 1.0. (See accompany-
rem ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 