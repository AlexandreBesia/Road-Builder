#include "Node.h"

#include <SFML/Window/Event.hpp>

#include "Misc.h"
#include "Road.h"

// Public constructor(s)
Node::Node(sf::RenderWindow& window)
	: mCircle(3.f, 10)
	, mIsHovered(false)
	, mWindow(window)
{
	mCircle.setOrigin(mCircle.getRadius(), mCircle.getRadius());
}

// Public method(s)
void Node::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		const sf::Vector2f MOUSE_POS = mWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
		mIsHovered = Math::magnitude(MOUSE_POS - getPosition()) < mCircle.getRadius();
		mCircle.setFillColor(mIsHovered ? sf::Color::White : sf::Color(140, 140, 140));
	}
	else if (mIsHovered) {
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
			mOnMouseButtonPress(this);
		}
		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left) {
			mOnMouseButtonRelease(this);
		}
	}
}

bool Node::addConnection(Node* node, Road* road)
{
	auto connection = std::find(mConnections.begin(), mConnections.end(), node);
	if (connection == mConnections.end()) {
		mConnections.push_back(node);
		mConnectingRoads.push_back(road);
		return true;
	}

	return false;
}

bool Node::removeConnection(Node* node)
{
	auto connection = std::find(mConnections.begin(), mConnections.end(), node);
	if (connection != mConnections.end()) {
		mConnections.erase(connection);
		return true;
	}

	return false;
}

void Node::highlightSharedRoads(Node* node)
{
	Road* sharedRoad = findSharedRoad(node);
	if (sharedRoad) {
		sharedRoad->highlight(true);
	}
}

Road* Node::findSharedRoad(Node* node)
{
	for (Road* road : mConnectingRoads) {
		for (Road* nodeRoad : node->mConnectingRoads) {
			if (road == nodeRoad) {
				return road;
			}
		}
	}

	return nullptr;
}

// Private method(s)
void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mCircle, states);
}