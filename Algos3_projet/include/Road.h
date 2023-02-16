#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class Road : public sf::Drawable, public sf::Transformable
{
public:
	// Public constructor(s)
	Road(sf::Font& font, sf::RenderWindow& window);

	// Public method(s)
	void handleEvent(sf::Event& event);
	void highlight(bool flag);
	void setEndPoint(const sf::Vector2f& pos);

	void setWeight(int32_t weight);
	[[nodiscard]] inline int32_t getWeight() const noexcept { return mWeight; }
	[[nodiscard]] inline float getLength() const noexcept { return mLine.getSize().y; }
private:
	// Private method(s)
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// Private static member(s)
	static const float LINE_WIDTH;

	// Private member(s)
	sf::RectangleShape mLine;
	int32_t            mWeight;
	sf::Text           mWeightText;
	sf::Vector2f       mEndPos;

	bool               mIsDrawingLine;
	bool               mIsHovered;
	sf::RenderWindow&  mWindow;
};