#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>

/* Constants definitions */
#define COLUMNS 5
#define PAUSE getch();
#define CLS (_WIN32 ? system("cls") : system("clear"));
#define SPAUSE                                \
    printf("Press any key to continue. . ."); \
    PAUSE
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define BLACK "\033[0m"
#define GRAY "\033[38;5;250m"
#define ORANGE "\033[38;5;202m"
#define NOCURSOR printf("\e[?25l"); // Hide text cursor
#define CURSOR printf("\e[?25h");   // Show text cursor
#define UP 72                       // Key "up" value
#define DOWN 80                     // Key "down" value
#define LEFT 75                     // Key "left" value
#define RIGHT 77                    // Key "right" value
#define ENTER 13                    // Key "Enter" value
#define ESC 27                      // Key "Esc" value
#define EXIT while(getch() != ESC); // Exit the menu

/* Struct definitions */
typedef struct
{
    int lin; // Position of the note (line)
    int col; // Position of the note (column)
} TPosition;

typedef struct
{
    char key;      // Corresponding key
    char *color;   // Key color
    TPosition pos; // Position of the note
} TNote;

typedef struct
{
    char *name;   // Player name
    int score;    // Player score
    int maxCombo; // Plint max combo
} TPlayer;

/* Functions declarations */
int selection(char **options, int size); // Show interactive selection
void start(); // Start the game
void title(char *title); // Type the title: "Guitar Hero"
void option(char op); // Choose option
void play(); // Play the game
void showRecords(); // Show player records
void sortPlayers(); // Sort players
void showRanking(); // Show players rankings
void printGame(TNote note); // Game logic
void exitGame(); // Exit the game
void displayScore(int score, int combo, bool miss); // Display current score
void addPlayer(); // Triggers the function to create a player
void selectPlayer(); // Select a existing player
TPlayer createPlayer(); // Create new player
void deletePlayer(); // Delete a player
void changePoints(); // Change points and combo
void changeGuitarSize(); // Change the number of lines of the guitar
void developerMode(); // Develint mode
void validateAllocation(void *ptr); // Validate memory allocation
void freeMemory(); // Free memory
void saveGame(); // Save game in a file
void loadGame(); // Load game from a file
void errorMessage(int errorCode); // Show eint message
void gotoxy(int x, int y); // Go to position X, Y (Very useful comment)
void tellemcontrols(); // Show controls
void settings(); // Show settings
void changecontrols(); // Change controls
int selectionControls(char *options); // Select controls
void NPC(int pos, char *str, int x, int y); // Game assistant

/* Global variables */
TPlayer *_players;      // Array of players
int _numPlayers = 0;    // Number of players
int currentID = 0;      // Current player index
bool newRecord = false; // Player record
int lines = 16;         // Lines of guitar
char controls[5] = {"ASJKL"}; // Guitar controls

int main()
{
    int op;
    srand((unsigned)time(NULL));
    SetConsoleOutputCP(65001);

    start();
    while (1)
    {
        char *options[] = {
            "🕹️  Play",
            "📀 Records",
            "🏆 Ranking",
            "❓ Help",
            "⚙️  Settings",
            "💾 Save game",
            "🚪 Quit game"
        };

        int numOps = 7;

        CLS NOCURSOR
        printf("-- Guitar Hero --");
        op = selection(options, numOps);

        option(op);
    }

    return 0;
}

int selection(char **options, int size){
    int pos = 0;
    int key;

    NOCURSOR // Hide cursor
    while (true)
    {
        for (int i = 0; i < size; i++)
        {
            gotoxy(0, 2 + i); // Go to print position X, Y

            if (i == pos)
            {
                printf(">> %s <<", options[i]); // Selected option
            }
            else
            {
                printf("   %s   ", options[i]);
            }
        }
        key = getch();

        // Increases or decreases the position according to the selected key
        if (key == UP)
        {
            if (pos <= 0)
            {
                pos = size - 1;
            }
            else
            {
                pos--;
            }
        }
        if(key == DOWN){
            if(pos >= size - 1){
                pos = 0;
            }
            else
            {
                pos++;
            }
        }

        if (key == ENTER)
        { // Select option
            return pos;
        }
        else if (key == 'g' || key == ESC)
        { // Selected developer mode (dont ask questions)
            return (int)key;
        }
    }
}

