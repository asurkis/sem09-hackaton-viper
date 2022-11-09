#ifndef Context_hpp_INCLUDED
#define Context_hpp_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>
#include <cmath>

inline bool isPaletteKey(sf::Uint32 c) {
  return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z');
}

class Context : public sf::Drawable {
  sf::Font mainFont;
  sf::Texture image;
  sf::Vector2u cursor;
  sf::Vector2u select;
  std::string lastFilepath;
  std::vector<sf::Vector2f> paletteCoordinates;
  unsigned int paletteSize = 22;
  unsigned int fontSize    = 16;
  bool drawPalette         = true;
  bool quitting            = false;

 public:
  std::vector<sf::Color> palette;
  std::wstring_view statusLinePrefix;
  std::wstring_view statusLine;

  Context() : palette(128), paletteCoordinates(128) {
    paletteCoordinates['0'] = sf::Vector2f(0, 9);

    for (int i = 1; i <= 9; ++i) {
      paletteCoordinates['0' + i] = sf::Vector2f(0, i - 1);
    }

    // red qaz
    paletteCoordinates['q'] = sf::Vector2f(1, 0);
    paletteCoordinates['a'] = sf::Vector2f(2, 0);
    paletteCoordinates['z'] = sf::Vector2f(3, 0);

    // orange wsx
    paletteCoordinates['w'] = sf::Vector2f(1, 1);
    paletteCoordinates['s'] = sf::Vector2f(2, 1);
    paletteCoordinates['x'] = sf::Vector2f(3, 1);

    // green edc
    paletteCoordinates['e'] = sf::Vector2f(1, 2);
    paletteCoordinates['d'] = sf::Vector2f(2, 2);
    paletteCoordinates['c'] = sf::Vector2f(3, 2);

    // blue rfv
    paletteCoordinates['r'] = sf::Vector2f(1, 3);
    paletteCoordinates['f'] = sf::Vector2f(2, 3);
    paletteCoordinates['v'] = sf::Vector2f(3, 3);

    // purple tgb
    paletteCoordinates['t'] = sf::Vector2f(1, 4);
    paletteCoordinates['g'] = sf::Vector2f(2, 4);
    paletteCoordinates['b'] = sf::Vector2f(3, 4);

    // pink yhn
    paletteCoordinates['y'] = sf::Vector2f(1, 5);
    paletteCoordinates['h'] = sf::Vector2f(2, 5);
    paletteCoordinates['n'] = sf::Vector2f(3, 5);

    // ligth blue ujm
    paletteCoordinates['u'] = sf::Vector2f(1, 6);
    paletteCoordinates['j'] = sf::Vector2f(2, 6);
    paletteCoordinates['m'] = sf::Vector2f(3, 6);

    // red pink ik
    paletteCoordinates['i'] = sf::Vector2f(1, 7);
    paletteCoordinates['k'] = sf::Vector2f(2, 7);

    // green blue olp
    paletteCoordinates['o'] = sf::Vector2f(1, 8);
    paletteCoordinates['l'] = sf::Vector2f(2, 8);
    paletteCoordinates['p'] = sf::Vector2f(1, 9);

    mainFont.loadFromFile("JetBrainsMonoNL-Regular.ttf");
    image.setSmooth(false);
  }

  void quit() { quitting = true; }
  bool isQuitting() const { return quitting; }

  void loadFile(std::string const& filepath) {
    lastFilepath = filepath;
    image.loadFromFile(filepath);
  }

  void newFile(int width, int height) {
    sf::Image buf;
    buf.create(width, height, sf::Color(0));
    image.loadFromImage(buf);
    lastFilepath.clear();
  }

  void saveFile(std::string const& filepath = "") {
    if (!filepath.empty()) {
      lastFilepath = filepath;
    }

    if (!lastFilepath.empty()) {
      sf::Image buf = image.copyToImage();
      buf.saveToFile(lastFilepath);
    }
  }

  void expand(int offset, const std::wstring& direction) {}

  void moveCursor(int dx, int dy) {
    int cx   = (int)cursor.x + dx;
    int cy   = (int)cursor.y + dy;
    cursor.x = std::max(0, std::min((int)image.getSize().x - 1, cx));
    cursor.y = std::max(0, std::min((int)image.getSize().y - 1, cy));
  }

  void dropSelection() {
    select = cursor;
  }

  void replaceColor(int paletteId) {
    sf::Image buf;
    auto xmax = std::max(cursor.x, select.x);
    auto ymax = std::max(cursor.y, select.y);
    auto xmin = std::min(cursor.x, select.x);
    auto ymin = std::min(cursor.y, select.y);
    buf.create(xmax - xmin + 1, ymax - ymin + 1, palette[paletteId]);
    image.update(buf, xmin, ymin);
  }

  void deleteColor() {
    sf::Image buf;
    auto xmax = std::max(cursor.x, select.x);
    auto ymax = std::max(cursor.y, select.y);
    auto xmin = std::min(cursor.x, select.x);
    auto ymin = std::min(cursor.y, select.y);
    buf.create(xmax - xmin + 1, ymax - ymin + 1, sf::Color(0));
    image.update(buf, xmin, ymin);
  }

