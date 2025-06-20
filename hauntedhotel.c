#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Inventory and progress flags
int hasKey = 0, hasPage = 0, hasCode = 0, hasSafeCode = 0;
int hasClockAnswer = 0, hasMazeWin = 0, hasTorch = 0, hasSecretAnswer = 0;
int trapTriggered = 0; // Trap penalty flag

// Game timing and scoring
time_t startTime;
int timeLimit = 600; // 10 minutes in seconds
int score = 0;

// Current room tracking
int currentRoom = 0; // Start room: 0 == Lobby

// Counters for hints and puzzle attempts
int wrongCommands = 0;
int safeAttempts = 0, clockAttempts = 0, mazeAttempts = 0, secretRiddleAttempts = 0;

// Function prototypes
void lobby();
void hallway();
void library();
void safeRoom();
void clockRoom();
void mazeRoom();
void secretRoom();

// Convert string to lowercase
void to_lower_str(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Check if time is up
int is_time_up() {
    return difftime(time(NULL), startTime) >= timeLimit;
}

// Save game state to a file
void save_game() {
    FILE *fp = fopen("savefile.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file for saving.\n");
        return;
    }
    fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d\n", 
            hasKey, hasPage, hasCode, hasSafeCode, 
            hasClockAnswer, hasMazeWin, hasTorch, trapTriggered,
            score, currentRoom, hasSecretAnswer);
    fclose(fp);
    printf("Game saved.\n");
}

// Load game state from a file
void load_game() {
    FILE *fp = fopen("savefile.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "No save file found.\n");
        return;
    }
    fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d",
           &hasKey, &hasPage, &hasCode, &hasSafeCode,
           &hasClockAnswer, &hasMazeWin, &hasTorch, &trapTriggered,
           &score, &currentRoom, &hasSecretAnswer);
    fclose(fp);
    printf("Game loaded.\n");
}

// Display available commands to the player
void show_help() {
    printf("\nAvailable commands:\n");
    printf("  look                - Describe the room\n");
    printf("  go       - Move north, south, east, or west\n");
    printf("  take          - Pick up an item\n");
    printf("  use           - Use an item\n");
    printf("  examine       - Inspect an item\n");
    printf("  solve       - Attempt to solve a puzzle or riddle\n");
    printf("  inventory           - Show your inventory\n");
    printf("  save                - Save the game\n");
    printf("  load                - Load the game\n");
    printf("  quit                - Quit the game\n");
    printf("  help                - Show this help menu\n\n");
}

// Show the player's inventory
void show_inventory() {
    printf("\nInventory:\n");
    if (!hasKey && !hasPage && !hasCode && !hasSafeCode && 
        !hasClockAnswer && !hasMazeWin && !hasTorch && !hasSecretAnswer) {
        printf("  (empty)\n");
        return;
    }
    if (hasKey) printf("  - Rusty Key\n");
    if (hasPage) printf("  - Torn Page from the diary\n");
    if (hasCode) printf("  - Code numbers\n");
    if (hasSafeCode) printf("  - Safe combination\n");
    if (hasClockAnswer) printf("  - Clock riddle answer\n");
    if (hasMazeWin) printf("  - Maze trophy\n");
    if (hasTorch) printf("  - Flickering Torch\n");
    if (hasSecretAnswer) printf("  - Secret answer\n");
    printf("\n");
}

// Room functions

void lobby() {
    printf("\nYou stand in the dilapidated hotel lobby. Shadows flicker where the chandelier once sparkled. To the north lies a shadowy hallway.\n");
    printf("An ancient, dust-covered book rests invitingly on a rickety table.\n");
}

void hallway() {
    printf("\nYou step into the dim hallway. Paint peels from the walls, and the cold air chills your bones.\n");
    printf("Doors are to the east and west; the lobby is behind you to the south.\n");
    if (!hasKey) {
        printf("A glint catches your eye - a rusty key lies half-buried in the dust.\n");
    }
}

void library() {
    printf("\nYou enter the musty library. Bookshelves tower, stuffed with long-forgotten tomes.\n");
    printf("A diary lies open, but a crucial page is missing. You sense something watches you from the dark corners.\n");
    if (!hasTorch) {
        printf("A flickering torch leans against a shelf - could be useful to see in darkness.\n");
    }
}

void safeRoom() {
    printf("\nA claustrophobic room with an old safe rests here. The air smells of rust and secrets.\n");
    printf("Write down the number carefully; the safe demands a 3-digit code.\n");
}