void tellemcontrols()
{
    char *color[] = {GREEN, RED, YELLOW, BLUE, ORANGE};

    CLS
    gotoxy(5, 0);
    printf("Controls:\n");
    printf("___________________\nControls -> Colors:\n\n");

    for(int i = 0; i < COLUMNS; i++)
    {
        printf("%s|%c|%s ", color[i], controls[i], RESET);
    }

    printf("\n___________________\n Menu ops -> Keys:\n\n");
    printf(" ENTER = Open menu \n");
    printf("ESC = Exit the menu\n\n");
    printf("       %s↑%s  %s↓%s        \n", GREEN, RESET, RED, RESET);
    printf("      UP DOWN\n");
    printf("       %s→%s  %s←%s        \n", BLUE, RESET, ORANGE, RESET);
    printf("    RIGHT LEFT\n");
    printf("___________________\n\n\n");
    printf(RESET);

    EXIT
}

void tutorial()
{
    TNote note;
    note.pos.lin = 0;
    note.pos.col = rand() % COLUMNS;
    
    NOCURSOR
    if (note.pos.col == 0)
    {
        note.key = controls[0];
        note.color = GREEN;
    }
    else if (note.pos.col == 1)
    {
        note.key = controls[1];
        note.color = RED;
    }
    else if (note.pos.col == 2)
    {
        note.key = controls[2];
        note.color = YELLOW;
    }
    else if (note.pos.col == 3)
    {
        note.key = controls[3];
        note.color = BLUE;
    }
    else if (note.pos.col == 4)
    {
        note.key = controls[4];
        note.color = ORANGE;
    }

    NPC(2, "- Welcome!!! Im your in-game assistant", 0, 0);
    printf("\n\n");
    SPAUSE

    CLS

    NPC(1, "- This is the tutorial", 0, 0);
    printf("\n\n");
    SPAUSE

    while(1){
        CLS
        printGame(note);

        NPC(1, "", 23, lines - 1);
        printf("\n\n");
        SPAUSE
    }
}

void NPC(int pos, char *str, int x, int y){
    gotoxy(x, y);

    if(pos == 1){
        printf(" O %s\n", str);
        gotoxy(x, y + 1);
        printf("/|\\");
    } else if(pos == 2){
        printf(" O/ %s\n", str);
        gotoxy(x, y + 1);
        printf("/|");
    } else if(pos == 3){
        printf("\\O/ %s\n", str);
        gotoxy(x, y + 1);
        printf(" |");
    }

    /* 
        1:
            O
           /|\
        2:
            O/
           /|
        3:
           \O/
            |
    */
}

void ophelp(char op)
{ 
    CLS 
    switch(op)
    {
        case 0:
            tutorial();
            break;
        case 1:
            tellemcontrols();
            break;
    }
}

void changecontrols()
{
    CLS
    int op = selectionControls(controls);
    
    gotoxy(0, 3);
    printf("Press the key you want to change!\n");

    if(op == ESC)
    {
        return;
    }
    
    do
    {
        char c = getch();
        if(c == ESC) return;
        
        if(!isalpha(c))
        {
            errorMessage(-7);
            sleep(1);
            continue;
        } else {
            int check = 0;
            
            for(int i = 0; i < 5; i++)
            {
                if(controls[i] == toupper(c))
                {
                    errorMessage(-8);
                    sleep(1);
                    check = 0;
                    break;
                } else {
                    check = 1;
                }
            }

            if (check)
            {
                controls[op] = toupper(c);
                break;
            }            
        }
    } while(1);
    
    char *color;
    if (op == 0) color = GREEN;
    if (op == 1) color = RED;
    if (op == 2) color = YELLOW;
    if (op == 3) color = BLUE;
    if (op == 4) color = ORANGE;
    
    gotoxy(1 + op * 4, 1); // Go to print position X, Y
    printf("%s%c%s", color, controls[op], RESET);
    
    gotoxy(0, 3);
    printf("                                 "); // Erase previous print line

    gotoxy(0, 3);
    printf("Controls changed sucessfully!\n");
    sleep(2);
    return;
}

