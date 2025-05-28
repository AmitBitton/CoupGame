// GuiButton.cpp

#include "GuiButton.h"

namespace coup {

    // Constructor: initializes button shape, label, position, and callback
    GuiButton::GuiButton(sf::Font& font, const std::string& text,
                         sf::Vector2f position, sf::Vector2f size,
                         std::function<void()> callback)
        : onClick(std::move(callback)), buttonText(text) {

        shape.setSize(size);// Set button size
        shape.setPosition(position);// Set button position on screen
        shape.setFillColor(sf::Color(100, 100, 200));// Set fill color
        shape.setOutlineColor(sf::Color::White);// Set initial outline color
        shape.setOutlineThickness(2.f);// Set outline thickness

        label.setFont(font);// Set font for the text
        label.setString(text);// Set displayed text
        label.setCharacterSize(16);// Set font size
        label.setFillColor(sf::Color::White); // Set font color
        sf::FloatRect bounds = label.getLocalBounds();// Get size of the text
        label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2); // Center the origin
        label.setPosition(position.x + size.x / 2, position.y + size.y / 2);  // Position text in the middle of the button
    }

    // Draws the button if it's visible
    void GuiButton::draw(sf::RenderWindow& window) {
        if (!visible) return; // Don't draw if not visible
        window.draw(shape);// Draw button background
        window.draw(label);// Draw button text
    }

    // Handles click events if enabled and visible
    void GuiButton::handleClick(sf::Vector2f mousePos) {
        if (enabled && visible && shape.getGlobalBounds().contains(mousePos)) {
            onClick(); // Call the assigned callback
        }
    }

    // Updates the button's hover effect based on mouse position
    void GuiButton::updateHover(sf::Vector2f mousePos) {
        if (!visible) return; // Skip if not visible
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setOutlineColor(sf::Color(255, 105, 180));// Pink border on hover
        } else {
            shape.setOutlineColor(sf::Color::White); // White border by default
        }
    }

    // Sets whether the button is enabled and changes color accordingly
    void GuiButton::setEnabled(bool value) {
        enabled = value;
        shape.setFillColor(enabled ? sf::Color(100, 100, 200) : sf::Color(80, 80, 80));// Darker color when disabled
    }

    // Sets whether the button is visible
    void GuiButton::setVisible(bool value) {
        visible = value;
    }

    // Returns true if the button is enabled
    bool GuiButton::isEnabled() const {
        return enabled;
    }

    // Returns true if the button is visible
    bool GuiButton::isVisible() const {
        return visible;
    }

    // Returns the text label of the button
    const std::string& GuiButton::getLabel() const {
        return buttonText;
    }

}
