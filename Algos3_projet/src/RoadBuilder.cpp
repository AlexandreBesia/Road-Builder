#include "RoadBuilder.h"

#include <SFML/Window/Event.hpp>

#include "Misc.h"
#include "PriorityQueue.h"

// Public constructor(s)
RoadBuilder::RoadBuilder(sf::RenderWindow& window)
	: mRoadStart(nullptr)
	, mRoadEnd(nullptr)
	, mWindow(window)
{
	mFont.loadFromFile("ressources/fonts/cream-DEMO.ttf");
}

// Public method(s)
void RoadBuilder::handleEvent(sf::Event& event)
{
	for (auto& road : mRoads) {
		road->handleEvent(event);
	}
	for (auto& node : mNodes) {
		node->handleEvent(event);
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
		if (!mRoadStart) {
			const sf::Vector2f MOUSE_POS = mWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			createNode(MOUSE_POS);
		}
		else {
			createRoad();
		}
	}
	else if (mRoadStart && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left) {
		(mRoadEnd) ? connectRoad() : mRoads.pop_back();
		mRoadStart = nullptr;
		mRoadEnd = nullptr;
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
		highlightShortestPath(mWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
	}
}

void RoadBuilder::highlightShortestPath(const sf::Vector2f& startPos)
{
	mVisitedNodes.clear();
	mVisitedNodes.reserve(mNodes.size());
	for (const auto& node : mNodes) {
		mVisitedNodes.emplace(node.get(), false);
	}

	for (auto& road : mRoads) {
		road->highlight(false);
	}

	Node* closestNode = findClosestNode(startPos);
	if (closestNode) {
		findShortestPath(closestNode);

		size_t visitedCount = 0;
		for (const auto& visitedNode : mVisitedNodes) {
			if (visitedNode.second) {
				++visitedCount;
			}
		}
	}
}

void RoadBuilder::findShortestPath(Node* node)
{
	if (mVisitedNodes[node]) {
		return;
	}

	std::unordered_map<Node*, Node*> nodeConnections;

	PriorityQueue<Node*, int32_t> nodeQueue;
	nodeQueue.push(node, 0);

	while (!nodeQueue.empty()) {
		auto frontNode = nodeQueue.top();
		nodeQueue.pop();

		mVisitedNodes[frontNode.first] = true;

		const std::vector<Node*>& connections = frontNode.first->getConnections();
		for (Node* neighbour : connections) {
			if (!mVisitedNodes[neighbour]) {
				int32_t priority = frontNode.second + neighbour->findSharedRoad(frontNode.first)->getWeight();
				auto enqueuedNeighbour = nodeQueue.find(neighbour);
				if (enqueuedNeighbour.first == neighbour) {
					if (enqueuedNeighbour.second > priority) {
						nodeQueue.decreasePriority(neighbour, priority);
						nodeConnections[neighbour] = frontNode.first;
					}
				}
				else {
					nodeQueue.push(neighbour, priority);
					nodeConnections[neighbour] = frontNode.first;
				}
			}
		}
	}
	for (auto& connection : nodeConnections) {
		connection.first->highlightSharedRoads(connection.second);
	}
}

// Private method(s)
void RoadBuilder::createNode(const sf::Vector2f& pos)
{
	auto node = std::make_unique<Node>(mWindow);
	node->setPosition(pos);
	node->setOnMouseButtonPress([this](Node* pressedNode) {
		mRoadStart = pressedNode;
	});
	node->setOnMouseButtonRelease([this](Node* releasedNode) {
		mRoadEnd = releasedNode;
	});

	mNodes.push_back(std::move(node));
}

void RoadBuilder::createRoad()
{
	auto road = std::make_unique<Road>(mFont, mWindow);
	road->setPosition(mRoadStart->getPosition());

	mRoads.push_back(std::move(road));
}

void RoadBuilder::connectRoad()
{
	Road* newRoad = mRoads.back().get();

	if (mRoadStart->addConnection(mRoadEnd, newRoad) && mRoadEnd->addConnection(mRoadStart, newRoad)) {
		mRoads.back()->setEndPoint(mRoadEnd->getPosition());
		updateRoadWeights();
	}
	else {
		mRoads.pop_back();
	}
}

void RoadBuilder::updateRoadWeights()
{
	// Find longest length
	float longestLength = std::numeric_limits<float>().min();
	for (const auto& road : mRoads) {
		float newLength = (*road).getLength();
		if (newLength > longestLength) {
			longestLength = newLength;
		}
	}

	// Update weights based on lengths
	for (auto& road : mRoads) {
		road->setWeight(static_cast<int32_t>(Math::lerp(1.f, 9.f, road->getLength() / longestLength)));
	}
}

Node* RoadBuilder::findClosestNode(const sf::Vector2f& pos)
{
	float closestDistance = std::numeric_limits<float>().max();
	Node* closestNode = nullptr;

	for (const auto& node : mNodes) {
		float newDistance = Math::magnitude(pos - node->getPosition());
		if (newDistance < closestDistance) {
			closestDistance = newDistance;
			closestNode = node.get();
		}
	}

	return closestNode;
}

void RoadBuilder::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const auto& road : mRoads) {
		target.draw(*road, states);
	}
	for (const auto& node : mNodes) {
		target.draw(*node, states);
	}
}