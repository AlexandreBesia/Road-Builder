#include "Road.h"

#include <iostream>

#include "Misc.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

// Static member(s)
const float Road::LINE_WIDTH = 1.5f;

// Public constructor(s)
Road::Road(sf::Font& font, sf::RenderWindow& window)
	: mLine(sf::Vector2f(LINE_WIDTH, 0.f))
	, mWeight(1)
	, mWeightText(std::to_string(mWeight), font, 10)
	, mIsDrawingLine(true)
	, mIsHovered(true)
	, mWindow(window)
{
	mWeightText.setFillColor(sf::Color(228, 228, 228));
}

// Public method(s)
void Road::handleEvent(sf::Event& event)
{
	if (!mIsDrawingLine) {
		if (event.type == sf::Event::MouseMoved) {
			const sf::Vector2f MOUSE_POS = mWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			mIsHovered = Math::angle(getPosition() - MOUSE_POS, mEndPos - MOUSE_POS) > Math::radians(172.f);
			mLine.setFillColor(mIsHovered ? sf::Color::White : sf::Color(92, 92, 92));
		}
		else if (mIsHovered && event.type == sf::Event::KeyPressed) {
			if (event.key.code >= sf::Keyboard::Key::Num1 && event.key.code <= sf::Keyboard::Key::Num9) {
				setWeight(event.key.code - sf::Keyboard::Key::Num0);
			}
		}
	}
	else {
		if (event.type == sf::Event::MouseMoved) {
			setEndPoint(mWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));
		}
		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			mIsDrawingLine = false;
		}
	}
}

void Road::highlight(bool flag)
{
	mLine.setFillColor(flag ? sf::Color::Green : sf::Color::Red);
}

void Road::setEndPoint(const sf::Vector2f& pos)
{
	const sf::Vector2f DISTANCE = pos - getPosition();

	mLine.setSize(sf::Vector2f(LINE_WIDTH, Math::magnitude(DISTANCE)));
	mLine.setRotation(Math::degrees(atan2(DISTANCE.y, DISTANCE.x)) - 90.f);
	mEndPos = pos;

	mWeightText.setPosition(DISTANCE / 2.f);
}

void Road::setWeight(int32_t weight)
{
	mWeight = weight;
	mWeightText.setString(std::to_string(mWeight));
}

// Private method(s)
void Road::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mLine, states);
	if (mIsHovered) {
		target.draw(mWeightText, states);
	}
}