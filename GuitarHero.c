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
#define SPAUSE printf("Press any key to continue. . ."); PAUSE
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define ORANGE "\033[38;5;202m"
#define NOCURSOR printf("\e[?25l"); // Hide text cursor
#define CURSOR printf("\e[?25h"); // Show text cursor
#define UP 72 // Key "up" value
#define DOWN 80 // Key "down" value
#define ENTER 13 // Key "Enter" value
#define ESC 27 // Key "Esc" value

/* Struct definitions */
typedef struct{
    int lin; // Position of the note (line)
    int col; // Position of the note (column)
} TPosition;

typedef struct{
    char key; // Corresponding key
    char *color; // Key color
    TPosition pos; // Position of the note
} TNote;

typedef struct{
    char *name; // Player name
    int score; // Player score
    int maxCombo; // Plint max combo
} TPlayer;

/* Functions declarations */
int menu(char **options, int totalOps); // Show interactive menu
void start(); // Start the game
void title(char *title); // Type the title: "Guitar Hero"
void option(char op); // Choose option
void play(); // Play the game
void showRecords(); // Show player records
void sortPlayers(); // Sort players
void showRanking(); // Show players rankings
void printGame(TNote note); // Game logic
void exitGame(); // Exit the game
void displayScore(int score, int combo); // Display current score
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
int isDigitString(char *str); // Verify if a string is a digit
int search(char *str); // Search a player by name
void gotoxy(int x, int y); // Go to position X, Y (Very useful comment)

/* Global variables */
TPlayer *_players; // Array of players
int _numPlayers = 0; // Number of players
int currentID = 0; // Current player index
bool newRecord = false; // Player record
int lines = 16; // Lines of guitar

int main(){
    int op;
    srand((unsigned)time(NULL));
    SetConsoleOutputCP(65001);

    start();
    while(1){
        char *options[] = {"🎮 Play", "📀 Records", "🏆 Ranking", "❓ Help", "💾 Save game", "🚪 Quit game"};
        int numOps = 6;

        CLS
        printf("-- Guitar Hero --");
        op = (char)menu(options, numOps);

        option(op);
    }

    return 0;
}

int menu(char **options, int totalOps){
    int pos = 0;
    int key;

    NOCURSOR // Hide cursor
    while(true){
        for(int i = 0; i < totalOps; i++){
            gotoxy(0, 2 + i); // Go to print position X, Y

            if(i == pos){
                printf(">> %s <<", options[i]); // Selected option
            } else{
                printf("   %s   ", options[i]);
            }
        }
        key = getch();

        // Increases or decreases the position according to the selected key
        if(key == UP && pos > 0)
            pos--;
        if(key == DOWN && pos < totalOps - 1)
            pos++;

        if(key == ENTER){ // Select option
            CURSOR
            return pos;
        } else if(key == 'g' || key == ESC){ // Selected developer mode (dont ask questions)
            CURSOR
            return (int)key;
        }
    }
}

void option(char op){
    CLS
    switch(op){
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
            printf("Not yet implemented\n");
            sleep(1);
            break;
        case 4:    
            saveGame();
            break;
        case 5:
            exitGame();
            break;
        case (int)'g': // No comments
            developerMode();
            break;
    }
}

