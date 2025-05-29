CXX = clang++-18
CXXFLAGS = -std=c++20 -Wall -Wextra -I. -IGUI -IFont
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = GUI/GuiMain.cpp GUI/Screens.cpp GUI/GameDisplay.cpp GUI/GuiButton.cpp \
      Game.cpp Player.cpp Governor.cpp General.cpp Baron.cpp \
      Judge.cpp Merchant.cpp Spy.cpp

TEST_SRC = Test.cpp Game.cpp Player.cpp Governor.cpp General.cpp \
           Baron.cpp Judge.cpp Merchant.cpp Spy.cpp \
           GUI/GameDisplay.cpp GUI/GuiButton.cpp

MAIN_SRC = main.cpp Game.cpp Player.cpp Governor.cpp General.cpp \
           Baron.cpp Judge.cpp Merchant.cpp Spy.cpp


MAIN_TARGET = Main
TARGET = coup_gui
TEST_TARGET = coup_test


$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS)  $(SRC)  -o $(TARGET) $(LDFLAGS)

$(TEST_TARGET): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_TARGET) $(LDFLAGS)

$(MAIN_TARGET): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -o $(MAIN_TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

run_main: $(MAIN_TARGET)
	./$(MAIN_TARGET)

valgrind: $(TEST_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes 	./$(MAIN_TARGET)



clean:
	rm -f $(TARGET) $(TEST_TARGET) $(MAIN_TARGET) *.o
