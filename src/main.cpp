#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window.hpp>
#include "Context.hpp"
#include "Handler.hpp"

/**  Put this file where your app was built,
 *  it might be in your cmake-build directory
 **/
std::string defaultFile = "tile.png";

int main(int argc, char** argv) {
  sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

  Handler handler;
  Context context;
  if (argc > 1) {
    context.loadFile(argv[1]);
  } else {
    context.loadFile(defaultFile);
  }

  // black-gray-white 1234567890
  context.palette[0] = sf::Color::Black;
  for (int i = 1; i <= 9; ++i) {
    int v              = 255 * (11 - i) / 10;
    context.palette[i] = sf::Color(v, v, v);
  }

  // red qaz
  context.palette['q' - 'a' + 10] = sf::Color(204, 0, 0);
  context.palette['a' - 'a' + 10] = sf::Color(255, 0, 0);
  context.palette['z' - 'a' + 10] = sf::Color(255, 102, 102);

  // orange wsx
  context.palette['w' - 'a' + 10] = sf::Color(204, 102, 0);
  context.palette['s' - 'a' + 10] = sf::Color(255, 153, 51);
  context.palette['x' - 'a' + 10] = sf::Color(255, 204, 153);

  // green edc
  context.palette['e' - 'a' + 10] = sf::Color(0, 204, 0);
  context.palette['d' - 'a' + 10] = sf::Color(51, 255, 51);
  context.palette['c' - 'a' + 10] = sf::Color(153, 255, 153);

  // blue rfv
  context.palette['r' - 'a' + 10] = sf::Color(0, 0, 204);
  context.palette['f' - 'a' + 10] = sf::Color(51, 51, 255);
  context.palette['v' - 'a' + 10] = sf::Color(153, 153, 255);

  // purple tgb
  context.palette['t' - 'a' + 10] = sf::Color(102, 0, 204);
  context.palette['g' - 'a' + 10] = sf::Color(153, 51, 255);
  context.palette['b' - 'a' + 10] = sf::Color(178, 102, 255);

  // pink yhn
  context.palette['y' - 'a' + 10] = sf::Color(204, 0, 204);
  context.palette['h' - 'a' + 10] = sf::Color(255, 51, 255);
  context.palette['n' - 'a' + 10] = sf::Color(178, 153, 255);

  // ligth blue ujm
  context.palette['u' - 'a' + 10] = sf::Color(0, 204, 204);
  context.palette['j' - 'a' + 10] = sf::Color(51, 255, 255);
  context.palette['m' - 'a' + 10] = sf::Color(153, 255, 255);

  // red pink ik
  context.palette['i' - 'a' + 10] = sf::Color(255, 0, 127);
  context.palette['k' - 'a' + 10] = sf::Color(255, 102, 178);

  // green blue olp
  context.palette['o' - 'a' + 10] = sf::Color(51, 255, 153);
  context.palette['l' - 'a' + 10] = sf::Color(153, 255, 204);
  context.palette['p' - 'a' + 10] = sf::Color(0, 153, 76);

  window.clear(sf::Color::White);
  window.draw(context);
  window.display();

  sf::Event evt;
  while (!context.isQuitting() && window.waitEvent(evt)) {
    switch (evt.type) {
      case sf::Event::Closed: context.quit(); break;

      case sf::Event::TextEntered:
        handler.handleCharacter(context, evt.text.unicode);
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
