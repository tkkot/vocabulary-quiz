# Vocabulary quiz (name WIP)

I am writing a program for training vocabulary. Partially because I wanted to have an app I could adjust to my needs and partially just for the fun and education of coding one myself.  
The app is written in C++ (although I started in Python, I switched to a language I am more familiar with).  
For now it functions as a commandline app.

## Current features
 - Fully functional set training
 - Reading sets from file
 - Updating file after training
 - Basic commandline interface
 - Basic parsing of commandline arguments when starting program

## Issues
 - Special characters (ą, ß, æ, π, ю, א etc.) not working on Windows, due to encoding problems. This also includes filepaths!!!
 - Training settings accessible only in code (will be fixed soon). Default training settings are: randomize, group, ~~repeat~~.

## User manual (WIP)
There is still **a lot** to write about.

### Commandline
When starting program from commandline, you can pass a command to be executed on startup. For example: `VocabularyQuiz t test.txt` will load sourcefile test.txt and start training it.

### Source files
The source file contains all the data about words to be trained.
Each line in the source file is one entry. Each entry consists of a key and answer pair, separated by a semicolon `;`.  
First goes the answer and then the key.  
`verbum; word` - while training the user will be asked to translate `word` into Latin. `verbum` is the answer they must enter.

You can request the user to enter multiple words (separated by commas). By default, all answers are mandatory:  
`pater, patris; father`  
The user's answer must contain both `pater` and `patris` to be correct.

However, if we want the other answer not to be mandatory, we put an asterisk `*` before it:  
`pater, *patris; father`  
Now `pater` is still mandatory, but `patris` is not - it can be added, but not writing it will still be accepted.

If we want an answer to be an alternative to the previous one, we put a pipe `|` before it.  
`verbum, verbi, |-i; word`  
Now be can put either `verbi` or `-i`, both will be accepted (`verbum` still must be present). 

While training the order of words doesn't matter.

### Sets and categories
The sourcefile is separated into sets, which are further separated into categories. While training you will be asked whether to train each set, sets are also scored separately.  
Categories are useful to group similar entries (for example same parts of speech or words with similar meaning). The _group_ option allows entries to be grouped according to category after being randomized.  
To define a new set start the line with `@`.  
To define a new category start the line with `$`.

#### Special symbol reference
 \# - comment  
 @ - new set  
 $ - new section  
 ! - entry with mistake  
 ^ - mastered entry  
answer modifiers:  
 (no symbol) - mandatory answer  
 \- - plural / ending  
 | - alternative to previous answer  
 \* - non-mandatory answer  
 
Above symbols work only if they are the first symbol in the line / word (not counting whitespaces).

## Final notes
I am just an amateur and coding is my hobby. Therefore I am aware that both the program itself and the development procedure are by no means professional.
