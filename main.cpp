#include "graphics.hpp"
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include "ship.hpp"
#include "boss.hpp"

using namespace genv;
using namespace std;
const int XX = 1920;
const int YY = 1200;


//Lövedék objektum
struct lovedek {

    //Lövedék koordinátái
    int x, y;

    //Az érték igaz, ha a lövést egy ellenfél lőtte, különben hamis
    bool enemy_shoot = false;

    //Lövedék kirajzolása a koordináták alapján
    void kirajz(int color_r, int color_g, int color_b) {

        gout << color(color_r,color_g,color_b) << move_to(x, y) << box(5, 14);

    }

};



//Enemy ship objektum
struct enemy {

    //Enemy ship koordinátái
    int x, y;

    int health;

    bool entering = true;

    int damage = 1;

    //Az ellenfél sebességét
    int vx, vy, move_frame_x, move_frame_y;

    float drift_phase;


    int get_damage() {return damage; }

    void kirajz() {
        gout << color(255, 0,0) << move_to(x, y) << dot << move_to(x - 7, y - 7) << box(15,15) << move_to(x - 22, y - 22) << box(45, 15);
    }


    bool hit (int bx, int by, bool enemy_shoot) {

        if (!enemy_shoot) {
            if (by <= y + 7 && by >= y - 7 &&bx >= x - 7 && bx <= x + 7) {
                return true;
            }

            if (by <= y - 7 && by >= y - 22 &&bx >= x - 25 && bx <= x + 25) {
                return true;
            }

                return false;

        }
        else {
            return false;
        }

        }

    void movement(int szint) {


        if (!entering) {
            drift_phase += 0.05;

            x += vx;


            if (x < 30 || x > XX - 30) {

                vx = -vx;


            }


            if (move_frame_x >= 600) {

                do {
                    vx = (rand()%5-2) * (1 + szint * 0.1);
                } while(vx == 0);

                move_frame_x = 0;

            }

            move_frame_x++;

            if (szint >= 3) {
                y += sin(drift_phase) * 2;

                y += vy;

                if (y > 400){
                    y = 400;
                    vy = -vy;
                }
                if (y < 100) {
                    y = 100;
                    vy = -vy;

                }



                if (move_frame_y >= 900) {

                    do {
                        vy = (rand()%5-2) * (1 + szint * 0.1);
                    } while(vy == 0);

                    move_frame_y = 0;

                }

                move_frame_y++;
            }
        }

        else {

            y += 2;

            if (y > 200) {

                entering = false;

            }

            return;
        }
    }

};


struct score_board {

int score = 0;

int next_level_score = 10;

void kirajz(int level) {
    stringstream ss1;
    ss1 << score;
    string str = ss1.str();
    gout << color(255,255,255) << move_to(10, 30) <<text("Score " + str);

    stringstream ss2;
    ss2<<level;
    str = ss2.str();
    gout << move_to(10, 50) << text("Level " + str);
    }



};

struct health_bar {

    void kirajz(int player_health){

        int x = 20;

        int y = 80;

        int health_size = 17;
        int spacing = 10;
        int start_x = x + 60;

        gout << move_to(x, y) << font("LiberationSans-Regular.ttf",80) <<  color(255,0,0) << text("Health:");

        for (int i = 0; i < player_health; i++) {

            gout << move_to(start_x + i * (health_size + spacing), y - 15) << box(health_size,health_size);

        }

    }

};

struct power_up {

    int x, y, type;


    void kirajz() {
        //Speed power-up
        if (type == 0) {

            gout << move_to(x - 17, y - 17) << color(255,255,255) << box(35,35) << move_to(x - 5, y - 5) << color(0,255,255) << box(11,11);

        }

        else if(type == 1) {

            gout << move_to(x - 17, y - 17) << color(255,255,255) << box(35,35) << move_to(x - 5, y - 5) << color(255,0,0) << box(11,11);

        }

        else if(type == 2) {

            gout << move_to(x - 17, y - 17) << color(255,255,255) << box(35,35) << move_to(x - 5, y - 5) << color(0,255,0) << box(11,11);

        }

    }



    void mozog() {

        y += 4;
    }

};


struct menu {

    void start () {

        gout << move_to(0,0) << color(0,0,0) << box(XX,YY) << color(255,255,255) << move_to(XX/2 - 40, YY/2 - 20) << text("Space Shooter by NeodyLead")
        << move_to(XX/2- 20, YY/2) << text("Press 'Enter' to start");

    }



    void game_over () {

        gout << move_to(0,0) << color(0,0,0) << box(XX,YY) << color(255,255,255) << move_to(XX/2 - 20, YY/2 - 20) << text("Game Over") << move_to(XX/2- 20, YY/2)
        << text("Press 'Enter' to restart") << move_to(XX/2- 20, YY/2 + 20) << text("Press 'Esc' to exit");

    }



};

