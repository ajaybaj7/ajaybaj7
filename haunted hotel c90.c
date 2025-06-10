// Haunted Hotel Escape Game - Dev C++ Compatible Version

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h> // Include for Beep function

#define MAX_ROOMS 10
#define MAX_ACHIEVEMENTS 5
#define TIME_LIMIT 1200 // 20 minutes

// Inventory and Game State
int hasKey = 0, hasPage = 0, hasCode = 0, hasSafeCode = 0, hasClockAnswer = 0;
int hasMazeWin = 0, hasCipherItem = 0, hasMemoryPuzzle = 0, hasSideQuest = 0;
int candlelight = 100, sanity = 100;
int score = 0, journalUpdated = 0, dynamicScareCount = 0;
int achievements[MAX_ACHIEVEMENTS] = {0};
int mapExplored[MAX_ROOMS] = {0};
time_t startTime;

// Utility Functions
void delay(int seconds) {
    Sleep(seconds * 1000);
}

void toLower(char* str) {
    for (int i = 0; str[i]; i++) str[i] = tolower(str[i]);
}

void play_sound() {
    Beep(750, 300); // Frequency 750 Hz for 300 milliseconds
}

void dynamic_scare_event() {
    if (rand() % 4 == 0) {
        printf("\n?? A ghost whispers in your ear... \"Leave... or stay forever...\"\n");
        sanity -= 5;
        dynamicScareCount++;
        play_sound(); // Play sound on scare event
    }
}

void update_journal(const char* note) {
    printf("\n?? Journal updated: %s\n", note);
    journalUpdated++;
}

void check_achievements() {
    if (score >= 90 && !achievements[0]) {
        printf("?? Achievement Unlocked: Puzzle Master\n");
        achievements[0] = 1;
    }
    if (hasSideQuest && !achievements[1]) {
        printf("?? Achievement Unlocked: Ghost Whisperer\n");
        achievements[1] = 1;
    }
    if (difftime(time(NULL), startTime) < 600 && !achievements[2]) {
        printf("?? Achievement Unlocked: Speedrunner\n");
        achievements[2] = 1;
    }
    if (sanity >= 90 && !achievements[3]) {
        printf("?? Achievement Unlocked: Clear Mind\n");
        achievements[3] = 1;
    }
    if (journalUpdated >= 5 && !achievements[4]) {
        printf("?? Achievement Unlocked: Archivist\n");
        achievements[4] = 1;
    }
}

void show_map() {
    printf("\n??? MAP OF THE HOTEL:\n");
    for (int i = 0; i < MAX_ROOMS; i++) {
        printf("Room %d: %s\n", i + 1, mapExplored[i] ? "Explored" : "Unknown");
    }
}

void check_timer() {
    time_t now = time(NULL);
    if (difftime(now, startTime) > TIME_LIMIT) {
        printf("\n? TIME'S UP! The spirits claim your soul...\nGame Over.\n");
        exit(0);
    }
}

void room_memory_puzzle() {
    char answer[50];
    printf("\n?? Room 3 - Haunted Music Room\nA melody plays. Repeat the pattern: C D E C\nEnter the pattern (no spaces, e.g., CDEC): ");
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = 0;

    toLower(answer);
    if (strcmp(answer, "cdec") == 0) {
        printf("Correct! A secret panel opens.\n");
        hasMemoryPuzzle = 1;
        score += 10;
        update_journal("Solved music memory puzzle.");
        play_sound(); // Play sound on correct answer
    } else {
        printf("Wrong! The piano slams shut.\n");
        sanity -= 10;
    }
    dynamic_scare_event();
    mapExplored[2] = 1;
    candlelight -= 4;
    delay(1);
    check_timer();
    check_achievements();
}

void room_cipher_puzzle() {
    char answer[50];
    printf("\n?? Room 6 - Maze of Mirrors\nA riddle is scratched into glass: \"Khoor\". Decode it. Hint: Caesar Cipher +3\nAnswer: ");
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = 0;

    toLower(answer);
    if (strcmp(answer, "hello") == 0) {
        printf("Correct! A mirror shatters revealing a passage.\n");
        hasCipherItem = 1;
        score += 10;
        update_journal("Solved Caesar cipher in the mirror maze.");
        play_sound(); // Play sound on correct answer
    } else {
        printf("Wrong! Your reflection distorts...\n");
        sanity -= 10;
    }
    dynamic_scare_event();
    mapExplored[5] = 1;
    candlelight -= 4;
    delay(1);
    check_timer();
    check_achievements();
}

void room_example(int roomNum, const char* desc) {
    printf("\n?? Room %d - %s\n", roomNum, desc);
    mapExplored[roomNum - 1] = 1;
    dynamic_scare_event();
    sanity -= 2;
    candlelight -= 3;
    score += 5;
    update_journal("Found something eerie.");
    play_sound(); // Play sound on exploring a room
    delay(1);
    check_timer();
    check_achievements();
}

void side_quest_room() {
    if (!hasSideQuest) {
        printf("\n?? Side Quest: A weeping spirit asks you to find her name. Clue: 'E__a'\nAnswer: ");
        char input[50];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        toLower(input);
        if (strcmp(input, "emma") == 0) {
            printf("? The ghost is freed!\n");
            hasSideQuest = 1;
            score += 10;
            update_journal("Freed Emma's soul.");
            play_sound(); // Play sound on freeing the ghost
        } else {
            printf("? The ghost wails and disappears...\n");
            sanity -= 10;
        }
    }
    delay(1);
    check_timer();
}

void show_inventory() {
    printf("\n?? Inventory:\n");
    printf("- Rusted Key: %s\n", hasKey ? "?" : "?");
    printf("- Candlelight: %d\n- Sanity: %d\n", candlelight, sanity);
    printf("- Score: %d\n- Journal Entries: %d\n", score, journalUpdated);
}

int main() {
    int choice;
    srand(time(NULL));
    startTime = time(NULL);

    printf("\nWelcome to the Haunted Hotel Escape Game!\nYou have 20 minutes to escape...\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Explore Room 1\n2. Explore Room 2\n3. Haunted Music Room (Memory Puzzle)\n4. Explore Room 4\n5. Explore Room 5\n6. Maze of Mirrors (Cipher Puzzle)\n7. Side Quest Room\n8. Show Inventory\n9. View Map\n10. Exit Game\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        // Clear input buffer
        while (getchar() != '\n');

        check_timer();

        switch (choice) {
            case 1: room_example(1, "Creepy Portrait Hall"); break;
            case 2: room_example(2, "Library of Whispers"); break;
            case 3: room_memory_puzzle(); break;
            case 4: room_example(4, "Bloodstained Bathroom"); break;
            case 5: room_example(5, "Clock Puzzle Chamber"); break;
            case 6: room_cipher_puzzle(); break;
            case 7: side_quest_room(); break;
            case 8: show_inventory(); break;
            case 9: show_map(); break;
            case 10: printf("?? Goodbye, brave soul!\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }

    return 0;
}

