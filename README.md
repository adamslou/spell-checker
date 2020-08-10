# Spell Checker

Spell Checker is a C application which loads a dictionary file and stores the contents in a hash map, which has been implemented from the bottom up. It takes a user-entered string and either outputs that the word is spelled correctly, or computes the Levenshtein Distance and gives the five most closely spelled words. A concordance functionality is also included.

## System Requirements

Linux, gcc, c99+, GNU Make

## Compilation

Use Make to compile: `$ make`

## Usage

Get the number of times each word appears in a document (the concordance).
```bash
$ ./prog [document.txt]
```
Start the spell checker and then enter a word to see if it's spelled correctly or 'quit' to quit the program.
```bash
$ ./spellChecker
```
