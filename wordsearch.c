#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);

// Global variables
int bSize;
int found;

typedef struct coord{
    int x;
    int y;
} coord;

// Main function
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }

}

// Helper Function for Recursive Search
// This function uses backtracking to find the word recursively in every direction 
// and updates the path array
void searchHelper(char** arr, char* word, int wordSize, int row, int col, coord* path, int index) {
    if ((*(path + wordSize - 1)).x != -1){ 
        found = 1;
    }
    if (found) {
        return;
    }

    // Define the search boundaries
    int urb, lrb, ucb, lcb;
    lrb = (row == 0) ? row : row - 1;
    urb = (row == bSize - 1) ? row : row + 1;
    lcb = (col == 0) ? col : col - 1;
    ucb = (col == bSize - 1) ? col : col + 1;
    
    for (int i = lrb; i <= urb; i++){
        for (int j = lcb; j <= ucb; j++){
            if (i != (*(path + index - 1)).x || j != (*(path + index - 1)).y){
                if (*(*(arr + i) + j) == *(word + index)) {
                    coord point;
                    point.x = i;
                    point.y = j;
                    *(path + index) = point;
                    searchHelper(arr, word, wordSize, i, j, path, index + 1);
                    if(found) {
                        break;
                    }
                    // Reset path values to (-1, -1)
                    else {
                        for(int i = 1; i < wordSize; i++){
                            (*(path + i)).x = -1;
                            (*(path + i)).y = -1;
                        }
                    }
                }
            }
        }
        if (found){
            break;
        }
    }

    return;
}

void printPathResult(char* word, int wordSize, coord* path) {
    // Create 2D array for result grid
    int **result = (int**)malloc(bSize * sizeof(int*));
    for(int i = 0; i < bSize; i++){
        *(result + i) = (int*)malloc(bSize * sizeof(int));
        for (int j = 0; j < bSize; j++){
            *(*(result + i) + j) = 0;
        }
    }

    // Replace result squares with index where it was found
    int index = 1;
    for (int i = 0; i < wordSize; i++){
        if (*(*(result + ((*(path + i)).x)) + ((*(path + i)).y)) != 0){
            *(*(result + ((*(path + i)).x)) + ((*(path + i)).y)) *= 10;
            *(*(result + ((*(path + i)).x)) + ((*(path + i)).y)) += index;
        }
        else {
            *(*(result + ((*(path + i)).x)) + ((*(path + i)).y)) = index;
        }
        index++;
    }
    
    // Print result
    for(int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            printf("%d\t", *(*(result + i) + j));
        }
        printf("\n");
    }

}

void searchPuzzle(char** arr, char* word) {
    int wordSize = 0;

    // Convert lowercase letters to uppercase
    char* p;
    for(p = word; *p != '\0'; p++){
        if (*p >= 97 && *p <= 122){
            *p -= 32;
        }
        wordSize++;
    }
    
    int index = 0;

    // Initialize path array of coords with (-1, -1)
    coord *path = malloc(wordSize * sizeof(*path));
    for(int i = 0; i < wordSize; i++){
        (*(path + i)).x = -1;
        (*(path + i)).y = -1;
    }
    
    for (int i = 0; i < bSize; i++){
        for (int j = 0; j < bSize; j++){
            if (*(*(arr + i) + j) == *(word + index)){
                (*(path + index)).x = i;
                (*(path + index)).y = j;
                searchHelper(arr, word, wordSize, i, j, path, index + 1);
                if(found){
                    break;
                }
            }
        }
        if(found){
            break;
        }
    }

    if (found) {
        printf("\nWord found!\n");
        printf("Printing the search path: \n");
        printPathResult(word, wordSize, path);
    }
    else {
        printf("\nWord not found!\n");
    }

}

