#pragma once
#include <sstream>
#include <cmath>
#include "SFML/Window/Keyboard.hpp"
#include "SFML/System/Vector2.hpp"


namespace util
{


std::string keyToString(const sf::Keyboard::Key key);


template <typename T>
std::string toString(const T& value);


template <typename T>
void centerOrigin(T& object);


float toRadian(const float degree);
float toDegree(const float radian);


float getVectorLength(const sf::Vector2f& vector);


sf::Vector2f getNormalizedVector(const sf::Vector2f& vector);


} // namespace util


#include "Utility.inl"
