#include <iostream>
#include "Core/Monument/Monument.h"

#include "Core/GreatBath/Logger.h"

#define formatwrapper(str, ...) std::vformat(str, std::make_format_args(__VA_ARGS__))

int main(int argc, char *argv[])
{
    REGISTER_LOGGING_STREAM(gbt::LOGLEVEL_TRACE, std::cout);
    LOG_TRACE_QUEUE("Hello");
    LOG_FLUSH_QUEUE();
    std::string s = "some error msgs {:#b}";
    int l = 5890;
    LOG_FATAL_QUEUE(s, l);
    std::cout << formatwrapper(s, l) << std::endl;
    std::cout << l << std::endl;
    std::cout << "\033[31mHello World!" << std::endl;
    return 0;
}