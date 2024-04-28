#include <iostream>
#include "Core/Monument/Monument.h"

#include "Core/GreatBath/Logger.h"

#define formatwrapper(str, ...) std::vformat(str, std::make_format_args(__VA_ARGS__))

int main(int argc, char *argv[])
{
    LOG_TRACE_QUEUE("Hello");
    LOG_FLUSH_QUEUE();
    std::string s = "hi {:#b}";
    int l = 5;
    LOG_TRACE_QUEUE(s, l);
    std::cout << formatwrapper(s, l) << std::endl;
    std::cout << l << std::endl;
    std::cout << "Hello World!" << std::endl;
    return 0;
}