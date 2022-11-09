#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
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

  // black-gray-white 1234567890
  for (int i = 1; i <= 10; ++i) {
    int v = 255 * (11 - i) / 10;

    context.palette['0' + i] = sf::Color(v, v, v);
  }

  // red qaz
  context.palette['q'] = sf::Color(204, 0, 0);
  context.palette['a'] = sf::Color(255, 0, 0);
  context.palette['z'] = sf::Color(255, 102, 102);

  // orange wsx
  context.palette['w'] = sf::Color(204, 102, 0);
  context.palette['s'] = sf::Color(255, 153, 51);
  context.palette['x'] = sf::Color(255, 204, 153);

  // green edc
  context.palette['e'] = sf::Color(0, 204, 0);
  context.palette['d'] = sf::Color(51, 255, 51);
  context.palette['c'] = sf::Color(153, 255, 153);

  // blue rfv
  context.palette['r'] = sf::Color(0, 0, 204);
  context.palette['f'] = sf::Color(51, 51, 255);
  context.palette['v'] = sf::Color(153, 153, 255);

  // purple tgb
  context.palette['t'] = sf::Color(102, 0, 204);
  context.palette['g'] = sf::Color(153, 51, 255);
  context.palette['b'] = sf::Color(178, 102, 255);

  // pink yhn
  context.palette['y'] = sf::Color(204, 0, 204);
  context.palette['h'] = sf::Color(255, 51, 255);
  context.palette['n'] = sf::Color(178, 153, 255);

  // ligth blue ujm
  context.palette['u'] = sf::Color(0, 204, 204);
  context.palette['j'] = sf::Color(51, 255, 255);
  context.palette['m'] = sf::Color(153, 255, 255);

  // red pink ik
  context.palette['i'] = sf::Color(255, 0, 127);
  context.palette['k'] = sf::Color(255, 102, 178);

  // green blue olp
  context.palette['o'] = sf::Color(51, 255, 153);
  context.palette['l'] = sf::Color(153, 255, 204);
  context.palette['p'] = sf::Color(0, 153, 76);

  window.clear(sf::Color::White);
  window.draw(context);
  window.display();

  sf::Event evt;
  while (!context.isQuitting() && window.waitEvent(evt)) {
    switch (evt.type) {
      case sf::Event::Closed: context.quit(); break;

      case sf::Event::TextEntered:
        handler.handleCharacter(context, evt.text.unicode);

      case sf::Event::Resized:
        window.clear(sf::Color::White);
        window.draw(context);
        window.display();
        break;

      default: break;
    }
  }

  window.close();
  return 0;
}
