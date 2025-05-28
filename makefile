# קומפיילר ודגלים
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I. -IGUI -IFont

# ספריות SFML
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# קבצי קוד
SRC = \
    GUI/GuiMain.cpp \
    GUI/Screens.cpp \
    GUI/GameDisplay.cpp \
    GUI/GuiButton.cpp \
    Game.cpp \
    Player.cpp \
    Governor.cpp \
    General.cpp \
    Baron.cpp \
    Judge.cpp \
    Merchant.cpp \
    Spy.cpp

# קבצי מקור לטסטים – כולל GUI
TEST_SRC = \
    Test.cpp \
    Game.cpp \
    Player.cpp \
    Governor.cpp \
    General.cpp \
    Baron.cpp \
    Judge.cpp \
    Merchant.cpp \
    Spy.cpp \
    GUI/GameDisplay.cpp \
    GUI/GuiButton.cpp
# קובץ main נפרד לבדיקה
MAIN_SRC = main.cpp \
    Game.cpp \
    Player.cpp \
    Governor.cpp \
    General.cpp \
    Baron.cpp \
    Judge.cpp \
    Merchant.cpp \
    Spy.cpp

MAIN_TARGET = Main



# שם קובץ ההרצה
TARGET = coup_gui
TEST_TARGET = coup_test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_TARGET): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(MAIN_TARGET): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

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
	rm -f $(TARGET)
