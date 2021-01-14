#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// #pragma comment(lib, "user32.lib")
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct _FLOATING_COORD {
    double X;
    double Y;
} FLOATING_COORD;

const double speed_start = 10;
const FLOATING_COORD pos_start = {5, 10};
const double angle_start = PI + PI /2 + PI / 4;
const double mass = 20;
const double u = 0;
const double width = 140;
const double timestep = 0.01; // s

const COORD hide_coord = {0, 0};

double speed;
FLOATING_COORD pos;
double angle;
double font_ratio;

COORD toCoord(FLOATING_COORD fc) {
    COORD c = {(short) fc.X, (short) (fc.Y * font_ratio)};
    return c;
}

void move(HANDLE hOutput) {
    double dist = speed * timestep;
    double dx, dy;

    if (angle >= 0 && angle < PI / 2) {
        dx = cos(angle) * dist;
        dy = -sin(angle) * dist;
    }
    else if (angle >= PI / 2 && angle < PI) {
        double current_angle = angle - PI / 2;
        dx = -sin(current_angle) * dist;
        dy = -cos(current_angle) * dist;
    }
    else if (angle >= PI && angle < PI + PI / 2) {
        double current_angle = 3 * PI / 2  - angle;
        dx = -cos(current_angle) * dist;
        dy = sin(current_angle) * dist;
    }
    else {
        double current_angle = PI * 2 - angle;
        dx = cos(current_angle) * dist;
        dy = sin(current_angle) * dist;
    }

    FLOATING_COORD new_pos = {pos.X + dx, pos.Y + dy};
    COORD old_coord = toCoord(pos);
    COORD new_coord = toCoord(new_pos);
    pos = new_pos;

    if (old_coord.X != new_coord.X || old_coord.Y != new_coord.Y) {
        SetConsoleCursorPosition(hOutput, old_coord);
        WriteConsole( hOutput, " ", 1, NULL, NULL );

        SetConsoleCursorPosition(hOutput, new_coord);
        WriteConsole( hOutput, "o", 1, NULL, NULL );

        SetConsoleCursorPosition(hOutput, hide_coord);
    }
}

void main( void )
{
    // scanf("\n");

    // TODO: enter width from console
    speed = speed_start; // TODO: enter from console
    angle = angle_start; // TODO: enter from console

    pos = pos_start;

    HANDLE hOutput = (HANDLE)GetStdHandle( STD_OUTPUT_HANDLE );

    CONSOLE_FONT_INFO info;
    GetCurrentConsoleFont(hOutput, false, &info);
    COORD font_size = GetConsoleFontSize(hOutput, info.nFont);

    font_ratio = (float)font_size.X / font_size.Y;

    // SMALL_RECT size;
    // size.Top = 0;
    // size.Left = 0;
    // size.Bottom = 50;
    // size.Right = 100;
    // SetConsoleWindowInfo(hOutput, true, &size);
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    GetConsoleScreenBufferInfo(hOutput, &SBInfo);

    SBInfo.srWindow.Right = width;
    SBInfo.srWindow.Bottom = width * font_ratio;

    // TODO: fix buffer to remove scroll bars
    // COORD size_c;
    // size_c.X = SBInfo.srWindow.Right + 1 / font_ratio;
    // size_c.Y = SBInfo.srWindow.Bottom + 1;
    // SetConsoleScreenBufferSize(hOutput, size_c);

    int Status = SetConsoleWindowInfo(hOutput, true, &SBInfo.srWindow);

    if (Status == 0)
    {
        Status = GetLastError();
    }


    // int x = GetSystemMetrics(SM_CXMIN);
    // int y = GetSystemMetrics(SM_CYMIN);




    // Set the text output position to (5,10)
    COORD sPos;
    sPos.X = 5;
    sPos.Y = 10;
    SetConsoleCursorPosition( hOutput, sPos );

    // Set the color to bright green
    // SetConsoleTextAttribute( hOutput,
    // FOREGROUND_INTENSITY | FOREGROUND_GREEN );

    while (true) {
        move(hOutput);
        Sleep(timestep * 1000);
    }

    scanf("\n");
}
