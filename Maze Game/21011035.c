#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#define MAX 50
typedef struct userData // user datas are hold in userData struct
{
    char name[MAX];
    char username[MAX];
    char password[MAX];
    char surname[MAX];
    struct userData *next;
    double points;
    double highestPoint;
} userData;

typedef struct Board // board elements are hold in Board struct
{
    int row, column;
    int *coordinatesOfX;
    int *collectedParticules;
    int *totalParticules;
    char **gameBoard;
} Board;

void printArt(char *);
void manuelMode(Board *board, int row, int column, userData *user);
Board *loadMap();
void animation(char *string);
int autoMode(Board *board, int X, int Y, int row, int column, char **visited, char *particules);
void printBoard(char **board, int row, int column);
void howToPlay();
void SortLinkedList(userData *userList);

// some signatures, all the code is below the main()!!!

userData *addUser(userData **userList)
{
    if (*userList == NULL) // if list is empty
    {
        (*userList) = (userData *)malloc(sizeof(userData));
        (*userList)->next = NULL;
        return *userList;
    }
    userData *iter = *userList; // if list is not empty
    while (iter->next != NULL)
        iter = iter->next;
    iter->next = (userData *)malloc(sizeof(userData));
    iter->next->next = NULL;
    return iter->next;
}
userData *createUser() // allocates memory for userData
{
    userData *newUser = (userData *)calloc(1, sizeof(userData));
    newUser->next = NULL;
    newUser->highestPoint = 0;
    return newUser;
}

userData *searchUsername(userData *userList, char *username) // searchs given username in the userData linked list, returns the user if found.
{
    userData *temp = userList;
    while (temp != NULL)
    {
        if (strcmp(username, temp->username) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}
void printUserData(userData *user) // used for printing user data after creating a new account
{
    printf("\n\t**********************");
    printf("\n\tUsername : %s", user->username);
    printf("\n\tName\t : %s", user->name);
    printf("\n\tSurname\t : %s", user->surname);
    printf("\n\tPassword : %s", user->password);
    printf("\n\tHighest point : %.2lf", user->highestPoint);
    printf("\n\t**********************");
}
void writeUserData(userData *userList, char *filename) // used for writing(saving) userData linked list right before program exit.
{

    int count = 0;
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("\nERROR!");
        return;
    }
    userData *iter = userList;
    while (iter != NULL)
    {
        fwrite(iter, sizeof(userData), 1, file);
        iter = iter->next;
        count++;
    }
    fclose(file);
}
void retrieveUsers(userData **userList, char *filename) // pulls the users from last save.
{                                                       // NOTICE: do not delete "data.bin", if you want to clear the user list, just delete txt inside of "data.bin"
    int count = 0;
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        perror("\nERROR!");
        return;
    }
    fseek(file, 0, SEEK_END); // checking if the file is empty or not
    if (ftell(file) == 0)
        return;

    (*userList) = createUser();
    rewind(file);
    userData *iter = (*userList);
    while (fread(iter, sizeof(userData), 1, file))
    {
        iter->next = createUser();
        iter = iter->next;
        count++;
    }
    iter = NULL;
    fclose(file);
}

