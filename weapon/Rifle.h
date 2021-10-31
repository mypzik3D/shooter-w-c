//
// Created by Иван Ильин on 06.06.2021.
//

#ifndef SHOOTER_RIFLE_H
#define SHOOTER_RIFLE_H

#include "Weapon.h"
#include "../engine/ResourceManager.h"
#include "../ShooterConsts.h"

class Rifle final : public Weapon {
public:
    explicit Rifle() : Weapon(5, 1, 1.0, 1.0, 30000, 0.5,
                              ShooterConsts::RIFLE_FIRE_SOUND, ShooterConsts::RIFLE_RELOAD_SOUND,
                              ObjectNameTag("rifle"), ShooterConsts::RIFLE_OBJ, Vec3D{3, 3, 3},
                              Vec3D{-2.3, 1, 1.3},Vec3D{0, Consts::PI, 0}) {}
};


#endif //SHOOTER_3DZAVR_RIFLE_H
