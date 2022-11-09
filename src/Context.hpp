#ifndef Context_hpp_INCLUDED
#define Context_hpp_INCLUDED

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string_view>
#include <vector>

inline bool isPaletteKey(sf::Uint32 c) {
  return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z');
}

inline sf::Color hsv(float h, float s, float v) {
  h     = h / 60.0f;
  int i = (int)h;
  if (i < 0) {
    --i;
  }
  float frac = h - i;

  i = (i % 6 + 6) % 6;

  sf::Vector3f fullSat;
  switch (i) {
    case 0: fullSat = sf::Vector3f(1.0f, frac, 0.0f); break;
    case 1: fullSat = sf::Vector3f(1.0f - frac, 1.0f, 0.0f); break;
    case 2: fullSat = sf::Vector3f(0.0f, 1.0f, frac); break;
    case 3: fullSat = sf::Vector3f(0.0f, 1.0f - frac, 1.0f); break;
    case 4: fullSat = sf::Vector3f(frac, 0.0f, 1.0f); break;
    case 5: fullSat = sf::Vector3f(1.0f, 0.0f, 1.0f - frac); break;
  }

  sf::Vector3f rgb =
      v * s * fullSat + v * (1.0f - s) * sf::Vector3f(1.0f, 1.0f, 1.0f);
  return sf::Color(255 * rgb.x, 255 * rgb.y, 255 * rgb.z);
}

enum SelectionType {
  ST_RECTANGLE,
  ST_LINE,
  // ST_ELLIPSE,
};

class Context : public sf::Drawable {
  std::vector<sf::Vector2f> paletteCoordinates;
  std::vector<sf::Vector2u> selectedPixels;
  std::string lastFilepath;
  sf::Texture image;
  sf::Font mainFont;
  sf::Vector2u cursor;
  sf::Vector2u select;
  sf::Uint32 prev_c;
  unsigned int paletteSize = 22;
  unsigned int gridSize    = 4;
  unsigned int fontSize    = 16;
  bool drawPalette         = true;
  bool quitting            = false;
  SelectionType selectionType = ST_RECTANGLE;

  void updateSelectionRectangle() {
    int x1 = select.x;
    int x2 = cursor.x;
    int dx = x2 < x1 ? -1 : 1;
    x2 += dx;

    int y1 = select.y;
    int y2 = cursor.y;
    int dy = y2 < y1 ? -1 : 1;
    y2 += dy;

    for (int y = y1; y != y2; y += dy) {
      for (int x = x1; x != x2; x += dx) {
        selectedPixels.push_back(sf::Vector2u(x, y));
      }
    }
  }

  void updateSelectionLine() {
    // Линия: (x - x1) * kx + (y - y1) * ky = 0
    // (x2 - x1) * kx = -(y2 - y1) * ky
    // kx = y1 - y2
    // ky = x2 - x1
    long long x1 = select.x;
    long long x2 = cursor.x;
    long long dx = x2 < x1 ? -1 : 1;

    long long y1 = select.y;
    long long y2 = cursor.y;
    long long dy = y2 < y1 ? -1 : 1;

    long long kx = y1 - y2;
    long long ky = x2 - x1;

    long long x = x1;
    long long y = y1;
    while (x != x2 || y != y2) {
      selectedPixels.push_back(sf::Vector2u(x, y));
      long long xns[] = {x, x + dx, x + dx};
      long long yns[] = {y + dy, y + dy, y};
      long long bestD = 0x7fffffff;
      long long bestX = 0;
      long long bestY = 0;
      for (int i = 0; i < 3; ++i) {
        long long d = std::abs(kx * (xns[i] - x1) + ky * (yns[i] - y1));
        if (d < bestD) {
          bestD = d;
          bestX = xns[i];
          bestY = yns[i];
        }
      }
      x = bestX;
      y = bestY;
    }
    selectedPixels.push_back(sf::Vector2u(x2, y2));
  }

 public:
  std::vector<sf::Color> palette;
  std::wstring_view statusLinePrefix;
  std::wstring_view statusLine;

  Context() : palette(128), paletteCoordinates(128), prev_c('0') {
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

    mainFont.loadFromFile("JetBrainsMono-Regular.ttf");
    image.setSmooth(false);
  }

  void quit() { quitting = true; }
  bool isQuitting() const { return quitting; }

  void loadFile(std::string const& filepath) {
    lastFilepath = filepath;
    image.loadFromFile(filepath);

    cursor = sf::Vector2u();
    select = sf::Vector2u();
    updateSelection();
  }

