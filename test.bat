umba-date.exe -h >umba-date.txt 2>&1

umba-date.exe -gmtime  ^
-macroprefix TEST_     ^
-macrosuffix ___       ^
-date      gmdate.txt  ^
-time      gmtime.txt  ^
-localtime             ^
-date      date.txt    ^
-time      time.txt    ^
-filename              ^
-date      fndate.txt  ^
-time      fntime.txt  ^
-notfilename           ^
-datelf    datelf.txt  ^
-timelf    timelf.txt  ^
-dateq     dateq.txt   ^
-timeq     timeq.txt   ^
-dateqlf   dateqlf.txt ^
-timeqlf   timeqlf.txt ^
-datepp    datepp.txt  ^
-timepp    timepp.txt  ^
-prefix    path/test   ^
-suffix    .exe        ^
-targetprefix another-path\test  ^
-bat       copy_to_archive.bat   ^
-sh        copy_to_archive.sh


