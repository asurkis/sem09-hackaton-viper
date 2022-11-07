#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main(int argc, char** argv) {
  sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
  sf::Texture texture;
  if (argc > 1) {
    texture.loadFromFile(argv[1]);
  } else {
  }
  sf::Sprite sprite(texture);

  int scale = 8;
  int cursorX = 0;
  int cursorY = 0;

  while (window.isOpen()) {
    sf::Event evt;
    while (window.pollEvent(evt)) {
      switch (evt.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::TextEntered:
          switch (evt.text.unicode) {
            case 'h':
              --cursorX;
              break;
            case 'j':
              ++cursorY;
              break;
            case 'k':
              --cursorY;
              break;
            case 'l':
              ++cursorX;
              break;
          }
          break;
          default: break;
      }
    }
    if (cursorX < 0) {
      cursorX = 0;
    }
    if (cursorX >= texture.getSize().x) {
      cursorX = texture.getSize().x - 1;
    }
    if (cursorY < 0) {
      cursorY = 0;
    }
    if (cursorY >= texture.getSize().y) {
      cursorY = texture.getSize().y - 1;
    }

    window.clear(sf::Color::White);

    sf::Vector2u pos = (window.getSize() - 8u * texture.getSize()) / 2u;
    sprite.setScale(8, 8);
    sprite.setPosition((float)pos.x, (float)pos.y);
    window.draw(sprite);

    sf::RectangleShape wrapAround;
    wrapAround.setPosition(pos.x + 8 * cursorX, pos.y + 8 * cursorY);
    wrapAround.setSize(sf::Vector2f(8, 8));
    wrapAround.setFillColor(sf::Color(0, 0, 0, 0));
    wrapAround.setOutlineColor(sf::Color::Cyan);
    wrapAround.setOutlineThickness(2.0f);
    window.draw(wrapAround);

    window.display();
  }
  return 0;
}
