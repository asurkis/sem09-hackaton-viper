#ifndef Context_hpp_INCLUDED
#define Context_hpp_INCLUDED

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

class Context {
  sf::Texture image;
  sf::Vector2u cursor;
  sf::Vector2u select;
  std::string lastFilepath;
  unsigned int currentScale = 16;
  bool quitting = false;

 public:
  std::vector<sf::Color> palette;

  Context() : palette(36) {}

  void quit() { quitting = true; }
  bool isQuitting() const { return quitting; }

  void loadFile(std::string const& filepath) {
    lastFilepath = filepath;
    image.loadFromFile(filepath);
    image.setSmooth(false);
  }

  void saveFile(std::string const& filepath = "") {
    if (!filepath.empty()) {
      lastFilepath = filepath;
    }

    sf::Image buf = image.copyToImage();
    buf.saveToFile(lastFilepath);
  }

  void moveCursor(int dx, int dy) {
    int cx = (int)cursor.x + dx;
    int cy = (int)cursor.y + dy;
    cursor.x = std::max(0, std::min((int)image.getSize().x - 1, cx));
    cursor.y = std::max(0, std::min((int)image.getSize().y - 1, cy));
  }

  void dropSelection() { select = cursor; }

  void replaceColor(int paletteId) {
    sf::Image buf;
    auto xmax = std::max(cursor.x, select.x);
    auto ymax = std::max(cursor.y, select.y);
    auto xmin = std::min(cursor.x, select.x);
    auto ymin = std::min(cursor.y, select.y);
    buf.create(xmax - xmin + 1, ymax - ymin + 1, palette[paletteId]);
    image.update(buf, xmin, ymin);
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

