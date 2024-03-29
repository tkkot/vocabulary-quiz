#!/bin/bash

DIR=/home/tkkot/dev/vocabulary-quiz

g++ $DIR/src/VocabularyQuiz.cpp $DIR/src/training.cpp $DIR/src/UI/UI.cpp $DIR/src/util.cpp $DIR/src/UI/parser.cpp -o $DIR/bin/Arch/VocabularyQuiz -std=c++17
