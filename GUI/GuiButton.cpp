// GuiButton.cpp

#include "GuiButton.h"

namespace coup {

    GuiButton::GuiButton(sf::Font& font, const std::string& text,
                         sf::Vector2f position, sf::Vector2f size,
                         std::function<void()> callback)
        : onClick(std::move(callback)), buttonText(text) {

        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color(100, 100, 200));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.f); // עובי למסגרת

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(16);
        label.setFillColor(sf::Color::White);
        sf::FloatRect bounds = label.getLocalBounds();
        label.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        label.setPosition(position.x + size.x / 2, position.y + size.y / 2);
    }

    void GuiButton::draw(sf::RenderWindow& window) {
        if (!visible) return;
        window.draw(shape);
        window.draw(label);
    }

    void GuiButton::handleClick(sf::Vector2f mousePos) {
        if (enabled && visible && shape.getGlobalBounds().contains(mousePos)) {
            onClick(); // הרצת פעולה
        }
    }

    void GuiButton::updateHover(sf::Vector2f mousePos) {
        if (!visible) return;
        if (shape.getGlobalBounds().contains(mousePos)) {
            shape.setOutlineColor(sf::Color(255, 105, 180)); // ורוד כשעוברים עם העכבר
        } else {
            shape.setOutlineColor(sf::Color::White);
        }
    }

    void GuiButton::setEnabled(bool value) {
        enabled = value;
        shape.setFillColor(enabled ? sf::Color(100, 100, 200) : sf::Color(80, 80, 80)); // כהה כשמושבת
    }

    void GuiButton::setVisible(bool value) {
        visible = value;
    }

    bool GuiButton::isEnabled() const {
        return enabled;
    }

    bool GuiButton::isVisible() const {
        return visible;
    }

    const std::string& GuiButton::getLabel() const {
        return buttonText;
    }

}
