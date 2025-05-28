//
// Created by amit on 5/11/25.
//

#ifndef GUIBUTTON_H
#define GUIBUTTON_H


#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace coup {
    class GuiButton {
    private:
        sf::RectangleShape shape; // The visual shape of the button
        sf::Text label; // The text displayed on the button
        std::function<void()> onClick; // Callback function triggered on click
        bool enabled = true; // Indicates if the button is clickable
        bool visible = true; // Indicates if the button is visible
        std::string buttonText; // The actual text label of the button

    public:
        // Constructor: Initializes a button with font, text, position, size, and click behavior
        GuiButton(sf::Font &font,
                  const std::string &text,
                  sf::Vector2f position,
                  sf::Vector2f size,
                  std::function<void()> callback);

        void draw(sf::RenderWindow &window); // Draws the button on the window
        void handleClick(sf::Vector2f mousePos); // Checks if the button was clicked and triggers the action
        void setEnabled(bool value); // Sets whether the button is clickable
        void setVisible(bool value); // Sets whether the button is visible

        bool isEnabled() const; // Returns true if the button is enabled
        bool isVisible() const; // Returns true if the button is visible
        const std::string &getLabel() const; // Returns the text label of the button
        void updateHover(sf::Vector2f mousePos); // Updates hover effect based on mouse position
    };
}


#endif //GUIBUTTON_H
