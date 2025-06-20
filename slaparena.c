#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX_HEALTH 100
#define ARENA_WIDTH 50
#define ARENA_HEIGHT 15
#define MAX_NAME_LENGTH 50
#define ATTACK_RANGE 3
#define MOVE_DISTANCE 1
#define STUN_DURATION 2 // turns

typedef struct {
    char name[MAX_NAME_LENGTH];
    int health;
    int position;
    int is_stunned;
    int stun_timer;
} Player;

// Function prototypes
void loading_screen();
void main_menu();
void display_arena(Player p1, Player p2);
void game_loop();
void clear_screen();
void print_centered(const char* text, int width);
void animate_loading();
int get_random_damage();
void display_health_bar(int health, int max_health);
void get_player_names(char* name1, char* name2);
void save_winner_to_file(const char* winner);
void play_sound_success();
void play_sound_hit();
void play_sound_stunned();
void play_sound_move();
void play_sound_menu();
void play_sound_start();
void print_instruction_banner();
void display_end_game(const char* winner);

int main() {
    srand((unsigned)time(NULL));
    loading_screen();
    main_menu();
    return 0;
}

void clear_screen() {
    system("cls");
}

void print_centered(const char* text, int width) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
}

void animate_loading() {
    char loading_chars[] = {'|', '/', '-', '\\'};
    for (int i = 0; i < 20; i++) {
        printf("\r    Loading %c", loading_chars[i % 4]);
        fflush(stdout);
        Sleep(100);
    }
    printf("\n");
}

// Sound effect functions using Windows Beep()
void play_sound_success() {
    Beep(1000, 150);
    Beep(1300, 150);
    Beep(1500, 100);
}

void play_sound_hit() {
    Beep(600, 100);
    Beep(400, 100);
    Beep(300, 150);
}

void play_sound_stunned() {
    Beep(250, 300);
    Beep(200, 200);
}

void play_sound_move() {
    Beep(500, 50);
}

void play_sound_menu() {
    Beep(800, 100);
}

void play_sound_start() {
    Beep(700, 100);
    Beep(900, 100);
    Beep(1100, 100);
}

void loading_screen() {
    clear_screen();
    printf("\n\n\n");
    print_centered("========================================", 50);
    print_centered("         SLAP ARENA", 50);
    print_centered("    Supreme Stickman Combat", 50);
    print_centered("========================================", 50);
    printf("\n\n");
    print_centered("Initializing combat systems...", 50);
    animate_loading();
    printf("\n");
    print_centered("Press any key to continue...", 50);
    play_sound_start();
    getch();
}

void main_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n\n");
        print_centered("+--------------------------------------+", 50);
        print_centered("|            SLAP ARENA               |", 50);
        print_centered("|          MAIN MENU                  |", 50);
        print_centered("|--------------------------------------|", 50);
        print_centered("|                                      |", 50);
        print_centered("|  1. Start Battle                    |", 50);
        print_centered("|  2. How to Play                     |", 50);
        print_centered("|  3. Exit Game                       |", 50);
        print_centered("|                                      |", 50);
        print_centered("+--------------------------------------+", 50);
        printf("\n\n");
        print_centered("Enter your choice: ", 50);

        int result = scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer
        
        if (result != 1) {
            // Invalid input (not a number)
            printf("\nInvalid input! Please enter a number 1-3.");
            play_sound_stunned();
            Sleep(1000);
            continue;
        }

        switch (choice) {
            case 1:
                play_sound_menu();
                game_loop();
                break;
            case 2:
                play_sound_menu();
                print_instruction_banner();
                break;
            case 3:
                clear_screen();
                print_centered("Thanks for playing SLAP ARENA!", 50);
                play_sound_success();
                exit(0);
            default:
                printf("\nInvalid choice! Press any key to try again...");
                play_sound_stunned();
                getch();
        }
    }
}

void print_instruction_banner() {
    clear_screen();
    printf("\n\n");
    print_centered("=== HOW TO PLAY ===", 50);
    printf("\n");
    print_centered("Welcome to the ultimate Slap Arena!", 50);
    print_centered("Two players fight by slapping each other.", 50);
    print_centered("Move close and slap to deal damage.", 50);
    printf("\n");
    print_centered("Player 1 Controls:", 50);
    print_centered("A - Move Left", 50);
    print_centered("D - Move Right", 50);
    print_centered("S - Slap Attack", 50);
    printf("\n");
    print_centered("Player 2 Controls:", 50);
    print_centered("J - Move Left", 50);
    print_centered("L - Move Right", 50);
    print_centered("K - Slap Attack", 50);
    printf("\n");
    print_centered("First to reduce opponent's health to 0 wins!", 50);
    print_centered("Beware: Slaps can STUN your enemy!", 50);
    printf("\n");
    print_centered("Press any key to return to menu...", 50);
    getch();
}

void get_player_names(char* name1, char* name2) {
    clear_screen();
    printf("\nEnter name for Player 1: ");
    fgets(name1, MAX_NAME_LENGTH, stdin);
    name1[strcspn(name1, "\n")] = '\0'; // Remove newline

    printf("Enter name for Player 2: ");
    fgets(name2, MAX_NAME_LENGTH, stdin);
    name2[strcspn(name2, "\n")] = '\0';

    FILE* file = fopen("players.txt", "w");
    if (file) {
        fprintf(file, "%s\n%s\n", name1, name2);
        fclose(file);
    }
    play_sound_start();
}

void save_winner_to_file(const char* winner) {
    FILE* file = fopen("match_result.txt", "a");
    if (file) {
        fprintf(file, "Winner: %s\n", winner);
        fclose(file);
    }
}