void clockRoom() {
    printf("\nA grand grandfather clock dominates this room, its pendulum swaying ominously.\n");
    printf("An eerie riddle etched above its face reads:\n");
    printf("\"I have hands but no fingers, a face but no eyes. What am I?\"\n");
}

void mazeRoom() {
    if (!trapTriggered) {
        printf("\nYou step into a labyrinthine maze wrapped in shadow and whispering winds. The air grows colder.\n");
    } else {
        printf("\nBruised and shaken, you cautiously navigate the twisted corridors.\n");
    }
    printf("Choose your direction wisely.\n");
}

void secretRoom() {
    printf("\nA hidden chamber filled with strange artifacts and relics.\n");
    printf("You finally found the heart of the mystery!\n");
    printf("A treasure chest gleams on a pedestal.\n");
    if (!hasSecretAnswer) {
        printf("A strange riddle is inscribed on the chest:\n");
        printf("\"What has keys but can't open locks?\"\n");
        printf("You might try to 'solve secret' to answer.\n");
    } else {
        printf("The chest is open, revealing ancient treasures and a secret scroll.\n");
    }
}

// Puzzle for safe
void puzzle_safe() {
    if (!hasSafeCode) {
        safeAttempts++;
        if (safeAttempts > 5) {
            printf("Hint: The code is a number related to emergency services.\n");
        }
        printf("Enter the 3-digit safe code carefully: ");
        char codeInput[10];
        if (!fgets(codeInput, sizeof(codeInput), stdin)) {
            printf("Input error.\n");
            return;
        }
        to_lower_str(codeInput);
        int codeVal = atoi(codeInput);
        if (codeVal == 317) {
            printf("The safe pops open, revealing a torn page inside!\n");
            hasSafeCode = 1;
            if (!hasPage) {
                hasPage = 1;
                score += 10;
                printf("You add the torn page to your inventory.\n");
            }
        } else {
            printf("Wrong code... the safe remains locked and you hear a distant creak.\n");
        }
    } else {
        printf("The safe is already open.\n");
    }
}

// Puzzle clock riddle
void puzzle_clock() {
    if (!hasClockAnswer) {
        clockAttempts++;
        if(clockAttempts > 5) {
            printf("Hint: It's a common household item that tells time.\n");
        }
        printf("Your answer to the riddle: ");
        char answer[50];
        if(!fgets(answer, sizeof(answer), stdin)) {
            printf("Input error.\n");
            return;
        }
        to_lower_str(answer);
        if (strstr(answer, "clock") != NULL || strstr(answer, "watch") != NULL) {
            printf("The clock chimes approvingly. You've solved the riddle.\n");
            hasClockAnswer = 1;
            score += 10;
        } else {
            printf("The clock ticks coldly - that is not correct.\n");
        }
    } else {
        printf("You have already solved this riddle.\n");
    }
}

// Maze puzzle with trap penalty
void puzzle_maze() {
    if (!hasMazeWin) {
        mazeAttempts++;
        if (mazeAttempts > 5) {
            printf("Hint: Avoid the left path that triggered the trap.\n");
        }
        printf("Choose direction (left, right, forward): ");
        char choice[20];
        if (!fgets(choice, sizeof(choice), stdin)) {
            printf("Input error.\n");
            return;
        }
        to_lower_str(choice);
        if (strstr(choice, "left") != NULL) {
            if (!trapTriggered) {
                printf("Snap! A hidden trapdoor opens beneath you! You fall and are hurt.\n");
                trapTriggered = 1;
                score -= 5;
                printf("Be more careful!\n");
            } else {
                printf("You cautiously avoid the trap this time.\n");
            }
        } else if (strstr(choice, "right") != NULL) {
            printf("You find a faint light and reach the maze exit.\n");
            hasMazeWin = 1;
            score += 20;
        } else if (strstr(choice, "forward") != NULL) {
            printf("The path leads you deeper; you hear eerie whispers around you.\n");
        } else {
            printf("That direction doesn't seem valid.\n");
        }
    } else {
        printf("You have already escaped the maze.\n");
    }
}

// Secret room chest riddle puzzle
void puzzle_secret() {
    if (!hasSecretAnswer) {
        secretRiddleAttempts++;
        if (secretRiddleAttempts > 3) {
            printf("Hint: Think of an object with many keys, but no locks to open.\n");
        }
        printf("Your answer to the chest's riddle: ");
        char answer[50];
        if (!fgets(answer, sizeof(answer), stdin)) {
            printf("Input error.\n");
            return;
        }
        to_lower_str(answer);
        if (strstr(answer, "piano") != NULL || strstr(answer, "keyboard") != NULL) {
            printf("The chest clicks open! You discover ancient treasures and a secret scroll.\n");
            hasSecretAnswer = 1;
            score += 30;
        } else {
            printf("Nothing happens. That doesn't seem right.\n");
        }
    } else {
        printf("The chest is already open.\n");
    }
}

