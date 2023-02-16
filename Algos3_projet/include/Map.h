#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "RoadBuilder.h"

class Map : public sf::Drawable, public sf::Transformable
{
public:
	// Public constructor(s)
	explicit Map(sf::RenderWindow& window);

	// Public method(s)
	void handleEvent(sf::Event& event);
	void update(sf::Time& dt);
private:
	// Private method(s)
	void enableMovement(sf::Keyboard::Key key, bool flag);
	void zoomAt(const sf::Vector2i& pixel, int wheelDelta);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// Private struct(s)
	struct Movement {
		float speed;
		bool up;
		bool down;
		bool left;
		bool right;

		Movement();
	};

private:
	// Private member(s)
	sf::Sprite        mSprite;
	RoadBuilder       mRoadBuilder;
	sf::Texture       mTexture;
	Movement          mMovement;
	float             mZoomFactor;

	sf::RenderWindow& mWindow;
};