void display_health_bar(int health, int max_health) {
    int bar_length = 20;
    int filled = (health * bar_length) / max_health;
    printf("[");
    for (int i = 0; i < filled; i++) printf("#");
    for (int i = filled; i < bar_length; i++) printf("-");
    printf("] %d/%d", health, max_health);
}

void display_arena(Player p1, Player p2) {
    clear_screen();
    printf("\n  %s", p1.name);
    for (int i = 0; i < 25 - strlen(p1.name); i++) printf(" ");
    printf("%s\n", p2.name);

    printf("  ");
    display_health_bar(p1.health, MAX_HEALTH);
    printf("     ");
    display_health_bar(p2.health, MAX_HEALTH);
    printf("\n\n  ");
    for (int i = 0; i < ARENA_WIDTH; i++) printf("-");
    printf("\n");

    for (int row = 0; row < ARENA_HEIGHT; row++) {
        printf("  |");
        for (int col = 0; col < ARENA_WIDTH - 2; col++) {
            if (row == ARENA_HEIGHT - 3) { // Player line
                if (col == p1.position) printf(p1.is_stunned ? "X" : "O");
                else if (col == p2.position) printf(p2.is_stunned ? "X" : "@");
                else printf(" ");
            } else if (row == ARENA_HEIGHT - 2) { // Weapon/arm line
                if (col == p1.position || col == p2.position) printf("|");
                else printf(" ");
            } else if (row == ARENA_HEIGHT - 1) { // Ground line
                if (col == p1.position || col == p2.position) printf("^");
                else printf("_");
            } else printf(" ");
        }
        printf("|\n");
    }
    printf("  ");
    for (int i = 0; i < ARENA_WIDTH; i++) printf("-");
    printf("\n\n  P1: A/D-Move, S-Slap    P2: J/L-Move, K-Slap    Q-Quit\n");
    if (p1.is_stunned) printf("  %s is STUNNED!\n", p1.name);
    if (p2.is_stunned) printf("  %s is STUNNED!\n", p2.name);
}

void display_end_game(const char* winner) {
    clear_screen();
    printf("\n\n\n");
    print_centered("========================================", 50);
    print_centered("           GAME OVER", 50);
    printf("\n");
    print_centered("The winner is...", 50);
    printf("\n");
    print_centered(winner, 50);
    printf("\n");
    print_centered("========================================", 50);
    printf("\n");
    print_centered("Press any key to return to menu...", 50);
    getch();
}

int get_random_damage() {
    return 15 + (rand() % 16); // 15-30 damage
}

void game_loop() {
    char name1[MAX_NAME_LENGTH], name2[MAX_NAME_LENGTH];
    get_player_names(name1, name2);

    Player p1 = {"", MAX_HEALTH, 10, 0, 0};
    Player p2 = {"", MAX_HEALTH, ARENA_WIDTH - 10, 0, 0};
    strncpy(p1.name, name1, MAX_NAME_LENGTH);
    strncpy(p2.name, name2, MAX_NAME_LENGTH);

    while (1) {
        // Update stun status
        if (p1.is_stunned && --p1.stun_timer <= 0) {
            p1.is_stunned = 0;
        }
        if (p2.is_stunned && --p2.stun_timer <= 0) {
            p2.is_stunned = 0;
        }

        display_arena(p1, p2);

        // Check for game over
        if (p1.health <= 0) {
            play_sound_success();
            save_winner_to_file(p2.name);
            display_end_game(p2.name);
            return;
        }
        if (p2.health <= 0) {
            play_sound_success();
            save_winner_to_file(p1.name);
            display_end_game(p1.name);
            return;
        }

        // Get player input
        char input = getch();

        // Player 1 controls
        if (!p1.is_stunned) {
            switch(tolower(input)) {
                case 'a':
                    if (p1.position > 1) {
                        p1.position -= MOVE_DISTANCE;
                        play_sound_move();
                    }
                    break;
                case 'd':
                    if (p1.position < ARENA_WIDTH - 4) {
                        p1.position += MOVE_DISTANCE;
                        play_sound_move();
                    }
                    break;
                case 's':
                    if (abs(p1.position - p2.position) <= ATTACK_RANGE) {
                        int dmg = get_random_damage();
                        p2.health -= dmg;
                        p2.is_stunned = 1;
                        p2.stun_timer = STUN_DURATION;
                        play_sound_hit();
                        play_sound_stunned();
                        printf("\n  ** %s slaps %s for %d damage! **\n", p1.name, p2.name, dmg);
                        Sleep(800);
                    }
                    break;
            }
        }

        // Player 2 controls
        if (!p2.is_stunned) {
            switch(tolower(input)) {
                case 'j':
                    if (p2.position > 1) {
                        p2.position -= MOVE_DISTANCE;
                        play_sound_move();
                    }
                    break;
                case 'l':
                    if (p2.position < ARENA_WIDTH - 4) {
                        p2.position += MOVE_DISTANCE;
                        play_sound_move();
                    }
                    break;
                case 'k':
                    if (abs(p1.position - p2.position) <= ATTACK_RANGE) {
                        int dmg = get_random_damage();
                        p1.health -= dmg;
                        p1.is_stunned = 1;
                        p1.stun_timer = STUN_DURATION;
                        play_sound_hit();
                        play_sound_stunned();
                        printf("\n  ** %s slaps %s for %d damage! **\n", p2.name, p1.name, dmg);
                        Sleep(800);
                    }
                    break;
            }
        }

        // Quit game
        if (tolower(input) == 'q') {
            play_sound_menu();
            return;
        }

        // Ensure health doesn't go below 0
        if (p1.health < 0) p1.health = 0;
        if (p2.health < 0) p2.health = 0;
    }
}

