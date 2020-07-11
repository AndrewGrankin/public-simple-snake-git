// Test project for any tests

#include <iostream>
#include <windows.h>
using namespace std;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

char keyChar = 'f';

const int G_OVER_WIDTH = 16;

void input()
{
    if (GetAsyncKeyState(VK_LEFT))
    {
        dir = LEFT;
        keyChar = 't';
    } else if (GetAsyncKeyState(VK_RIGHT))
            {
                dir = RIGHT;
                keyChar = 't';
            } else if (GetAsyncKeyState(VK_UP))
                    {
                        dir = UP;
                        keyChar = 't';
                    } else if (GetAsyncKeyState(VK_DOWN))
                            {
                                dir = DOWN;
                                keyChar = 't';
                            } else {
                                keyChar = 'f';
                            }
}

int main()
{

    

    while (true)
    {


    cout << "Test score" << endl;
    // add score info to the message
    int score = 40;
    char scoreString[100];
    sprintf(scoreString, "Score: %d", score);          // convert int score to char array

    cout << scoreString;


    cout << endl;


        input();        

        bool key = false;
        
        if (GetAsyncKeyState(VK_LEFT))
        {
            key = true;
            
        }

        char pKey = ' ';
        if (GetAsyncKeyState(0x50))
        {
            pKey = 'P';
        }

        cout << "dir = " << dir << endl;
        cout << "Get Right Key: " << GetAsyncKeyState(VK_RIGHT) << endl;
        cout << "Key is " << key << endl;
        cout << "Key Char is " << keyChar << endl;
        cout << "39 % 20 = " << 39%20 << endl;
        cout << "P key = " << pKey << endl;

        system("cls");
    }

    return 0;
}

