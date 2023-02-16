#pragma once

#include <functional>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

// Forward declaration(s)
class Road;

class Node : public sf::Drawable, public sf::Transformable
{
public:
	// Public constructor(s)
	explicit Node(sf::RenderWindow& window);

	// Public method(s)
	void handleEvent(sf::Event& event);
	bool addConnection(Node* node, Road* road);
	bool removeConnection(Node* node);
	void highlightSharedRoads(Node* node);
	Road* findSharedRoad(Node* node);

	inline void setOnMouseButtonPress(std::function<void(Node*)> f) { mOnMouseButtonPress = f; }
	inline void setOnMouseButtonRelease(std::function<void(Node*)> f) { mOnMouseButtonRelease = f; }
	[[nodiscard]] inline const std::vector<Node*>& getConnections() const noexcept { return mConnections; }
	[[nodiscard]] inline const std::vector<Road*>& getConnectingRoads() const noexcept { return mConnectingRoads; }
private:
	// Private method(s)
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// Private member(s)
	sf::CircleShape            mCircle;
	std::function<void(Node*)> mOnMouseButtonPress;
	std::function<void(Node*)> mOnMouseButtonRelease;
	std::vector<Node*>         mConnections;
	std::vector<Road*>         mConnectingRoads;
	bool                       mIsHovered;

	sf::RenderWindow&          mWindow;
};