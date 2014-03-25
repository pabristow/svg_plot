rem SVGplot_docs.bat
echo off
rem regeneration of SVG plot html and pdf docs (no indexes).
rem and pipes the output to log files including the date and time,
rem so that each log file has a unique name, for example:
rem quickdox_10-Jun-2010_930_html.log
rem The DOS time format is assumed 12:34 and the : separator is not used.
set t=%time% /T
rem echo %t%
set tim=%t:~0,2%%t:~3,2%
rem pick just hours and minutes.
rem echo "%tim%"
rem may include leading space, like " 915", so remove space.
set tim=%tim: =%
b2 -a html > svgplot_doc_html_%date%_%tim%.log
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
if ERRORLEVEL 1 goto :exit
b2 -a pdf > svgplot_doc_pdf_%date%_%tim%.log
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
echo "boost-no-inspect" >  inspect_%date%_%tim%.html
cd ./html
inspect.exe >> ../inspect_%date%_%tim%.html 2>&1
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
:exit
pause
rem Copyright Paul A. Bristow 2013
rem Distributed under the Boost Software License, Version 1.0. (See accompany-
rem ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 