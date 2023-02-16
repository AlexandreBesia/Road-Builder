#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Road.h"
#include "Node.h"

class RoadBuilder : public sf::Drawable
{
public:
	// Public constructor(s)
	explicit RoadBuilder(sf::RenderWindow& window);

	// Public method(s)
	void handleEvent(sf::Event& event);
	void highlightShortestPath(const sf::Vector2f& startPos);
	void findShortestPath(Node* node);
private:
	// Private method(s)
	void createNode(const sf::Vector2f& pos);
	void createRoad();
	void connectRoad();
	void updateRoadWeights();
	Node* findClosestNode(const sf::Vector2f& pos);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// Private member(s)
	std::vector<std::unique_ptr<Road>> mRoads;
	std::vector<std::unique_ptr<Node>> mNodes;
	std::unordered_map<Node*, bool>    mVisitedNodes;

	Node*                              mRoadStart;
	Node*                              mRoadEnd;

	sf::Font                           mFont;
	sf::RenderWindow&                  mWindow;
};