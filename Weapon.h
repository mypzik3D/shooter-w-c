//
// Created by Иван Ильин on 01.06.2021.
//

#ifndef SHOOTER_WEAPON_H
#define SHOOTER_WEAPON_H


#include <string>
#include <World.h>
#include <Camera.h>
#include <SFML/Audio/Sound.hpp>
#include "Mesh.h"
#include "utils/Time.h"

class Weapon {
protected:
    int _initialPack = 100; // how much ammo do you have when you find the weapon

    int _clipCapacity = 30; // how much ammo can be stored in one clip
    int _stockAmmo = _initialPack - _clipCapacity; // how much ammo do you have in stock
    int _clipAmmo = _clipCapacity; // how much ammo do you have in current clip

    double _reloadTime = 3;
    double _fireDelay = 0.1; // time delay between fires
    double _damage = 300;

    double _spreading = 2.0;

    std::string _name = "Weapon_name";
    std::map<std::string, std::shared_ptr<Mesh>> _objects;

    double _lastFireTime = -INFINITY;
    double _lastReloadTime = -INFINITY;

    sf::Sound fireSound;
    sf::Sound reloadSound;
    sf::Sound noAmmoSound;

    static void deleteTrace(const std::shared_ptr<World> &world, const std::string& traceName);

    int fireTraces = 0;

    std::function<void(const Point4D&, const Point4D&)> addTraceCallBack;

public:
    Weapon(const std::string& weaponName, const std::string& objFileName, const std::string& matFileName, const Point4D& scale, const Point4D& translate, const Point4D& rotate);

    std::map<std::string, double> fire(const std::shared_ptr<World>& world, const std::shared_ptr<Camera>& camera);
    void reload();

    void addToWorld(const std::shared_ptr<World>& world);
    void removeFromWorld(const std::shared_ptr<World>& world);

    void attachToPlayer(Mesh& player);

    [[nodiscard]] std::pair<double, double> balance() const{ return std::make_pair(_clipAmmo, _stockAmmo); }

    void rotateRelativePoint(const Point4D& point4D, const Point4D& v, double val);

    void rotate(const Point4D& point4D, double val);
    void translate(const Point4D& point4D);

    void setAddTraceCallBack(std::function<void(Point4D, Point4D)> add) {
        addTraceCallBack = std::move(add);
    }

    [[nodiscard]] std::string name() const { return _name; }

    void addAmmo(int ammoAdd) { _stockAmmo += ammoAdd; }

    virtual std::map<std::string, double> processFire(const std::shared_ptr<World>& world, const std::shared_ptr<Camera>& camera);

    [[nodiscard]] int initialPack() const {return _initialPack; }
};


#endif //SHOOTER_3DZAVR_WEAPON_H
