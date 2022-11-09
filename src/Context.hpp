#ifndef Context_hpp_INCLUDED
#define Context_hpp_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
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
  unsigned int paletteSize = 22;
  sf::Vector2f paletteCoordinates[36];
  bool drawPalette = true;
  unsigned int fontSize     = 16;
  bool quitting             = false;

 public:
  std::vector<sf::Color> palette;
  std::wstring_view statusLinePrefix;
  std::wstring_view statusLine;

  Context() : palette(36) {
    paletteCoordinates[0] = sf::Vector2f(0, 10);

    for (int i = 0; i <= 9; ++i) {
      paletteCoordinates[i] = sf::Vector2f(0, i);
    }
    
    // red qaz
    paletteCoordinates['q' - 'a' + 10] = sf::Vector2f(1, 0);
    paletteCoordinates['a' - 'a' + 10] = sf::Vector2f(2, 0);
    paletteCoordinates['z' - 'a' + 10] = sf::Vector2f(3, 0);

    // orange wsx
    paletteCoordinates['w' - 'a' + 10] = sf::Vector2f(1, 1);
    paletteCoordinates['s' - 'a' + 10] = sf::Vector2f(2, 1);
    paletteCoordinates['x' - 'a' + 10] = sf::Vector2f(3, 1);

    // green edc
    paletteCoordinates['e' - 'a' + 10] = sf::Vector2f(1, 2);
    paletteCoordinates['d' - 'a' + 10] = sf::Vector2f(2, 2);
    paletteCoordinates['c' - 'a' + 10] = sf::Vector2f(3, 2);

    // blue rfv
    paletteCoordinates['r' - 'a' + 10] = sf::Vector2f(1, 3);
    paletteCoordinates['f' - 'a' + 10] = sf::Vector2f(2, 3);
    paletteCoordinates['v' - 'a' + 10] = sf::Vector2f(3, 3);

    // purple tgb
    paletteCoordinates['t' - 'a' + 10] = sf::Vector2f(1, 4);
    paletteCoordinates['g' - 'a' + 10] = sf::Vector2f(2, 4);
    paletteCoordinates['b' - 'a' + 10] = sf::Vector2f(3, 4);

    // pink yhn
    paletteCoordinates['y' - 'a' + 10] = sf::Vector2f(1, 5);
    paletteCoordinates['h' - 'a' + 10] = sf::Vector2f(2, 5);
    paletteCoordinates['n' - 'a' + 10] = sf::Vector2f(3, 5);

    // ligth blue ujm
    paletteCoordinates['u' - 'a' + 10] = sf::Vector2f(1, 6);
    paletteCoordinates['j' - 'a' + 10] = sf::Vector2f(2, 6);
    paletteCoordinates['m' - 'a' + 10] = sf::Vector2f(3, 6);

    // red pink ik
    paletteCoordinates['i' - 'a' + 10] = sf::Vector2f(1, 7);
    paletteCoordinates['k' - 'a' + 10] = sf::Vector2f(2, 7);

    // green blue olp
    paletteCoordinates['o' - 'a' + 10] = sf::Vector2f(1, 8);
    paletteCoordinates['l' - 'a' + 10] = sf::Vector2f(2, 8);
    paletteCoordinates['p' - 'a' + 10] = sf::Vector2f(1, 9);

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
    sf::View currentView(sf::FloatRect(0.f, 0.f, target.getSize().x, target.getSize().y));
    target.setView(currentView);

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

    mainSize.y -= 4 * paletteSize;

    if (drawPalette) {
      for (int i = 0; i < palette.size(); ++i) {
        sf::RectangleShape paletteRectangle;
        paletteRectangle.setPosition(
            sf::Vector2f(paletteCoordinates[i].y * paletteSize,
                         mainSize.y + paletteCoordinates[i].x * paletteSize));
        paletteRectangle.setSize(sf::Vector2f(paletteSize, paletteSize));
        paletteRectangle.setFillColor(palette[i]);
        target.draw(paletteRectangle);
      }
    }
    
    sf::Sprite sprite(image);
    sf::View imageView(sf::FloatRect(0.f, 0.f, image.getSize().x, image.getSize().y));
    imageView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, (float)mainSize.y/target.getSize().y));
    target.setView(imageView);
    target.draw(sprite);

    sf::RectangleShape wrapAround;
    wrapAround.setPosition(sf::Vector2f(cursor));
    wrapAround.setSize(sf::Vector2f(1, 1));
    wrapAround.setFillColor(sf::Color(0, 0, 0, 0));
    wrapAround.setOutlineColor(sf::Color::Cyan);
    wrapAround.setOutlineThickness(0.1f);
    target.draw(wrapAround);
    wrapAround.setOutlineColor(sf::Color::Red);
    wrapAround.setOutlineThickness(-0.1f);
    target.draw(wrapAround);
  }

  void newFile(std::pair<int32_t,int32_t> size){
    sf::Image buf;
    buf.create(size.first, size.second, sf::Color::Black);
    image.loadFromImage(buf);

    lastFilepath="";
  }
};

#endif  // Context_hpp_INCLUDED
