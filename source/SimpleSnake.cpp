// Simple Snake Game

#include <iostream>
#include <windows.h>
#include <ctime>

using namespace std;

HANDLE hConsole;
DWORD dwBytesWritten;

const int SCREEN_WIDTH = 25;        // to be able clear screen
const int SCREEN_HEIGHT = 25;       // to be able clear screen
const int MENU_WIDTH = 13;
const int MENU_HEIGHT = 5;
const int SETTINGS_WIDTH = 24;
const int SETTINGS_HEIGHT = 7;
const int GAME_WIDTH = 20;
const int GAME_HEIGHT = 20;
const int G_OVER_WIDTH = 14;
const int G_OVER_HEIGHT = 5;

enum eGameStage {MENU, GAME, SETTINGS};
enum eKey {ESC_KEY, ENTER_KEY, LEFT_KEY, RIGHT_KEY, UP_KEY, DOWN_KEY, P_KEY, R_KEY};
enum eDirection {STOP, LEFT, RIGHT, UP, DOWN};
eGameStage gameStage;
eKey key;
eDirection direction;

char gameTitleLine[] = "*** Welcome to Simple Snake game! ***";
char menuList[MENU_HEIGHT][MENU_WIDTH] = {"/MENU/", " ", "[ ] Play", "[ ] Settings", "[ ] Exit"};
char settingsList[SETTINGS_HEIGHT][SETTINGS_WIDTH] = {"/SETTINGS/", " ", "ESC - to menu", "ARROWS - movement", "ENTER - apply selection", "P - pause game", "R - restart game"}; 
char gameOverList[G_OVER_HEIGHT][G_OVER_WIDTH] = {"Game Over!", " ", " ", "Restart - R", "Menu - ESC"};
char gameField[GAME_HEIGHT][GAME_WIDTH];
char scoreLine[GAME_WIDTH];

int selectedMenuRow;
int fruitX, fruitY;
int snakeX[GAME_HEIGHT * GAME_WIDTH], snakeY[GAME_HEIGHT * GAME_WIDTH];
int snakeLength;
int score;
int gameSpeed;

bool quit;
bool gameOver;
bool isKeyPressed;


void Setup()
{
    // create screen buffer for drawings
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    dwBytesWritten = 0;
    SetConsoleActiveScreenBuffer(hConsole);

    srand(static_cast<unsigned int>(time(0)));  // seed random number generator

    gameStage = MENU;       // start game from MENU view
    quit = false;
    gameOver = false;
    selectedMenuRow = 3;    // set menu selection to 1st row - "Play"

    isKeyPressed = false;
    
    gameSpeed = 500;
    direction = STOP;
    snakeLength = 1;
    snakeX[0] = GAME_WIDTH / 2;  // snake starts from the width center
    snakeY[0] = GAME_HEIGHT / 2; // snake starts from the height center
    fruitX = (rand() % (GAME_WIDTH - 2)) + 1;    // get random number between 1 and WIDTH-2 (0 and WIDTH-1 reserved for border '#') 
    fruitY = (rand() % (GAME_HEIGHT - 2)) + 1;   // get random numbet between 1 and HEIGHT-2 (0 and HEIGHT-2 reserved for border '#')
    score = 0;
    strcat(scoreLine, "Score: ");
}

void ResetGame()
{
    direction = STOP;
    snakeLength = 1;
    memset(snakeX, 0, sizeof(snakeX));      // clean snake X coords array
    memset(snakeY, 0, sizeof(snakeY));      // clean snake Y coords array
    snakeX[0] = GAME_WIDTH / 2;             // snake starts from the width center
    snakeY[0] = GAME_HEIGHT / 2;            // snake starts from the height center
    score = 0;
    memset(scoreLine, 0, sizeof(scoreLine));    // clean scoreLine 
    strcat(scoreLine, "Score: ");
    gameOver = false;
}

// function to clear screen after game stage selection
void ClearScreen()
{
    for (int i = 0; i < SCREEN_HEIGHT; ++i)
    {
        char line[] = "                         ";
        WriteConsoleOutputCharacter(hConsole, line, SCREEN_WIDTH, {2,static_cast<SHORT>(i + 3)}, &dwBytesWritten);
    }
}

// function to format scoreLine char array to display score in the end of the line
void BuildScoreLine(int& score)
{
    char scoreString[GAME_WIDTH];
    sprintf(scoreString, "%d", score);          // convert int score to char array
    memset(scoreLine, 0, sizeof(scoreLine));    // clean scoreLine 
    strcat(scoreLine, "Score: ");

    int scoreSize = 0;
    for (int i = 0; i < GAME_WIDTH; ++i)
    {
        if (scoreString[i] != '\0')
        {
            scoreSize++;
        } 
        else 
        {
            break;
        }
    }

    for (int i = scoreSize - 1; i >= 0; --i)
    {
        scoreLine[GAME_WIDTH - scoreSize + i] = scoreString[i];
    }
}

