/*! @file
    @brief Выводит дату или время в формате C/C++ макроса или просто строки. Предназначен для генерации макросов даты и времени сборки для всего проекта 
 */

#include <iostream>
#include <fstream>
#include <string>

//#include <time.h>
#include <ctime>
#include <clocale>



using std::cout;
using std::cerr;

enum OutputType
{
   otNone   ,
   otDate   ,
   otTime   ,
   otDateLf ,
   otTimeLf ,
   otDateQ  ,
   otTimeQ  ,
   otDateQlf,
   otTimeQlf,
   otDatePp ,
   otTimePp
};


inline
std::string strftimeUtil(const char *fmt, const std::tm & t)
{
    char buf[256];
    buf[std::strftime(&buf[0], sizeof(buf), fmt, &t)] = 0;
    return std::string(&buf[0]);
}

inline
void printUsage()
{
    cerr << "Usage: umba-date [-lttime|-localtime|-gmtime] [-filename] -Type1 File1.txt [-Type2 File2.txt...]\n";
    cerr << "  Where:\n"
            "    -lttime,\n"
            "    -localtime   - set local time to output\n"
            "    -gmtime      - set GMT time to output\n"
            "    -filename    - use file name compatible date and time format. Date is also in form: 'YYYY-MM-DD' for best sorting\n"
            "    -notfilename - turns '-filename' mode OFF\n"
            "    -date        - simple date, no quotes, no linefeed\n"
            "    -time        - simple time, no quotes, no linefeed\n"
            "    -datelf      - simple date with linefeed\n"
            "    -timelf      - simple time with linefeed\n"
            "    -dateq       - simple date in quotes\n" 
            "    -timeq       - simple time in quotes\n" 
            "    -dateqlf     - simple date in quotes with linefeed\n"
            "    -timeqlf     - simple time in quotes with linefeed\n"
            "    -datepp      - date as C/C++ macro expanding to string\n" 
            "    -timepp      - time as C/C++ macro expanding to string\n"
            ;
}


