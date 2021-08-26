#include "Application.h"
#include <iostream>


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