// Examine items for descriptions or clues
void examine_item(const char* item) {
    if (strcmp(item, "key") == 0) {
        if (hasKey) {
            printf("A rusty key covered in dirt. Might open an old door.\n");
        } else {
            printf("You don't have the key.\n");
        }
    } else if (strcmp(item, "page") == 0) {
        if (hasPage) {
            printf("A torn page from a diary. Some faded writing hints at a secret passage.\n");
        } else {
            printf("You don't have any torn pages.\n");
        }
    } else if (strcmp(item, "torch") == 0) {
        if (hasTorch) {
            printf("A flickering flame burns in the torch. Useful for dark places.\n");
        } else {
            printf("You don't have a torch.\n");
        }
    } else {
        printf("You see nothing special about it.\n");
    }
}

// Process user commands
void process_command(char* input) {
    char command[50] = {0};
    char arg[50] = {0};
    sscanf(input, "%s %[^\n]", command, arg);
    to_lower_str(command);

    if (strcmp(command, "help") == 0) {
        show_help();
    } else if (strcmp(command, "quit") == 0) {
        printf("Thanks for playing! Your final score is %d.\n", score);
        exit(0);
    } else if (strcmp(command, "inventory") == 0) {
        show_inventory();
    } else if (strcmp(command, "save") == 0) {
        save_game();
    } else if (strcmp(command, "load") == 0) {
        load_game();
        // Show current room description after loading
        switch (currentRoom) {
            case 0: lobby(); break;
            case 1: hallway(); break;
            case 2: library(); break;
            case 3: safeRoom(); break;
            case 4: clockRoom(); break;
            case 5: mazeRoom(); break;
            case 6: secretRoom(); break;
            default: printf("You see nothing special.\n"); break;
        }
    } else if (strcmp(command, "look") == 0) {
        switch (currentRoom) {
            case 0: lobby(); break;
            case 1: hallway(); break;
            case 2: library(); break;
            case 3: safeRoom(); break;
            case 4: clockRoom(); break;
            case 5: mazeRoom(); break;
            case 6: secretRoom(); break;
            default: printf("You see nothing special.\n"); break;
        }
    } else if (strcmp(command, "go") == 0) {
        char dir[20];
        strcpy(dir, arg);
        to_lower_str(dir);

        switch (currentRoom) {
            case 0:
                if (strcmp(dir, "north") == 0) {
                    currentRoom = 1;
                    score++;
                    printf("You move north to the hallway.\n");
                    hallway();
                } else {
                    printf("You can't go that way.\n");
                }
                break;
            case 1:
                if (strcmp(dir, "south") == 0) {
                    currentRoom = 0;
                    printf("You return to the lobby.\n");
                    lobby();
                } else if (strcmp(dir, "east") == 0) {
                    currentRoom = 2;
                    printf("You enter the library.\n");
                    library();
                } else if (strcmp(dir, "west") == 0) {
                    if (hasKey) {
                        currentRoom = 3;
                        printf("You use your rusty key to unlock the door to the safe room.\n");
                        safeRoom();
                    } else {
                        printf("The door to the west is locked. You need a key.\n");
                    }
                } else if (strcmp(dir, "north") == 0) {
                    if (!hasPage) {
                        printf("A strange force prevents you from moving north. Perhaps something is missing.\n");
                    } else {
                        currentRoom = 4;
                        printf("You step into the clock room.\n");
                        clockRoom();
                    }
                } else {
                    printf("You can't go that way.\n");
                }
                break;
            case 2:
                if (strcmp(dir, "west") == 0) {
                    currentRoom = 1;
                    printf("You return to the hallway.\n");
                    hallway();
                } else {
                    printf("You can't go that way.\n");
                }
                break;
            case 3:
                if (strcmp(dir, "east") == 0) {
                    currentRoom = 1;
                    printf("You leave the safe room and return to the hallway.\n");
                    hallway();
                } else if (strcmp(dir, "north") == 0 && hasSafeCode) {
                    currentRoom = 5;
                    printf("You enter a secret passage to the maze room.\n");
                    mazeRoom();
                } else {
                    printf("You can't go that way.\n");
                }
                break;
            case 4:
                if (strcmp(dir, "south") == 0) {
                    currentRoom = 1;
                }
            break;
        case 5:
            if (strcmp(dir, "south") == 0) {
                currentRoom = 3;
                printf("You exit the maze back to the safe room.\n");
                safeRoom();
            } else if (strcmp(dir, "forward") == 0) {
                printf("You move deeper into the maze.\n");
                mazeRoom();
            } else {
                printf("You can't go that way.\n");
            }
            break;
        case 6:
            printf("You cannot leave the secret chamber yet.\n");
            break;
        default:
            printf("You can't go that way.\n");
            break;
        }
    } else if (strcmp(command, "take") == 0) {
        if (strcmp(arg, "key") == 0) {
            if (currentRoom == 1 && !hasKey) {
                hasKey = 1;
                score += 5;
                printf("You pick up the rusty key.\n");
            } else {
                printf("There is no key here to take.\n");
            }
        } else if (strcmp(arg, "torch") == 0) {
            if (currentRoom == 2 && !hasTorch) {
                hasTorch = 1;
                score += 5;
                printf("You take the flickering torch.\n");
            } else {
                printf("There is no torch here to take.\n");
            }
        } else if (strcmp(arg, "page") == 0) {
            if (currentRoom == 3 && hasSafeCode && !hasPage) {
                hasPage = 1;
                score += 10;
                printf("You take the torn page from the safe.\n");
            } else {
                printf("There is no page here to take.\n");
            }
        } else {
            printf("You can't take that.\n");
        }
    } else if (strcmp(command, "use") == 0) {
        if (strcmp(arg, "key") == 0) {
            if (hasKey && currentRoom == 1) {
                printf("You try the key on the west door.\n");
                printf("It fits perfectly! The door unlocks.\n");
                // No need to set hasKey to 0 as you keep the key
            } else {
                printf("Nothing happens when you try to use the key here.\n");
            }
        } else if (strcmp(arg, "torch") == 0) {
            if (hasTorch) {
                printf("The flickering torch lights up your surroundings.\n");
            } else {
                printf("You don't have a torch to use!\n");
            }
        } else if (strcmp(arg, "page") == 0 && currentRoom == 2 && hasPage) {
            printf("You place the torn page into the diary. A hidden latch clicks!\n");
            printf("A secret passage to the north has revealed itself!\n");
        } else if (strcmp(arg, "safe") == 0 && currentRoom == 3) {
            puzzle_safe();
        } else if (strcmp(arg, "clock") == 0 && currentRoom == 4) {
            puzzle_clock();
        } else {
            printf("You can't use that here.\n");
        }
    } else if (strcmp(command, "examine") == 0) {
        examine_item(arg);
    } else if (strcmp(command, "solve") == 0) {
        if (strcmp(arg, "safe") == 0 && currentRoom == 3) {
            puzzle_safe();
        } else if (strcmp(arg, "clock") == 0 && currentRoom == 4) {
            puzzle_clock();
        } else if (strcmp(arg, "maze") == 0 && currentRoom == 5) {
            puzzle_maze();
        } else if (strcmp(arg, "secret") == 0 && currentRoom == 6) {
            puzzle_secret();
        } else {
            printf("There is nothing to solve here with that name.\n");
        }
    } else {
        wrongCommands++;
        if (wrongCommands >= 5) {
            printf("Hint: Try typing 'help' to see available commands.\n");
            wrongCommands = 0;
        } else {
            printf("I don't understand that command. Type 'help' for a list of commands.\n");
        }
    }
}

int main() {
    printf("Welcome to Haunted Hotel! An atmospheric puzzle adventure.\n");
    printf("Solve puzzles, find secrets, and escape before time runs out.\n");
    printf("Type 'help' for a list of commands.\n");

    startTime = time(NULL);
    lobby();

    char input[100];
    while (1) {
        if (is_time_up()) {
            printf("\nThe clock strikes! Time's up! The hotel swallows you whole...\n");
            printf("Your final score: %d\n", score);
            break;
        }

        printf("\n> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\nInput error. Exiting.\n");
            break;
        }
        input[strcspn(input, "\n")] = 0;
        to_lower_str(input);

        if (strlen(input) == 0) continue;

        process_command(input);

        // Check for win condition
        if (hasSecretAnswer && currentRoom == 6) {
            printf("\n\nCONGRATULATIONS!\n");
            printf("You've solved all the mysteries of the haunted hotel!\n");
            printf("With the secret scroll in hand, you escape just as the building collapses behind you.\n");
            printf("Final score: %d\n", score + 50); // Bonus for completing the game
            break;
        }
    }

    return 0;
}
