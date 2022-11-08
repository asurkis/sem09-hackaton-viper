#ifndef Context_hpp_INCLUDED
#define Context_hpp_INCLUDED

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>

class Context : public sf::Drawable {
  sf::Font mainFont;
  sf::Texture image;
  sf::Vector2u cursor;
  sf::Vector2u select;
  std::string lastFilepath;
  unsigned int currentScale = 16;
  unsigned int fontSize     = 16;
  bool quitting             = false;

 public:
  std::vector<sf::Color> palette;
  std::wstring_view statusLinePrefix;
  std::wstring_view statusLine;

  Context() : palette(36) {
    mainFont.loadFromFile("JetBrainsMono-Regular.ttf");
  }

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

  void expand(int offset, const std::wstring& direction) {}

  void moveCursor(int dx, int dy) {
    int cx   = (int)cursor.x + dx;
    int cy   = (int)cursor.y + dy;
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

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    sf::Sprite sprite(image);
    sf::Vector2u pos = (target.getSize() - currentScale * image.getSize()) / 2u;
    sprite.setScale(currentScale, currentScale);
    sprite.setPosition((float)pos.x, (float)pos.y);
    target.draw(sprite);

    sf::RectangleShape wrapAround;
    wrapAround.setPosition(sf::Vector2f(pos + currentScale * cursor));
    wrapAround.setSize(sf::Vector2f(currentScale, currentScale));
    wrapAround.setFillColor(sf::Color(0, 0, 0, 0));
    wrapAround.setOutlineColor(sf::Color::Cyan);
    wrapAround.setOutlineThickness(2.0f);
    target.draw(wrapAround);

    sf::Text text;
    text.setString(std::wstring(statusLinePrefix) + std::wstring(statusLine));
    text.setFont(mainFont);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(fontSize);

    sf::Vector2u mainSize = target.getSize();
    auto localBounds      = text.getLocalBounds();
    auto lineSpacing      = mainFont.getLineSpacing(fontSize);
    mainSize.y -= lineSpacing * 3 / 2;
    text.setPosition(sf::Vector2f(0.0f, mainSize.y + lineSpacing * 1 / 4));

    sf::RectangleShape backgroundRect;
    backgroundRect.setPosition(sf::Vector2f(0.0f, mainSize.y));
    backgroundRect.setSize(sf::Vector2f(mainSize.x, lineSpacing * 3 / 2));
    backgroundRect.setFillColor(sf::Color(0xcccc00ff));

    target.draw(backgroundRect);
    target.draw(text);
  }

  void newFile(std::pair<int32_t,int32_t> size){
    sf::Image buf;
    buf.create(size.first, size.second, sf::Color::Black);
    image.loadFromImage(buf);

    lastFilepath="";
  }
};

#endif  // Context_hpp_INCLUDED
