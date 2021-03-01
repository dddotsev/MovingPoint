#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// #pragma comment(lib, "user32.lib")
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.14159265358979323846
#define g 2

typedef struct _FLOATING_COORD {
    double X;
    double Y;
} FLOATING_COORD;

const FLOATING_COORD pos_start = {5, 10};
const double width = 70;
const double timestep = 0.01; // s

const COORD hide_coord = {0, 0};

double mass = 20;
double u = 0.1;
double speed;
FLOATING_COORD pos;
double angle;
double font_ratio;

short actual_width;
short actual_height;

COORD toCoord(FLOATING_COORD fc) {
    COORD c = {(short) fc.X, (short) (fc.Y * font_ratio)};
    return c;
}

bool move(HANDLE hOutput) {
    double acc = - u * mass * g;
    double dist = speed * timestep + acc * timestep * timestep / 2;
    speed += acc * timestep;

    if (speed < 0) {
        speed = 0;
        return false;
    }

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
        if (new_coord.X < 0) {
            new_coord.X = 0;
            angle = 3 * PI - angle;
        }
        else if (new_coord.Y < 0) {
            new_coord.Y = 0;
            angle = 2 * PI - angle;
        }
        else if (new_coord.X >= actual_width + 1) {
            new_coord.X = actual_width;
            angle = PI - angle;
        }
        else if (new_coord.Y >= actual_height + 1) {
            new_coord.Y = actual_height;
            angle = 2 * PI - angle;
        }

        SetConsoleCursorPosition(hOutput, old_coord);
        WriteConsole( hOutput, " ", 1, NULL, NULL );

        SetConsoleCursorPosition(hOutput, new_coord);
        WriteConsole( hOutput, "o", 1, NULL, NULL );

        SetConsoleCursorPosition(hOutput, hide_coord);
    }

    return true;
}

void main( void )
{
    printf("Enter start angle in radians relative to horizonal axis:\n");
    scanf("%lf", &angle);
    printf("Enter start speed:\n");
    scanf("%lf", &speed);
    printf("Enter mass:\n");
    scanf("%lf", &mass);
    printf("Enter friction coefficient:\n");
    scanf("%lf", &u);

    pos = pos_start;

    HANDLE hOutput = (HANDLE)GetStdHandle( STD_OUTPUT_HANDLE );

    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    
    GetConsoleScreenBufferInfo(hOutput, &SBInfo);

    actual_width = SBInfo.srWindow.Right;
    actual_height = SBInfo.srWindow.Bottom;

    COORD sPos;
    sPos.X = 5;
    sPos.Y = 10;
    SetConsoleCursorPosition( hOutput, sPos );

    // Set the color to bright green
    SetConsoleTextAttribute( hOutput,
    FOREGROUND_INTENSITY | FOREGROUND_GREEN );

    CONSOLE_FONT_INFO info;
    GetCurrentConsoleFont(hOutput, false, &info);
    COORD font_size = GetConsoleFontSize(hOutput, info.nFont);

    font_ratio = (float)font_size.X / font_size.Y;

    while (move(hOutput)) {
        Sleep(timestep * 1000);
    }

    scanf("\n");
}
