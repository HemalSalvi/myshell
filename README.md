# myshell
### Project 1 for UTD CS 4348 - Operating Systems
#### Hemal Salvi, Aahlad Madireddy
## Compile and Run

We include a minimal Makefile so compilation can be done on most -nix systems with a simple 
```
make
``` 

If you don't have the make tool installed, you can run 
``` 
cc myshell.c -o myshell
```

To run the shell, the output binary is called `myshell` by default so just do 
```
./myshell
``` 

## Screenshot

![Screenshot](/screenshot.png)

## Assumptions
1. The empty input doesn't count as a command in the final tally. 
2. We support a cd command as a built-in feature of the shell. 
3. The HOME environment variable is assumed to be set, which allows us to support a default location for `cd` with no arguments: the value of HOME. 
4. If a command is not found, it will print command not found and take no other action to rectify. 
5. When a command is called in background mode, there is no reporting of an invalid command, and all output is hidden. 
6. We have colored output, it's fun to run on a terminal with color support.