  void newFile(int width, int height) {
    sf::Image buf;
    buf.create(width, height, sf::Color(0));
    image.loadFromImage(buf);
    lastFilepath.clear();

    cursor = sf::Vector2u();
    select = sf::Vector2u();
    updateSelection();
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

  void swapCursor() {
    std::swap(cursor, select);
    updateSelection();
  }

  void setSelectionType(SelectionType st) {
    selectionType = st;
    updateSelection();
  }

  void updateSelection() {
    selectedPixels.clear();
    switch (selectionType) {
      case ST_RECTANGLE: updateSelectionRectangle(); break;
      case ST_LINE: updateSelectionLine(); break;
    }
  }

  void dropSelection() {
    select = cursor;
    updateSelection();
  }

  void replaceColorRgb(sf::Color color) {
    sf::Image buf;
    buf.create(1, 1, color);
    for (auto const& [x, y] : selectedPixels) {
      image.update(buf, x, y);
    }
  }

  void replaceColor(int paletteId) {
    replaceColorRgb(palette[paletteId]);
    prev_c = paletteId;
  }

  void replacePrevColor() { replaceColor(prev_c); }

  void deleteColor() { replaceColorRgb(sf::Color(0)); }

  void changePalette(int paletteId, sf::Color rgb) { palette[paletteId] = rgb; }

  void changePaletteHsv(int paletteId, float h, float s, float v) {
    sf::Color color;
  }

  void pickUpColor(sf::Uint32 c) {
    sf::Color currentColor = image.copyToImage().getPixel(cursor.x, cursor.y);
    currentColor.a         = 255;
    changePalette(c, currentColor);
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    sf::View currentView(
        sf::FloatRect(0.f, 0.f, target.getSize().x, target.getSize().y));
    target.setView(currentView);

    // CommandLine and text
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

    // Palette
    if (drawPalette) {
      mainSize.y -= 4 * paletteSize;
      float paletteShift = (mainSize.x - 10.5f * paletteSize) / 2.0f;
      sf::RectangleShape currentColor;
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
        if (i == prev_c) {
          currentColor = paletteRectangle;
        }
        paletteRectangle.setOutlineThickness(1.0f);
        paletteRectangle.setOutlineColor(sf::Color::Black);
        target.draw(paletteRectangle);
        paletteRectangle.setOutlineThickness(-1.0f);
        target.draw(paletteRectangle);
      }
      currentColor.setOutlineThickness(2.0f);
      currentColor.setOutlineColor(sf::Color::Cyan);
      target.draw(currentColor);
      currentColor.setOutlineThickness(-2.0f);
      currentColor.setOutlineColor(sf::Color::Red);
      target.draw(currentColor);
    }

    // Image
    sf::View imageView;
    sf::Vector2f viewSize(image.getSize().x, 0.f);
    viewSize.y = viewSize.x * mainSize.y / mainSize.x;
    float pixelSize = 1.0f*mainSize.x / image.getSize().x;
    if (viewSize.y < image.getSize().y) {
      float tmp = image.getSize().y / viewSize.y;
      viewSize.y *= tmp;
      viewSize.x *= tmp;
      pixelSize = 1.0f*mainSize.y / image.getSize().y;
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

    backgroundRect.setSize(sf::Vector2f(1, 1));
    backgroundRect.setFillColor(sf::Color(0xcc00ffff));
    for (size_t i = 0; i + 1 < selectedPixels.size(); ++i) {
      auto const& [x, y] = selectedPixels[i];
      backgroundRect.setPosition(x, y);
      target.draw(backgroundRect);
      sprite.setPosition(x, y);
      sprite.setTextureRect(sf::IntRect(x, y, 1, 1));
      sprite.setColor(sf::Color(0xccccffff));
      target.draw(sprite);
    }

    // Grid
    target.setView(currentView);

    sf::Vector2f imageTopLeftPos((mainSize.x - pixelSize * image.getSize().x) / 2.0f - 1,
                                 (mainSize.y - pixelSize * image.getSize().y) / 2.0f - 1);
    sf::RectangleShape imageFrame;
    imageFrame.setPosition(imageTopLeftPos);
    imageFrame.setSize(sf::Vector2f(pixelSize * image.getSize().x + 2, 
                                    pixelSize * image.getSize().y + 2));
    imageFrame.setFillColor(sf::Color::Transparent);
    imageFrame.setOutlineThickness(-2.0f);
    imageFrame.setOutlineColor(sf::Color::Black);
    target.draw(imageFrame);

    sf::Vector2f currentLinePos = imageTopLeftPos;
    for (unsigned int i = 0; i <= image.getSize().y; i += gridSize) {
      sf::RectangleShape horizontalLine;
      horizontalLine.setPosition(currentLinePos);
      horizontalLine.setSize(sf::Vector2f(pixelSize * image.getSize().x, 2));
      horizontalLine.setFillColor(sf::Color::Black);
      target.draw(horizontalLine);
      currentLinePos.y += pixelSize * gridSize; 
    }

    currentLinePos = imageTopLeftPos;
    for (unsigned int i = 0; i <= image.getSize().x; i += gridSize) {
      sf::RectangleShape verticalLine;
      verticalLine.setPosition(currentLinePos);
      verticalLine.setSize(sf::Vector2f(2, pixelSize * image.getSize().y));
      verticalLine.setFillColor(sf::Color::Black);
      target.draw(verticalLine);
      currentLinePos.x += pixelSize * gridSize; 
    }

    // Cursor
    sf::RectangleShape wrapAround;
    wrapAround.setPosition(imageTopLeftPos +
                           sf::Vector2f(pixelSize * cursor.x, pixelSize * cursor.y));
    wrapAround.setSize(sf::Vector2f(pixelSize, pixelSize));
    wrapAround.setFillColor(sf::Color(0, 0, 0, 0));
    wrapAround.setOutlineColor(sf::Color::Cyan);
    wrapAround.setOutlineThickness(2.0f);
    target.draw(wrapAround);
    wrapAround.setOutlineColor(sf::Color::Red);
    wrapAround.setOutlineThickness(-2.0f);
    target.draw(wrapAround);
  }
};

#endif  // Context_hpp_INCLUDED
