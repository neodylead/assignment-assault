#ifndef SHIP_HPP
#define SHIP_HPP

#include "graphics.hpp"

using namespace genv;

class Ship {

    private:

        int x;
        int y;
        int health;
        int speed;
        int shoot_number;

    public:

        Ship();

        void kirajz();

        void move_left();
        void move_right();

        int get_x();
        int get_y();
        int get_health();
        int get_shoot_number();
        int get_speed();

        void damage(int amount);
        void heal(int amount);

        void upgrade_shoot();
        void upgrade_speed();

        bool hit(int bx, int by, bool enemy_shoot);

        void reset();
};

#endif

