#include "Application.h"
#include <iostream>


int getSum(int lhs, int rhs)
{
    return lhs + rhs;
}

using namespace std::placeholders;

int main()
{
    try 
    {
        Application app;
        app.run();
    }
    catch (const std::exception& ex)
    {
        std::cout << "EXCEPTION: " << ex.what() << '\n';

          system("pause"); // FIXME: It's not portable!
    }

    return 0;
}


