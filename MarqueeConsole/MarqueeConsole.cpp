#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// Function to write marquee text

typedef struct bounds
{
    short x0;
    short y0;
    short xf;
    short yf;
} bounds;

int fps = 60;

bounds bnd = { 5, 5, 40, 20 };
bool stopmarquee = false;

void marqueeText()
{
    HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    ci.bVisible = false;
    SetConsoleCursorInfo(hnd, &ci);
    COORD marqueecrd = { bnd.x0, bnd.y0 };
    COORD prev = { -1, -1 };
    short xp = 1;
    short yp = 1;
    while (!stopmarquee)
    {
        if (prev.X != -1 && prev.Y != -1)
        {
            SetConsoleCursorPosition(hnd, prev);
            std::cout << "         ";
        }
        SetConsoleCursorPosition(hnd, marqueecrd);
        std::cout << "Marquee!";
        prev = marqueecrd;
        if (marqueecrd.X > bnd.xf)
        {
            xp = -1;
        }
        else if (marqueecrd.X < bnd.x0)
        {
            xp = 1;
        }
        if (marqueecrd.Y > bnd.yf)
        {
            yp = -1;
        }
        else if (marqueecrd.Y < bnd.y0)
        {
            yp = 1;
        }
        marqueecrd.X += xp;
        marqueecrd.Y += yp;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
    }
}

void handleInput()
{
    HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SetConsoleCursorPosition(hnd, {0, 0});
    std::cout << "====================THIS IS A MARQUEE CONSOLE====================";
    GetConsoleScreenBufferInfo(hnd, &csbi);
    int bottomRow = csbi.dwSize.Y - 1;
    std::string inputcmd;
    do
    {
        SetConsoleCursorPosition(hnd, { 0, SHORT(bottomRow) });
        std::cout << "Enter Command Here: ";
        std::cin >> inputcmd;
        
    } while (inputcmd.compare("exit") != 0);
    stopmarquee = true;
}


int main() {
    
    std::thread marquee_thread(marqueeText);

    handleInput();

    marquee_thread.join();
    system("cls");
    return 0;
}