int selectionControls(char *options){
    int pos = 0;
    int size = strlen(options);
    int key;

    NOCURSOR // Hide cursor
    while (true)
    {
        for (int i = 0; i < size; i++)
        {
            char *color;
            if(i == 0){
                color = GREEN;
            } else if(i == 1){
                color = RED;
            } else if(i == 2){
                color = YELLOW;
            } else if(i == 3){
                color = BLUE;
            } else if(i == 4){
                color = ORANGE;
            }
            gotoxy(0 + i * 4, 0); // Go to print position X, Y
            
            if (i == pos)
            {
                printf("👇"); // Gay selection indicator
                gotoxy(0 + i * 4, 1); // Go to print position X, Y
                printf(" %s%c%s   \n\n", color, options[i], RESET); // Selected key
            }
            else
            {
                printf(" "); // Erase gay selection indicator
                gotoxy(0 + i * 4, 1); // Go to print position X, Y
                printf(" %s%c%s  \n\n", color, options[i], RESET);
            }
        }
        printf("Press enter to select the key\n");
        key = getch();
        
        // Increases or decreases the position according to the selected key
        if (key == LEFT)
        {
            if (pos <= 0)
            {
                pos = size - 1;
            }
            else
            {
                pos--;
            }
        }
        if(key == RIGHT){
            if(pos >= size - 1){
                pos = 0;
            }
            else
            {
                pos++;
            }
        }
        
        if (key == ENTER)
        { // Select option
            gotoxy(1 + pos * 4, 1); // Go to print position X, Y
            printf("%s%c%s", GRAY, options[pos], RESET);
            return pos;
        }
        else if (key == ESC)
        { // Selected developer mode (dont ask questions)
            return key;
        }
    }
}

void settings()
{

    char *options[] = {
        "📏 Guitar size",
        "🎮 Guitar controls"
    };

    int nops = 2;

    CLS
    printf("-- Settings --\n\n");
    int op = selection(options, nops);

    switch(op)
    {
        case 0:
            changeGuitarSize();
            break;
        case 1:
            changecontrols();
            break;
    }
}

void option(char op)
{
    char opl;
    char *ops[] = {
        "👶 Tutorial",
        "🎮 Controls"
    };
    int nops = 2;

    CLS switch (op)
    {
    case 0:
        play();
        break;
    case 1:
        showRecords();
        break;
    case 2:
        showRanking();
        break;
    case 3:
        printf("-- Help menu --\n\n");
        opl = selection(ops, nops);
        ophelp(opl);
        break;
    case 4:
        settings();
        break;
    case 5:
        saveGame();
        break;
    case 6:
        exitGame();
        break;
    case (int)'g': // No comments
        developerMode();
        break;
    }
}

void play()
{
    TNote note;
    char *options[] = {
        "Cry baby👶",
        "Normal😐", 
        "Rock 'n' Roll🤘"
    };

    int delay, numOps = 3;

    printf("Set difficulty:");
    int dif = selection(options, numOps);

    if (dif == 0)
        delay = 500;
    if (dif == 1)
        delay = 300;
    if (dif == 2)
        delay = 200;
    if (dif == ESC)
        return;

    int combo = 0;
    bool miss = false;
    note.pos.lin = 0;
    note.pos.col = rand() % COLUMNS;

    NOCURSOR
    if (note.pos.col == 0)
    {
        note.key = controls[0];
        note.color = GREEN;
    }
    else if (note.pos.col == 1)
    {
        note.key = controls[1];
        note.color = RED;
    }
    else if (note.pos.col == 2)
    {
        note.key = controls[2];
        note.color = YELLOW;
    }
    else if (note.pos.col == 3)
    {
        note.key = controls[3];
        note.color = BLUE;
    }
    else if (note.pos.col == 4)
    {
        note.key = controls[4];
        note.color = ORANGE;
    }

    while (1)
    {
        CLS
            printGame(note);
        displayScore(_players[currentID].score, combo, miss);

        int check = 0;
        if (kbhit())
        { // Check if a key is pressed
            char pressed = getch();
            if (toupper(pressed) == note.key && note.pos.lin == (lines / 2))
            {
                _players[currentID].score += 5;
                miss = false;
                combo++;
                if (combo > _players[currentID].maxCombo)
                {
                    _players[currentID].maxCombo = combo;
                    newRecord = true; // Mark the new record player for the explosion effect on the showRecord function
                }
            }
            else if ((int)pressed == ESC)
            {
                break;
            }
            else
            {
                if (_players[currentID].score >= 5) 
                    _players[currentID].score -= 5;
                combo = 0;
                miss = true;
            }
            check = 1;
        }

        if (note.pos.lin >= lines)
        {
            _players[currentID].score -= 5;
            combo = 0;
        }

        if (check)
        {
            note.pos.lin = 0;
            note.pos.col = rand() % COLUMNS;

            if (note.pos.col == 0)
            {
                note.key = controls[0];
                note.color = GREEN;
            }
            else if (note.pos.col == 1)
            {
                note.key = controls[1];
                note.color = RED;
            }
            else if (note.pos.col == 2)
            {
                note.key = controls[2];
                note.color = YELLOW;
            }
            else if (note.pos.col == 3)
            {
                note.key = controls[3];
                note.color = BLUE;
            }
            else if (note.pos.col == 4)
            {
                note.key = controls[4];
                note.color = ORANGE;
            }

            continue;
        }
        else
        {
            note.pos.lin++;
        }

        Sleep(delay);
    }
}

