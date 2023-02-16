#include <SFML/Graphics.hpp>

#include "Map.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Road Builder", sf::Style::Default, sf::ContextSettings(0, 0, 4, 4, 5));
    Map map(window);

    sf::Time timeElapsed;
    sf::Clock clock;
    while (window.isOpen()) {
        timeElapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            map.handleEvent(event);
        }

        map.update(timeElapsed);

        window.clear();
        window.draw(map);
        window.display();
    }

    return 0;
}