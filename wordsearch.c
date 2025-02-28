#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations
void printPuzzle(char **arr);
void searchPuzzle(char **arr, char *word);
void freeMemory(char **block, int size, char *word);

// Global variables
int gridSize;
int found = 0;

typedef struct coord {
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
    fscanf(fptr, "%d\n", &gridSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(gridSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < gridSize; i++) {
        block[i] = (char*)malloc(gridSize * sizeof(char));
        for (j = 0; j < gridSize - 1; ++j) {
            fscanf(fptr, "%c ", &block[i][j]);            
        }
        fscanf(fptr, "%c \n", &block[i][j]);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to search the word in the puzzle
    searchPuzzle(block, word);

    // Free allocated memory
    freeMemory(block, gridSize, word);
    
    return 0;
}

void printPuzzle(char **arr) {
    int i, j;
    for (i = 0; i < gridSize; i++){
        for (j = 0; j < gridSize; j++){
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }
}

void freeMemory(char **block, int size, char *word) {
    for (int i = 0; i < size; i++) {
        free(block[i]);
    }
    free(block);
    free(word);
}

// Helper Function for Recursive Search
// This function uses backtracking to find the word recursively in every direction 
// and updates the path array
void searchHelper(char **arr, char *word, int wordSize, int row, int col, coord *path, int wordIndex) {
    // Base case: if the last element in the path has been set, mark as found
    if (path[wordSize - 1].x != -1) { 
        found = 1;
        return;
    }

    // Define the search boundaries
    // Define a frame around the puzzle grid element
    int urb, lrb, ucb, lcb;
    lrb = (row == 0) ? row : row - 1;
    urb = (row == gridSize - 1) ? row : row + 1;
    lcb = (col == 0) ? col : col - 1;
    ucb = (col == gridSize - 1) ? col : col + 1;
    
    for (int i = lrb; i <= urb; i++){
        for (int j = lcb; j <= ucb; j++){
            if (i != path[wordIndex - 1].x || j != path[wordIndex - 1].y){
                if (arr[i][j] == word[wordIndex]) {
                    coord point;
                    point.x = i;
                    point.y = j;
                    path[wordIndex] = point;
                    searchHelper(arr, word, wordSize, i, j, path, wordIndex + 1);
                    if (found) {
                        break;
                    }
                    // Reset path values to (-1, -1)
                    else {
                        for (int k = 1; k < wordSize; k++){
                            path[k].x = -1;
                            path[k].y = -1;
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

void printPathResult(char *word, int wordSize, coord *path) {
    // Create 2D array for result grid
    int **pathResult = (int**)malloc(gridSize * sizeof(int*));
    for (int i = 0; i < gridSize; i++){
        pathResult[i] = (int*)malloc(gridSize * sizeof(int));
        for (int j = 0; j < gridSize; j++){
            pathResult[i][j] = 0;
        }
    }

    // Replace result squares with the wordIndex where it was found
    int wordIndex = 1;
    for (int i = 0; i < wordSize; i++){
        if (pathResult[path[i].x][path[i].y] != 0){
            pathResult[path[i].x][path[i].y] *= 10;
            pathResult[path[i].x][path[i].y] += wordIndex;
        }
        else {
            pathResult[path[i].x][path[i].y] = wordIndex;
        }
        wordIndex++;
    }
    
    // Print result grid
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            printf("%d\t", pathResult[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < gridSize; i++) {
        free(pathResult[i]);
    }
    free(pathResult);
}

void searchPuzzle(char **arr, char *word) {
    int wordSize = 0;

    // Convert lowercase letters to uppercase and compute word size
    for (int i = 0; word[i] != '\0'; i++){
        if (word[i] >= 'a' && word[i] <= 'z'){
            word[i] -= 32;
        }
        wordSize++;
    }
    
    int wordIndex = 0;

    // Initialize path array of coords with (-1, -1)
    coord *path = malloc(wordSize * sizeof(*path));
    for (int i = 0; i < wordSize; i++){
        path[i].x = -1;
        path[i].y = -1;
    }
    
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            if (arr[i][j] == word[wordIndex]){
                path[wordIndex].x = i;
                path[wordIndex].y = j;
                searchHelper(arr, word, wordSize, i, j, path, wordIndex + 1);
                if (found){
                    break;
                }
            }
        }
        if (found){
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

    free(path);
}
