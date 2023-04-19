OBJ = ./obj/VocabularyQuiz.o ./obj/util.o ./obj/training.o ./obj/UI/UI.o
SRC = ./src/VocabularyQuiz.cpp ./src/util.cpp ./src/training.cpp ./src/UI/UI.cpp
headers = ./src/functions.h ./src/types.h ./src/UI/messages.h
output = ./bin/Arch/VocabularyQuiz

all: $(output)
	echo $(LDFLAGS) $(CPPFLAGS) $(CXXFLAGS)
#	touch $(OBJ)
#	touch $(SRC)

#$(output): $(OBJ)
#	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(output): $(SRC)
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $(CXXFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJ): ./obj/%.o : ./src/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

./src/UI/UI.cpp: ./src/UI/messages.h
	touch $@

clean:
	rm -f $(OBJ)

.PHONY: all clean
