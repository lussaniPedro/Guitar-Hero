#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#include <conio.h>
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
    int maxCombo; // Player max combo
} TPlayer;

/* Global variables */
TPlayer *_players; // Array of players
int _numPlayers = 0; // Number of players
int currentID = 0; // Current player index
int record = 0; // Player record
int lines = 16;

/* Functions declarations */
void menu(); // Show main menu
void start(); // Start the game
void title(char *title); // Type the title: "Guitar Hero"
void option(char op); // Choose option
void play(); // Play the game
void showRecords(); // Show player records
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
void developerMode(); // Developer mode
void validateAllocation(void *ptr); // Validate memory allocation
void freeMemory(); // Free memory
void saveGame(); // Save game in a file
void loadGame(); // Load game from a file
void errorMessage(int errorCode); // Show error message

int main(){
    char op;
    srand((unsigned)time(NULL));
    SetConsoleOutputCP(65001);

    CLS
    start();
    while(1){
        menu();
        op = getch();

        option(op);
    }

    return 0;
}

void menu(){
    CLS
    printf("-- Guitar Hero --\n");
    printf("1. Play\n");
    printf("2. Records\n");
    printf("3. Ranking\n");
    printf("4. Save\n");
    printf("5. Exit\n\n");
    printf("** Enter your choice: ");
}

void option(char op){
    CLS
    switch(op){
        case '1':
            play();
            break;
        case '2':
            showRecords();
            break;
        case '3':
            showRanking();
            break;
        case '4':
            saveGame();
            break;
        case '5':
            exitGame();
            break;
        case 'g':
            developerMode();
            break;
        default:
            errorMessage(-5);
            SPAUSE
    }
}

