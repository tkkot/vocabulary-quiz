#!/bin/bash

DIR=/home/tkkot/dev/vocabulary-quiz

g++ $DIR/src/VocabularyQuiz.cpp $DIR/src/util.cpp -o $DIR/bin/Arch/VocabularyQuiz
g++ $DIR/src/UI.cpp $DIR/src/util.cpp -o $DIR/bin/Arch/UI
