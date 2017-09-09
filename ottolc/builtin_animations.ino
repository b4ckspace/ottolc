void basic_step(void*){
prependAnimationFrame(90, 90, 90, 90, 100);
prependAnimationFrame(90, 90, 105, 98, 200);
prependAnimationFrame(68, 65, 105, 98, 100);
prependAnimationFrame(61, 45, 105, 107, 300);
prependAnimationFrame(61, 30, 73, 73, 165);
prependAnimationFrame(90, 90, 73, 73, 250);
prependAnimationFrame(145, 110, 73, 73, 200);
prependAnimationFrame(155, 110, 90, 73, 200);
prependAnimationFrame(155, 119, 90, 90, 165);
prependAnimationFrame(90, 90, 90, 90, 80);
//prependAnimationFrame(90, 90, 90, 90, 1000);
}

void keep_walking(void*){
    prependAnimationCallback(keep_walking);
    long num = random(0, 11);
    switch(num){
        case 0: prependAnimationCallback(turnleft);break;
        case 1: prependAnimationCallback(turnright);break;
        default:prependAnimationCallback(basic_step);break;
    }
}


void basic_back(void*){
prependAnimationFrame(90, 90, 90, 90, 100);
prependAnimationFrame(90, 90, 75, 82, 200);
prependAnimationFrame(68, 65, 75, 82, 100);
prependAnimationFrame(61, 45, 75, 73, 300);
prependAnimationFrame(61, 30, 107, 107, 165);
prependAnimationFrame(90, 90, 107, 107, 250);
prependAnimationFrame(145, 110, 107, 107, 200);
prependAnimationFrame(155, 110, 90, 107, 200);
prependAnimationFrame(155, 119, 90, 90, 165);
prependAnimationFrame(90, 90, 90, 90, 80);
prependAnimationFrame(90, 90, 90, 90, 1000);
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
void turnright(void*){
prependAnimationFrame(90, 90, 90, 90, 114);
prependAnimationFrame(90, 104, 90, 50, 200);
prependAnimationFrame(90, 104, 90, 50, 200);
prependAnimationFrame(90, 115, 90, 50, 425);
prependAnimationFrame(150, 115, 90, 90, 102);
prependAnimationFrame(90, 120, 90, 90, 102);
}


void randomAction(void*){
    long num = random(0, 33);
    /*if (num > 4){
      num = 0;
    }*/
    prependAnimationCallback(randomAction);
    switch(num){
        case 0:prependAnimationCallback(sitdown);break;
        case 1:prependAnimationCallback(moonwalk_r);break;
        case 2:prependAnimationCallback(huffing);break;
        case 3:prependAnimationCallback(wink);break;
        case 4:prependAnimationCallback(turnleft);
               prependAnimationCallback(turnleft);break;
        case 5:prependAnimationCallback(turnright);
               prependAnimationCallback(turnright);break;
        case 6:prependAnimationCallback(ballet);break;
        //case 1:prependAnimationCallback(basic_back);break;
        //// case 7:prependAnimationCallback(acrobatic1);break;
        //case 8:prependAnimationCallback(acrobatic2);break;
        default:prependAnimationCallback(basic_step);break;
    }
}


void keep_walking_back(void*){
    prependAnimationCallback(keep_walking_back);
    prependAnimationCallback(basic_back);
}


void walk_back_collision(){
    prependAnimationCallback(mode_onWalkbackEnd);
    if(random(0, 2)){
        prependAnimationCallback(turnleft);
        prependAnimationCallback(turnleft);
    }else{
        prependAnimationCallback(turnright);
        prependAnimationCallback(turnright);
    }
    prependAnimationCallback(basic_back);
    prependAnimationCallback(basic_back);
    prependAnimationFrame(90, 90, 90, 90, 400);
}


void acrobatic1(void*){
prependAnimationFrame(89, 90, 91, 89, 407);
prependAnimationFrame(90, 119, 90, 90, 1000);
prependAnimationFrame(42, 135, 90, 90, 1000);
prependAnimationFrame(90, 119, 90, 90, 1000);
prependAnimationFrame(42, 135, 90, 90, 1000);
prependAnimationFrame(155, 125, 90, 90, 209);
prependAnimationFrame(155, 125, 90, 90, 160);
}

void acrobatic2(void*){
prependAnimationFrame(90, 90, 90, 90, 232);
prependAnimationFrame(151, 125, 127, 127, 100);
prependAnimationFrame(0, 125, 127, 127, 100);
prependAnimationFrame(155, 121, 127, 127, 84);
prependAnimationFrame(155, 121, 127, 127, 700);
prependAnimationFrame(155, 125, 90, 90, 160);
}

void ballet(void*){
prependAnimationFrame(90, 90, 90, 90, 1200);
prependAnimationFrame(148, 32, 90, 90, 2500);
prependAnimationFrame(148, 32, 90, 90, 3000);
prependAnimationFrame(90, 90, 90, 90, 100);
}

void sitdown(void*){
prependAnimationFrame(90, 90, 90, 90, 500);
prependAnimationFrame(0, 180, 110, 70, 200);
prependAnimationFrame(0, 180, 143, 37, 3000);
prependAnimationFrame(0, 180, 143, 37, 300);
prependAnimationFrame(0, 180, 120, 60, 200);
prependAnimationFrame(30, 150, 90, 90, 500);
}

void wink(void*){
prependAnimationFrame(90, 90, 90, 90, 600);
prependAnimationFrame(155, 118, 90, 90, 250);
prependAnimationFrame(25, 118, 90, 90, 250);
prependAnimationFrame(155, 118, 90, 90, 250);
prependAnimationFrame(25, 118, 90, 90, 250);
prependAnimationFrame(155, 118, 90, 90, 250);
prependAnimationFrame(25, 118, 90, 90, 250);
prependAnimationFrame(155, 118, 90, 90, 400);
prependAnimationFrame(155, 118, 90, 90, 165);
}
