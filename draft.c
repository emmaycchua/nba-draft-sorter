#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// define the struct to hold the vars name, college,
// team, draft position, and the next player
typedef struct draftPick {
    char name[63];
    char college[63];
    char team[63];
    int pos;
    struct draftPick *next; // pointer to the next node
} draftPick;

// define a function to print the final list
void printList(draftPick *player) {
    while (player != NULL) {
        printf("%s: %s, %d, %s\n",
            player->college, player->name,
            player->pos, player->team);

        player = player->next;
    }
}

// define a function to free up memory
void freeList(draftPick *player) {
    while (player != NULL) {
        // use the pointer temp to keep track of the current node
        draftPick *temp = player;
        
        // advance to the next node
        player = player->next; 

        // then free the old node
        free(temp); 
    }
}

bool goesBefore(draftPick *player1, draftPick *player2) {
    // compare college names alphabetically
    int comp1 = strcmp(player1->college, player2->college);
    // sort by ascending alphabetical order based on the college name
    if (comp1 < 0) {
        return true; // player1's college comes first alphabetically
    } else if (comp1 > 0) {
        return false; // player2's college comes first alphabetically
    } else {
        // players from the same school are sorted by draft positions
        // so if two players are from the same school, the player with
        // the lower position goes first
        return player1->pos < player2-> pos;
    }
    
}

// insert new player in the correct place to keep the list sorted
// returns head
draftPick* insertPlayer(draftPick *newPlayer, draftPick *head) {
    if (head == NULL || goesBefore(newPlayer, head)) {
        // if list is empty OR if goesBefore == True: new player goes
        // at the start of the list --> result is just "newPlayer"
        newPlayer->next = head;
        return newPlayer;

    } else {
        // "current" loops through the list, and we compare with "newPlayer"
        // on each iteration
        // "previous" points to the thing before "current" so we can
        // insert if needed

        // declares "current" as a pointer to the draftPick struct after "head"
        draftPick *current = head -> next;

        // declares "previous" as a pointer to a draftPick struct "head"
        draftPick *previous = head;

        while (1) { // means "run forever" until "break"
            if (current == NULL) {
                // the end of the list is reached, and "newPlayer"
                // goes at the end of the list
                previous->next = newPlayer;
                newPlayer->next = NULL;
                break;
            }
            if (goesBefore(newPlayer, current)) {
                // "newPlayer" has to go between "previous" and "current"
                previous->next = newPlayer;
                newPlayer->next = current;
                break;
            } else {
                // "newPlayer" goes after "current" so we iterate forward
                previous = current;
                current = current->next;
            }
        }
        return head;
    }
}

// checks syntax
void usage() {
    printf("Syntax: ./sequence <fileName.txt>\n");
}

// removes the newline char from the end of a string
void chomp(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

int main(int argc, char *argv[]) {
    draftPick *head = NULL;
    if (argc != 2) {
        usage();
        return EXIT_SUCCESS;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    int count = 1; // keep track of how many people we've read so far
    char buffer[63]; // string used to read each line from the file

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // fgets() reads one line from the file into "buffer"
        // it stops at a new line
        // when it reaches EOF, fgets() returns NULL

        // remove "\n" from each line
        chomp(buffer);

        // stop reading if "DONE" encountered
        if (strcmp(buffer, "DONE") == 0) {
            break;
        }

        // allocate memory for a new node
        draftPick *node = (draftPick*) malloc(sizeof *node);

        // strcpy(dest, src) copies the string from src to dest, including the null terminator
        // copy the player's name from buffer into the node
        strcpy(node->name, buffer);

        // read line 2: the college
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            chomp(buffer);
            strcpy(node->college, buffer);
        }

        // read line 3: team
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            chomp(buffer);
            strcpy(node->team, buffer);
        }

        // put count into draft position
        node->pos = count;

        // set the "next" field to NULL for a new node
        node->next = NULL;
        count ++;   

        // add new player to the list in the correct place
        head = insertPlayer(node, head);
    }

    fclose(file);
    printList(head);
    freeList(head);
    
    return EXIT_SUCCESS;

}
