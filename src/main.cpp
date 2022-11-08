#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Context.hpp"
#include "Handler.hpp"

int main(int argc, char** argv) {
  sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
  Handler handler;
  Context context;
  if (argc > 1) {
    context.loadFile(argv[1]);
  }

  context.palette[0] = sf::Color::Black;
  context.palette[1] = sf::Color::White;
  for (int i = 1; i <= 9; ++i) {
    int v = 255 * (11 - i) / 10;
    context.palette[i] = sf::Color(v, v, v);
  }

  sf::Event evt;
  while (!context.isQuitting() && window.waitEvent(evt)) {
    switch (evt.type) {
      case sf::Event::Closed:
        context.quit();
        break;

      case sf::Event::KeyPressed:
        handler.handleKey(context, evt.key);
        break;

      case sf::Event::TextEntered:
        handler.handleCharacter(context, evt.text.unicode);
        break;

      default:
        break;
    }

    window.clear(sf::Color::White);
    context.redraw(window);
    window.display();
  }

  window.close();
  return 0;
}
