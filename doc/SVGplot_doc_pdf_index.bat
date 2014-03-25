rem SVGplot_doc_pdf_index.bat
echo off
rem regeneration of SVG plot pdf with index docs.
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
b2 -a --enable-index pdf > svgplot_doc_pdf_index_%date%_%tim%.log
if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
if ERRORLEVEL 1 goto :exit
rem echo "boost-no-inspect" > inspect_pdf_index_%date%_%tim%.html
rem cd ./html
rem inspect.exe >> ../inspect_pdf_index_%date%_%tim%.html 2>&1
rem if not ERRORLEVEL 0 (echo Errorlevel is %ERRORLEVEL%) else (echo OK)
:exit

pause
rem Copyright Paul A. Bristow 2010 to 2013
rem
rem Distributed under the Boost Software License, Version 1.0.
rem (See accompanying file LICENSE_1_0.txt 
rem or copy at http://www.boost.org/LICENSE_1_0.txt)

 