void play(){
    TNote note;
    char *options[] = {"Cry baby👶", "Normal😐", "Rock 'n' Roll🤘"};
    int delay, numOps = 3;

    printf("Set difficulty:");
    int dif = menu(options, numOps);

    if(dif == 0) delay = 500;
    if(dif == 1) delay = 300;
    if(dif == 2) delay = 200;
    if(dif == ESC) return;

    int combo = 0;
    note.pos.lin = 0;
    note.pos.col = rand() % COLUMNS;

    NOCURSOR
    if(note.pos.col == 0){
        note.key = 'A';
        note.color = GREEN;
    } else if(note.pos.col == 1){
        note.key = 'S';
        note.color = RED;
    } else if(note.pos.col == 2){
        note.key = 'J';
        note.color = YELLOW;
    } else if(note.pos.col == 3){
        note.key = 'K';
        note.color = BLUE;
    } else if(note.pos.col == 4){
        note.key = 'L';
        note.color = ORANGE;
    }

    while(1){
        CLS
        printGame(note);
        displayScore(_players[currentID].score, combo);

        int check = 0;
        if(kbhit()){ // Check if a key is pressed
            char pressed = getch();
           if(toupper(pressed) == note.key && note.pos.lin == (lines / 2)){
                _players[currentID].score += 5;
                combo++;
                if(combo > _players[currentID].maxCombo){
                    _players[currentID].maxCombo = combo;
                    newRecord = true; // Mark the new record player for the explosion effect on the showRecord function
                }
            } else if((int)pressed == ESC){
                return;
            } else{
                if(_players[currentID].score >= 5) _players[currentID].score -= 5;
                combo = 0;
            }
            check = 1;
        }

        if(note.pos.lin >= lines){
            break;
        }

        if(check){
            note.pos.lin = 0;
            note.pos.col = rand() % COLUMNS;

            if(note.pos.col == 0){
                note.key = 'A'; 
                note.color = GREEN;
            } else if(note.pos.col == 1){
                note.key = 'S'; 
                note.color = RED;
            } else if(note.pos.col == 2){
                note.key = 'J'; 
                note.color = YELLOW;
            } else if(note.pos.col == 3){
                note.key = 'K';
                note.color = BLUE;
            } else if(note.pos.col == 4){
                note.key = 'L';
                note.color = ORANGE;
            }

            continue;
        } else{
            note.pos.lin++;
        }

        Sleep(delay);
    }
}

void printGame(TNote note){
    char *color[] = {GREEN, RED, YELLOW, BLUE, ORANGE};

    printf("  %s_________________", BLACK);
    for (int i = 0; i < lines; i++) {
        gotoxy(0, i + 1);
        for (int j = 0; j < COLUMNS; j++) {
            if(i == (lines / 2) - 1){
                if(j == 0){
                    printf("  %s|%s", color[j], BLACK);
                } else{
                    printf("___%s|%s", color[j], BLACK);
                }
            } else{
                printf("  %s| ", BLACK);
            }
        }
    }

    gotoxy(note.pos.col * 4, (note.pos.lin + 1));
    if(note.pos.lin == (lines / 2) - 1){
        if(note.pos.col == 0){
            printf("  %s%c%s", note.color, note.key, BLACK);
        } else{
            printf("__%s%c%s", note.color, note.key, BLACK);
        }
    } else{
        printf("  %s%c%s ", note.color, note.key, BLACK);
    }

    printf(RESET);
}

void displayScore(int score, int combo){
    gotoxy(0, lines + 1);
    printf("\nScore: %d | Combo: x%d", score, combo);
    if(combo >= 10){
        printf("🔥🔥🔥");
    }
    printf("\n");
}

void showRecords(){
    char *explosion[4] = {"@", "*", ".", " "}; // Explosion particles

    if(newRecord){ // Displays an explosion effect if the player has a new record
        for(int i = 0; i < 4; i++){
            printf("%s%s NEW %sRECORD %s\n", RED, explosion[i], YELLOW, explosion[i]);
            Sleep(500);
            CLS
        }
        printf(RESET);
        newRecord = false;
    }

    gotoxy(23, 0); printf("--- Records ---\n");
    printf("Player: %s | Score: %d | Max combo: x%d🔥\n\n", _players[currentID].name, _players[currentID].score, _players[currentID].maxCombo);
    SPAUSE
}

void sortPlayers(){
    for(int i = 0; i < _numPlayers - 1; i++){
        for(int j = 0; j < _numPlayers - 1 - i; j++){
            if(_players[j].score < _players[j + 1].score || (_players[j].score == _players[j + 1].score && _players[j].maxCombo < _players[j + 1].maxCombo)){                
                TPlayer temp = _players[j];
                _players[j] = _players[j + 1];
                _players[j + 1] = temp;
            }
        }
    }
}

void showRanking(){
    sortPlayers();
    printf("      --- Ranking ---\n");
    for(int i = 0; i < _numPlayers; i++){
        if(i == 0){
            printf("%s%d%s - %-12s [%.2d Points | Max combo: x%.2d]🔥\n", YELLOW, i + 1, RESET, _players[i].name, _players[i].score, _players[i].maxCombo);
        } else if(i == _numPlayers - 1){
            printf("%s%d%s - %-12s [%.2d Points | Max combo: x%.2d]👎\n", RED, i + 1, RESET, _players[i].name, _players[i].score, _players[i].maxCombo);
        }else{
            printf("%d - %-12s [%.2d Points | Max combo: x%.2d]\n", i + 1, _players[i].name, _players[i].score, _players[i].maxCombo);
        }
    }

    printf("\n");    
    SPAUSE
}

