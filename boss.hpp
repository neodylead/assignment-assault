#ifndef BOSS_HPP
#define BOSS_HPP

#include "graphics.hpp"


class Boss {

    private:

        int x;
        int y;
        int left_health;
        int right_health;
        int speed;
        int shoot_speed;
        int damage;
        bool entering;
        bool rage;

    public:

        Boss(int level);

        void kirajz ();

        void boss_move();
        int get_x();
        int get_y();
        int get_left_health();
        int get_right_health();
        int get_shoot_speed();
        bool is_entering();

        void damage_left(int bx, int by);
        void damage_right(int bx, int by);

        bool hit(int bx, int by, bool enemy_shoot);

};

#endif // BOSS_HPP
