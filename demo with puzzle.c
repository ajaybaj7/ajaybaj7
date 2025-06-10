#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

int timer = 15;
int playerPos = 5;
int health = 100;

void startScreen() {
    system("cls");
    printf("=========================================\n");
    printf("      Haunted Hotel Escape Game\n");
    printf("=========================================\n");
    printf("Press ENTER to Start...\n");
    while (getch() != 13);
}

void drawRoom() {
    int i;
    system("cls");
    printf("Time left: %d seconds | Health: %d\n", timer, health);
    printf("Use W (up) / S (down) to move.\n");
    printf("-----------------------------------------\n");

    for (i = 0; i < 10; i++) {
        if (i == playerPos)
            printf(">> You are here <<\n");
        else
            printf("Room %d\n", i + 1);
    }

    printf("-----------------------------------------\n");
}

void waitOneSecond() {
    clock_t start = clock();
    while ((clock() - start) < CLOCKS_PER_SEC);
}


void puzzleRoom3() {
    int answer;
    printf("\nA voice whispers: 'I have keys but no locks. I have space but no room. What am I?'\n");
    printf("1. Keyboard  2. Map  3. Ghost\n");
    printf("Your answer (1-3): ");
    scanf("%d", &answer);

    if (answer == 1) {
        printf("Correct!\n");
    } else {
        printf("Wrong! You feel weaker...\n");
        health -= 20;
    }
    getch();  
}


void puzzleRoom7() {
    int answer;
    printf("\nA message appears on the wall: What is 3 * 4 + 2?\n");
    printf("1. 12  2. 14  3. 18\n");
    printf("Your answer (1-3): ");
    scanf("%d", &answer);

    if (answer == 2) {
        printf("Correct!\n");
    } else {
        printf("Wrong! Time and health slip away...\n");
        timer -= 3;
        health -= 20;
    }
    getch();  
}

void checkForPuzzles() {
    if (playerPos == 2) puzzleRoom3(); 
    if (playerPos == 6) puzzleRoom7(); 
}

int askReplay() {
    char choice;
    printf("\nGame over! Play again? (y/n): ");
    scanf(" %c", &choice);
    return (choice == 'y' || choice == 'Y');
}

void gameLoop() {
    char key;

    while (timer > 0 && health > 0) {
        if (kbhit()) {
            key = getch();
            if (key == 'w' || key == 'W') {
                if (playerPos > 0) playerPos--;
            } else if (key == 's' || key == 'S') {
                if (playerPos < 9) playerPos++;
            }
            checkForPuzzles();
        }

        drawRoom();
        waitOneSecond();
        timer--;
    }

    if (health <= 0) {
        printf("\nYou have lost all your health! You faint in the haunted hotel...\n");
    } else {
        printf("\nTime's up! You couldn't escape in time.\n");
    }
}

int main() {
    do {
        timer = 15;
        health = 100;
        playerPos = 5;
        startScreen();
        gameLoop();
    } while (askReplay());

    printf("\nThank you for playing!\n");
    return 0;
}

