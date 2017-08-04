void basic_step(void*){
    prependAnimationFrame(90, 90, 90, 90, 100);
    prependAnimationFrame(90, 90, 115, 90, 250);
    prependAnimationFrame(64, 55, 110, 80, 150);
    prependAnimationFrame(64, 41, 110, 65, 500);
    prependAnimationFrame(59, 30, 65, 65, 50);
    prependAnimationFrame(59, 30, 65, 65, 160);
    prependAnimationFrame(90, 90, 65, 65, 350);
    prependAnimationFrame(125, 117, 80, 65, 150);
    prependAnimationFrame(139, 117, 115, 65, 250);
    prependAnimationFrame(155, 125, 90, 90, 50);
    prependAnimationFrame(155, 125, 90, 90, 160);
}

void keep_walking(void*){
    prependAnimationCallback(keep_walking);
    prependAnimationCallback(basic_step);
}


void basic_back(void*){
prependAnimationFrame(90, 90, 90, 90, 500);
prependAnimationFrame(90, 90, 90, 90, 140);
prependAnimationFrame(65, 91, 91, 91, 304);
prependAnimationFrame(60, 30, 120, 120, 200);
prependAnimationFrame(90, 90, 120, 120, 200);
prependAnimationFrame(90, 90, 120, 120, 100);
prependAnimationFrame(90, 120, 120, 120, 100);
prependAnimationFrame(90, 120, 120, 120, 300);
prependAnimationFrame(123, 120, 119, 75, 90);
prependAnimationFrame(138, 120, 119, 75, 98);
prependAnimationFrame(147, 120, 118, 75, 54);
prependAnimationFrame(149, 120, 109, 75, 104);
prependAnimationFrame(145, 115, 77, 75, 160);
prependAnimationFrame(90, 91, 62, 62, 168);
prependAnimationFrame(61, 25, 60, 60, 500);
prependAnimationFrame(60, 25, 90, 90, 70);
prependAnimationFrame(55, 90, 90, 90, 116);
prependAnimationFrame(89, 90, 90, 90, 4);
}

void kneel_down(void*){
prependAnimationFrame(90, 90, 90, 90, 626);
prependAnimationFrame(0, 180, 90, 90, 200);
prependAnimationFrame(0, 180, 27, 153, 1000);
prependAnimationFrame(0, 180, 27, 153, 1000);
prependAnimationFrame(0, 180, 27, 153, 400);
prependAnimationFrame(50, 130, 90, 90, 150);
}
void moonwalk_r(void*){
prependAnimationFrame(90, 90, 90, 90, 400);
prependAnimationFrame(145, 104, 90, 90, 400);
prependAnimationFrame(117, 64, 90, 90, 400);
prependAnimationFrame(70, 35, 90, 90, 400);
prependAnimationFrame(90, 90, 90, 90, 400);
prependAnimationFrame(145, 104, 90, 90, 400);
prependAnimationFrame(117, 64, 90, 90, 400);
prependAnimationFrame(70, 35, 90, 90, 400);
}
void huffing(void*){
prependAnimationFrame(90, 90, 90, 90, 150);
prependAnimationFrame(117, 99, 90, 90, 300);
prependAnimationFrame(81, 63, 90, 90, 300);
prependAnimationFrame(117, 99, 90, 90, 300);
prependAnimationFrame(81, 63, 90, 90, 300);
prependAnimationFrame(117, 99, 90, 90, 300);
prependAnimationFrame(81, 63, 90, 90, 150);
}
void turnleft(void*){
prependAnimationFrame(90, 90, 90, 90, 114);
prependAnimationFrame(76, 90, 130, 90, 200);
prependAnimationFrame(76, 90, 130, 90, 200);
prependAnimationFrame(65, 90, 130, 90, 425);
prependAnimationFrame(65, 30, 90, 90, 102);
prependAnimationFrame(60, 90, 90, 90, 102);
}
void randomAction(void*){
    long num = random(0, 5);
    prependAnimationCallback(randomAction);
    switch(num){
        case 0:prependAnimationCallback(basic_step);break;
        case 1:prependAnimationCallback(basic_back);break;
        case 2:prependAnimationCallback(kneel_down);break;
        case 3:prependAnimationCallback(moonwalk_r);break;
        case 4:prependAnimationCallback(huffing);break;
    }
}

void keep_walking_back(void*){
    prependAnimationCallback(keep_walking_back);
    prependAnimationCallback(basic_back);
}


void walk_back_collision(){
    prependAnimationCallback(mode_onWalkbackEnd);
    prependAnimationCallback(turnleft);
    prependAnimationCallback(turnleft);
    prependAnimationCallback(basic_back);
    prependAnimationCallback(basic_back);
    prependAnimationFrame(90, 90, 90, 90, 400);
}