#include <string>
#include <SFML\Graphics.hpp>

#define BLACK 1
#define WHITE 0

#pragma once
class Display
{
private:
	sf::RenderWindow *window;
	sf::View view;

	sf::Vector2f textPos;
	sf::Color textColor = sf::Color::White;
	sf::Font font;
	int textSize = 1;
	float zoomFactor = 1;
public:

	Display(sf::RenderWindow *window, float zoomFactor = 1) {
		this->window = window;
		font.loadFromFile("Pixeled.ttf");
		this->zoomFactor = zoomFactor;
	}
	Display() {

	}
	~Display();

	void begin() {
		view = window->getDefaultView();
		view.zoom(1.0f / zoomFactor);
		view.setCenter(84 / 2, 48 / 2);
		window->setView(view);		
	}

	void setContrast(int contrast) {

	}

	void setTextSize(int size) {
		textSize = size;
	}

	void setCursor(int x, int y) {
		textPos = sf::Vector2f(x, y);
	}

	void println(std::string text) {
		sf::Text txt(text, font, 5 * textSize);
		txt.setPosition(textPos);
		txt.setFillColor(textColor);
		window->draw(txt);
	}

	void drawLine(int x, int y, int x1, int y1, int color) {
		//sf::Vertex line[] =
		//{
		//	sf::Vertex(sf::Vector2f(x, y)),
		//	sf::Vertex(sf::Vector2f(x+1, y+1)),
		//	sf::Vertex(sf::Vector2f(x1 + 1, y1 + 1)),
		//	sf::Vertex(sf::Vector2f(x1, y1))
		//	
		//};
		//
		//window->draw(line, 4, sf::PrimitiveType::Quads);

		sf::RectangleShape line(sf::Vector2f(x1-x +1, y1-y+1));
		line.setPosition(x, y);
		line.setFillColor(sf::Color(255 * color, 255 * color, 255 * color, 255));
		
		line.setOrigin(0.5f, 0.5f);
		window->draw(line);
	}

	void drawRect(int x, int y, int width, int height, int color) {
		sf::RectangleShape rectangle(sf::Vector2f(width-1, height-2));
		rectangle.setPosition(x+1, y+1);
		rectangle.setFillColor(sf::Color::Transparent);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(1);

		rectangle.setOrigin(0.5f, 0.5f);
		
		window->draw(rectangle);
	}

	void fillRect(int x, int y, int width, int height, int color) {
		sf::RectangleShape rectangle(sf::Vector2f(width, height));
		rectangle.setPosition(x, y);
		rectangle.setFillColor(sf::Color::White);

		rectangle.setOrigin(0.5f, 0.5f);
		window->draw(rectangle);
	}

	void drawFastHLine(int x, int y, int width, int color) {
		sf::RectangleShape line(sf::Vector2f(width, 1));
		line.setPosition(x, y);

		line.setOrigin(0.5f, 0.5f);
		window->draw(line);
	}

	void drawFastVLine(int x, int y, int height, int color) {
		sf::RectangleShape line(sf::Vector2f(1, height));
		line.setPosition(x, y);

		line.setOrigin(0.5f, 0.5f);
		window->draw(line);
	}

	void fillCircle(int x, int y, int radius, int color) {
		sf::CircleShape circle(radius);
		circle.setPosition(x, y);
		circle.setOrigin(radius, radius);
		circle.setPointCount(8);
		circle.setFillColor(sf::Color(255 * color, 255 * color, 255 * color, 255));

		window->draw(circle);
	}

	void setTextColor(int color) {
		textColor = sf::Color(255 * color, 255 * color, 255 * color, 255);
	}

	void display() {
		window->display();
	}

	void clearDisplay() {
		window->clear();
	}
};