void Input()
{
    if (GetAsyncKeyState(VK_LEFT)) 
    {
        key = LEFT_KEY;
        direction = LEFT;
        isKeyPressed = true;
    } 
    else if (GetAsyncKeyState(VK_RIGHT)) 
    {     
        key = RIGHT_KEY;
        direction = RIGHT;
        isKeyPressed = true;
    } 
    else if (GetAsyncKeyState(VK_UP)) 
    { 
        key = UP_KEY;
        direction = UP;
        isKeyPressed = true;
    } 
    else if (GetAsyncKeyState(VK_DOWN)) 
    {
        key = DOWN_KEY;
        direction = DOWN;
        isKeyPressed = true;
    }
    else if (GetAsyncKeyState(VK_RETURN))
    {
        key = ENTER_KEY;
        isKeyPressed = true;
    }
    else if (GetAsyncKeyState(VK_ESCAPE))
    {
        key = ESC_KEY;
        isKeyPressed = true;
    }
    else if (GetAsyncKeyState(0x50))
    {
        key = P_KEY;
        isKeyPressed = true;
    }
    else if (GetAsyncKeyState(0x52))
    {
        key = R_KEY;
        isKeyPressed = true;
    }
    else 
    {
        isKeyPressed = false;
    }
}

void MenuLogic()
{
    // set menu speed
    gameSpeed = 100;

    // moving between menu items
    if (key == UP_KEY && isKeyPressed == true)
    {
        switch (selectedMenuRow)
        {
            case 3: break;
            default: selectedMenuRow--;
        }
    }
    else if (key == DOWN_KEY && isKeyPressed == true)
    {
        switch (selectedMenuRow)
        {
            case MENU_HEIGHT: break;
            default: selectedMenuRow++;
        }
    }

    // detect menu selection
    if (key == ENTER_KEY && isKeyPressed == true)
    {
        ClearScreen();
        switch (selectedMenuRow)
        {
            case 3: 
            {
                gameStage = GAME;
                gameSpeed = 500;
                direction = STOP;
            } break;
            case 4: gameStage = SETTINGS; break;
            case 5: quit = true; break;
        }
    }
}

void SettingsLogic()
{
    // detect if ESC key was pressed
    if (key == ESC_KEY && isKeyPressed == true)
    {
        ClearScreen();
        gameStage = MENU;
    }
}

void GameLogic()
{
    // detect if ESC key was pressed
    if (key == ESC_KEY && isKeyPressed == true)
    {
        ClearScreen();
        gameStage = MENU;
        direction = STOP;
    }

    // detect if P (pause) key was pressed
    if (key == P_KEY && isKeyPressed == true)
    {
        direction = STOP;
    }

    // detect if R (reset) key was pressed
    if (key == R_KEY && isKeyPressed == true)
    {
        ResetGame();
    }

    // calculate coordinates for snake's tail
    if (direction != STOP)
    {
        for (int i = snakeLength - 1; i > 0; --i)
        {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        } 
    }  

    // move snake's head
    if (gameOver == false)
    {
        switch(direction)
        {
            case LEFT: snakeX[0]--; break;
            case RIGHT: snakeX[0]++; break;
            case UP: snakeY[0]--; break;
            case DOWN: snakeY[0]++; break;
            case STOP: break;
        }
    }

    // detect collision with walls
    if (snakeX[0] == GAME_WIDTH - 1 || snakeX[0] == 0 || snakeY[0] == GAME_HEIGHT - 1 || snakeY[0] == 0)
    {
        switch(snakeX[0])
        {
            case GAME_WIDTH - 1: snakeX[0] = 1; break;
            case 0: snakeX[0] = GAME_WIDTH - 2; break;
        }

        switch(snakeY[0])
        {
            case GAME_HEIGHT - 1: snakeY[0] = 1; break;
            case 0: snakeY[0] = GAME_HEIGHT - 2; break;
        }
    }

    // detect collison with the tail
    if (direction != STOP) 
    {
        for (int i = 1; i < snakeLength; ++i)
        {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
            {
                gameOver = true;
                direction = STOP;
            }
        }
    }

    // detect collision with fruit
    if (snakeX[0] == fruitX && snakeY[0] == fruitY)
    {
        gameSpeed -= 5;
        score += 10;
        BuildScoreLine(score);
        //int lineSize = strlen(gameTitleLine);
        //BuildScoreLine(lineSize);

        fruitX = (rand() % (GAME_WIDTH - 2)) + 1;
        fruitY = (rand() % (GAME_HEIGHT - 2)) + 1;
        snakeX[snakeLength] = snakeX[0];
        snakeY[snakeLength] = snakeY[0];
        snakeLength++;
    }
}

void Logic()
{
    switch (gameStage)
    {
        case MENU: MenuLogic(); break;
        case GAME: GameLogic(); break;
        case SETTINGS: SettingsLogic(); break;
    } 
}

