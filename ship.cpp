#include "ship.hpp"

const int XX = 1920;
const int YY = 1200;

Ship::Ship() {
    x = XX / 2;
    y = 900;
    health = 3;
    speed = 4;
    shoot_number = 1;
    }

    void Ship::kirajz() {
        gout << color(0,255,255)
             << move_to(x,y) << dot
             << move_to(x-22,y-7) << box(45,15)
             << move_to(x-7,y-22) << box(15,15);
    }

    void Ship::move_left() {
        if (x > 25)
            x -= speed;
    }

    void Ship::move_right() {
        if (x < XX - 25)
            x += speed;
    }

    int Ship::get_x() { return x; }
    int Ship::get_y() { return y; }
    int Ship::get_health() { return health; }
    int Ship::get_shoot_number() { return shoot_number; }
    int Ship::get_speed() {return speed; }

    void Ship::damage(int amount) {
        health -= amount;
        if (health < 0)
            health = 0;
    }

    void Ship::heal(int amount) {
        health += amount;
        if (health > 5)
            health = 5;
    }

    void Ship::upgrade_shoot() {
        if (shoot_number < 7)
            shoot_number++;
    }

    void Ship::upgrade_speed() {
        speed++;
    }

    bool Ship::hit(int bx, int by, bool enemy_shoot) {

        if (!enemy_shoot)
            return false;

        if (by <= y + 7 && by >= y - 7 && bx >= x - 7 && bx <= x + 7)
            return true;

        if (by <= y - 7 && by >= y - 22 && bx >= x - 25 && bx <= x + 25)
            return true;

        return false;
    }

    void Ship::reset() {
        x = XX/2;
        y = 900;
        health = 3;
        speed = 4;
        shoot_number = 1;
}
