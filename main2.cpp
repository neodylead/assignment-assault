#include "graphics.hpp"
#include <vector>
#include <cmath>
using namespace genv;
using namespace std;
const int XX = 1600;
const int YY = 1000;

struct pont{
	double x, y, vx, vy;

    bool szele(){
        if (x > XX){
            return true;
        }
        if (x < 0){
            return true;
        }
        if (y > YY) {
            return true;
        }
        if (y < 0) {
            return true;
        }
        return false;
    }

    void mozog() {

        gout << move_to(x, y) << box(2,2);
    }
};



int main() {
    gout.open(XX,YY);


	gout << font("LiberationSans-Regular.ttf",40);
   	gout << refresh;
	event ev;
	gin.timer(10);
	bool lenyomva = false;

	int mouseX = XX/2;
	int mouseY = YY/2;



    vector<pont*> pontok;
	for (int i = 0; i < 2000; i++){

		pont *p = new pont;
		p->x = rand()%XX;
		p->y = rand()%YY;
		pontok.push_back(p);

	}



	while (gin >> ev && ev.keycode != key_escape) {

		if (ev.type == ev_mouse) {
			mouseX = ev.pos_x;
			mouseY = ev.pos_y;
		}

		if (ev.type == ev_key && ev.keycode == key_space) {
			for (pont *p : pontok) {
				double dx = p->x - mouseX;
				double dy = p->y - mouseY;
				double length = sqrt(dx*dx + dy*dy);

				if (length > 0.0001) {

					double dirX = dx / length;
					double dirY = dy / length;
					double strength = 50;
					p->vx = dirX * strength;
					p->vy = dirY * strength;
				}

			}
		}

		if (ev.type == ev_timer) {
            gout <<move_to(0,0) << color(0,0,0) << box(XX,YY) << color(255,255,255) << move_to(400,300);



		for (pont *p  : pontok) {

			if (p){



                double dx = mouseX - p->x;
                double dy = mouseY - p->y;
                double length = sqrt(dx * dx + dy * dy);

                double dirX, dirY;

                if (length > 0.0001) {
                    dirX = dx / length;
                    dirY = dy / length;
                    p->vx += dirX * 0.6;
                    p->vy += dirY * 0.6;
                }

                p->vx *= 0.97;
                p->vy *= 0.97;
                p->x += p->vx;
                p->y += p->vy;

                if (p->szele()){
                    p = 0;

                }
                p->mozog();

            }



		}

       		gout << refresh;
		}
	}
}