void DrawMenu()
{
    // fill row selection and remove null characters
    for (int i = 0; i < MENU_HEIGHT; ++i)
    {
        if (i != 0 && i != 1)     // skip MENU title and empty lines
        {
            if (i + 1 == selectedMenuRow)
            {
                menuList[i][1] = '#';
            }
            else
            {
                menuList[i][1] = ' ';
            }
        }
        

        // remove '\0' characters
        for (int j = 0; j < MENU_WIDTH; ++j)
        {
            if (menuList[i][j] == '\0')
            {
                menuList[i][j] = ' ';
            }
        }
    }

    // draw menu list
    for (int i = 0; i < MENU_HEIGHT; ++i)
    {
        char line[MENU_WIDTH];
        for (int j = 0; j < MENU_WIDTH; ++j)
        {
            line[j] = menuList[i][j];
        }   
        WriteConsoleOutputCharacter(hConsole, line, MENU_WIDTH, {2,static_cast<SHORT>(i + 3)}, &dwBytesWritten);
    }
}

void DrawSettings()
{
    // remove '\0' characters
    for (int i = 0; i < SETTINGS_HEIGHT; ++i)
    {
        for (int j = 0; j < SETTINGS_WIDTH; ++j)
        {
            if (settingsList[i][j] == '\0')
            {
                settingsList[i][j] = ' ';
            }
        }
    }

    // draw settings list
    for (int i = 0; i < SETTINGS_HEIGHT; ++i)
    {
        char line[SETTINGS_WIDTH];
        for (int j = 0; j < SETTINGS_WIDTH; ++j)
        {
            line[j] = settingsList[i][j];
        }
        WriteConsoleOutputCharacter(hConsole, line, SETTINGS_WIDTH, {2, static_cast<SHORT>(i+3)}, &dwBytesWritten);
    }
}

void DrawGameOverMessage()
{        
    // remove '\0' characters
    for (int i = 0; i < G_OVER_HEIGHT; ++i)
    {
        for (int j = 0; j < G_OVER_WIDTH; ++j)
        {
            if (gameOverList[i][j] == '\0')
            {
                gameOverList[i][j] = ' ';
            }
        }
    }  

    // draw game over message 
    for (int i = 0; i < G_OVER_HEIGHT; ++i)
    {
        char line[G_OVER_WIDTH];
        for (int j = 0; j < G_OVER_WIDTH; ++j)
        {
            line[j] = gameOverList[i][j];
        }
        WriteConsoleOutputCharacter(hConsole, line, G_OVER_WIDTH, {6, static_cast<SHORT>(i+7)}, &dwBytesWritten);
    }

    // draw score 
    char scoreString[]= " ";
    sprintf(scoreString, "Score: %d", score);  
    WriteConsoleOutputCharacter(hConsole, scoreString, strlen(scoreString), {6, 8}, &dwBytesWritten); 
}

void DrawGame()
{
    for (int i = 0; i < GAME_HEIGHT; ++i)
    {
        for (int j = 0; j < GAME_WIDTH; ++j)
        {
            if (i == 0 || j == 0 || i == GAME_HEIGHT - 1 || j == GAME_WIDTH - 1)  // fill walls in the game field
            {
                gameField[i][j] = '#';
            }
            else if (j == fruitX && i == fruitY)    // fill fruit 'F' in the game field
            {
                gameField[i][j] = 'F';
            }
            else
            {
                gameField[i][j] = ' ';

                // fill snake in the game field
                for (int k = snakeLength - 1; k >= 0; --k)  // start from the end to fill the head last
                {
                    if (j == snakeX[k] && i == snakeY[k])
                    {
                        if (k == 0)
                        {
                            gameField[i][j] = 'O';
                        }
                        else
                        {
                            gameField[i][j] = 'o';
                        }
                    }
                }   
            }   
        }
    }

    // draw the score
    WriteConsoleOutputCharacter(hConsole, scoreLine, GAME_WIDTH, {2,3}, &dwBytesWritten);

    // draw the game field
    for (int i = 0; i < GAME_HEIGHT; ++i)
    {
        char line[GAME_WIDTH];
        for (int j = 0; j < GAME_WIDTH; ++j)
        {
            line[j] = gameField[i][j];
        }   
        WriteConsoleOutputCharacter(hConsole, line, GAME_WIDTH, {2,static_cast<SHORT>(i + 4)}, &dwBytesWritten);
    }

    if (gameOver == true)
    {
        DrawGameOverMessage();
    }
}

void Draw()
{
    // draw the game title
    WriteConsoleOutputCharacter(hConsole, gameTitleLine, strlen(gameTitleLine), {2,1}, &dwBytesWritten);

    switch (gameStage)
    {
        case MENU: DrawMenu(); break;
        case GAME: DrawGame(); break;
        case SETTINGS: DrawSettings(); break;
    }   
}

int main()
{
    Setup();

    // a game loop
    while (!quit)
    {
        Input();
        Logic();
        Draw();
        Sleep(gameSpeed);  // game timing
    }

    return 0;
}