void printGame(TNote note)
{
    char *color[] = {GREEN, RED, YELLOW, BLUE, ORANGE};

    printf("  %s_________________", BLACK);
    for (int i = 0; i < lines; i++)
    {
        gotoxy(0, i + 1);
        for (int j = 0; j < COLUMNS; j++)
        {
            if (i == (lines / 2) - 1)
            {
                if (j == 0)
                {
                    printf("  %s|%s", color[j], BLACK);
                }
                else
                {
                    printf("___%s|%s", color[j], BLACK);
                }
            }
            else
            {
                printf("  %s| ", BLACK);
            }
        }
    }

    gotoxy(note.pos.col * 4, (note.pos.lin + 1));
    if (note.pos.lin == (lines / 2) - 1)
    {
        if (note.pos.col == 0)
        {
            printf("  %s%c%s", note.color, note.key, BLACK);
        }
        else
        {
            printf("__%s%c%s", note.color, note.key, BLACK);
        }
    }
    else
    {
        printf("  %s%c%s ", note.color, note.key, BLACK);
    }

    printf(RESET);
}

void displayScore(int score, int combo, bool miss)
{
    gotoxy(0, lines + 1);
    printf("\nScore: %d | Combo: x%d", score, combo);
    if (combo >= 10)
    {
        printf("🔥🔥🔥");
    }

    if(miss){
        printf(" MISS!");
    }

    printf("\n");
}

void showRecords()
{
    char *explosion[4] = {"@", "*", ".", " "}; // Explosion particles

    if (newRecord)
    { // Displays an explosion effect if the player has a new record
        for (int i = 0; i < 4; i++)
        {
            printf("%s%s NEW %sRECORD %s\n", RED, explosion[i], YELLOW, explosion[i]);
            Sleep(500);
            CLS
        }
        printf(RESET);
        newRecord = false;
    }

    gotoxy(23, 0);
    printf("-- Records --\n");
    printf("Player: %s | Score: %d | Max combo: x%d🔥\n\n", _players[currentID].name, _players[currentID].score, _players[currentID].maxCombo);
    EXIT
}

void sortPlayers()
{
    for (int i = 0; i < _numPlayers - 1; i++)
    {
        for (int j = 0; j < _numPlayers - 1 - i; j++)
        {
            if (_players[j].score < _players[j + 1].score || (_players[j].score == _players[j + 1].score && _players[j].maxCombo < _players[j + 1].maxCombo))
            {
                TPlayer temp = _players[j];
                _players[j] = _players[j + 1];
                _players[j + 1] = temp;
            }
        }
    }
}

void showRanking()
{
    sortPlayers();
    printf("      -- Ranking --\n");
    for (int i = 0; i < _numPlayers; i++)
    {
        if (i == 0)
        {
            printf("%s%d%s - %-12s [%.2d Points | Max combo: x%.2d]🔥\n", YELLOW, i + 1, RESET, _players[i].name, _players[i].score, _players[i].maxCombo);
        }
        else if (i == _numPlayers - 1)
        {
            printf("%s%d%s - %-12s [%.2d Points | Max combo: x%.2d]👎\n", RED, i + 1, RESET, _players[i].name, _players[i].score, _players[i].maxCombo);
        }
        else
        {
            printf("%d - %-12s [%.2d Points | Max combo: x%.2d]\n", i + 1, _players[i].name, _players[i].score, _players[i].maxCombo);
        }
    }

    printf("\n");
    EXIT
}