int main() {

    srand(time(0));
    gout.open(XX,YY);


	gout << font("LiberationSans-Regular.ttf",40);
   	gout << refresh;
	event ev;
	gin.timer(16);
	double lenyomva = false;
    vector<lovedek*> lovedekek;

    int szint = 1;

    int game_state = 0;

    bool nyome_bal = false;
    bool nyome_jobb = false;
    bool uj_lovedek = false;
    int frame_counter = 0;

    int spawn_counter = 0;

    bool boss_active = false;

    bool now_spawned = false;

    Boss* boss = nullptr;

    int     boss_shoot_timer = 0;

    Ship hajo;


    score_board scores;

    health_bar hp;

    menu m;

    vector<enemy*> enemies;


    //Power-up-ok
    bool if_power = false;

    int bullet_offset = 20;
    power_up *power;
    int power_x, power_y;

	while (gin >> ev && ev.keycode != key_escape) {


            if (game_state == 0) {

                m.start();
                gout << refresh;

                if (ev.type == ev_key && ev.keycode == key_enter) {

                    game_state = 1;


                }
            }

            if (game_state == 1) {


                if (ev.type == ev_key && ev.keycode == key_space && !uj_lovedek) {

                    uj_lovedek = true;

                        for (int i = 0; i < hajo.get_shoot_number(); i++) {

                            lovedek *loves = new lovedek;
                            loves->enemy_shoot = false;

                            float center = (hajo.get_shoot_number() - 1) / 2.0;
                            int offset = (i - center) * bullet_offset;

                            loves->x = hajo.get_x() - 2 + offset;
                            loves->y = hajo.get_y() - 35;

                            lovedekek.push_back(loves);
                        }

                }

                if (ev.type == ev_key && ev.keycode == -key_space) {

                    uj_lovedek = false;

                }





                if (ev.type == ev_key && ev.keycode == key_left) {

                    nyome_bal = true;

                }


                if (ev.type == ev_key && ev.keycode == -key_left) {

                    nyome_bal = false;

                }





                if (ev.type == ev_key && ev.keycode == key_right) {

                    nyome_jobb = true;

                }


                if (ev.type == ev_key && ev.keycode == -key_right) {

                    nyome_jobb = false;

                }





                if (ev.type == ev_timer) {
                    gout << move_to(0,0) << color(0,0,0) << box(XX,YY);


                    if (nyome_bal) {
                        if (hajo.get_x() <= 50) {

                        }
                        else {
                            hajo.move_left();
                        }

                    }


                    if (nyome_jobb) {
                        if (hajo.get_x() >= XX - 50) {

                        }
                        else{
                            hajo.move_right();
                        }
                    }


                    for (size_t i = 0; i < lovedekek.size(); ) {

                        bool bullet_deleted = false;

                        if (lovedekek[i]->y <= 0 || lovedekek[i]->y >= YY) {

                            delete lovedekek[i];
                            lovedekek.erase(lovedekek.begin() + i);
                            continue;

                        }


                        for (size_t j = 0; j < enemies.size(); ) {


                            if (enemies[j]->hit(lovedekek[i]->x, lovedekek[i]->y, lovedekek[i]->enemy_shoot)) {

                                enemies[j]->health--;

                                delete lovedekek[i];
                                lovedekek.erase(lovedekek.begin() + i);

                                bullet_deleted = true;

                                if (enemies[j]->health <= 0){

                                        int rand_power = rand()% 99;

                                        if (rand_power < 25 && !if_power) {

                                            power = new power_up;
                                            power->type = 0;
                                            if_power = true;

                                            power->x = enemies[j]->x;
                                            power->y = enemies[j]->y;
                                         }

                                         if (rand_power == 67 || rand_power == 69 || rand_power == 42 || rand_power == 71 && hajo.get_health() <= 6) {

                                            power = new power_up;
                                            power->type = 1;
                                            if_power = true;

                                            power->x = enemies[j]->x;
                                            power->y = enemies[j]->y;

                                         }

                                         if (rand_power == 28 || rand_power == 43 && hajo.get_shoot_number() <= 8) {

                                            power = new power_up;
                                            power->type = 2;
                                            if_power = true;

                                            power->x = enemies[j]->x;
                                            power->y = enemies[j]->y;

                                         }

                                        delete enemies[j];
                                        enemies.erase(enemies.begin() + j);

                                        scores.score++;

                                        break;

                                    }


                                }



                                else {


                                    j++;
                                }

                            }



                            if (!bullet_deleted && boss_active && boss->get_y() > 0 && boss->hit(lovedekek[i]->x, lovedekek[i]->y, lovedekek[i]->enemy_shoot))
                                {
                                    delete lovedekek[i];
                                    lovedekek.erase(lovedekek.begin() + i);
                                    bullet_deleted = true;
                                    break;
                                }

                            if (hajo.hit(lovedekek[i]->x, lovedekek[i]->y, lovedekek[i]->enemy_shoot)) {

                                    hajo.damage(1);
                                    delete lovedekek[i];
                                    lovedekek.erase(lovedekek.begin() + i);

                                    bullet_deleted = true;
                                    if (hajo.get_health() < 1) {

                                        game_state = 2;

                                    }
                                }

                            if (!bullet_deleted) {

                                if (lovedekek[i]->enemy_shoot == false) {

                                    lovedekek[i]->kirajz(0,255,0);
                                    lovedekek[i]->y -= 7 ;
                                    i++;
                                }
                                else {

                                    lovedekek[i]->kirajz(255,0,0);
                                    lovedekek[i]->y += 7 ;
                                    i++;

                                }

                            }

                        }




                if (if_power) {

                    if (power->y >= YY - 1) {

                        if_power = false;
                        delete power;

                    }
                    else if (power->y >= hajo.get_y() && power->x >= hajo.get_x() - 22 && power->x <= hajo.get_x() + 22 && power->y <= hajo.get_y() + 22) {

                        if_power = false;


                        if (power->type == 0 && hajo.get_speed() < 11){
                            hajo.upgrade_speed();

                        }

                        if (power->type == 1) {

                            hajo.heal(1);

                        }

                        if (power->type == 2 && hajo.get_shoot_number() < 9) {

                            hajo.upgrade_shoot();

                        }
                        delete power;

                    }
                    else {

                        power->mozog();
                        power->kirajz();

                    }

                }

                if (scores.score >= scores.next_level_score) {

                    szint += 1;
                    scores.next_level_score += 10 + szint * 5;

                }

                if (szint % 3 == 0 && !boss_active && !now_spawned && enemies.empty()) {

                    boss = new Boss(szint);
                    boss_active = true;
                    now_spawned = true;

                }

                if (boss_active) {

                    if (boss_active)
                    {
                        boss->boss_move();

                        if (boss->get_y() - 60 > 0) {
                            boss->kirajz();

                            int max_health = 2 * (20 + szint * 5);
                            int current = boss->get_left_health() + boss->get_right_health();

                            gout << color(255,0,0) << move_to(XX/2 - 300, 40) << box((current * 600) / max_health, 15);


                        }


                        boss_shoot_timer++;

                        if (boss_shoot_timer >= boss->get_shoot_speed())
                        {
                            boss_shoot_timer = 0;

                            if (boss->get_left_health() > 0) {

                                lovedek* l1 = new lovedek;
                                l1->enemy_shoot = true;
                                l1->x = boss->get_x() - 40;
                                l1->y = boss->get_y() + 20;
                                lovedekek.push_back(l1);
                            }

                            if (boss->get_right_health() > 0) {
                                lovedek* l2 = new lovedek;
                                l2->enemy_shoot = true;
                                l2->x = boss->get_x() + 40;
                                l2->y = boss->get_y() + 20;
                                lovedekek.push_back(l2);
                            }
                        }

                        if ((boss->get_left_health() + boss->get_right_health()) <= 0)
                        {
                            delete boss;
                            boss = nullptr;
                            boss_active = false;
                            now_spawned = false;

                            szint++;
                            scores.score += 10;
                        }
                    }

                }
                if (spawn_counter <= 0 && scores.next_level_score - scores.score > enemies.size() && !boss_active) {

                    enemy *e_ship = new enemy;
                    e_ship->x = rand()% (XX - 60) + 30;
                    e_ship->y = -30;

                    e_ship->vx = (rand()% 5-2 ) * (1 + szint * 0.1);
                    do {
                        e_ship->vx = (rand()%5 - 2) * (1 + szint * 0.1);
                    } while(e_ship->vx == 0);

                    do {
                        e_ship->vy = (rand()%5 - 2) * (1 + szint * 0.1);
                    } while(e_ship->vy == 0);

                    e_ship->move_frame_x = 0;
                    e_ship->move_frame_y = 0;

                    e_ship->drift_phase = rand()%360;

                    e_ship->health = 3;

                    enemies.push_back(e_ship);

                    spawn_counter = 300 - (szint * 2);


                }

                for (enemy *e_ship : enemies) {

                    e_ship->movement(szint);

                    double shoot_chance = 1 + szint * 0.8;

                    if((rand()%500) < shoot_chance) {

                        lovedek *loves = new lovedek;
                        loves->enemy_shoot = true;
                        lovedekek.push_back(loves);
                        loves->x = e_ship->x + 2;
                        loves->y = e_ship->y + 35;

                    }


                    if (e_ship->y > 22) {
                        e_ship->kirajz();
                    }

                }

                hp.kirajz(hajo.get_health());
                scores.kirajz(szint);
                spawn_counter--;
                hajo.kirajz();
                gout << refresh;
                }

            }

            }

            if (game_state == 2) {

                m.game_over();
                gout << refresh;

                if (ev.type == ev_key && ev.keycode == key_enter) {

                    hajo.reset();

                    game_state = 1;

                    szint = 1;


                    scores.score = 0;

                    lovedekek.clear();
                    enemies.clear();

                    if (boss != nullptr) {

                        delete boss;
                        boss = nullptr;

                    }
                    boss_active = false;
                    now_spawned = false;

                    uj_lovedek = false;
                    nyome_bal = false;
                    nyome_jobb = false;

                }

            }

            gout << refresh;


    }