  void pickUpColor(sf::Uint32 c) {
    sf::Color currentColor = image.copyToImage().getPixel(cursor.x, cursor.y);
    currentColor.a = 255;
    palette[c] = currentColor;
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    sf::View currentView(
        sf::FloatRect(0.f, 0.f, target.getSize().x, target.getSize().y));
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

    if (drawPalette) {
      mainSize.y -= 4 * paletteSize;
      float paletteShift = (mainSize.x - 10.5f * paletteSize) / 2.0f;
      for (int i = 0; i < palette.size(); ++i) {
        if (!isPaletteKey(i)) {
          continue;
        }
        sf::RectangleShape paletteRectangle;
        sf::Vector2f palettePos;
        palettePos.x = paletteCoordinates[i].y * paletteSize + paletteShift +
                       paletteCoordinates[i].x * paletteSize / 2;
        palettePos.y = mainSize.y + paletteCoordinates[i].x * paletteSize;
        paletteRectangle.setPosition(palettePos);
        paletteRectangle.setSize(sf::Vector2f(paletteSize, paletteSize));
        paletteRectangle.setFillColor(palette[i]);
        target.draw(paletteRectangle);
      }
    }

    sf::View imageView;
    sf::Vector2f viewSize(image.getSize().x, 0.f);
    viewSize.y = viewSize.x * mainSize.y / mainSize.x;
    if (viewSize.y < image.getSize().y) {
      float tmp = image.getSize().y / viewSize.y;
      viewSize.y *= tmp;
      viewSize.x *= tmp;
    }
    imageView.setSize(viewSize);
    imageView.setCenter(sf::Vector2f(image.getSize().x, image.getSize().y) /
                        2.0f);
    imageView.setViewport(
        sf::FloatRect(0.f, 0.f, 1.f, (float)mainSize.y / target.getSize().y));
    target.setView(imageView);

    backgroundRect.setPosition(0.0f, 0.0f);
    backgroundRect.setSize(sf::Vector2f(image.getSize().x, image.getSize().y));
    backgroundRect.setFillColor(sf::Color::Magenta);
    target.draw(backgroundRect);

    sf::Sprite sprite(image);
    target.draw(sprite);

    auto xmax = std::max(cursor.x, select.x);
    auto ymax = std::max(cursor.y, select.y);
    auto xmin = std::min(cursor.x, select.x);
    auto ymin = std::min(cursor.y, select.y);

    backgroundRect.setPosition(xmin, ymin);
    backgroundRect.setSize(sf::Vector2f(xmax - xmin + 1, ymax - ymin + 1));
    backgroundRect.setFillColor(sf::Color(0xcc00ffff));
    target.draw(backgroundRect);

    sprite.setPosition(xmin, ymin);
    sprite.setTextureRect(sf::IntRect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1));
    sprite.setColor(sf::Color(0xccccffff));
    target.draw(sprite);

    backgroundRect.setPosition(cursor.x, cursor.y);
    backgroundRect.setSize(sf::Vector2f(1, 1));
    backgroundRect.setFillColor(sf::Color::Magenta);
    target.draw(backgroundRect);

    sprite.setPosition(cursor.x, cursor.y);
    sprite.setTextureRect(sf::IntRect(cursor.x, cursor.y, 1, 1));
    sprite.setColor(sf::Color::White);
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

  void putColor(std::string const& key,std::string const& mode, int32_t fst, int32_t snd, int32_t thd){
      int indPal=0;

      if ('0' <= key[0] && key[0] <= '9')
        indPal = key[0];
      else
        indPal = key[0];

      if(mode=="rgb") {
        if(0<=fst && fst<=255 && 0<=snd && snd<=255 && 0<=thd && thd<=255) {
          // fst - red, snd - green, thd - blue
          palette[indPal] = sf::Color(fst, snd, thd);
        }
      }else{
        if(fst>360 || fst<0 || snd>100 || snd<0 || thd>100 || thd<0){
          return;
        }

        float s = snd/100, v = thd/100;
        float C = s*v;
        float X = C*(1-std::abs(fmod(fst/60.0, 2)-1)), m = v-C;
        float r,g,b;
        if(fst >= 0 && fst < 60){
          r = C,g = X,b = 0;
        }
        else if(fst >= 60 && fst < 120){
          r = X,g = C,b = 0;
        }
        else if(fst >= 120 && fst < 180){
          r = 0,g = C,b = X;
        }
        else if(fst >= 180 && fst < 240){
          r = 0,g = X,b = C;
        }
        else if(fst >= 240 && fst < 300){
          r = X,g = 0,b = C;
        }
        else{
          r = C,g = 0,b = X;
        }
        int R = (r+m)*255,G = (g+m)*255, B=(b+m)*255;
        palette[indPal] = sf::Color(R, G, B);
      }
  }
};

#endif  // Context_hpp_INCLUDED
