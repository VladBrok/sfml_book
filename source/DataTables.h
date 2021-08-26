#pragma once
#include "ResourceIdentifiers.h"
#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Time.hpp"
#include <vector>
#include <functional>
class Aircraft;


struct Direction
{
    explicit Direction(const float angle, const float distance)
             : angle(angle), 
               distance(distance)
             {
             }

    float    angle;
    float    distance;
};                            
                              
                              
struct AircraftData           
{                             
    int                       hitpoints;
    float                     speed;
    Textures::ID              textureId;
    sf::IntRect               textureRect;
    sf::Time                  fireInterval;
    std::vector<Direction>    directions;
};


struct ProjectileData
{
    int             damage;
    float           speed;
    Textures::ID    textureId;
    sf::IntRect     textureRect;
};


struct PickupData
{
    std::function<void(Aircraft&)>    action;
    Textures::ID                      textureId;
    sf::IntRect                       textureRect;
};


struct ParticleData
{
    sf::Color    color;
    sf::Time     lifetime;
};


std::vector<AircraftData>      initAircraftData();
std::vector<ProjectileData>    initProjectileData();
std::vector<PickupData>        initPickupData();
std::vector<ParticleData>      initParticleData();
