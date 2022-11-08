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

  sf::Event evt;
  while (window.isOpen() && window.waitEvent(evt)) {
    switch (evt.type) {
      case sf::Event::Closed:
        window.close();
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
  return 0;
}
