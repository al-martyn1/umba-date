@if exist %~dp0\umba-date.exe @del %~dp0\umba-date.exe
g++ -std=c++17 umba-date.cpp -o %~dp0\umba-date.exe
@rem Copy it to your PATHs
