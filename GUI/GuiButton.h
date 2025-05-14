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
        sf::RectangleShape shape;
        sf::Text label;
        std::function<void()> onClick;
        bool enabled = true;
        bool visible = true;
        std::string buttonText;

    public:
        GuiButton(sf::Font& font,
                  const std::string& text,
                  sf::Vector2f position,
                  sf::Vector2f size,
                  std::function<void()> callback);

        void draw(sf::RenderWindow& window);
        void handleClick(sf::Vector2f mousePos);
      //  void handleHover(sf::Vector2f mousePos);

        void setEnabled(bool value);
        void setVisible(bool value);

        bool isEnabled() const;
        bool isVisible() const;
        const std::string& getLabel() const;
        void updateHover(sf::Vector2f mousePos);

    };

}




#endif //GUIBUTTON_H
