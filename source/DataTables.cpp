#include "DataTables.h"
#include "Aircraft.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Particle.h"


std::vector<AircraftData> initAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);


    data[Aircraft::Eagle].hitpoints = 100;
    data[Aircraft::Eagle].speed = 200.f;
    data[Aircraft::Eagle].textureId = Textures::Entities;
    data[Aircraft::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
    data[Aircraft::Eagle].fireInterval = sf::seconds(1.f);

    data[Aircraft::Raptor].hitpoints = 20;
    data[Aircraft::Raptor].speed = 80.f;
    data[Aircraft::Raptor].textureId = Textures::Entities;
    data[Aircraft::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
    data[Aircraft::Raptor].fireInterval = sf::Time::Zero; // Unable to fire
    data[Aircraft::Raptor].directions.push_back(Direction( 45.f,  80.f));
    data[Aircraft::Raptor].directions.push_back(Direction(-45.f, 160.f));
    data[Aircraft::Raptor].directions.push_back(Direction( 45.f,  80.f));

    data[Aircraft::Avenger].hitpoints = 40;
    data[Aircraft::Avenger].speed = 50.f;
    data[Aircraft::Avenger].textureId = Textures::Entities;
    data[Aircraft::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
    data[Aircraft::Avenger].fireInterval = sf::seconds(2.f);
    data[Aircraft::Avenger].directions.push_back(Direction( 45.f,  50.f));
    data[Aircraft::Avenger].directions.push_back(Direction(  0.f,  50.f));
    data[Aircraft::Avenger].directions.push_back(Direction(-45.f, 100.f));
    data[Aircraft::Avenger].directions.push_back(Direction(  0.f,  50.f));
    data[Aircraft::Avenger].directions.push_back(Direction( 45.f,  50.f));

    return data;
}


std::vector<ProjectileData> initProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);


    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 300.f;
    data[Projectile::AlliedBullet].textureId = Textures::Entities;
    data[Projectile::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

    data[Projectile::EnemyBullet].damage = 10;
    data[Projectile::EnemyBullet].speed = 300.f;
    data[Projectile::EnemyBullet].textureId = Textures::Entities;
    data[Projectile::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);

    data[Projectile::Missile].damage = 200;
    data[Projectile::Missile].speed = 150.f;
    data[Projectile::Missile].textureId = Textures::Entities;
    data[Projectile::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

    return data;
}


std::vector<PickupData> initPickupData()
{
    std::vector<PickupData> data(Pickup::TypeCount);


    data[Pickup::HealthRefill].textureId = Textures::Entities;
    data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
    data[Pickup::HealthRefill].action = [] (Aircraft& a) { a.repair(25); };

    data[Pickup::MissileRefill].textureId = Textures::Entities;
    data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
    data[Pickup::MissileRefill].action = [] (Aircraft& a) { a.collectMissiles(2); };
    
    data[Pickup::FireSpread].textureId = Textures::Entities;
    data[Pickup::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
    data[Pickup::FireSpread].action = [] (Aircraft& a) { a.increaseSpread(); };
    
    data[Pickup::FireRate].textureId = Textures::Entities;
    data[Pickup::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
    data[Pickup::FireRate].action = [] (Aircraft& a) { a.increaseFireRate(); };

    return data;
}


std::vector<ParticleData> initParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);


    data[Particle::Propellant].color = sf::Color(255, 255, 50);
    data[Particle::Propellant].lifetime = sf::seconds(0.6f);

    data[Particle::Smoke].color = sf::Color(50, 50, 50);
    data[Particle::Smoke].lifetime = sf::seconds(4.f);

    return data;
}