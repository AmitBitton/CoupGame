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
    GUI/ActionHistory.cpp \
    Game.cpp \
    Player.cpp \
    Governor.cpp \
    General.cpp \
    Baron.cpp \
    Judge.cpp \
    Merchant.cpp \
    Spy.cpp

# שם קובץ ההרצה
TARGET = coup_gui

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
