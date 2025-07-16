# Git-minus : A tiny simulation of git
## Usage

1. gitm version              -- print a specified icon
2. gitm log                  -- show the history commit in order
3. gitm commit <message>     -- make a commit (Unlike the behavior of git, it will automatically commit all files in working directory)
4. gitm checkout <commit-id> -- checkout to any status according to given commit id
5. gitm merge <commit-id>    -- merge the specified commit with HEAD

## Structure
1. command.h                 -- contain main function and usage
2. committree.h              -- deal with the structure of historical commit
3. filetree.h                -- deal with the structure of files
4. filemanage.h              -- some functions to save and restore files
5. filearr.h                 -- helper for filetree.h and filemanage.h
6. pair.h                    -- file is saved in struct Pair
7. pairarr.h                 -- provide basic method for array of pairs
8. structures.h              -- define struct CTree, FTree, Pair, etc...
9. macros.h                  -- Macros for this program
10. gitm.h/utils.c           -- methods to create or remover directory, hash a file... 

## Build
this Program is built by cmake

```shell
# create Makefile
mkdir build && cd build && cmake ..
# compile
make
```

