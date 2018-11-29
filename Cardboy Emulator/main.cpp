#include "Display.h"
#include "cardboy.h"
#include <functional>

#define zoomFactor 4

sf::Clock clock_;

void wait(int time) {
	int t = clock_.getElapsedTime().asMilliseconds();
	while (clock_.getElapsedTime().asMilliseconds() - t < time) {
		;
	}
}

int millis() {
	return clock_.getElapsedTime().asMilliseconds();
}

void main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	settings.depthBits = 1;
	settings.attributeFlags = sf::ContextSettings::Default;
	sf::RenderWindow window(sf::VideoMode(84* zoomFactor + zoomFactor, 48* zoomFactor + zoomFactor, 1), "CardBoy", 1, settings);
	int keypressed = 0;

	CardBoy cardboy(&window, zoomFactor, &keypressed);

	cardboy.delay = wait;
	cardboy.millis = millis;

	clock_.restart();
	cardboy.setup();
	sf::Event event;

	while (window.isOpen()) {

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				keypressed = 1;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				keypressed = 2;
			}
			else
			{
				keypressed = 0;
			}			
		}

		cardboy.loop();
	}
}