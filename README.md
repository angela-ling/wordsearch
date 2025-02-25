# Word Search Puzzle Solver
This program reads a word search puzzle from a file, displays the grid, and allows the user to search for a word within the puzzle. The program locates words in all possible directions (horizontal, vertical, diagonal) and prints the path taken to find the word.
## Features
- Reads a square puzzle grid from a file.
- Allows users to enter a word to search.
- Searches for the word in all directions.
- Prints the path of the found word, displaying the order in which letters appear.
- Indicates when a word is not found.

## How to Compile and Run
**Compile the program using:**
```
gcc -o wordsearch wordsearch.c
```
**Run the program:**
```
./wordsearch input.txt
```
Replace input.txt with your puzzle file.

## Input File Format
- The first line contains a single integer N, the size of the puzzle grid.
- The next N lines contain N characters each, representing the grid.

### Example File
Below is an example puzzle file (puzzle1.txt):
```
5
W E B M O
I L H L L
M L Z E L
M Y E K O
A O A B A
```
- The first line represents the grid size (5x5).
- The following lines contain the letters in the puzzle grid.

## Sample Output
- Searching for "hello"
```
$ ./wordsearch puzzle1.txt
Enter the word to search: hello

Printing puzzle before search:
W E B M O 
I L H L L 
M L Z E L 
M Y E K O 
A O A B A 

Word found!
Printing the search path: 
0       0       0       0       5
0       0       1       3       4
0       0       0       2       0
0       0       0       0       0
0       0       0       0       0
```
- Searching for "banana"
```
$ ./wordsearch puzzle1.txt
Enter the word to search: banana

Printing puzzle before search:
W E B M O 
I L H L L 
M L Z E L 
M Y E K O 
A O A B A 

Word not found!
```