void play(){
    TNote note;
    int delay;

    int dif = 0;
    while(dif < 1 || dif > 3){
        printf("Set difficulty:\n");
        printf("1. Cry baby👶\n");
        printf("2. Normal😐\n");
        printf("3. Rock 'n' Roll🤘\n\n");
        printf("** Enter your choice: ");
        scanf("%d", &dif);

        if(dif < 1 || dif > 3){
            errorMessage(-5);
        }
        CLS
    }

    if(dif == 1){
        delay = 500;
    } else if(dif == 2){
        delay = 300;
    } else if(dif == 3){
        delay = 200;
    }

    int combo = 0;
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
    } else{
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
           if(toupper(pressed) == note.key && note.pos.lin == (lines / 2) + 1){
                _players[currentID].score += 10;
                combo++;
                if(combo > _players[currentID].maxCombo){
                    _players[currentID].maxCombo = combo;
                    record = 1;
                }
            } else{
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
            } else{
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
    printf("  %s_________________\n", BLACK);  
    for(int i = 0; i < lines; i++){
        for(int j = 0; j < COLUMNS; j++){
            if (i == note.pos.lin && j == note.pos.col && i != (lines / 2)){
                printf("  %s%c%s ", note.color, note.key, BLACK);
            } else if(i == (lines / 2) && j == 0){
                if(note.pos.lin == (lines / 2) && note.pos.col == 0){
                    printf("  %s%c%s___%s|%s___%s|%s___%s|%s___%s|%s", note.color ,note.key, BLACK, RED, BLACK, YELLOW, BLACK, BLUE, BLACK, ORANGE, BLACK);
                } else if(note.pos.lin == (lines / 2) && note.pos.col == 1){
                    printf("  %s|%s___%s%c%s___%s|%s___%s|%s___%s|%s", GREEN, BLACK, note.color, note.key, BLACK, YELLOW, BLACK, BLUE, BLACK, ORANGE, BLACK);
                } else if(note.pos.lin == (lines / 2) && note.pos.col == 2){
                    printf("  %s|%s___%s|%s___%s%c%s___%s|%s___%s|%s", GREEN, BLACK, RED, BLACK, note.color, note.key, BLACK, BLUE, BLACK, ORANGE, BLACK);
                } else if(note.pos.lin == (lines / 2) && note.pos.col == 3){
                    printf("  %s|%s___%s|%s___%s|%s___%s%c%s___%s|%s", GREEN, BLACK, RED, BLACK, YELLOW, BLACK, note.color, note.key, BLACK, ORANGE, BLACK);
                } else if(note.pos.lin == (lines / 2) && note.pos.col == 4){
                    printf("  %s|%s___%s|%s___%s|%s___%s|%s___%s%c%s", GREEN, BLACK, RED, BLACK, YELLOW, BLACK, BLUE, BLACK, note.color, note.key, BLACK);
                } else{
                    printf("  %s|%s___%s|%s___%s|%s___%s|%s___%s|%s", GREEN, BLACK, RED, BLACK, YELLOW, BLACK, BLUE, BLACK, ORANGE, BLACK);
                }
            } else if(i != (lines / 2)){
                printf("  %s| ", BLACK);
            }
        }
        printf("\n");
    }

    printf(RESET);
}

void displayScore(int score, int combo){
    printf("\nScore: %d | Combo: x%d", score, combo);
    if(combo >= 10){
        printf("🔥🔥🔥");
    }
    printf("\n");
}

void showRecords(){
    char *explosion[4] = {"@", "*", ".", " "};
    if(record == 1){
        for(int i = 0; i < 4; i++){
            printf("%s%s NEW %sRECORD %s\n", RED, explosion[i], YELLOW, explosion[i]);
            Sleep(500);
            CLS
        }
        printf(RESET);
        record = 0;
    }

    printf("        --- Records ---\n");
    printf("Player: %s | Score: %d | Max combo: x%d🔥\n\n", _players[currentID].name, _players[currentID].score, _players[currentID].maxCombo);
    SPAUSE
}

void showRanking(){
    for(int i = 0; i < _numPlayers - 1; i++){
        for(int j = 0; j < _numPlayers - 1 - i; j++){
            if(_players[j].score < _players[j + 1].score || (_players[j].score == _players[j + 1].score && _players[j].maxCombo < _players[j + 1].maxCombo)){                
                TPlayer temp = _players[j];
                _players[j] = _players[j + 1];
                _players[j + 1] = temp;
            }
        }
    }

    printf("      --- Ranking ---\n");
    for(int i = 0; i < _numPlayers; i++){
        if(i == 0){
            printf("%s%d%s - %s [%-3d Points | Max combo: x%.2d]🔥\n", YELLOW, i + 1, RESET, _players[i].name, _players[i].score, _players[i].maxCombo);
        } else if(i == _numPlayers - 1){
            printf("%s%d%s - %s [%-3d Points | Max combo: x%.2d]👎\n", RED, i + 1, RESET, _players[i].name, _players[i].score, _players[i].maxCombo);
        }else{
            printf("%d - %s [%-3d Points | Max combo: x%.2d]\n", i + 1, _players[i].name, _players[i].score, _players[i].maxCombo);
        }
    }

    printf("\n");    
    SPAUSE
}

void start(){
    const char *start_messages[3] = {"Connecting amplifiers", "Adjusting volume", "Tuning the guitar"};
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

    char choice = '0';

    while(choice != '1' && choice != '2' && _numPlayers > 0){
        printf("1. Select a existing player\n");
        printf("2. Add a new player\n\n");
        printf("** Enter your choice: ");
        choice = getch();
    }

    if(choice == '1'){
        selectPlayer();
    } else if(choice == '2'){
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
    currentID = id - 1;
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

    do{
        CLS
        printf("--- Players ---\n");
        for(int i = 0; i < _numPlayers; i++){
            printf("[%d - %s]\n", i + 1, _players[i].name);
        }
        printf("\n** Select the player: ");
        scanf("%d", &id);
        fflush(stdin);

        if(id == currentID){
            check = 1;
        }

        if(id < 1 || id > _numPlayers){
            errorMessage(-4);
        }
    } while(id < 1 || id > _numPlayers);

    for(int i = id - 1; i < _numPlayers - 1; i++){
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
    char op;

    while(1){
        CLS
        printf("1. Add a new player\n");
        printf("2. Delete a player\n");
        printf("3. Change points and combo\n");
        printf("4. Change guitar size\n");
        printf("5. Exit\n\n");
        printf("** Enter your choice: ");
        op = getch();

        if(op == '1'){
            addPlayer();
        } else if(op == '2'){
            deletePlayer();
        } else if(op == '3'){
            changePoints();
        } else if(op == '4'){
            changeGuitarSize();
        } else if(op == '5'){
            break;
        } else if(op == 'g'){
            printf("Not so fast\n");
        } else{
            errorMessage(-5);
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
    char *exit_messages[3] = {"Saving the setlist", "Packing the gear", "Turning off the amps"};

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

    fclose(gameFile);

    CLS
    printf("Game saved successfully!\n");
    SPAUSE
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
            if(c != ';' && c != '\n' && c != EOF){
                strAux[i] = c;
                i++;
            } else if(c == ';' || c == '\n' || c == EOF){
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