void start(){
    const char *start_messages[] = {"Connecting amplifiers", "Adjusting volume", "Tuning the guitar"};

    CLS 
    NOCURSOR
    printf("%s...\n", start_messages[rand()%3]);
    for(int i = 0; i <= 50; i++){
        int percent = (i * 100) / 50;
        
        printf("\r[");
        for(int j = 0; j < 50; j++){
            if(j < i){
                printf("#");
            } else{
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

    CURSOR
    char *options[] = {"Select an existing player", "Add new player"};
    int choice;

    if(_numPlayers > 0){
        printf("-- Choice player --");
        choice = menu(options, 2);
    }

    if(choice == 0){
        selectPlayer();
    } else if(choice == 1 || _numPlayers == 0){
        addPlayer();
    }

    title("-- Guitar Hero --");
}

void addPlayer(){
    if(_numPlayers == 0){
        _players = (TPlayer*)malloc(1 * sizeof(TPlayer));
    } else{
        _players = (TPlayer*)realloc(_players, (_numPlayers + 1) * sizeof(TPlayer));
    }
    validateAllocation(_players);

    currentID = _numPlayers;
    _players[_numPlayers] = createPlayer();
    _numPlayers++;
}

void selectPlayer(){
    int id;
    char str[100];

    do{
        CLS
        printf("--- Players ---\n");
        for(int i = 0; i < _numPlayers; i++){
            printf("[%d - %s]\n", i + 1, _players[i].name);
        }
        printf("\n** Select the player [by index or name]: ");
        gets(str);
        id = isDigitString(str);

        if(id < 0 || id > _numPlayers - 1){
            errorMessage(-4);
        }
    } while(id < 0 || id > _numPlayers - 1);
    currentID = id;
}

int isDigitString(char *str)
{
    if (isdigit(str[0]) && strlen(str) == 1)
    {
        return (atoi(str) - 1);
    }

    return search(str);
}

int search(char *str){
    printf("Searching [%s]", str);
    for(int i = 0; i < 3; i++){
        printf(".");
        Sleep(500);
    }
    printf("\n");

    for(int i = 0; i < _numPlayers; i++){
        if(strcmp(_players[i].name, str) == 0){
            printf("\nPlayer [%s] found in position %d!\n", _players[i].name, i + 1);
            sleep(1);
            return i;
        }
    }

    return -1;
}

TPlayer createPlayer(){
    char strAux[50];
    TPlayer player;

    CLS
    printf("Enter your name: ");
    gets(strAux);

    if(_numPlayers > 0){
        for(int i = 0; i < _numPlayers; i++){
            if(strcmp(_players[i].name, strAux) == 0){
                errorMessage(-3);
                return createPlayer();
            }
        }
    }

    player.name = (char*)malloc((strlen(strAux) + 1) * sizeof(char));
    validateAllocation(player.name);
    strcpy(player.name, strAux);

    player.score = 0;
    player.maxCombo = 0;

    return player;
}

void deletePlayer(){
    int id, check = 0;
    char str[100];

    do{
        CLS
        printf("--- Players ---\n");
        for(int i = 0; i < _numPlayers; i++){
            printf("[%d - %s]\n", i + 1, _players[i].name);
        }
        printf("\n** Select the player: ");
        gets(str);
        id = isDigitString(str);

        if(id == currentID){
           check = 1;
        }

        if(id < 0 || id > _numPlayers - 1){
            errorMessage(-4);
        }
    } while(id < 0 || id > _numPlayers - 1);

    for(int i = id; i < _numPlayers - 1; i++){
        _players[i] = _players[i + 1];
    }

    _players = (TPlayer*)realloc(_players, (_numPlayers - 1) * sizeof(TPlayer));
    _numPlayers--;


    CLS
    printf("Player deleted!\n");
    SPAUSE

    if(check){
        selectPlayer();
    }
}

void changePoints(){
    int points, combo, id;

    do{
        CLS
        printf("--- Players ---\n");
        for(int i = 0; i < _numPlayers; i++){
            printf("[%d - %s]\n", i + 1, _players[i].name);
        }
        printf("\n** Select the player: ");
        scanf("%d", &id);
        fflush(stdin);

        if(id < 1 || id > _numPlayers){
            errorMessage(-4);
        }
    } while(id < 1 || id > _numPlayers);
    id--;

    CLS
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

    CLS
    printf("Points and combo changed!\n");
    SPAUSE
}

void changeGuitarSize(){

    do{
        CLS
        printf("Current guitar size: %d\n", lines);
        printf("New guitar size: ");
        scanf("%d", &lines);
        fflush(stdin);

        if(lines < 1){
            errorMessage(-6);
        }
    } while(lines < 1);

    CLS
    printf("Guitar size changed!\n");
    SPAUSE
}

void developerMode(){
    char *options[] = {"🆕 Add a new player", "🚫 Delete a player", "🔢 Change points and combo", "🎸 Change guitar size"};
    int numOps = 4;
    int op;

    while(1){
        CLS
        printf("-- Developer mode --");
        op = menu(options, numOps);

        CLS
        if(op == 0)
            addPlayer();
        if(op == 1)
            deletePlayer();
        if(op == 2)
            changePoints();
        if(op == 3)
            changeGuitarSize();
        if(op == ESC)
            break;
        if(op == (int)'g'){ // ...
            printf("Not so fast");
            sleep(1);
        }
    }
}   

void title(char *title){
    CLS
    for(int i = 0; title[i] != '\0'; i++){
        printf("%c", title[i]);
        Sleep(100);
    }
    Sleep(500);
}

void exitGame(){
    char *exit_messages[] = {"Saving the setlist", "Packing the gear", "Turning off the amps"};

    printf("%s", exit_messages[rand()%3]);
    for(int i = 0; i < 3; i++){
        Sleep(500);
        printf(".");
    }

    freeMemory();
    exit(0);
}

void saveGame(){
    char *spin[4] = {"|", "/", "-", "\\"};
    int i = 0;

    for(int j = 0; j < 10; j++){
        printf("\r%s Saving...", spin[i]);
        fflush(stdout);
        Sleep(300);
        i = (i + 1) % 4;
    }

    FILE *gameFile = fopen("GuitarHero.txt", "w");
    if(gameFile == NULL){
        errorMessage(-2);
    }

    for(int i = 0; i < _numPlayers; i++){
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

void loadGame(){
    int sep, i;
    char strAux[100], c;

    FILE *gameFile = fopen("GuitarHero.txt", "r");
    if(gameFile){
        i = 0;
        sep = 0;

        while (!feof(gameFile)){
            c = fgetc(gameFile);
            if(c != ';' && c != '#' && c != '\n' && c != EOF){
                strAux[i] = c;
                i++;
            } else if(c == ';' || c == '#' || c == '\n' || c == EOF){
                strAux[i] = '\0';
                i = 0;

                if(c == ';'){
                    if(sep == 0){
                        if(_numPlayers == 0){
                            _players = (TPlayer*)malloc(1 * sizeof(TPlayer));
                        } else{
                            _players = (TPlayer*)realloc(_players, (_numPlayers + 1) * sizeof(TPlayer));
                        }
                        validateAllocation(_players);

                        _players[_numPlayers].name = (char*)malloc((strlen(strAux) + 1) * sizeof(char));
                        validateAllocation(_players[_numPlayers].name);
                        strcpy(_players[_numPlayers].name, strAux);
                        sep++;
                    } else if(sep == 1){
                        _players[_numPlayers].score = atoi(strAux);
                        sep++;
                    } else if(sep == 2){
                        _players[_numPlayers].maxCombo = atoi(strAux);
                        sep = 0;
                        _numPlayers++;
                    }
                } else if(c == '#'){
                    lines = atoi(strAux);
                }
            }
        }
    }

    fclose(gameFile);
}

void freeMemory(){
    for(int i = 0; i < _numPlayers; i++){
        free(_players[i].name);
    }
    free(_players);
}

void validateAllocation(void *ptr){
    if(ptr == NULL){
        errorMessage(-1);
    }
}

void errorMessage(int errorCode){
    switch(errorCode){
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
        default:
            printf("\nERROR: Unknown error!!!\n");
            break;
    }

    SPAUSE
}

void gotoxy(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Windows API to move text cursor
}