userData *createAccount(userData **userList) // used for creating new accounts
{
    char temp[MAX] = {0};
    system("cls");
    printf("# Enter 0 to go back main menu #");
    printf("\n\n\tCreate Account\n");
    if (*userList == NULL) // list is empty, first element is going to be added
    {
        printf("\nYou are the first user!\n");
        *userList = addUser(userList);

        printf("\nEnter username -> ");
        scanf("%s", temp);
        if (strcmp(temp, "0") == 0) // gives opportunity to exit program if ever wanted
        {
            system("cls");
            printf("Returning main menu.");
            Sleep(400);
            system("cls");
            printf("Returning main menu..");
            Sleep(400);
            system("cls");
            printf("Returning main menu...");
            Sleep(400);
            return NULL;
        }

        system("cls");
        strcpy((*userList)->username, temp);
        printf("\nSelected username -> %s\nEnter password -> ", (*userList)->username);
        scanf("%s", temp);
        strcpy((*userList)->password, temp); // copying userData fields
        system("cls");
        printf("\nSelected username -> %s\nEnter your name -> ", (*userList)->username);
        scanf("%s", temp);
        strcpy((*userList)->name, temp);
        system("cls");
        printf("\nSelected name -> %s\nEnter surname -> ", (*userList)->name);
        scanf("%s", temp);
        strcpy((*userList)->surname, temp);
        system("cls");
        (*userList)->highestPoint = 0.0;
        (*userList)->points = 0.0;
        printUserData(*userList);
        printf("\n\tNew user is successfully created!");
        sleep(2);
        return *userList;
    }
    do // if the list is not empty then this part goes in
    {
        printf("\nEnter username -> ");
        scanf("%s", temp);
        if (strcmp(temp, "0") == 0)
        {
            system("cls");
            printf("Returning main menu.");
            Sleep(400);
            system("cls");
            printf("Returning main menu..");
            Sleep(400);
            system("cls");
            printf("Returning main menu...");
            Sleep(400);
            return NULL;
        }
        if (searchUsername(*userList, temp) != NULL) // checking if the given username is already taken
        {
            system("cls");
            printf("\n%s username is already taken! Please select another username.", temp);
            sleep(2);
        }
    } while (searchUsername(*userList, temp) != NULL);
    userData *newUser = addUser(userList); // adding the user to the list
    strcpy(newUser->username, temp);
    system("cls");
    printf("\nSelected username -> %s\nEnter password -> ", newUser->username);
    scanf("%s", temp);
    strcpy(newUser->password, temp);
    system("cls");
    printf("\nSelected username -> %s\nEnter name -> ", newUser->username);
    scanf("%s", temp);
    strcpy(newUser->name, temp); // copying fields
    system("cls");
    printf("\nSelected name -> %s\nEnter surname -> ", newUser->name);
    scanf("%s", temp);
    strcpy(newUser->surname, temp);
    system("cls");
    newUser->highestPoint = 0.0;
    newUser->points = 0.0;
    printUserData(newUser);
    printf("\n\tNew user is successfully created!");
    sleep(2);
    return newUser;
}
int checkUsernamePassword(userData *user, char *password) // checking password with the users
{
    if (strcmp(password, user->password))
        return 0;
    else
        return 1;
}
userData *loginScreen(userData *userList) // login screen
{
    if (userList == NULL) // if no users have found in the list, then it is not necessary to go login screen
    {
        system("cls");
        printf("\nUser list is empty!");
        printf("\nReturning main menu.");
        Sleep(600);
        system("cls");
        printf("\nUser list is empty!");
        printf("\nReturning main menu..");
        Sleep(600);
        system("cls");
        printf("\nUser list is empty!");
        printf("\nReturning main menu...");
        Sleep(600);
        return NULL;
    }
    char *temp = (char *)malloc(sizeof(char));
    do
    {
        system("cls");
        printf("# Enter 0 to go back previous menu #\n");
        printf("\nEnter username -> ");
        fflush(stdin);
        scanf("%s", temp);
        if (strcmp(temp, "0") && !searchUsername(userList, temp))
        {
            printf("\n%s username is not found. Please try again.", temp);
            sleep(1);
            system("cls");
        }
    } while (!searchUsername(userList, temp) && strcmp(temp, "0")); // checking username is already taken & gives opportunity to exit if wanted
    system("cls");
    if (!strcmp(temp, "0"))
    {
        system("cls");
        printf("Returning main menu.");
        Sleep(400);
        system("cls");
        printf("Returning main menu..");
        Sleep(400);
        system("cls");
        printf("Returning main menu...");
        Sleep(400);
        return NULL;
    }
    userData *currentUser = searchUsername(userList, temp);
    do
    {
        printf("# Enter 0 to go back previous menu #\n");
        printf("\nUsername : %s\nEnter password -> ", currentUser->username);
        scanf("%s", temp);
        checkUsernamePassword(currentUser, temp);
        if (strcmp(temp, "0") && !checkUsernamePassword(currentUser, temp))
        {
            printf("\nWrong password! Please try again.");
            sleep(1);
            system("cls");
        }

    } while (strcmp(temp, "0") && !checkUsernamePassword(currentUser, temp));
    if (!strcmp(temp, "0"))
    {
        system("cls");
        printf("Returning main menu.");
        Sleep(400);
        system("cls");
        printf("Returning main menu..");
        Sleep(400);
        system("cls");
        printf("Returning main menu...");
        Sleep(400);
        return NULL;
    }
    system("cls");
    printf("Welcome %s %s! \nYou have logged in successfully!", currentUser->name, currentUser->surname);
    sleep(1);
    return currentUser;
}
char **allocVisited(int row, int column) // allocates memory for visited matrix (used in Auto Mode)
{
    char **visited = (char **)calloc(row, sizeof(char *));
    int i;
    for (i = 0; i < row; i++)
    {
        visited[i] = (char *)calloc(column, sizeof(char));
    }
    return visited;
}
void game(userData *user) // game function
{
    fflush(stdin);
    char mode, a;
    Board *board;
    char **visited;
    char *dum = (char *)calloc(100, sizeof(char)); // used for printing particules after every move
    board = loadMap();                             // creates board for game
    if (board == NULL)
        return;

    do
    {
        system("cls");
        printf("# Enter 0 to go back previous menu #\n");
        printf("\n\t# Selected Map #\n");
        printBoard(board->gameBoard, board->row, board->column);
        printf("\n# Select the game mode");
        printf("\n1 -> Manuel Mode");
        printf("\n2 -> Auto Mode");
        printf("\nYour choice -> ");
        fflush(stdin);
        scanf("%c", &mode);
        switch (mode)
        {
        case '1':
            manuelMode(board, board->row, board->column, user);
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);
            printf("\nPoints awarded this game -> %.2lf", user->points);
            if (user->highestPoint == user->points && user->points > 0)
            {
                SetConsoleTextAttribute(hConsole, 2);
                printf("\nCongratulations!!! You passed your all time high score!!!");
            }
            SetConsoleTextAttribute(hConsole, 15);
            printf("\n\nPress any key to continue...\n");
            a = getch();
            return;
            break;
        case '2':
            visited = allocVisited(board->row, board->column);
            autoMode(board, board->coordinatesOfX[0], board->coordinatesOfX[1], board->row, board->column, visited, dum);
            user->points = 0;
            SetConsoleTextAttribute(hConsole, 4);
            printf("\nYou did not awarded with points because you played in Auto Mode. Try to beat the game in manuel mode :)");
            SetConsoleTextAttribute(hConsole, 15);
            printf("\nPress any key to continue...\n");
            a = getch();
            return;
            break;
        case '0':
            printf("\nReturning previous menu...");
            sleep(1);
            return;
        default:
            fflush(stdin);
            printf("\nYou entered invalid input. Please try again.");
            sleep(1);
            break;
        }
    } while (mode);
    return;
}
void printBoard(char **board, int row, int column) // printing board
{
    int i, j, k;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < column; j++)
        {
            if (board[i][j] == '+')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 2);
                printf(" P-");
                SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
            }
            else if (board[i][j] == '-')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 6);
                printf(" e+");
                SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
            }
            else if (board[i][j] == 'P')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 1);
                printf(" P+");
                SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
            }
            else if (board[i][j] == 'e')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 5);
                printf(" e-");
                SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
            }
            else if (board[i][j] == 'X')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 4);
                printf(" X ");
                SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
            }
            else if (board[i][j] == 'K')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 12);
                printf(" K ", board[i][j]);
            }
            else if (board[i][j] == 'C')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 11);
                printf(" C ", board[i][j]);
            }
            else if (board[i][j] == '1')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 0);
                printf(" 1 ", board[i][j]);
            }
            else if (board[i][j] == 'Z')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 0);
                printf(" Z ", board[i][j]);
            }
            else if (board[i][j] == 0 || board[i][j] == '0')
            {
                SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
                printf(" 0 ", board[i][j]);
            }
        }
        SetConsoleTextAttribute(hConsole, 16 * 15 + 15);
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, 15);
}

