# Umba-date - Date and Time Generation Utility

Pirpouse: use it in your build pipeline to generate macros `UMBA_DATE__` and `UMBA_TIME__` 
to insert them to your code, and for add the same time and date to the output file name.
Generate it single time, and use it for all of your targets (unlike the __DATE__ and __TIME__
macros which are has different values for each C/C++ file, and also different with `date` output).

Compile it using VSCode (`type code .` in command prompt) or by calling `umba-date.bat` 
batch file (requires G++ available in your PATH).

Run `test.bat` to see how it works.

```
Usage: umba-date [-lttime|-localtime|-gmtime] [-filename] -Type1 File1.txt [-Type2 File2.txt...]
  Where:
    -lttime,
    -localtime   - set local time to output
    -gmtime      - set GMT time to output
    -filename    - use file name compatible date and time format. Date is also in form: 'YYYY-MM-DD' for best sorting
    -notfilename - turns '-filename' mode OFF
    -date        - simple date, no quotes, no linefeed
    -time        - simple time, no quotes, no linefeed
    -datelf      - simple date with linefeed
    -timelf      - simple time with linefeed
    -dateq       - simple date in quotes
    -timeq       - simple time in quotes
    -dateqlf     - simple date in quotes with linefeed
    -timeqlf     - simple time in quotes with linefeed
    -datepp      - date as C/C++ macro expanding to string
    -timepp      - time as C/C++ macro expanding to string
```

Example: **test.bat**
```
umba-date.exe -gmtime  ^
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
-timepp    timepp.txt
```

Enjoy it!