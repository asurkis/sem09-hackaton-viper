#ifndef Context_hpp_INCLUDED
#define Context_hpp_INCLUDED

#include <SFML/Graphics.hpp>
#include <algorithm>

class Context {
  sf::Texture image;
  unsigned int currentScale = 8;
  sf::Vector2u cursor;
  sf::Vector2u select;

 public:
  void loadFile(std::string const& filepath) { image.loadFromFile(filepath); }

  void moveCursor(int dx, int dy) {
    bool normal = select == cursor;
    int cx = (int)cursor.x + dx;
    int cy = (int)cursor.y + dy;
    cursor.x = std::max(0, std::min((int)image.getSize().x - 1, cx));
    cursor.y = std::max(0, std::min((int)image.getSize().y - 1, cy));
    if (normal) {
      select = cursor;
    }
  }

  void redraw(sf::RenderWindow& window) {
    sf::Sprite sprite(image);
    sf::Vector2u pos = (window.getSize() - currentScale * image.getSize()) / 2u;
    sprite.setScale(currentScale, currentScale);
    sprite.setPosition((float)pos.x, (float)pos.y);
    window.draw(sprite);

    sf::RectangleShape wrapAround;
    wrapAround.setPosition(sf::Vector2f(pos + currentScale * cursor));
    wrapAround.setSize(sf::Vector2f(currentScale, currentScale));
    wrapAround.setFillColor(sf::Color(0, 0, 0, 0));
    wrapAround.setOutlineColor(sf::Color::Cyan);
    wrapAround.setOutlineThickness(2.0f);
    window.draw(wrapAround);
  }
};

#endif  // Context_hpp_INCLUDED