void start()
{
    const char *start_messages[] = {"Connecting amplifiers", "Adjusting volume", "Tuning the guitar"};

    CLS
        NOCURSOR
            printf("%s...\n", start_messages[rand() % 3]);
    for (int i = 0; i <= 50; i++)
    {
        int percent = (i * 100) / 50;

        printf("\r[");
        for (int j = 0; j < 50; j++)
        {
            if (j < i)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("] %d%%", percent);

        fflush(stdout);
        Sleep(50);
    }
    printf(" Ready!");
    sleep(1);

    CLS
    loadGame();

    char *options[] = {"Select an existing player", "Add new player"};
    int choice;

    if (_numPlayers > 0)
    {
        printf("-- Choice player --");
        choice = selection(options, 2);
    }

    if (choice == 0)
    {
        selectPlayer();
    }
    else if (choice == 1 || _numPlayers == 0)
    {
        addPlayer();
    }

    title("-- Guitar Hero --");
}

void addPlayer()
{
    if (_numPlayers == 0)
    {
        _players = (TPlayer *)malloc(1 * sizeof(TPlayer));
    }
    else
    {
        _players = (TPlayer *)realloc(_players, (_numPlayers + 1) * sizeof(TPlayer));
    }
    validateAllocation(_players);

    currentID = _numPlayers;
    _players[_numPlayers] = createPlayer();
    _numPlayers++;
}

void selectPlayer()
{
    int id;
    char *players[_numPlayers];

    for (int i = 0; i < _numPlayers; i++)
    {
        players[i] = _players[i].name;
    }

    CLS
    printf("-- Players --");
    id = selection(players, _numPlayers);

    currentID = id;
}

TPlayer createPlayer()
{
    char strAux[50];
    TPlayer player;

    CLS
        CURSOR
            printf("Enter your name: ");
    gets(strAux);

    if (_numPlayers > 0)
    {
        for (int i = 0; i < _numPlayers; i++)
        {
            if (strcmp(_players[i].name, strAux) == 0)
            {
                errorMessage(-3);
                SPAUSE
                return createPlayer();
            }
        }
    }

    player.name = (char *)malloc((strlen(strAux) + 1) * sizeof(char));
    validateAllocation(player.name);
    strcpy(player.name, strAux);

    player.score = 0;
    player.maxCombo = 0;

    return player;
}

void deletePlayer()
{
    int id, check = 0;
    char *players[_numPlayers];

    for (int i = 0; i < _numPlayers; i++)
    {
        players[i] = _players[i].name;
    }

    CLS
    printf("-- Players --");
    id = selection(players, _numPlayers);
    if(id == currentID){
        check = 1;
    }

    for (int i = id; i < _numPlayers - 1; i++)
    {
        _players[i] = _players[i + 1];
    }

    _players = (TPlayer *)realloc(_players, (_numPlayers - 1) * sizeof(TPlayer));
    _numPlayers--;

    CLS NOCURSOR
        printf("Player deleted!\n");
    SPAUSE

    if (_numPlayers == 0)
    {
        createPlayer();
    }

    if (check)
    {
        selectPlayer();
    }
}

void changePoints()
{
    int points, combo, id;
    char *players[_numPlayers];

    for (int i = 0; i < _numPlayers; i++)
    {
        players[i] = _players[i].name;
    }

    CLS
    printf("--- Players ---\n");
    id = selection(players, _numPlayers);

    CLS CURSOR
        printf("Current points: %d\n", _players[id].score);
    printf("New points: ");
    scanf("%d", &points);
    fflush(stdin);

    CLS
        printf("Current combo: %d\n", _players[id].maxCombo);
    printf("New combo: ");
    scanf("%d", &combo);
    fflush(stdin);

    _players[id].score = points;
    _players[id].maxCombo = combo;

    CLS NOCURSOR
        printf("Points and combo changed!\n");
    SPAUSE
}

void changeGuitarSize()
{

    do
    {
        CLS CURSOR
            printf("Current guitar size: %d\n", lines);
        printf("New guitar size: ");
        scanf("%d", &lines);
        fflush(stdin);

        if (lines < 1)
        {
            errorMessage(-6);
            SPAUSE
        }
    } while (lines < 1);

    CLS NOCURSOR
        printf("Guitar size changed!\n");
    SPAUSE
}

void developerMode()
{
    char *options[] = {
        "🆕 Add a new player",
        "🚫 Delete a player",
        "🔢 Change points and combo",
    };

    int numOps = 3;
    int op;

    while (1)
    {
        CLS
        printf("-- Developer mode --");
        op = selection(options, numOps);

        CLS if (op == 0)
            addPlayer();
        if (op == 1)
            deletePlayer();
        if (op == 2)
            changePoints();
        if (op == ESC)
            break;
        if (op == (int)'g'){ // ...
            printf("Not so fast");
            sleep(1);
        }
    }
}

void title(char *title)
{
    CLS for (int i = 0; title[i] != '\0'; i++)
    {
        printf("%c", title[i]);
        Sleep(100);
    }
    Sleep(500);
}

void exitGame()
{
    char *exit_messages[] = {"Saving the setlist", "Packing the gear", "Turning off the amps"};

    printf("%s", exit_messages[rand() % 3]);
    for (int i = 0; i < 3; i++)
    {
        Sleep(500);
        printf(".");
    }

    freeMemory();
    exit(0);
}

void saveGame()
{
    char *spin[] = {"|", "/", "-", "\\"};
    int i = 0;

    for (int j = 0; j < 10; j++)
    {
        printf("\r%s Saving...", spin[i]);
        fflush(stdout);
        Sleep(300);
        i = (i + 1) % 4;
    }

    FILE *gameFile = fopen("GuitarHero.txt", "w");
    if (gameFile == NULL)
    {
        errorMessage(-2);
        SPAUSE
    }

    for (int i = 0; i < _numPlayers; i++)
    {
        fprintf(gameFile, "%s;", _players[i].name);
        fprintf(gameFile, "%d;", _players[i].score);
        fprintf(gameFile, "%d;", _players[i].maxCombo);
        fprintf(gameFile, "\n");
    }
    fprintf(gameFile, "%d#", lines);

    fclose(gameFile);

    CLS
        printf("Game saved successfully!\n");
    sleep(1);
}

void loadGame()
{
    int sep, i;
    char strAux[100], c;

    FILE *gameFile = fopen("GuitarHero.txt", "r");
    if (gameFile)
    {
        i = 0;
        sep = 0;

        while (!feof(gameFile))
        {
            c = fgetc(gameFile);
            if (c != ';' && c != '#' && c != '\n' && c != EOF)
            {
                strAux[i] = c;
                i++;
            }
            else if (c == ';' || c == '#' || c == '\n' || c == EOF)
            {
                strAux[i] = '\0';
                i = 0;

                if (c == ';')
                {
                    if (sep == 0)
                    {
                        if (_numPlayers == 0)
                        {
                            _players = (TPlayer *)malloc(1 * sizeof(TPlayer));
                        }
                        else
                        {
                            _players = (TPlayer *)realloc(_players, (_numPlayers + 1) * sizeof(TPlayer));
                        }
                        validateAllocation(_players);

                        _players[_numPlayers].name = (char *)malloc((strlen(strAux) + 1) * sizeof(char));
                        validateAllocation(_players[_numPlayers].name);
                        strcpy(_players[_numPlayers].name, strAux);
                        sep++;
                    }
                    else if (sep == 1)
                    {
                        _players[_numPlayers].score = atoi(strAux);
                        sep++;
                    }
                    else if (sep == 2)
                    {
                        _players[_numPlayers].maxCombo = atoi(strAux);
                        sep = 0;
                        _numPlayers++;
                    }
                }
                else if (c == '#')
                {
                    lines = atoi(strAux);
                }
            }
        }
    }

    fclose(gameFile);
}

void freeMemory()
{
    for (int i = 0; i < _numPlayers; i++)
    {
        free(_players[i].name);
    }
    free(_players);
}

void validateAllocation(void *ptr)
{
    if (ptr == NULL)
    {
        errorMessage(-1);
        SPAUSE
    }
}

void errorMessage(int errorCode)
{
    switch (errorCode)
    {
    case -1:
        printf("\nFATAL ERROR: Memory allocation failed!!!\n");
        exit(1);
    case -2:
        printf("\nFATAL ERROR: File creation failed!!!\n");
        exit(1);
    case -3:
        printf("\nERROR: Player already exists!!!\n");
        break;
    case -4:
        printf("\nERROR: Player does not exist!!!\n");
        break;
    case -5:
        printf("\nERROR: Option not found!!!\n");
        break;
    case -6:
        printf("\nERROR: Negative lines not allowed!!!\n");
        break;
    case -7:
        printf("\nERROR: Invalid key!!!\n");
        break;
    case -8:
        printf("\nERROR: Key already in use!!!\n");
        break;
    default:
        printf("\nERROR: Unknown error!!!\n");
        break;
    }
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Windows API to move text cursor
}