int main(int argc, char* argv[])
{
    // https://en.cppreference.com/w/cpp/locale/setlocale

    std::setlocale(LC_ALL, "C");

    // https://en.cppreference.com/w/cpp/chrono/c/asctime
    // https://en.cppreference.com/w/cpp/chrono/c/ctime
    // https://en.cppreference.com/w/cpp/chrono/c/tm
    // https://en.cppreference.com/w/cpp/chrono/c/localtime
    // https://en.cppreference.com/w/cpp/chrono/c/gmtime
    // https://en.cppreference.com/w/cpp/chrono/c/time_t

    auto timeNow = std::time(0);

	std::tm ltNow = *std::localtime(&timeNow);
    std::tm gmNow = *std::gmtime(&timeNow);

    //std::string ltDateNowStr = strftimeUtil("%b %e %Y", ltNow); // %e is listed here - https://en.cppreference.com/w/cpp/chrono/c/strftime - but not working
    std::string ltDateNowStr = strftimeUtil("%b %d %Y", ltNow);
    std::string ltTimeNowStr = strftimeUtil("%H:%M:%S", ltNow);
    std::string gmDateNowStr = strftimeUtil("%b %d %Y", gmNow);
    std::string gmTimeNowStr = strftimeUtil("%H:%M:%S", gmNow);

    std::string ltDateNowFnStr = strftimeUtil("%Y-%m-%d", ltNow);
    std::string ltTimeNowFnStr = strftimeUtil("%H-%M-%S", ltNow);
    std::string gmDateNowFnStr = strftimeUtil("%Y-%m-%d", gmNow);
    std::string gmTimeNowFnStr = strftimeUtil("%H-%M-%S", gmNow);

    bool filenameMode = false;
    bool localTimeUsed = true;

    std::string dateNowStr = ltDateNowStr;
    std::string timeNowStr = ltTimeNowStr;

    // dateNowStr = gmDateNowStr;
    // timeNowStr = gmTimeNowStr;

    // std::cout << "Date: " << dateNowStr << "\n";
    // std::cout << "Time: " << timeNowStr << "\n";

    //  
    // std::tm tm{};
    // std::localtime
    // tm.tm_year = 2022 - 1900;
    // tm.tm_mday = 1;
    // std::mktime(&tm);

    if (argc<=1)
    {
        cerr << "No arguments taken\n";
        printUsage();
    
    }


    OutputType ot = otNone;

    for(int argN=1; argN<argc; ++argN)
    {
        std::string argStr = argv[argN];
        if (argStr.empty())
            continue;

        if (argStr.front()=='-')
        {
            // an option
            argStr.erase(0,1);
            if (argStr.empty())
            {
                cerr << "Invalid (empty) option\n";
                printUsage();
                return 1;
            }

            if (argStr=="gmtime")
            {
                if (!filenameMode)
                {
                    dateNowStr = gmDateNowStr;
                    timeNowStr = gmTimeNowStr;
                }
                else 
                {
                    dateNowStr = gmDateNowFnStr;
                    timeNowStr = gmTimeNowFnStr;
                }
                localTimeUsed = false;
                ot = otNone;
            }
            else if (argStr=="lttime" || argStr=="localtime")
            {
                if (!filenameMode)
                {
                    dateNowStr = ltDateNowStr;
                    timeNowStr = ltTimeNowStr;
                }
                else
                {
                    dateNowStr = ltDateNowFnStr;
                    timeNowStr = ltTimeNowFnStr;
                }
                localTimeUsed = true;
                ot = otNone;
            }
            else if (argStr=="filename")
            {
                if (localTimeUsed)
                {
                    dateNowStr = ltDateNowFnStr;
                    timeNowStr = ltTimeNowFnStr;
                }
                else
                {
                    dateNowStr = gmDateNowFnStr;
                    timeNowStr = gmTimeNowFnStr;
                }
                filenameMode = true;
                ot = otNone;
            }
            else if (argStr=="notfilename")
            {
                if (localTimeUsed)
                {
                    dateNowStr = ltDateNowStr;
                    timeNowStr = ltTimeNowStr;
                }
                else
                {
                    dateNowStr = gmDateNowStr;
                    timeNowStr = gmTimeNowStr;
                }
                filenameMode = false;
                ot = otNone;
            }
            else if (argStr=="date")
                ot = otDate;
            else if (argStr=="time")
                ot = otTime;
            else if (argStr=="datelf")
                ot = otDateLf;
            else if (argStr=="timelf")
                ot = otTimeLf;
            else if (argStr=="dateq")
                ot = otDateQ;
            else if (argStr=="timeq")
                ot = otTimeQ;
            else if (argStr=="dateqlf")
                ot = otDateQlf;
            else if (argStr=="timeqlf")
                ot = otTimeQlf;
            else if (argStr=="datepp")
                ot = otDatePp;
            else if (argStr=="timepp")
                ot = otTimePp;
            else
            {
                cerr << "Invalid option: '-"<<argStr<<"'\n";
                printUsage();
                return 1;
            
            }
        }
        else
        {
            // an file name to write output
            if (ot==otNone)
            {
                 cerr << "Output type not specified\n";
                 printUsage();
                 return 1;
            }

            std::ofstream ofs = std::ofstream(argStr, std::ios_base::out|std::ios_base::trunc);
            if (!ofs)
            {
                cerr << "Failed to open output file: '"<<argStr<<"'\n";
                printUsage();
                return 1;
            }

            switch(ot)
            {
                case otDate   : ofs << dateNowStr; break;
                case otTime   : ofs << timeNowStr; break;
                case otDateLf : ofs << dateNowStr <<"\n"; break;
                case otTimeLf : ofs << timeNowStr <<"\n"; break;
                case otDateQ  : ofs << "\""<<dateNowStr<<"\""; break;
                case otTimeQ  : ofs << "\""<<timeNowStr<<"\""; break;
                case otDateQlf: ofs << "\""<<dateNowStr<<"\"\n"; break;
                case otTimeQlf: ofs << "\""<<timeNowStr<<"\"\n"; break;
                case otDatePp : ofs << "#define UMBA_DATE__  \""<<dateNowStr<<"\"\n"; break;
                case otTimePp : ofs << "#define UMBA_TIME__  \""<<timeNowStr<<"\"\n"; break;
                default: 
                {
                    cerr << "Unknown state while writting output file '"<<argStr<<"'\n";
                    printUsage();
                    return 1;
                }
            }
        }
    }

    return 0;
}


