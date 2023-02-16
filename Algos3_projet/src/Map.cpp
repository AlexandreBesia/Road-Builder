#include "Map.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Misc.h"

Map::Movement::Movement()
	: speed(350.f)
	, up(false)
	, down(false)
	, left(false)
	, right(false)
{
}

// Public constructor(s)
Map::Map(sf::RenderWindow& window)
	: mSprite()
	, mRoadBuilder(window)
	, mTexture()
	, mMovement()
	, mZoomFactor(1.25f)
	, mWindow(window)
{
	mTexture.loadFromFile("ressources/textures/gtav_map_road.jpg");
	mSprite.setTexture(mTexture);

	sf::View view = window.getView();
	view.setCenter(sf::Vector2f(mTexture.getSize()) / 2.f);
	view.zoom(mTexture.getSize().y / window.getSize().y);
	window.setView(view);
}

// Public method(s)
void Map::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel) {
		zoomAt(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y), event.mouseWheelScroll.delta);
	}
	else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Key::Escape) {
			mWindow.close();
		}
		else {
			enableMovement(event.key.code, true);
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		enableMovement(event.key.code, false);
	}

	mRoadBuilder.handleEvent(event);
}

void Map::update(sf::Time& dt)
{
	sf::Vector2f offset;
	if (mMovement.up) {
		offset.y -= mMovement.speed;
	}
	if (mMovement.down) {
		offset.y += mMovement.speed;
	}
	if (mMovement.left) {
		offset.x -= mMovement.speed;
	}
	if (mMovement.right) {
		offset.x += mMovement.speed;
	}

	if (offset.x != 0.f || offset.y != 0.f) {
		sf::View view = mWindow.getView();
		view.move(offset * (view.getSize().x / mWindow.getSize().x) * dt.asSeconds());
		mWindow.setView(view);
	}
}

// Private method(s)
void Map::enableMovement(sf::Keyboard::Key key, bool flag)
{
	switch (key) {
	case sf::Keyboard::Key::W: mMovement.up = flag; break;
	case sf::Keyboard::Key::S: mMovement.down = flag; break;
	case sf::Keyboard::Key::A: mMovement.left = flag; break;
	case sf::Keyboard::Key::D: mMovement.right = flag;
	}
}

void Map::zoomAt(const sf::Vector2i& pixel, int wheelDelta)
{
	const sf::Vector2f BEFORE_COORD = mWindow.mapPixelToCoords(pixel);
	sf::View view = mWindow.getView();
	view.zoom((wheelDelta > 0) ? 1.f / mZoomFactor : mZoomFactor);
	mWindow.setView(view);

	const sf::Vector2f AFTER_COORD = mWindow.mapPixelToCoords(pixel);
	view.move(BEFORE_COORD - AFTER_COORD);
	mWindow.setView(view);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(mSprite, states);
	target.draw(mRoadBuilder, states);
}