char **createBoard(int row, int column) // allocates memory for board
{
    int i, j;
    char **board = (char **)calloc(row, sizeof(char *));
    for (i = 0; i < row; i++)
    {
        board[i] = (char *)calloc(column, sizeof(char));
    }
    return board;
}
void *showMaps(Board *board) // used for showing default maps
{

    fflush(stdin);
    system("cls");
    printf("\n# Default Maps #\n");
    char **tempBoard = (char **)calloc(MAX, sizeof(char *));
    int i, j, row, column;
    char select = '1';
    FILE *file;

    for (i = 0; i < 10; i++)
    {
        tempBoard[i] = (char *)calloc(MAX, sizeof(char));
    }

    file = fopen("map1.txt", "r");
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 10; j++)
        {
            fscanf(file, "%c ", &tempBoard[i][j]); // reading board from file
        }
    }
    printf("\tMAP 1\n");
    printBoard(tempBoard, 6, 10);
    fclose(file);
    file = fopen("map2.txt", "r");
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 10; j++)
        {
            fscanf(file, "%c ", &tempBoard[i][j]); // reading board from file
        }
    }
    printf("\tMAP 2\n");
    printBoard(tempBoard, 8, 10);
    fclose(file);
    printf("# Enter 0 to go back main menu #\n");
    do
    {
        fflush(stdin);
        printf("\nSelect a map (1-2) ->");
        scanf("%c", &select);
        switch (select)
        {
        case '0':
            system("cls");
            printf("Returning to main menu...");
            Sleep(1000);
            break;
        case '1':
            file = fopen("map1.txt", "r");
            for (i = 0; i < 6; i++)
            {
                for (j = 0; j < 10; j++)
                {
                    fscanf(file, "%c ", &tempBoard[i][j]); // reading board from file
                    if (tempBoard[i][j] == 'X')
                    {
                        board->coordinatesOfX[0] = i;
                        board->coordinatesOfX[1] = j;
                    }
                }
            }
            row = 6;
            column = 10;
            break;
        case '2':
            file = fopen("map2.txt", "r");
            for (i = 0; i < 8; i++)
            {
                for (j = 0; j < 10; j++)
                {
                    fscanf(file, "%c ", &tempBoard[i][j]); // reading board from file
                    if (tempBoard[i][j] == 'X')
                    {
                        board->coordinatesOfX[0] = i;
                        board->coordinatesOfX[1] = j;
                    }
                }
            }
            row = 8;
            column = 10;
            break;
        default:
            fflush(stdin);
            printf("\nInvalid input! Please enter a valid input -> ");
            break;
        }

    } while (select < '0' || select > '2');
    if (select == 0)
        return NULL;
    board->gameBoard = tempBoard;
    board->row = row;
    board->column = column;
}
Board *loadMap() // users can choose between loading their own maps and selecting default maps
{

    Board *board = (Board *)calloc(1, sizeof(Board));
    board->coordinatesOfX = (int *)calloc(2, sizeof(int));
    board->collectedParticules = (int *)calloc(2, sizeof(int));
    board->totalParticules = (int *)calloc(2, sizeof(int));
    char *filename = (char *)malloc(sizeof(char) * MAX);
    int i, j;
    char select = '1';
    FILE *file;
    do
    {
        fflush(stdin);
        system("cls");
        printf("# Enter 0 to go back to main menu #\n");
        printf("\n# Load Map #\n");
        printf("\n1 -> Select map from default maps");
        printf("\n2 -> Load from .txt file");
        printf("\n\nYour choice -> ");
        scanf("%c", &select);
        switch (select)
        {
        case '0':
            system("cls");
            printf("\nReturning main menu...");
            Sleep(1000);
            return NULL;
            break;
        case '1':
            showMaps(board);
            system("cls");
            break;
        case '2':
            do
            {
                system("cls");
                printf("\nEnter the filename of the map -> ");
                scanf("%s", filename);
                file = fopen(filename, "r");
                if (!file)
                    perror("\nError!");

            } while (!file);
            system("cls");
            printf("\nFile opened successfully.");
            printf("\nEnter the row and column -> ");
            scanf("%d %d", &board->row, &board->column);
            board->gameBoard = createBoard(board->row, board->column);
            for (i = 0; i < board->row; i++)
            {
                for (j = 0; j < board->column; j++)
                {
                    fscanf(file, "%c ", &board->gameBoard[i][j]); // reading board from file
                    if (board->gameBoard[i][j] == 'X')
                    {
                        board->coordinatesOfX[0] = i;
                        board->coordinatesOfX[1] = j;
                    }
                }
            }
            return board;
        default:
            fflush(stdin);
            printf("\nInvalid input!");
            sleep(1);
            break;
        }
    } while (select < '0' || select > '2');
    if (select == '0')
        return NULL;
    printBoard(board->gameBoard, board->row, board->column);
    return board;
}
void showHighScores(userData *userList) // displays high scores
{

    if (userList == NULL)
    {

        system("cls");
        printf("\nNo one has played the game yet!");
        printf("\nReturning main menu.");
        Sleep(500);
        system("cls");
        printf("\nNo one has played the game yet!");
        printf("\nReturning main menu..");
        Sleep(500);
        system("cls");
        printf("\nNo one has played the game yet!");
        printf("\nReturning main menu...");
        Sleep(500);
        return;
    }

    userData *iter = userList;
    int count = 1;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SortLinkedList(userList); // sort userlist descending order
    system("cls");
    while (iter != NULL && count <= 5)
    {
        if (strcmp(iter->name, "") != 0 && strcmp(iter->username, "") != 0)
        {
            SetConsoleTextAttribute(hConsole, 7 - count);
            printf("\t\t ### Number %d ###\nPoints : %.2lf", count, iter->highestPoint);
            SetConsoleTextAttribute(hConsole, 15);
            printf(" is achieved by %s %s\n", iter->name, iter->surname);
            count++;
        }
        iter = iter->next;
    }
    SetConsoleTextAttribute(hConsole, 15);
    printf("\n\n# Press any key to continue... ");
    getch();
}
void mainMenu() // the main function
{
    printArt("ascii.txt"); // printing ascii art
    char select, select2 = '0';
    userData *userList = NULL;
    userData *currentUser;
    retrieveUsers(&userList, "data.bin"); // retrieving users from last save
    do                                    // menu
    {
        fflush(stdin);
        system("cls");
        printf("\t\t# MENU #");
        printf("\n1 -> Play Game\n2 -> How to Play\n3 -> Show Top 5 Scores\n\n0 -> Exit\n\n");
        printf("\nYour choice : ");
        scanf("%c", &select);
        switch (select)
        {
        case '1':
            // ask for login or create account
            do
            {
                fflush(stdin);
                system("cls");
                printf("# Enter 0 to go back main menu #\n");
                printf("\n1 -> Login with existing account");
                printf("\n2 -> Create new account");
                printf("\n\nYour choice -> ");
                scanf("%c", &select2);
                switch (select2)
                {
                case '1':
                    currentUser = loginScreen(userList); // login screen
                    if (currentUser != NULL)             // if exited without logging in, returns to main menu
                    {
                        system("cls");
                        game(currentUser); // starts game
                    }

                    break;
                case '2':
                    currentUser = createAccount(&userList); // creating new account
                    if (currentUser != NULL)                // if exited without creating account, returns to main menu
                    {
                        system("cls");
                        game(currentUser); // starts game
                    }
                    break;
                case '0':
                    system("cls"); // return to main menu
                    break;
                default:
                    system("cls");
                    printf("\nInvalid input!"); // if invalid input is given
                    Sleep(1000);
                    break;
                }
            } while (select2 < '0' || select2 > '2');
            break;
        case '2':
            // information screen
            system("cls");
            howToPlay(); // displays how to play screen
            break;
        case '3':
            showHighScores(userList); // displays maximum 5 top users.
            break;
        case '0': // exiting program
            system("cls");
            writeUserData(userList, "data.bin"); // saves data before exit.
            printArt("ascii2.txt");
            return;
        default:
            system("cls");
            printf("Invalid input! Please enter a valid input."); // if entered invalid input
            Sleep(1000);
            fflush(stdin);
            break;
        }
    } while (select != '0');
}
void swap(char **board, int x1, int x2, int y1, int y2) // swaps cells in board
{
    char temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;
}
int checkOutOfBounds(int X, int Y, int row, int column) // checks if the coordinates are out of bounds
{
    if (X < 0 || X >= row || Y < 0 || Y >= column)
        return 1;
    else
        return 0;
}
int checkGameOver(Board *board, int X, int Y) // checks the board cell if it is black hole or exit
{
    if (board->gameBoard[X][Y] == 'C')
        return 1;
    else if (board->gameBoard[X][Y] == 'K')
        return 2;
    else
        return 0;
}
int updateParticules(Board *board, int x, int y) // updates collected particules
{
    if (board->gameBoard[x][y] == 'P')
    {
        printf("\nYou collected P+");
        board->collectedParticules[0] = -1;
        board->totalParticules[0]--;
        return 1;
    }
    else if (board->gameBoard[x][y] == 'e')
    {
        printf("\nYou collected e-");
        board->collectedParticules[1] = -1;
        board->totalParticules[1]--;
        return 1;
    }
    else if (board->gameBoard[x][y] == '+')
    {
        printf("\nYou collected P-");
        board->collectedParticules[0] = 1;
        board->totalParticules[0]++;
        return 1;
    }
    else if (board->gameBoard[x][y] == '-')
    {
        printf("\nYou collected e+");
        board->collectedParticules[1] = 1;
        board->totalParticules[1]++;
        return 1;
    }
    else if (board->gameBoard[x][y] == 'K' || board->gameBoard[x][y] == 'C')
        return 1;
    return 0;
}
double calculatePoints(Board *board, double timePassed) // at the end of the game, calculates points
{

    int count = 0;
    double points = 0;
    if (board->totalParticules[0] <= 0 || board->totalParticules[1] <= 0)
    {
        printf("\nYou have collected 0 antiparticles.");
        points += 100.0 - (timePassed * (0.25));
        return points;
    }
    while (board->totalParticules[0] > 0 && board->totalParticules[1] > 0)
    {
        board->totalParticules[0]--;
        board->totalParticules[1]--;
        count++;
    }
    printf("\nYou have collected %d antiparticles.", count);

    points += 100.0 + (50.0 * count) - (timePassed * (0.25));
    return points;
}
void updateBoard(Board *board, int row, int column, int direction) // updates board for each move
{

    if (direction == 72) // up
    {
        if (checkOutOfBounds(board->coordinatesOfX[0] - 1, board->coordinatesOfX[1], row, column))
            return;
        if (board->gameBoard[board->coordinatesOfX[0] - 1][board->coordinatesOfX[1]] == '1')
            return;
        if (board->gameBoard[board->coordinatesOfX[0] - 1][board->coordinatesOfX[1]] == '0')
        {
            swap(board->gameBoard, board->coordinatesOfX[0] - 1, board->coordinatesOfX[0], board->coordinatesOfX[1], board->coordinatesOfX[1]);
            board->coordinatesOfX[0]--;
            return;
        }
        else if (updateParticules(board, board->coordinatesOfX[0] - 1, board->coordinatesOfX[1]))
        {
            board->gameBoard[board->coordinatesOfX[0] - 1][board->coordinatesOfX[1]] = '0';
            swap(board->gameBoard, board->coordinatesOfX[0] - 1, board->coordinatesOfX[0], board->coordinatesOfX[1], board->coordinatesOfX[1]);
            board->coordinatesOfX[0]--;
            return;
        }
        return;
    }
    else if (direction == 80) // down
    {
        if (checkOutOfBounds(board->coordinatesOfX[0] + 1, board->coordinatesOfX[1], row, column))
            return;
        if (board->gameBoard[board->coordinatesOfX[0] + 1][board->coordinatesOfX[1]] == '1')
            return;
        if (board->gameBoard[board->coordinatesOfX[0] + 1][board->coordinatesOfX[1]] == '0')
        {
            swap(board->gameBoard, board->coordinatesOfX[0] + 1, board->coordinatesOfX[0], board->coordinatesOfX[1], board->coordinatesOfX[1]);
            board->coordinatesOfX[0]++;
            return;
        }
        else if (updateParticules(board, board->coordinatesOfX[0] + 1, board->coordinatesOfX[1]))
        {
            board->gameBoard[board->coordinatesOfX[0] + 1][board->coordinatesOfX[1]] = '0';
            swap(board->gameBoard, board->coordinatesOfX[0] + 1, board->coordinatesOfX[0], board->coordinatesOfX[1], board->coordinatesOfX[1]);
            board->coordinatesOfX[0]++;
            return;
        }
        return;
    }
    else if (direction == 75) // left
    {
        if (checkOutOfBounds(board->coordinatesOfX[0], board->coordinatesOfX[1] - 1, row, column))
            return;
        if (board->gameBoard[board->coordinatesOfX[0]][board->coordinatesOfX[1] - 1] == '1')
            return;
        if (board->gameBoard[board->coordinatesOfX[0]][board->coordinatesOfX[1] - 1] == '0')
        {
            swap(board->gameBoard, board->coordinatesOfX[0], board->coordinatesOfX[0], board->coordinatesOfX[1] - 1, board->coordinatesOfX[1]);
            board->coordinatesOfX[1]--;
            return;
        }
        else if (updateParticules(board, board->coordinatesOfX[0], board->coordinatesOfX[1] - 1))
        {
            board->gameBoard[board->coordinatesOfX[0]][board->coordinatesOfX[1] - 1] = '0';
            swap(board->gameBoard, board->coordinatesOfX[0], board->coordinatesOfX[0], board->coordinatesOfX[1] - 1, board->coordinatesOfX[1]);
            board->coordinatesOfX[1]--;
            return;
        }
        return;
    }
    else if (direction == 77) // right
    {
        if (checkOutOfBounds(board->coordinatesOfX[0], board->coordinatesOfX[1] + 1, row, column))
            return;
        if (board->gameBoard[board->coordinatesOfX[0]][board->coordinatesOfX[1] + 1] == '1')
            return;
        if (board->gameBoard[board->coordinatesOfX[0]][board->coordinatesOfX[1] + 1] == '0')
        {
            swap(board->gameBoard, board->coordinatesOfX[0], board->coordinatesOfX[0], board->coordinatesOfX[1] + 1, board->coordinatesOfX[1]);
            board->coordinatesOfX[1]++;
            return;
        }
        else if (updateParticules(board, board->coordinatesOfX[0], board->coordinatesOfX[1] + 1))
        {
            board->gameBoard[board->coordinatesOfX[0]][board->coordinatesOfX[1] + 1] = '0';
            swap(board->gameBoard, board->coordinatesOfX[0], board->coordinatesOfX[0], board->coordinatesOfX[1] + 1, board->coordinatesOfX[1]);
            board->coordinatesOfX[1]++;
            return;
        }
        return;
    }
}
char *printParticules(Board *board, char *temp) // prints collected particules after every move and updates it
{
    if (board->collectedParticules[0] == 1)
    {
        strcat(temp, " P-");
        board->collectedParticules[0] = 0;
    }
    else if (board->collectedParticules[0] == -1)
    {
        strcat(temp, " P+");
        board->collectedParticules[0] = 0;
    }
    else if (board->collectedParticules[1] == 1)
    {
        strcat(temp, " e+");
        board->collectedParticules[1] = 0;
    }
    else if (board->collectedParticules[1] == -1)
    {
        strcat(temp, " e-");
        board->collectedParticules[1] = 0;
    }
    return temp;
}
void manuelMode(Board *board, int row, int column, userData *user) // manuel game mode
{
    char *str = (char *)calloc(MAX, sizeof(char));
    int i, press, terminate = 0;
    clock_t start, end;
    double timePassed;
    start = clock();
    end = clock();

    while ((end - start) / (CLOCKS_PER_SEC) < 60 && !terminate) // time limit & endgame controls
    {
        printf("\e[1;1H\e[2J");
        printf("\t# Manuel Mode #\n");
        printBoard(board->gameBoard, row, column);
        str = printParticules(board, str);
        printf("\nCollected particules -> %s", str);
        timePassed = (end - start) / CLOCKS_PER_SEC;
        printf("\nTime passed %.2lf", timePassed);
        printf("\nYour input -> ");
        fflush(stdin);
        press = getch();
        if (press == 27)
        {
            printf("\nAre you sure want to exit? Press esc to exit, press another key to continue.");
            press = getch();
            if (press == 27)
            {
                system("cls");
                printf("\nReturning to main menu...");
                user->points = 0;
                Sleep(3000);
                return;
            }
        }
        else if (press == 224)
        {
            press = getch();
            if (press == 72)
            {
                if (!checkOutOfBounds(board->coordinatesOfX[0] - 1, board->coordinatesOfX[0], row, column))
                {
                    terminate = checkGameOver(board, board->coordinatesOfX[0] - 1, board->coordinatesOfX[1]);
                    updateBoard(board, row, column, press);
                }
            }
            else if (press == 80)
            {
                if (!checkOutOfBounds(board->coordinatesOfX[0] + 1, board->coordinatesOfX[1], row, column))
                {
                    terminate = checkGameOver(board, board->coordinatesOfX[0] + 1, board->coordinatesOfX[1]);
                    updateBoard(board, row, column, press);
                }
            }

            else if (press == 75)
            {
                if (!checkOutOfBounds(board->coordinatesOfX[0], board->coordinatesOfX[1] - 1, row, column))
                {
                    terminate = checkGameOver(board, board->coordinatesOfX[0], board->coordinatesOfX[1] - 1);
                    updateBoard(board, row, column, press);
                }
            }

            else if (press == 77)
            {
                if (!checkOutOfBounds(board->coordinatesOfX[0], board->coordinatesOfX[1] + 1, row, column))
                {
                    terminate = checkGameOver(board, board->coordinatesOfX[0], board->coordinatesOfX[1] + 1);
                    updateBoard(board, row, column, press);
                }
            }
        }
        else
        {
            printf("\nInvalid input!");
            sleep(1);
        }
        end = clock();
        sleep(0.5);
    }
    printf("\e[1;1H\e[2J");

    if (terminate == 2) // if pressed blackhole
    {
        system("cls");
        printBoard(board->gameBoard, board->row, board->column);
        printf("\nOops! Blackhole is drowning you...");
        printf("\nYou lost!");
        printf("\nReturning main menu...");
        user->points = 0;
        sleep(3);
        return;
    }
    else if (terminate == 1) // if reached exit
    {
        system("cls");
        printBoard(board->gameBoard, board->row, board->column);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 6);
        printf("\nCongratulations!!! You managed to reach the exit!");
        printf("\nYou have collected %s this game!", str);
        user->points = calculatePoints(board, timePassed);
        user->highestPoint = (user->highestPoint < user->points) ? user->points : user->highestPoint;
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }
    else // if time limit is exceeded
    {
        printf("\nTime's up! You could not reach the exit before time runs up!");
        printf("\nReturning main menu...");
        sleep(3);
    }
}
int findDirection(Board *board, int X, int Y) // for auto mode, calculates direction
{
    if (board->coordinatesOfX[0] == X)
    {
        if (board->coordinatesOfX[1] - Y == 1)
        {
            return 75;
        }
        else if (board->coordinatesOfX[1] - Y == -1)
        {
            return 77;
        }
    }
    else if (board->coordinatesOfX[1] == Y)
    {
        if (board->coordinatesOfX[0] - X == 1)
        {
            return 80;
        }
        else if (board->coordinatesOfX[0] - X == -1)
        {
            return 72;
        }
    }
    return -1;
}
void updateCoordinates(Board *board, int x1, int y1) // updates coordinates for board and displaying
{
    int temp1 = board->coordinatesOfX[0];
    int temp2 = board->coordinatesOfX[1];
    board->coordinatesOfX[0] = x1;
    board->coordinatesOfX[1] = y1;
    if (board->gameBoard[x1][y1] == 'P' || board->gameBoard[x1][y1] == 'e' || board->gameBoard[x1][y1] == '+' || board->gameBoard[x1][y1] == '-')
    {
        updateParticules(board, x1, y1);
        board->gameBoard[x1][y1] = '0';
    }

    swap(board->gameBoard, temp1, x1, temp2, y1);
}
int autoMode(Board *board, int X, int Y, int row, int column, char **visited, char *particules) // auto game mode
{
    printf("\e[1;1H\e[2J");
    printf("\t# Auto Mode #\n");
    printBoard(board->gameBoard, row, column);
    particules = printParticules(board, particules);
    printf("\nCollected particules -> %s", particules);
    Sleep(300);
    if (checkOutOfBounds(X, Y, row, column)) // checking out of bounds
        return 0;
    if (board->gameBoard[X][Y] == '1')
        return 0;
    if (visited[X][Y] == '1') // if it is already visited, do not go again
        return 0;
    if (checkGameOver(board, X, Y) == 2) // if pressed blackhole
        return 0;
    if (checkGameOver(board, X, Y) == 1) // if reached exit
    {
        visited[X][Y] = '1';
        board->gameBoard[X][Y] = '0';
        swap(board->gameBoard, X, board->coordinatesOfX[0], Y, board->coordinatesOfX[1]);
        printf("\e[1;1H\e[2J");
        printBoard(board->gameBoard, row, column);
        printf("\nCollected particules -> %s", particules);
        printf("\n-> Auto Mode has found the exit!");
        sleep(2);
        return 1;
    }
    // move order -> up,right,down,left
    visited[X][Y] = '1'; // labeling the visited matrix, to not go back again and again
    updateCoordinates(board, X, Y);
    if (autoMode(board, X - 1, Y, row, column, visited, particules)) // calling function recursively for each direction and updating coordinates before calling
    {
        return 1;
    }
    updateCoordinates(board, X, Y);

    if (autoMode(board, X, Y + 1, row, column, visited, particules))
    {
        return 1;
    }
    updateCoordinates(board, X, Y);

    if (autoMode(board, X + 1, Y, row, column, visited, particules))
    {
        return 1;
    }
    updateCoordinates(board, X, Y);

    if (autoMode(board, X, Y - 1, row, column, visited, particules))
    {
        return 1;
    }
    updateCoordinates(board, X, Y);

    visited[X][Y] = '0'; // if there is no clear way, then mark it unvisited again.
    return 0;
}
void howToPlay() // how to play display
{
    char **temp = (char **)calloc(6, sizeof(char *));
    int i, j;
    for (i = 0; i < 10; i++)
    {
        temp[i] = (char *)calloc(10, sizeof(char));
    }
    FILE *file = fopen("map1.txt", "r");
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 10; j++)
        {
            fscanf(file, "%c ", &temp[i][j]); // reading board from file for displaying example board
        }
    }
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 2);
    printf("\t\t\t\t# How To Play #\n");
    SetConsoleTextAttribute(hConsole, 6);
    printf("\n-> Your goal is creating anti-particles by collecting the required particles in the maze.");
    printf("\n-> You have to reach exit without losing the needed particles to create anti-particles.");
    printf("\n-> Collecting any particle's anti particle will cause to eliminate each other.");
    printf("\n-> Going into blackholes will cause you to lose the game.");
    printf("\n-> You have to reach to the exit from a random start location.");
    printf("\n-> Your location is represented as X.");
    printf("\n-> Blackholes are represented as K.");
    printf("\n-> Exit is represented as C.");
    printf("\n-> Obstacles are represented as 1 meanwhile empty spaces are represented as blank (0).");
    printf("\n-> Particles represented as :\n");
    SetConsoleTextAttribute(hConsole, 15);
    printf("\nProtons -> P+\tElectrons -> e-");
    printf("\nAnti-Protons -> P-\tAnti-Electrons -> e+\n");
    SetConsoleTextAttribute(hConsole, 4);
    printf("\n-> You have to collect P- & e+ to obtain an anti-particle. Each collected anti-particle will award you with 50 points.");
    printf("\n-> Each second you spent in the game will subtract 0.25 points. You have 60 seconds to beat the game!");
    SetConsoleTextAttribute(hConsole, 15);
    printf("\n\nAn example map is shown below.\n");
    printBoard(temp, 6, 10);
    SetConsoleTextAttribute(hConsole, 3);
    printf("\n-> You can choose mods between Manuel & Auto.");
    printf("\n-> You can move with arrow keys in Manuel Mode.");
    printf("\n-> NOTE : Playing in Auto mode will not award you with points. Also in Auto Mode, there is no time limit.");
    SetConsoleTextAttribute(hConsole, 15);
    printf("\n\n# Press any key to continue... ");
    getch();
}

