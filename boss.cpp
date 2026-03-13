#include "boss.hpp"

const int XX = 1920;
const int YY = 1200;



Boss::Boss(int level) {

    x = XX/2;
    y = -120;
    entering = true;
    rage = false;

    left_health = 20 + (level * 5);
    right_health = 20 + (level * 5);

    speed = 1 + level/2;
    shoot_speed = std::max(15, 100 - level * 2);

    damage = 1;
}

    void Boss::kirajz(){

        genv::gout << genv::move_to(x - 100, y - 60) << genv::color(255,0,0) << genv::box(201, 41)
        << genv::move_to(x - 60, y - 20) << genv::box(41,41) << genv::move_to(x + 20, y - 20) << genv::box(41,41);

    }

    void Boss::boss_move(){



            if (entering)
            {
                y += 2;

                if (y >= 200) {
                    entering = false;
                }

                return;
            }

            x += speed;

            if (x < 200) {
                x = 200;
                speed = -speed;
            }

            if (x > XX - 200) {
                x = XX - 200;
                speed = -speed;
            }

            if (!rage && (left_health <= 0 || right_health <= 0)){

                rage = true;
                speed = speed *2;

            }
        }


    int Boss::get_x() { return x; }
    int Boss::get_y() { return y; }
    int Boss::get_left_health() { return left_health; }
    int Boss::get_right_health() { return right_health; }
    int Boss::get_shoot_speed() { return shoot_speed; }
    bool Boss::is_entering() {return entering; }

    void Boss::damage_left(int bx, int by) {

        if (left_health >= 0) {
            left_health--;
        }
    }

    void Boss::damage_right(int bx, int by){
        if (right_health >= 0) {
            right_health--;
        }
    }

    bool Boss::hit(int bx, int by, bool enemy_shoot) {
        if (enemy_shoot)
            return false;

        if (bx >= x - 60 && bx <= x - 19 &&
            by >= y - 20 && by <= y + 21)
        {
            left_health--;
            return true;
        }

        if (bx >= x + 20 && bx <= x + 61 &&
            by >= y - 20 && by <= y + 21)
        {
            right_health--;
            return true;
        }

        if (bx >= x - 100 && bx <= x + 101 &&
            by >= y - 60 && by <= y - 19)
        {

            //left_health--;
            //right_health--;
            return true;
        }

        return false;
    }