void printArt(char *filename) // just for fun, displays a ascii art from a file, please do not delete ascii.txt & ascii2.txt
{
    FILE *file;

    int i;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (i = 0; i < 14; i++)
    {

        SetConsoleTextAttribute(hConsole, i + 1);
        file = fopen(filename, "r");
        if (file == NULL)
        {
            perror("Error reading!");
            return;
        }
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), file) != NULL)
            printf("%s", buffer);
        fclose(file);
        Sleep(200);
        system("cls");
    }
    SetConsoleTextAttribute(hConsole, i + 1);
    return;
}
void swapNodes(userData *node1, userData *node2) // it is for sorting linked list, used in showing highscores
{
    double temp = node1->highestPoint;
    char tempName[MAX];
    char tempUsername[MAX];
    char tempSurname[MAX];
    char tempPassword[MAX];
    strcpy(tempName, node1->name);
    strcpy(tempUsername, node1->username);
    strcpy(tempPassword, node1->password);
    strcpy(tempSurname, node1->surname);
    strcpy(node1->name, node2->name);
    strcpy(node1->username, node2->username);
    strcpy(node1->surname, node2->surname);
    strcpy(node1->password, node2->password);
    node1->highestPoint = node2->highestPoint;
    strcpy(node2->name, tempName);
    strcpy(node2->username, tempUsername);
    strcpy(node2->surname, tempSurname);
    strcpy(node2->password, tempPassword);
    node2->highestPoint = temp;
}
void SortLinkedList(userData *userList) // sorting linked list by highest points(descending order).
{
    printf("\n");
    userData *iter = NULL;
    userData *lastNode = NULL;
    while (userList->next != lastNode)
    {
        iter = userList;
        while (iter->next != lastNode)
        {
            if (iter->highestPoint < iter->next->highestPoint)
            {
                swapNodes(iter, iter->next);
            }
            iter = iter->next;
        }
        lastNode = iter;
    }
}

int main()
{
    mainMenu();
    return 0;
}
