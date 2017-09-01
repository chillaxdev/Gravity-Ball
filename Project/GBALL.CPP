/*
--------------------------------
 #####   #     #  ###  #     #
 #       #     #   #   #     #
 #       #     #   #   #     #
 #####   #######   #   #     #
     #   #     #   #   #     #
     #   #     #   #    #   #
 #####   #     #  ###     #
--------------------------------
 ///       MAIN FILE       ///
--------------------------------
To get the updated version visit:
https://github.com/shiv-sankar/Gravity-Ball
*/

#include <iostream.h>
#include <math.h>
#include <dos.h>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
//HELPER file containing helper classess & methods
#include "HELPER.CPP"

//never change size of ball
#define MAX_PLAYERS 4
#define MAX_TERRAINS 256
#define MAX_VERTICAL_TERRAINS 128
#define MAX_COLLECTIBLES 128
#define NO_VALUE 12345
#define SPEED_INCREASE_INTERVAL 5

#define MAX_SPEED 16

int speed = 3; //1 - 20 (MAX = 15)
int mode = 0;
char continueGame = 'y';
int cheatsUsed = 0;

#define TERRAIN_SIZE_CONSTANT 22
#define COLLECTIBLE_SIZE_CONSTANT 16

class Terrain {
protected:

long fromX, fromY;
long toX, toY;
long mTemp;
int color;
int isActive;

public:

Terrain() {
        isActive = 1;
        fromX = -32767;
        fromY = -32767;
        toX = -32767;
        toY = -32767;
}

Terrain(long fromX, long fromY, long toX, long toY, int color = BROWN) {
        this->fromX = fromX;
        this->fromY = fromY;
        this->toX = toX;
        this->toY = toY;
        this->color = color;
        isActive = 1;
}

Terrain setActive(int isActive) {
        this->isActive = isActive;
}

Terrain setToX(long toX) {
        this->toX = toX;
        return *this;
}

Terrain setToY(long toY) {
        this->toY = toY;
        return *this;
}

long getToX() {
        return toX;
}

long getToY() {
        return toY;
}

long getFromX() {
        return fromX;
}

long getFromY() {
        return fromY;
}

long get_mTemp() {
        return mTemp;
}

int containsPoint(int x, int y)  {
        int left = fromX;
        int top = fromY;
        int right = toX;
        int bottom = toY;

        if(x >= left && x <= right) {
                if(y <= bottom && y >= top) {
                        return 1;
                }
        }

        return 0;
}

int equals(Terrain terrain) {
        if(fromX == terrain.fromX && fromY == terrain.fromY && toX == terrain.toX && toY == terrain.toY && color == terrain.color) {
                return 1;
        }
        return 0;
}

int getColor() {
        return color;
}

void render(long mTemp = 0) {
        this->mTemp = mTemp;
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        //left, top, right, bottom
        long from, to;
        from = fromX - mTemp;
        to = toX - mTemp;
        if(to > 0 && from < getmaxx()) {

                if(from < 0) {
                        from = 0;
                }
                if(to > getmaxx()) {
                        to = getmaxx();
                }

                rectangle(from, fromY, to, toY);
                floodfill(from + 1, toY - 1, color);
                isActive = 1;
        } else {
                isActive = 0;
        }
//not rendering items crossed away &&  not yet came
}

int isRenderedOnMap() {
        return isActive;
}

};


class VerticalTerrain : public Terrain {

public:

VerticalTerrain() {
        isActive = 1;
        fromX = -32767;
        fromY = -32767;
        toX = -32767;
        toY = -32767;
}

VerticalTerrain(int fromX, int fromY, int toX, int toY, int color = BROWN) {
        this->fromX = fromX;
        this->fromY = fromY;
        this->toX = toX;
        this->toY = toY;
        this->color = color;
        isActive = 1;
}


void render(int mTemp = 0) {
        this->mTemp = mTemp;
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        //left, top, right, bottom
        int from, to;
        from = fromX - mTemp;
        to = toX - mTemp;
        if(to > 0 && from < getmaxx()) {

                if(from < 0) {
                        from = 0;
                }
                if(to > getmaxx()) {
                        to = getmaxx();
                }

                rectangle(from, fromY, to, toY);
                floodfill(from + 1, toY - 1, color);

        }

//not rendering items crossed away &&  not yet came

}



};

//Pickup Constants
struct _Pickup {
        int POSITIVE_POINT;
        int NEGATIVE_POINT;
        int MEGA_POSITIVE_POINT;
        int MEGA_NEGATIVE_POINT;
        int TELEPORT;
        int SUPER_MEGA_POSITIVE_POINT;

        _Pickup() {
                POSITIVE_POINT = 0;
                NEGATIVE_POINT = 1;
                MEGA_POSITIVE_POINT = 2;
                MEGA_NEGATIVE_POINT = 3;
                TELEPORT = 4;
                SUPER_MEGA_POSITIVE_POINT = 5;
        }

} Pickup;


//default collectible is positive point
class Collectible {
protected:

long x, y;
long mTemp;
int type;
int color;
int collected;
int isRendered;
int pointValue;

public:

Collectible() {
        collected = 0;
        pointValue = 10;
        type = Pickup.POSITIVE_POINT;
        color = CYAN;
}


Collectible(long x, long y, int color = CYAN) {
        collected = 0;
        pointValue = 10;
        type = Pickup.POSITIVE_POINT;
        this->x = x;
        this->y = y;
        this->color = color;
}

Collectible(long x, long y, int type, int temp, int xTemp) {
        this->x = x;
        this->y = y;
        this->type = type;
}

//default collectible design
virtual void render(long mTemp = 0) {
//drawing only if on the screen
        if(collected == 0) {
                this->mTemp = mTemp;
                int actualX = x - mTemp;
                if(actualX <= getmaxx() && actualX >= 0) {
                        setcolor(color);
                        setfillstyle(SOLID_FILL, color);
                        circle(actualX, y, 4);
                        floodfill(actualX, y, color);
                        isRendered = 1;
                } else {
                        isRendered = 0;
                }
        } else {
                isRendered = 0;
        }

}

Collectible *setCoordinates(long x, long y) {
        this->x = x;
        this->y = y;
        return this;
}

void setCollected() {
        collected = 1;
}

int isRenderedOnMap() {
        return isRendered;
}

int getPointValue() {
        return pointValue;
}

long getX() {
        return x;
}

long getY() {
        return y;
}

int get_mTemp() {
        return mTemp;
}

Collectible setType(int type) {
        this->type = type;
        return *this;
}

virtual int getType() {
        return type;
}

//methods exclusively for Teleports ie. purely virtual!
virtual long getNewY() {
        return -1;
}
virtual long getNewX() {
        return -1;
}
virtual long get_New_mTemp() {
        return -1;
}


};

Collectible *collectible[MAX_COLLECTIBLES];
int num_collectibles = 0;

//special points
class NegativePoint : public Collectible {

public:

NegativePoint() {
        pointValue = -10;
        color = RED;
        type = Pickup.NEGATIVE_POINT;
}

int getType() {
        return Pickup.NEGATIVE_POINT;
}

};


class TeleportPickup : public Collectible {
private:
int anim;
int animMode;
long newMTemp, newX, newY;

public:

TeleportPickup() {
        pointValue = 250;
        color = CYAN;
        anim = 1;
        animMode = 0;
        type = Pickup.TELEPORT;

        newMTemp = 472;
        newX = 25;
        newY = getmaxy()/2;
}

void render(long mTemp = 0) {
//drawing only if on the screen
        if(collected == 0) {
                this->mTemp = mTemp;
                int actualX = x - mTemp;
                if(actualX <= getmaxx() && actualX >= 0) {
                        setcolor(color);
                        setfillstyle(SOLID_FILL, color);
//circle(actualX, y, 12);
//left, top, right, bottom
                        if(animMode == 0) {
                                anim += 2;
                        } else {
                                anim -= 2;
                        }
                        rectangle(actualX - anim, y - anim, actualX + anim, y + anim);
                        if(anim == 20) {
                                animMode = 1;
                        } else if(anim == 0) {
                                animMode = 0;
                        }

                        floodfill(actualX, y, color);
                        isRendered = 1;
                } else {
                        isRendered = 0;
                }
        } else {
                isRendered = 0;
        }

}

long get_New_mTemp() {
        return newMTemp;
}

long getNewY() {
        return newY;
}

long getNewX() {
        return newX;
}

int getType() {
        return Pickup.TELEPORT;
}

};

class MegaNegativePoint : public Collectible {

public:

MegaNegativePoint() {
        pointValue = -50;
        color = RED;
        type = Pickup.MEGA_NEGATIVE_POINT;
}


void render(long mTemp = 0) {
//drawing only if on the screen
        if(collected == 0) {
                this->mTemp = mTemp;
                int actualX = x - mTemp;
                if(actualX <= getmaxx() && actualX >= 0) {
                        setcolor(color);
                        setfillstyle(SOLID_FILL, color);
                        circle(actualX, y, 12);
                        floodfill(actualX, y, color);
                        isRendered = 1;
                } else {
                        isRendered = 0;
                }
        } else {
                isRendered = 0;
        }

}

int getType() {
        return Pickup.MEGA_NEGATIVE_POINT;
}

};

class SuperMegaPositivePoint : public Collectible {
int anim;

public:

SuperMegaPositivePoint() {
        pointValue = 500;
        color = CYAN;
        type = Pickup.SUPER_MEGA_POSITIVE_POINT;
        anim = 0;
}

void render(long mTemp = 0) {
//drawing only if on the screen
        if(collected == 0) {
                this->mTemp = mTemp;
                int actualX = x - mTemp;
                if(actualX <= getmaxx() && actualX >= 0) {
                        color = anim;
                        anim++;
                        if(anim == 15) {
                                anim = 0;
                        }
                        int theta = 24*anim;
                        setcolor(color);
                        setfillstyle(SOLID_FILL, color);

                        int cx = actualX, cy = y;
                        int x1 = actualX - 9, y1 = y - 9;
                        int x2 = actualX + 9, y2 = y + 9;


                        rectangle(x1, y1, x2, y2);
                        floodfill(actualX, y, color);
                        isRendered = 1;
                } else {
                        isRendered = 0;
                }
        } else {
                isRendered = 0;
        }

}

int getType() {
        return Pickup.SUPER_MEGA_POSITIVE_POINT;
}

};

class MegaPositivePoint : public Collectible {

public:

MegaPositivePoint() {
        pointValue = 50;
        color = CYAN;
        type = Pickup.MEGA_POSITIVE_POINT;
}


void render(long mTemp = 0) {
//drawing only if on the screen
        if(collected == 0) {
                this->mTemp = mTemp;
                int actualX = x - mTemp;
                if(actualX <= getmaxx() && actualX >= 0) {
                        setcolor(color);
                        setfillstyle(SOLID_FILL, color);
                        circle(actualX, y, 12);
                        floodfill(actualX, y, color);
                        isRendered = 1;
                } else {
                        isRendered = 0;
                }
        } else {
                isRendered = 0;
        }

}

int getType() {
        return Pickup.MEGA_POSITIVE_POINT;
}

};


class Ball {
int x, y;
int color;
int gravity;
int num_strikes;
double strikeX[3], strikeY[3];
Terrain currentTerrain;

public:

Ball() {
}

Ball(int x, int y, int gravity, int color = BLUE) {
        num_strikes = 1;
        this->color = color;
        this->x = x;
        this->y = y;
        this->gravity = gravity;
}

Ball makeAdjustments(int x, int y, int gravity, int color = BLUE) {
        num_strikes = 1;
        this->color = color;
        this->x = x;
        this->y = y;
        this->gravity = gravity;
        return *this;
}

Ball(int color) {
        num_strikes = 1;
        this->color = color;
}



Ball setStrikeCoordinates(int strikeX1, int strikeY1) {
        strikeX[0] = strikeX1 + 9;
        strikeY[0] = strikeY1 - 9;
        strikeX[1] = strikeX1 + 9;
        strikeY[1] = strikeY1 + 9;
        strikeX[2] = x + 9;
        strikeY[2] = y;
        strikeX[3] = x - 9;
        strikeY[3] = y;
        return *this;
}

//future
Ball setStrikeAdjustedYCoordinate(int strikeY1) {
        strikeY[0] = strikeY1;
        strikeY[1] = strikeY1;
        return *this;
}

Ball render() {
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        circle(x, y, 9);
        floodfill(x, y, color);
        setcolor(WHITE);

        //rotating
        int degree = 8*speed;
        double theta = (3.14 * degree)/180.0; //radians


        setlinestyle(SOLID_LINE, 1, 3);
        if(gravity != -1) {
                for(int i = 0; i < num_strikes; i++) {
                        double x1 = strikeX[i];
                        double y1 = strikeY[i];
                        double centerX = x;
                        double centerY = y;

                        if(y1 - centerY > 9) {
                                y1 -= y1 - centerY - 9;
                        } else if(centerY - y1 > 9) {
                                y1 += centerY - y1 - 9;
                        }

                        double tempX = x1 - centerX;
                        double tempY = y1 - centerY;

                        // now apply rotation
                        double rotatedX = tempX*cos(theta) - tempY*sin(theta);
                        double rotatedY = tempX*sin(theta) + tempY*cos(theta);

                        // translate back
                        x1 = rotatedX + centerX;
                        y1 = rotatedY + centerY;

                        double result = pow(x1 - centerX, 2) + pow(y1 - centerY, 2);

                        if(result > 81) {
                                if(x1 > centerX + 9) {
                                        x1 = centerX + 9;
                                } else if(x1 < centerX - 9) {
                                        x1 = centerX - 9;
                                }

                                if(y1 > centerY + 9) {
                                        y1 = centerY + 9;
                                } else if(y1 < centerY - 9) {
                                        y1 = centerY - 9;
                                }

                        }

                        strikeX[i] = x1;
                        strikeY[i] = y1;

                        //setcolor(RED);
                        line(centerX, centerY, strikeX[i], strikeY[i]);
                        //getch();
                }
        }



        //line(centerX, centerY, strikeX[1], strikeY[1]);

        //setlinestyle(SOLID_LINE, 1, 3);
        //gravity symbol (down)
        if(gravity) {
                //outtextxy(x - 3, y - 2, "v");
                line(x, y + 1, x - 3, y - 2);
                line(x, y + 1, x + 3, y - 2);
        } else {
                //outtextxy(x - 3, y - 2, "^");
                line(x, y - 2, x - 3, y + 1);
                line(x, y - 2, x + 3, y + 1);
        }
        setlinestyle(SOLID_LINE, 1, 1);
        return *this;
}

Ball setTerrain(Terrain currentTerrain) {
        this->currentTerrain = currentTerrain;
        return *this;
}

Ball setY(int y) {
        this->y = y;
        return *this;
}

Ball setX(int x) {
        this->x = x;
        return *this;
}

Terrain getTerrain() {
        return currentTerrain;
}

//left, top, right, bottom
int isOnTerrain(Terrain terrain, int gravity) {

        int centerX = x;
        int centerY = y;
        //only for horizontal ones,,,vertical -> inheritance :)
        long mTemp = terrain.get_mTemp();
        int right = terrain.getToX() - mTemp;
        int left = terrain.getFromX() - mTemp;
        int top = terrain.getFromY();
        int bottom = terrain.getToY();

        if(centerX <= right && centerX >= left) {
                //either above or below (gravity xD)
                //check bottom also :)

                //if(top == (ballY + 7) || bottom == (ballY + 20)) {
                //if(top == (ballY + 7) || bottom == (ballY - 7)) {
                //upper part detection
                //left, top, right, bottom
                //NO_VALUE - to continue existing smooth motion
                //based on fixed ball radius 9
//UP

                if(gravity == 1) {
                        int intersectionY = bottom;
                        if(intersectionY - centerY > 12) {
                                return 0;
                        } else {

                                if(intersectionY - centerY == 7) {
                                        return NO_VALUE;
                                } else if(intersectionY - centerY < 12 && top < centerY + 9) {
                                        int temp = intersectionY - centerY + 7;
                                        if(temp > 0) {
                                                return temp;
                                        }
                                        //return intersectionY - centerY + 7;
                                }

                        }
                } else if(gravity == 2) {
                        int intersectionY = top;
                        if(intersectionY - centerY > 12) {
                                return 0;
                        } else {
                                if(intersectionY - centerY == 7) {
                                        return NO_VALUE;
                                } else if(intersectionY - centerY < 12 && bottom > centerY + 9) {
                                        return intersectionY - centerY - 7;
                                }

                        }
                } else {
//final detection
                        // centerY + 9 > top
                        if(centerY - 9 > bottom) { //UP
                                return 0;
                        } else if(centerY + 9 < top) { //BOTTOM
                                return 0;
                        }


//continue
                        return NO_VALUE;
                }



        }



        return 0;
}

int hasHitVerticalPartOfTerrain(Terrain mVerticalTerrain, int &update) {
        update = 0;
        if(mVerticalTerrain.isRenderedOnMap() && !mVerticalTerrain.equals(currentTerrain)) {
                int ballX = x;
                int ballY = y;
                long mTemp = mVerticalTerrain.get_mTemp();
                int terrainLeft = mVerticalTerrain.getFromX() - mTemp;
                int terrainRight = mVerticalTerrain.getToX() - mTemp;
                int terrainTop = mVerticalTerrain.getFromY();
                int terrainBottom = mVerticalTerrain.getToY();


                if(ballY + 9 >= terrainTop && ballY - 9 <= terrainBottom) {
                        //primary testing
                        if(ballX + 9 == terrainLeft) {
                                return 1;
                        } else if(ballX + 9 > terrainLeft && ballX + 9 - terrainLeft <= 20) {
                                return ballX + 9 - terrainLeft;
                        }
                }

        }

        return 0;
}

int hasHitCollectible(Collectible mCollectible, int ballIsOnTerrain, int up) {
        //checking only if rendered for efficiency & optimization!
        if(mCollectible.isRenderedOnMap()) {
                long mTemp = mCollectible.get_mTemp();
                int collectibleX = mCollectible.getX() - mTemp;
                int collectibleY = mCollectible.getY();
                int ballX = x;
                int ballY = y;

                if(ballY - 9 <= collectibleY && ballY + 9 >= collectibleY) {
                        if(ballX + 7 <= collectibleX || ballX - 7 <= collectibleX) {
                                double result = pow(collectibleX - ballX, 2) + pow(collectibleY - ballY, 2);
                                if(result <= 81) {
                                        return 1;
                                }

                                //secondonary testing (fast)
                                collectibleX -= speed;

                                if(ballIsOnTerrain) {
                                        if(ballX > collectibleX) {
                                                return 1;
                                        }
                                } else {
                                        if(ballX > collectibleX) {
                                                if(up) {
                                                        ballY -= speed;
                                                        if(ballY < collectibleY) {
                                                                return 1;
                                                        }
                                                } else {
                                                        ballY += speed;
                                                        if(ballY > collectibleY) {
                                                                return 1;
                                                        }
                                                }

                                        }
                                }

                        }
                }

        }
        return 0;
}

int isBelowOtherBall(Ball ball, int update) {
        update = 0;
        if(x > ball.x - 9 && x < ball.x + 9) {
                if(y - 7 == ball.y + 9) {
                        return 1;
                } else if(y - 7 > ball.y - 7 && y - 7 < ball.y + 9) {
                        update = 1;
                        return ball.y - y + 16;
                }
        }
        return 0;
}

int isAboveOtherBall(Ball ball, int update) {
        update = 0;
        if(x > ball.x - 9 && x < ball.x + 9) {
                if(y + 9 == ball.y - 7) {
                        return 1;
                } else if(y + 9 > ball.y - 7 && y + 9 < ball.y + 7) {
                        update = 1;
                        return y - ball.y + 16;
                }
        }
        return 0;
}

int isOutOfScreen() {
        int ballX = x;
        int ballY = y;

        if(x > getmaxx() || x < 0) {
                return 1;
        } else if(y > getmaxy() || y < 0) {
                return 1;
        }
        return 0;
}


int getX() {
        return x;
}

int getY() {
        return y;
}

};

struct User {
        char name[24];
        int score;
        int color;
} user;

class Player {
char name[24];
int score;
int color;
//ball
Ball ball;
//session based
int buttonPressed;
int tempBallX, tempBallY;
int ball_status, ball_vStatus;
int lastButtonPressedState;
//misc
int status; //alive or dead
//point collection tracking
int points[6];
int timeTaken;

void resetPoints() {
        for (int i = 0; i < 6; i++)   {
                points[i] = 0;
        }
}

public:

Player() {
        resetPoints();
}

Player(const char name[24], Ball ball, int color) {
        strcpy(this->name, name);
        this->ball = ball;
        //session based
        buttonPressed = 0;
        tempBallX = getmaxx()/2;
        tempBallY = getmaxy()/2 - 20 - 7;
        ball_status = 0;
        ball_vStatus = 0;
        lastButtonPressedState = 0; //GRAVITY UP BY DEFAULT
        this->color = color;
        status = 1;
        resetPoints();
}

//insert into database
void insert() {
        strcpy(user.name, this->name);
        user.score = this->score;
        user.color = this->color;
        ofstream fout = ofstream(Folder.USERS.getData(), ios::binary | ios::app);
        fout.write((char *) &user, sizeof(User));
        fout.close();
}

//retreive database && not forget to deallocate
User *retreiveDatabase(int &size) {
        ifstream fin = ifstream(Folder.USERS.getData(), ios::binary);
        size = 0;
        User *users = new User[128];
        while(fin.read((char *) &users[size], sizeof(User))) {
                size++;
        }
        fin.close();
        return users;
}

//get methods
int *getPoints() {
        return points;
}
int getTimeTaken() {
        return timeTaken;
}
int getColor() {
        return color;
}
int getTempBallY() {
        return tempBallY;
}

int getTempBallX() {
        return tempBallX;
}

Ball getBall() {
        return ball;
}

int getBallColor() {
        return color;
}

int getBallVStatus() {
        return ball_vStatus;
}

int getBallStatus() {
        return ball_status;
}

int getScore() {
        return score;
}

int getButtonPressed() {
        return buttonPressed;
}

int getLastButtonPressedState() {
        return lastButtonPressedState;
}

int isAlive() {
        return status;
}

char *getName() {
        return name;
}

//set methods
Player setTempBallY(int tempBallY) {
        this->tempBallY = tempBallY;
        return *this;
}

Player setTimeTaken(int timeTaken) {
        this->timeTaken = timeTaken;
        return *this;
}

Player setPoints(int *points) {
        for (int i = 0; i < 6; i++) {
                this->points[i] = points[i];
        }
}

Player setColor(int color) {
        this->color = color;
        return *this;
}

Player setTempBallX(int tempBallX) {
        this->tempBallX = tempBallX;
        return *this;
}

Player setBall(Ball ball) {
        this->ball = ball;
        return *this;
}

Player setName(char name[24]) {
        strcpy(this->name, name);
        return *this;
}

Player setDead() {
        status = 0;
        return *this;
}

Player setBallVStatus(int ball_vStatus) {
        this->ball_vStatus = ball_vStatus;
        return *this;
}

Player setBallStatus(int ball_status) {
        this->ball_status = ball_status;
        return *this;
}

Player setScore(int score) {
        this->score = score;
        return *this;
}

Player setButtonPressed(int buttonPressed) {
        this->buttonPressed = buttonPressed;
        return *this;
}

Player setLastButtonPressedState(int lastButtonPressedState) {
        this->lastButtonPressedState = lastButtonPressedState;
        return *this;
}

};

Player player[MAX_PLAYERS];
int num_players = 1;
int buttonSize;

class TerrainsDatabase {
Terrain *mTerrain;
File file;
String uri;

void close() {
        file.close();
}

public:

TerrainsDatabase(Terrain mTerrain[], String uri) {
        //this->_folder = _folder;
        this->mTerrain = mTerrain;
        this->uri = uri;
        file.open(uri, Files.BINARY);
}

TerrainsDatabase(Terrain mTerrain[], String uri, String dat) {
        //this->_folder = _folder;
        this->mTerrain = mTerrain;
        char str[128];
        sprintf(str, "%s\\%s", uri.getData(), dat.getData());
        uri.setData(str);
        this->uri = uri;
        /*
           cout << this->uri.getData();
           getch();
         */
        file.open(this->uri, Files.BINARY);
}

//fetch bulk records from file

int fetchRecords(int max = MAX_RECORDS) {
        if(file.getStatus()) {
                int i = 0;
                while(file.read((char *) &mTerrain[i], TERRAIN_SIZE_CONSTANT)) {
                        i++;
                        if(i == max) {
                                close();
                                return i;
                        }
                }
                close();

                return i;

        } else {
                close();
                return -1;
        }

}

int setRecords(int max = MAX_RECORDS) {
        if(max == 0) {
                return -1;
        }
        remove(uri.getData());
        close();
        file.open(this->uri, Files.BINARY);
        int i = 0;
        while(file.write((char *) &mTerrain[i], TERRAIN_SIZE_CONSTANT)) {
                i++;
                if(i == max) {
                        close();
                        return i;
                }
        }
        close();
        return i;
}

};

class CollectiblesDatabase {
Collectible **mCollectible;
File file;
//folder _folder;
String uri;

void close() {
        file.close();
}

public:

CollectiblesDatabase(Collectible **mCollectible, String uri) {
        //this->_folder = _folder;
        this->mCollectible = mCollectible;
        this->uri = uri;
        file.open(uri, Files.BINARY);
}

CollectiblesDatabase(Collectible **mCollectible, String uri, String dat) {
        //this->_folder = _folder;
        this->mCollectible = mCollectible;
        char str[128];
        sprintf(str, "%s\\%s", uri.getData(), dat.getData());
        uri.setData(str);
        this->uri = uri;
        /*
           cout << this->uri.getData();
           getch();
         */
        file.open(this->uri, Files.BINARY);
}

//fetch bulk records from file

int fetchRecords(int max = MAX_RECORDS) {
        //return -1;
        if(file.getStatus()) {
                int i = 0;
                Collectible tempCollectible;
                while(file.read((char *) &tempCollectible, COLLECTIBLE_SIZE_CONSTANT)) {

                        //allocating accordingly
                        int type = tempCollectible.getType();
                        //cout << type << endl;
                        //getch();
                        if(type == Pickup.POSITIVE_POINT) {
                                mCollectible[i] = new Collectible()->setCoordinates(tempCollectible.getX(), tempCollectible.getY());
                        } else if(type == Pickup.MEGA_POSITIVE_POINT) {
                                mCollectible[i] = new MegaPositivePoint()->setCoordinates(tempCollectible.getX(), tempCollectible.getY());
                        } else if(type == Pickup.NEGATIVE_POINT) {
                                mCollectible[i] = new NegativePoint()->setCoordinates(tempCollectible.getX(), tempCollectible.getY());
                        } else if(type == Pickup.MEGA_NEGATIVE_POINT) {
                                mCollectible[i] = new MegaNegativePoint()->setCoordinates(tempCollectible.getX(), tempCollectible.getY());
                        } else if(type == Pickup.TELEPORT) {
                                mCollectible[i] = new TeleportPickup()->setCoordinates(tempCollectible.getX(), tempCollectible.getY());
                        } else if(type == Pickup.SUPER_MEGA_POSITIVE_POINT) {
                                mCollectible[i] = new SuperMegaPositivePoint()->setCoordinates(tempCollectible.getX(), tempCollectible.getY());
                        }


                        i++;
                        if(i == max) {
                                close();
                                return i;
                        }
                }
                close();

                return i;

        } else {
                close();
                return -1;
        }

}

int setRecords(int max = MAX_RECORDS) {
        //return -1;
        if(max == 0) {
                return -1;
        }
        remove(uri.getData());
        close();
        file.open(this->uri, Files.BINARY);
        int i = 0;
        //Collectible tempCollectible = Collectible();
        Collectible tempCollectible = Collectible(mCollectible[i]->getX(), mCollectible[i]->getY(), mCollectible[i]->getType(), -1, -1); // to prevent casting

/*
    tempCollectible.setX(mCollectible[i]->getX());
    tempCollectible.setY(mCollectible[i]->getY());
    tempCollectible.setType(mCollectible[i]->getType());
 */
//tempCollectible.setType(mCollectible[i]->getType());
//getch();
        while(file.write((char *) &tempCollectible, COLLECTIBLE_SIZE_CONSTANT)) {
                i++;
                if(i == max) {
                        close();
                        return i;
                }
                /*
                   tempCollectible.setX(mCollectible[i]->getX());
                   tempCollectible.setY(mCollectible[i]->getY());
                   tempCollectible.setType(mCollectible[i]->getType());
                 */
                tempCollectible = Collectible(mCollectible[i]->getX(), mCollectible[i]->getY(), mCollectible[i]->getType(), -1, -1); // to prevent casting
                //cout << tempCollectible.getType() << endl;
                //getch();
        }
        close();
        return i;
}

};

class Time {
time_t timeT;
time_t start, end;
time_t lastStart, lastEnd;
tm *timeLive;
int lastSecondsRequest;

public:

Time() {
        timeT = time(NULL);
        start = time(NULL);
        timeLive = localtime(&timeT);
        lastSecondsRequest = -1;
        lastStart = time(NULL);
}

Time refresh() {
        timeT = time(NULL);
        start = time(NULL);
        timeLive = localtime(&timeT);
        lastSecondsRequest = -1;
        lastStart = time(NULL);
        return *this;
}

int isTimeOver(int seconds) {
        lastEnd = time(NULL);
        long lastDiffTime = difftime(lastEnd, lastStart);
//delay of at least 1 seconds to prevent loop issues
        if(lastDiffTime >= 1) {
                end = time(NULL);
                long diffTime = difftime(end, start);
                if(diffTime % seconds == 0 && diffTime >= seconds) {
                        lastStart = time(NULL);
                        return 1;
                }
        }

        return 0;
}

double getTime() {
        return timeT/1.0;
}

};

void levelEditor(long mTemp);

int num_terrains = 0;
Terrain terrain[MAX_TERRAINS];


//initialising the engine globally
GraphicsEngine mGraphicsEngine = GraphicsEngine();

const int MAX_CHEATS = 3;
class CheatEngine {
char cheatsList[MAX_CHEATS][64];
char cheat[64];

public:

//initializing inbuilt cheat codes using a constructor
CheatEngine() {
        strcpy(cheatsList[0], "BOOOOOORING");
        strcpy(cheatsList[1], "GETTHEREVERYFASTINDEED");
        strcpy(cheatsList[2], "GETTHEREAMAZINGLYFAST");
}

int isValidCheat(char cheat[64]) {
        strcpy(this->cheat, cheat);
        for(int i = 0; i < MAX_CHEATS; i++) {
                if(!strcmpi(cheat, cheatsList[i])) {
                        return i + 1;
                }
        }
        return -1;
}


};

CheatEngine mCheatEngine = CheatEngine();

class Transition {
int x, y;
int color;
int duration; //seconds

public:

Transition(int x, int y, int color) {
        this->x = x;
        this->y = y;
        setbkcolor(color);
}

Transition rippleEffect(int color) {
        this->color = color;
        return *this;
}

Transition setDuration(int duration) {
        this->duration = duration;
        return *this;
}

void start(int check = 0, int defaultSize = 0) {
        int rippleCircleSize = defaultSize;
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        Time time = Time();
        //400
        while(1) {

                if(check == 0) {
                        if(rippleCircleSize >= 400) {
                                break;
                        }
                } else if(check == 1) {
                        if(rippleCircleSize >= 650) {
                                break;
                        }
                }

                mGraphicsEngine.setScreen();
                cleardevice();
                circle(x, y, rippleCircleSize);
                rippleCircleSize += 8;
                floodfill(x, y, color);
                mGraphicsEngine.showScreen();
                delay(20);

        }
        delay(1000);
        if(check == 1) {
                mGraphicsEngine.setScreen();
                cleardevice();
                setbkcolor(BLUE);
                mGraphicsEngine.showScreen();
                return;
        }
        while(1) {
                if(rippleCircleSize == 222) {
                        break;
                }
                mGraphicsEngine.setScreen();
                cleardevice();
                circle(x, y, rippleCircleSize);
                rippleCircleSize -= 2;
                floodfill(x, y, color);
                mGraphicsEngine.showScreen();
                delay(20);
        }

        const char *HEAD = "GRAVITY BALL";
        setcolor(WHITE);
        settextstyle(0, 0, 3);
        outtextxy(getmaxx()/2 - 12*strlen(HEAD) + 6, getmaxy()/2 - 36, HEAD);
        delay(1000);
        settextstyle(10, 0, 1);
        setcolor(YELLOW);
        outtextxy(getmaxx()/2 - 200, getmaxy()/2 - 12, "COUNTERACTS GRAVITATION");
        delay(1000);
        setcolor(LIGHTGREEN);
        settextstyle(8, 0, 1);
        outtextxy(getmaxx()/2 - 140, getmaxy()/2 + 50, "DEVELOPED BY: Shiv Sankar");
        getch();

        //Introduction
        cleardevice();
        setbkcolor(BLUE);
        cout << "\nINTRODUCTION:\n\n1. Counteract the force of gravity, make flat and ceiling runnings and challenge your friend in these interesting racings. \n\n2. After the game has been started, first player uses \"W\" button and the second player uses \"UP\" arrow button to pass his runner to opposite way. \n\n3. Be careful on the holes and obstacles, and try to complete the race at 1st position. \n\n4. This game can be played up to 4 players. \n\n5. Have a nice running. \n\n6. Playing this game will strengthen your attention and concentration.\n\n\n\n\t\t\tPRESS ANY KEY TO START...";
        getch();
        //transformToMenuBall
        cleardevice();
        setcolor(BLUE);
        delay(1000);
        rippleCircleSize = 500;
        int t = 0;
        while(1) {
                if(rippleCircleSize <= 9 && x <= 100 && y == getmaxy() - 50) {
                        break;
                }
                mGraphicsEngine.setScreen();
                cleardevice();
                circle(x, y, rippleCircleSize);
                if(rippleCircleSize >= 9) {
                        rippleCircleSize -= 2;
                }
                if(rippleCircleSize <= 350) {
                        if(x > 100) {
                                x--;
                        }
                        if(y < getmaxy() - 50) {
                                y++;
                        }
                }
                floodfill(x, y, color);
                if(t == 0) {
                        setbkcolor(WHITE);
                        t = 1;
                }
                mGraphicsEngine.showScreen();
                delay(20);
        }

        settextstyle(11, 0, 1);
        //return *this;
}

void onlyFirstPartOfStart(int check = 0, int defaultSize = 0) {
        int rippleCircleSize = defaultSize;
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        Time time = Time();
        //400
        while(1) {
                if(check == 0) {
                        if(rippleCircleSize >= 450) {
                                break;
                        }
                } else if(check == 1) {
                        if(rippleCircleSize >= 650) {
                                break;
                        }
                }
                mGraphicsEngine.setScreen();
                cleardevice();
                circle(x, y, rippleCircleSize);
                rippleCircleSize += 16;
                floodfill(x, y, color);
                mGraphicsEngine.showScreen();
                delay(20);
        }
        delay(1000);
        mGraphicsEngine.setScreen();
        cleardevice();
        setbkcolor(BLUE);
        mGraphicsEngine.showScreen();
}


};


int start() {
        mode = 1;
        int score = 0;

//========================GAME LOOP===============================
        int buttonPressed = 0;
        int tempBallX, tempBallY = getmaxy() - 50;
        int ball_status = 0, ball_vStatus = 0;
        //speed = 3;

        int lastButtonPressedState = 0; //GRAVITY UP BY DEFAULT

        tempBallX = 100;
        char button;
        int firstTime = 1;
        int mTemp = 0;

        Ball ball = Ball(); //creating a new ball

        Terrain testTerrain[8];

        testTerrain[0] = Terrain(0, 0, getmaxx(), 50);
        testTerrain[1] = Terrain(0, 130, getmaxx(), 155);


        testTerrain[2] = Terrain(0, getmaxy() - 125, getmaxx(), getmaxy() - 125 + 25);
        testTerrain[3] = Terrain(0, getmaxy() - 25, getmaxx() + 50,  getmaxy() - 25 + 25);


        testTerrain[4] = Terrain(getmaxx() + 25, getmaxy() - 125, getmaxx() + 25 + 300, getmaxy() - 100, RED);
        testTerrain[5] = Terrain(getmaxx() + 325, getmaxy() - 200, getmaxx() + 325 + 300, getmaxy() - 175, LIGHTBLUE);
        testTerrain[6] = Terrain(getmaxx() + 625, getmaxy() - 275, getmaxx() + 625 + 300, getmaxy() - 250, GREEN);
        testTerrain[7] = Terrain(getmaxx() + 925, 0, getmaxx() + 925 + 300, 25, 13); //PINK

        char mTextArray[][45] = {
                " ~~ GRAVITY BALL ~~  || By Shiv Sankar ~~ DAV",
                "Press TAB to enter level editor.",
                "Press \'W\' to reverse gravitation.",
                "Land on any terrain and hit enter.",
                "SINGLEPLAYER",
                "MULTIPLAYER",
                "LEADERBOARD",
                "EXIT"
        };

        Transition(getmaxx()/2, getmaxy()/2, WHITE).rippleEffect(BLUE)
        .setDuration(5)
        .start();

        int microFirst = 1;

        Time timer = Time();
        while(mode) {


                if(kbhit()) {
                        button = getch();

                        if(button == Key.ESCAPE) {
                                exit(0);
                        } else if(button == Key.TAB) {
                                return -1;
                        }



                        if(!buttonPressed) {
                                if(button == 'w' || button == 'W') {
                                        if(lastButtonPressedState == 0) {
                                                buttonPressed = 1;
                                                lastButtonPressedState = 1;

                                                if(ball_vStatus == 0) {
                                                        int randomNumber = random(10) + 1;
                                                        if(randomNumber % 6) {
                                                                if(tempBallX < getmaxx()/2) {
                                                                        tempBallX++;
                                                                }
                                                        } else {
                                                                tempBallX--;
                                                        }
                                                }


                                        } else {
                                                buttonPressed = 2;
                                                lastButtonPressedState = 0;

                                                if(ball_vStatus == 0) {

                                                        int randomNumber = random(10) + 1;
                                                        if(randomNumber % 6) {
                                                                if(tempBallX < getmaxx()/2) {
                                                                        tempBallX++;
                                                                }
                                                        } else {
                                                                tempBallX--;
                                                        }
                                                }


                                        }
                                } else if(button == Key.ENTER) {
                                        int color = ball.getTerrain().getColor();
                                        //differentiating based on colors
                                        if(color == RED) {
                                                delay(350);
                                                Transition(tempBallX, tempBallY, WHITE)
                                                .rippleEffect(BLUE)
                                                .setDuration(5)
                                                .start(1, 9);

                                                return 1;
                                        } else if(color == LIGHTBLUE) {
                                                return 2;
                                        } else if(color == GREEN) {
                                                return 3;
                                        } else if(color == 13) {
                                                return 4;
                                        }

                                }
                        }

                }



                mTemp += speed;

                mGraphicsEngine.setScreen();
                cleardevice();
                setbkcolor(WHITE);

                //up
                if(buttonPressed == 1) {
                        //animate ball
                        //tempBallY--;
                        tempBallY -= 6;
                } else if(buttonPressed == 2) {
                        //tempBallY++;
                        tempBallY += 6;
                }






                //rendering the ball
                if(firstTime) {
                        ball = Ball(tempBallX, tempBallY, !lastButtonPressedState)
                               //  .setTerrain(testTerrain[2]) //default terrain for the first time
                               .setStrikeCoordinates(tempBallX - 9, tempBallY) //inserting a strike
                               .render();
                        firstTime = 0;
                } else {
                        //ball.makeAdjustments(tempBallX, tempBallY, !lastButtonPressedState).render();
                        ball.makeAdjustments(tempBallX, tempBallY, !lastButtonPressedState);
                        //ball.setY(tempBallY);
                        //ball.setX(tempBallX);
                        ball.render();
                        //setStrikeAdjustedYCoordinate(tempBallY)
                }

                ball_vStatus = 0;


                //Terrain(0 , getmaxy()/2 + 9, getmaxx(), getmaxy()/2 + 9).render(mTemp);
                for(int i = 0; i < 8; i++) {
                        testTerrain[i].render(mTemp);

                        //extra render for text
                        // if(i == 0) {
                        setcolor(WHITE);
                        int xFactor = (testTerrain[i].getFromX() + testTerrain[i].getToX())/2 - mTemp - 4*strlen(mTextArray[i]);
                        int xFactorDecrease = 0;
                        if(xFactor < 0) {
                                xFactorDecrease = 0 - xFactor;
                        }
                        outtextxy((testTerrain[i].getFromX() + testTerrain[i].getToX())/2 - mTemp - 4*strlen(mTextArray[i]) + speed*xFactorDecrease, ((testTerrain[i].getFromY() + testTerrain[i].getToY())/2) - 2, mTextArray[i] + xFactorDecrease);

                        //terrain has to be redered first to check if it contains a ball on it
                        //repeatation - check for efficiency!

                        int diff = ball.isOnTerrain(testTerrain[i], buttonPressed);

                        if(diff) {
                                ball_status = 1;
                                //if its on the same terrain
                                if(!ball.getTerrain().equals(testTerrain[i])) {
                                        ball.setTerrain(testTerrain[i]);
                                        if(diff != NO_VALUE) {
                                                tempBallY += diff; //adjusting on terrain for higher frame rates (animation speed)
                                                ball.setY(tempBallY);
                                        }

                                        //only if its not the same terrain
                                }

                        }




                }

                //setcolor(RED);
                if(ball_status) {
                        ball_status = 0;
                        buttonPressed = 0;
                        /*
                           char str[128] = "BALL IS ON TERRAIN! buttonPressed = ";
                           char tempInt[3];
                           strcat(str, itoa(buttonPressed, tempInt, 10));
                           outtextxy(25, 25, str);
                         */


                        if(ball_vStatus == 0) {
                                if(timer.isTimeOver(5)) {
                                        //increase speed of user randomly 1 - 10
                                        int randomNumber = random(10) + 1;
                                        if(randomNumber % 5) {
                                                if(tempBallX < getmaxx()/2) {
                                                        tempBallX += 3;
                                                }
                                        } else {
                                                tempBallX -= 1;
                                        }

                                }
                        }



                } else {
                        /*
                           char str[128] = "BALL IS ON GRAVITY! buttonPressed = ";
                           char tempInt[3];
                           strcat(str, itoa(buttonPressed, tempInt, 10));
                           outtextxy(25, 25, str);
                         */
                        //user fell down from terrain
                        if(!buttonPressed) {
                                //gravity detection
                                if(lastButtonPressedState == 1) {
                                        buttonPressed = 1;
                                } else {
                                        buttonPressed = 2;
                                }
                        }

                }



                //detecting if the ball is gone or not
                if(ball.isOutOfScreen()) {
                        //break;
                        //reseting
                        mTemp = 0;
                        firstTime = 1;
                        tempBallY = getmaxy() - 50;
                        tempBallX = 100;
                        lastButtonPressedState = 0;
                        buttonPressed = 2;
                        microFirst = 1;
                }


                mGraphicsEngine.showScreen();
                delay(20);
                if(firstTime) {
                        continue;
                }
                if(microFirst) {
                        microFirst = 0;
                        getch();
                }

        }

}

class Colors {
int color;
char str[52];

public:
Colors() {
        color = -1;
}

Colors(int color) {
        this->color = color;
}

char *getColorName() {
        if(color == 0) {
                strcpy(str, "BLACK");
        } else if(color ==  1) {
                strcpy(str, "BLUE");
        } else if(color == 2) {
                strcpy(str, "GREEN");
        } else if(color ==  3) {
                strcpy(str, "CYAN");
        } else if(color == 4) {
                strcpy(str, "RED");
        } else if(color == 5) {
                strcpy(str, "MAGENTA");
        } else if(color == 6) {
                strcpy(str, "BROWN");
        } else if(color == 7) {
                strcpy(str, "LIGHTGRAY");
        } else if(color ==  8) {
                strcpy(str, "DARKGRAY");
        } else if(color == 9) {
                strcpy(str, "LIGHTBLUE");
        } else if(color ==  10) {
                strcpy(str, "LIGHTGREEN");
        } else if(color == 11) {
                strcpy(str, "LIGHTCYAN");
        } else if(color ==  12) {
                strcpy(str, "LIGHTRED");
        } else if(color ==  13) {
                strcpy(str, "LIGHTMAGENTA");
        } else if(color ==  14) {
                strcpy(str, "YELLOW");
        } else if(color == 15) {
                strcpy(str, "WHITE");
        } else {
                strcpy(str, "NULL");
        }

        return str;
}

};

void LeaderBoard() {
        cleardevice();
        setbkcolor(BLUE);
        setcolor(WHITE);
        settextstyle(0, 0, 3);
        outtextxy(30, 30, "LEADERBOARD");
        settextstyle(11, 0, 1);
        gotoxy(14, 7);
        setcolor(YELLOW);
        outtextxy(103, 85, "SN.");
        outtextxy(167, 85, "NAME");
        outtextxy(280, 85, "SCORE");
        outtextxy(385, 85, "BALL");
        setcolor(WHITE);
        gotoxy(14, 9);
        int num_users;
        User *users = Player().retreiveDatabase(num_users);
        for(int i = 0; i < num_users; i++) {
                cout << "#" << i+1 << "      " << users[i].name << "          " << users[i].score << "         ";
                Ball(getmaxx()/2 + 80, 119 + 28*i, -1, users[i].color).setStrikeCoordinates(getmaxx()/2 + 9, getmaxy()/2 - 14).render();
                cout << endl;
                gotoxy(14, 11 + 2*i);
        }

//deallocating
        delete users;
}


char playerName[24];
int playerColor = BLUE;

void DetailsEntry(int max = 4) {
/*
   mGraphicsEngine.setScreen();
   cleardevice();
   setbkcolor(BLUE);
   mGraphicsEngine.showScreen();
   delay(1000);
 */
        for(int i = 0; i < max; i++) {
                num_players += i;
                cleardevice();
                setbkcolor(BLUE);
                gotoxy(20, 7);
                cout << "PLAYER #" << i;
                gotoxy(25, 10);
//char name[24];
                cout << "Enter name: ";
                gets(playerName);
                player[i].setName(playerName);
//player[0].setName(name);
                gotoxy(27, 12);
                cout << "Select ball color: ";
                int color = 1;
                while(1) {
                        char ch = getch();
                        if(ch == 'w') {
                                gotoxy(1, 14);
                                for(int i = 0; i < 50; i++) {
                                        gotoxy(i, 14);
                                        cout << "\b ";
                                }
                                gotoxy(29, 14);
                                cout << '(' << Colors(color).getColorName() << ')';
                                Ball(getmaxx()/2 + 48, getmaxy()/2 - 14, -1, color).setStrikeCoordinates(getmaxx()/2 + 48 + 9, getmaxy()/2 - 14).render();
                                playerColor = color;
//player[0].setColor(color);
                                color++;
                                if(color == 15) {
                                        color = 1;
                                }
                        } else {
                                break;
                        }

                }
                player[i].setColor(color);
                gotoxy(37, 20);
                if(num_players != 4) {
                        cout << "PRESS \'N'\ FOR NEXT PLAYER TO JOIN";
                        char ch = getch();
                        if(ch == 'n' || ch == 'N') {
                                continue;
                        } else {
                                break;
                        }
                }


        }
        cleardevice();
        setbkcolor(BLUE);
        cout << "Press any key to start the game...";
        getch();
}

char *concatenateCount(const char *scoreName, int count) {
        char *temp = "";
        strcat(temp, scoreName);
        strcat(temp, " (");
        char tempCount[6];
        itoa(count, tempCount, 10);
        strcat(temp, tempCount);
        strcat(temp, ")");
        return temp;
}



void main() {
        int graphicDriver = Graphics.DRIVER, graphicMode = Graphics.MODE;
        int err, midx, midy;
        initgraph(&graphicDriver, &graphicMode, "C:\\TC\\BGI");
        err = graphresult();
        if (err != grOk) {
                printf("Graphic Error: %s\n", grapherrormsg(err));
                getch();
                return;
        }
/*
   Player tempPlayer;
   tempPlayer.setScore(10000);
   tempPlayer.setColor(RED);
   tempPlayer.setName("Vish");
   tempPlayer.insert();
   cout << "Insert completed!" << tempPlayer.getScore() << ' ' << tempPlayer.getName() << ' ' << Colors(tempPlayer.getColor()).getColorName();
   LeaderBoard();
   getch();
   exit(0);
 */

        /* mid position in x and y-axis */
        midx = getmaxx() / 2;
        midy = getmaxy() / 2;

        int t = 0;
        int mPos = 0;
        long mTemp = 0;
        char button;



/*
      char ch = getch();
      cout << (int) ch;
      delay(1000);
      getch();
      return;
 */
        //loading all the terains

        num_terrains = TerrainsDatabase(terrain, Folder.LEVEL_START, Dat.TERRAIN).fetchRecords(MAX_TERRAINS);
        if(num_terrains == -1) {
                num_terrains = 0;
        }

        num_collectibles = CollectiblesDatabase(collectible, Folder.LEVEL_START, Dat.COLLECTIBLE).fetchRecords(MAX_COLLECTIBLES);
        if(num_collectibles == -1) {
                num_collectibles = 0;
        }

//levelEditor(mTemp);

        do {

                int choice = start();
                if(choice == 1) {
                        DetailsEntry(1);
                } else if(choice == 2) {
                        DetailsEntry();
                } else if(choice == 3) {
                        LeaderBoard();
                } else if(choice == 4) {
                        exit(0);
                } else if(choice == -1) {
                        levelEditor(mTemp);
                }


                int singlePlayer = 0;
                int firstTime = 1;

                if(singlePlayer) {
                        //debugging AKA Beta TESTING
                        //NULL STATEMENT
                } else {
//========================GAME LOOP MULTIPLAYER===============================
                        //Time implementation
                        Time timer = Time();
                        time_t start;
                        time(&start);
                        time_t pausedTimeStart, pausedTimeEnd;
                        int pausedTime = 0;
                        int quit = 0;
                        int dead = 0;
                        mTemp = 0;

                        char button[8];
                        int innerFirstTime = 1;
                        int buttonSize = 2*num_players;
                        int anim = 1;

                        while(1) {
                                //break;
                                buttonSize = 2*(num_players - dead);
                                //Accepting muliple inputs for simultaneous action experience
                                for(int buttonIndex = 0; buttonIndex < buttonSize; buttonIndex++) {
                                        //reset & accept if hit
                                        button[buttonIndex] = Key.INVALID;
                                        if(kbhit()) {
                                                button[buttonIndex] = getch();
                                        }
                                }

                                //Gradual speed increse
                                if(timer.isTimeOver(SPEED_INCREASE_INTERVAL)) {
                                        if(speed < MAX_SPEED) {
                                                speed++;
                                        }
                                }


                                if(!anim) {
                                        mTemp += speed;
                                }


                                //setpage();
                                mGraphicsEngine.setScreen();
                                cleardevice();


                                setbkcolor(WHITE);
                                //setbkcolor(BLACK);



//multiplayer loop ie Individuality check (iteration)
                                for(int livePlayer = 0; livePlayer < num_players; livePlayer++) {
//skipping iteration if dead
                                        if(!player[livePlayer].isAlive()) {
                                                continue;
                                        }
//retreiving
                                        int tempBallY = player[livePlayer].getTempBallY();
                                        int tempBallX = player[livePlayer].getTempBallX();
                                        Ball ball = player[livePlayer].getBall();
                                        int ball_vStatus = player[livePlayer].getBallVStatus();
                                        int ball_status = player[livePlayer].getBallStatus();
                                        int score = player[livePlayer].getScore();
                                        int buttonPressed = player[livePlayer].getButtonPressed();
                                        int lastButtonPressedState = player[livePlayer].getLastButtonPressedState();
                                        int ballColor = player[livePlayer].getBallColor();
                                        int *points = player[livePlayer].getPoints();



//(((button1 == 'w' || button1 == 'W') && livePlayer == 0) || (button1 == Key.UP && livePlayer == 1))

                                        for(int buttonIndex = 0; buttonIndex < buttonSize; buttonIndex++) {
                                                char buttonHit = button[buttonIndex];
                                                if(buttonHit == Key.ESCAPE) {
                                                        quit = 1;
                                                } else if(buttonHit == 'p' || buttonHit == 'P') {
                                                        Transition(getmaxx()/2, getmaxy()/2, WHITE).rippleEffect(BLUE)
                                                        .setDuration(2)
                                                        .onlyFirstPartOfStart();
                                                        setbkcolor(BLUE);
                                                        char cheat[64];
                                                        int inner = 0;
                                                        int result = -1;
                                                        time(&pausedTimeStart);
                                                        while(1) {
                                                                mGraphicsEngine.setScreen();
                                                                cleardevice();
                                                                settextstyle(0, 0, 3);
                                                                setcolor(WHITE);
                                                                outtextxy(30, 30, "PAUSED!");
                                                                settextstyle(0, 0, 2);
                                                                outtextxy(getmaxx()/2 - 175, getmaxy()/2 - 15, "Press 'p' to continue...");
                                                                if(result != -1) {
                                                                        result = -1;
                                                                        inner = 0;
                                                                        setcolor(YELLOW);
                                                                        outtextxy(getmaxx()/2 - 100, getmaxy()/2 + 45, "CHEAT ACTIVATED!");
                                                                }
                                                                settextstyle(11, 0, 1);
                                                                mGraphicsEngine.showScreen();
                                                                cheat[inner] = getch();
                                                                if(cheat[inner] == 'p' || cheat[inner] == 'P') {
                                                                        break;
                                                                }
                                                                inner++;
                                                                cheat[inner] = '\0';
                                                                result = mCheatEngine.isValidCheat(cheat);
                                                                if(result != -1) {
                                                                        cheatsUsed = 1;
                                                                        if(result == 1) {
                                                                                speed = 1;
                                                                        } else if(result == 2) {
                                                                                speed = 15;
                                                                        } else if(result == 3) {
                                                                                speed = 10;
                                                                        }
                                                                }

                                                        }
                                                        time(&pausedTimeEnd);
                                                        pausedTime += (int) (pausedTimeEnd - pausedTimeStart);
                                                        setbkcolor(WHITE);
                                                }

                                                if(!buttonPressed) {
                                                        if(((buttonHit == 'w' || buttonHit == 'W') && livePlayer == 0) || (buttonHit == Key.UP && livePlayer == 1) || ((buttonHit == 'm' || buttonHit == 'M') && livePlayer == 2) || ((buttonHit == 'n' || buttonHit == 'N') && livePlayer == 3)) {
                                                                if(lastButtonPressedState == 0) {
                                                                        buttonPressed = 1;
                                                                        lastButtonPressedState = 1;

                                                                        if(ball_vStatus == 0) {
                                                                                int randomNumber = random(10) + 1;
                                                                                if(randomNumber % 6) {
                                                                                        if(tempBallX < getmaxx()/2) {
                                                                                                tempBallX++;
                                                                                        }
                                                                                } else {
                                                                                        tempBallX--;
                                                                                }
                                                                        }


                                                                } else {
                                                                        buttonPressed = 2;
                                                                        lastButtonPressedState = 0;

                                                                        if(ball_vStatus == 0) {

                                                                                int randomNumber = random(10) + 1;
                                                                                if(randomNumber % 6) {
                                                                                        if(tempBallX < getmaxx()/2) {
                                                                                                tempBallX++;
                                                                                        }
                                                                                } else {
                                                                                        tempBallX--;
                                                                                }
                                                                        }


                                                                }
                                                                //if successful, ending to reduce invalid check for better performances
                                                                break;
                                                        }
                                                }

                                        }






                                        //rendering the ball
                                        if(firstTime) {
                                                ball = Ball(tempBallX, tempBallY, !lastButtonPressedState, ballColor)
                                                       .setTerrain(terrain[0]) //default terrain for the first time
                                                       .setStrikeCoordinates(tempBallX - 9, tempBallY) //inserting a strike
                                                       .render();
                                        } else {
                                                ball.makeAdjustments(tempBallX, tempBallY, !lastButtonPressedState, ballColor);
                                                ball.render();
                                        }

                                        ball_vStatus = 0;


                                        //Terrain(0 , getmaxy()/2 + 9, getmaxx(), getmaxy()/2 + 9).render(mTemp);
                                        for(int i = 0; i < num_terrains; i++) {
                                                if(innerFirstTime) {
                                                        terrain[i].render(mTemp);

                                                }
                                                //terrain has to be redered first to check if it contains a ball on it

                                                //repeatation - check for efficiency!
                                                int diff = ball.isOnTerrain(terrain[i], buttonPressed);

                                                if(diff) {
                                                        ball_status = 1;
                                                        //if its on the same terrain
                                                        if(!ball.getTerrain().equals(terrain[i])) {
                                                                buttonPressed = 0;
                                                                ball.setTerrain(terrain[i]);
                                                                if(diff != NO_VALUE) {
                                                                        tempBallY += diff; //adjusting on terrain for higher frame rates (animation speed)
                                                                        ball.setY(tempBallY);
                                                                }

                                                                //only if its not the same terrain
                                                        }

                                                }


                                                //detecting vertical part of the terrain hit collisions
                                                int update;
                                                diff = ball.hasHitVerticalPartOfTerrain(terrain[i], update);
                                                if(diff) {
                                                        tempBallX -= speed;
                                                        ball_vStatus = 1;

                                                        if(update == 1) {
                                                                tempBallX -= diff;
                                                        }

                                                }

                                        }

                                        //rendering the collectibles
                                        for(i = 0; i < num_collectibles; i++) {
                                                //to make sure its rendering only once
                                                if(innerFirstTime) {
                                                        collectible[i]->render(mTemp);
                                                }

                                                if(ball.hasHitCollectible(*collectible[i], ball_status, buttonPressed == 1)) {
                                                        //default actions
                                                        collectible[i]->setCollected();
                                                        score += collectible[i]->getPointValue();


                                                        //specific target actions callback listener
                                                        if(collectible[i]->getType() == Pickup.POSITIVE_POINT) {
                                                                points[Pickup.POSITIVE_POINT]++;
                                                        } else if(collectible[i]->getType() == Pickup.MEGA_POSITIVE_POINT) {
                                                                points[Pickup.MEGA_POSITIVE_POINT]++;
                                                        } else if(collectible[i]->getType() == Pickup.NEGATIVE_POINT) {
                                                                points[Pickup.NEGATIVE_POINT]++;

                                                        } else if(collectible[i]->getType() == Pickup.MEGA_NEGATIVE_POINT) {
                                                                points[Pickup.MEGA_NEGATIVE_POINT]++;

                                                        } else if(collectible[i]->getType() == Pickup.TELEPORT) {
                                                                points[Pickup.TELEPORT]++;

                                                                /*
                                                                   tempBallY =  collectible[i]->getNewY();
                                                                   tempBallX = collectible[i]->getNewX();
                                                                   mTemp = collectible[i]->get_New_mTemp();
                                                                 */
                                                        } else if(collectible[i]->getType() == Pickup.SUPER_MEGA_POSITIVE_POINT) {
                                                                points[Pickup.SUPER_MEGA_POSITIVE_POINT]++;

                                                        }

                                                }

                                        }


                                        if(ball_status) {
                                                ball_status = 0;
                                                buttonPressed = 0;
//making sure ball is not on vertical part of terrain to prevent incorrect collision detection
                                                if(ball_vStatus == 0) {
                                                        if(timer.isTimeOver(5)) {
                                                                //increase speed of user randomly 1 - 10
                                                                int randomNumber = random(10) + 1;
                                                                if(randomNumber % 5) {
                                                                        if(tempBallX < getmaxx()/2) {
                                                                                tempBallX += 3;
                                                                        }
                                                                } else {
                                                                        tempBallX -= 1;
                                                                }

                                                        }
                                                }

                                        } else {
                                                //user fell down from terrain
                                                if(!buttonPressed) {
                                                        //gravity detection
                                                        if(lastButtonPressedState == 1) {
                                                                buttonPressed = 1;
                                                        } else {
                                                                buttonPressed = 2;
                                                        }
                                                }

                                        }


                                        //ball collision
                                        //up
                                        if(buttonPressed == 1) {
                                                int update;
                                                for(int otherPlayer = 0; otherPlayer < num_players; otherPlayer++) {
                                                        if(player[otherPlayer].isAlive() && otherPlayer != livePlayer) {
                                                                int otherPlayerButtonPressed = player[otherPlayer].getButtonPressed();
                                                                if(ball.isBelowOtherBall(player[otherPlayer].getBall(), update)) {
                                                                        //if(otherPlayerButtonPressed == 2) {
                                                                        buttonPressed = 0;
                                                                        if(update) {
                                                                                tempBallY += update;
                                                                                //player[otherPlayer].setTempBallY(player[otherPlayer].getTempBallY() - update);
                                                                                //ball.setY(tempBallY);
                                                                        }
                                                                        if(otherPlayerButtonPressed == 2) {
                                                                                player[otherPlayer].setTempBallY(player[otherPlayer].getTempBallY() - 7);
                                                                                //player[otherPlayer].setButtonPressed(0);
                                                                                //exit(0);
                                                                        }
                                                                        // }
                                                                        //exit(0);
                                                                        break;
                                                                }
                                                        }
                                                }
                                        } else if(buttonPressed == 2) {
                                                int update;
                                                for(int otherPlayer = 0; otherPlayer < num_players; otherPlayer++) {
                                                        if(player[otherPlayer].isAlive() && otherPlayer != livePlayer) {
                                                                int otherPlayerButtonPressed = player[otherPlayer].getButtonPressed();
                                                                if(ball.isAboveOtherBall(player[otherPlayer].getBall(), update)) {
                                                                        // if(otherPlayerButtonPressed == 1) {
                                                                        buttonPressed = 0;
                                                                        //player[otherPlayer].setButtonPressed(0);
                                                                        if(update) {
                                                                                tempBallY -= update;
                                                                                //player[otherPlayer].setTempBallY(player[otherPlayer].getTempBallY() + update);
                                                                                //ball.setY(tempBallY);
                                                                        }
                                                                        if(otherPlayerButtonPressed == 1) {
                                                                                player[otherPlayer].setTempBallY(player[otherPlayer].getTempBallY() + 7);
                                                                                //player[otherPlayer].setButtonPressed(0);
                                                                                //tempBallY -= 13;
                                                                                //exit(0);
                                                                        }
                                                                        //}
                                                                }
                                                                break;
                                                        }
                                                }
                                        }


                                        if(!anim) {

                                                //up
                                                if(buttonPressed == 1) {
                                                        //Moving ball
                                                        //tempBallY--;
                                                        tempBallY -= 6;
                                                } else if(buttonPressed == 2) {
                                                        //tempBallY++;
                                                        tempBallY += 6;
                                                }


                                        }



                                        //detecting if the ball is gone or not
                                        if(ball.isOutOfScreen()) {
                                                player[livePlayer].setDead();
                                                time_t end;
                                                time(&end);
                                                player[livePlayer].setTimeTaken(((int) (end - start)) - pausedTime - 4);
                                                dead++;
                                        }


//setting it back
                                        player[livePlayer] = player[livePlayer].setTempBallY(tempBallY)
                                                             .setTempBallX(tempBallX)
                                                             .setBall(ball)
                                                             .setBallVStatus(ball_vStatus)
                                                             .setBallStatus(ball_status)
                                                             .setScore(score)
                                                             .setButtonPressed(buttonPressed)
                                                             .setLastButtonPressedState(lastButtonPressedState)
                                                             .setPoints(points);
                                        innerFirstTime = 0;
                                } //end of multiplayer individuality


                                if(anim) {
                                        settextstyle(0, 0, 10);
                                        if(anim == 1) {
                                                setcolor(RED);
                                                outtextxy(getmaxx()/2 - 25, getmaxy()/2 - 55, "3");
                                                anim++;
                                        } else if(anim == 2) {
                                                setcolor(GREEN);
                                                outtextxy(getmaxx()/2 - 25, getmaxy()/2 - 55, "2");
                                                anim++;
                                        } else if(anim == 3) {
                                                setcolor(BLUE);
                                                outtextxy(getmaxx()/2 - 25, getmaxy()/2 - 55, "1");
                                                anim++;
                                        } else if(anim == 4) {
                                                setcolor(LIGHTBLUE);
                                                outtextxy(getmaxx()/2 - 70, getmaxy()/2 - 55, "GO!");
                                                anim++;
                                        } else if(anim == 5) {
                                                anim = 0;
                                                delay(1000);
                                        }
                                        settextstyle(11, 0, 1);
                                        delay(1000);
                                }

                                if(!anim) {
                                        if(firstTime) {
                                                firstTime = 0;
                                        }
                                }
                                innerFirstTime = 1;
                                if(quit || dead == num_players) {
                                        break;
                                }
                                mGraphicsEngine.showScreen();

                                delay(20);

                        }



                }


                Transition(getmaxx()/2, getmaxy()/2, WHITE).rippleEffect(BLUE)
                .setDuration(5)
                .onlyFirstPartOfStart();

                char temp[12];
                for(int i = 0; i < num_players; i++) {
                        strcpy(playerName, player[i].getName());
                        strcat(playerName, " (");
                        itoa(i, temp, 10);
                        strcat(playerName, temp);
                        strcat(playerName, ")");
                        player[i].setName(playerName);

                        int *points = player[i].getPoints();

                        setcolor(WHITE);
                        settextstyle(0, 0, 1);
                        outtextxy(getmaxx()/2 - 3*strlen(player[i].getName()) - 5, 45, player[i].getName());
                        Ball(getmaxx()/2, 30, -1, player[i].getColor()).setStrikeCoordinates(getmaxx()/2 + 9, getmaxy()/2 - 14).render();
                        delay(250);
                        char temp[6];
                        int total = 0;

                        outtextxy(100, 75, "Distance travelled:");
                        itoa(mTemp/100, temp, 10);
                        outtextxy(260, 75, strcat(temp, "m"));
                        total += mTemp/100;
                        outtextxy(100, 95, "Cheats Used:");
                        if(cheatsUsed) {
                                outtextxy(200, 95, "-100");
                                total -= 100;
                        } else {
                                outtextxy(200, 95, "+100");
                                total += 100;
                        }
                        outtextxy(100, 115, "Time Taken:");
                        itoa(player[i].getTimeTaken(), temp, 10);
                        total += player[i].getTimeTaken();
                        outtextxy(190, 115, strcat(temp, " seconds"));
                        outtextxy(100, 135, "Collectibles:");

                        outtextxy(125, 160, concatenateCount("POSITIVE POINT", points[0]));
                        outtextxy(125, 185, concatenateCount("NEGATIVE POINT", points[1]));
                        outtextxy(125, 210, concatenateCount("MEGA POSITIVE POINT", points[2]));
                        outtextxy(125, 235, concatenateCount("MEGA NEGATIVE POINT", points[3]));
                        outtextxy(125, 260, concatenateCount("TELEPORTS USED", points[4]));
                        outtextxy(125, 285, concatenateCount("SUPER MEGA POSITIVE POINT", points[5]));

                        int pointValues[6];
                        pointValues[Pickup.POSITIVE_POINT] = Collectible().getPointValue();
                        pointValues[Pickup.NEGATIVE_POINT] = NegativePoint().getPointValue();
                        pointValues[Pickup.MEGA_POSITIVE_POINT] = MegaPositivePoint().getPointValue();
                        pointValues[Pickup.MEGA_NEGATIVE_POINT] = MegaNegativePoint().getPointValue();
                        pointValues[Pickup.TELEPORT] = TeleportPickup().getPointValue();
                        pointValues[Pickup.SUPER_MEGA_POSITIVE_POINT] = SuperMegaPositivePoint().getPointValue();

                        for(int i = 0; i < 6; i++) {
                                outtextxy(125 + 250, 160 + 25*i, "X");
                                itoa(pointValues[i], temp, 10);
                                outtextxy(125 + 300, 160 + 25*i, temp);
                                outtextxy(125 + 350, 160 + 25*i, "=");
                                itoa(pointValues[i]*points[i], temp, 10);
                                outtextxy(125 + 400, 160 + 25*i, temp);
                                total += pointValues[i]*points[i];
                                delay(350);
                        }
                        line(125 + 380, 160 + 145, 125 + 430, 160 + 145);
                        outtextxy(125 + 250, 160 + 150, "TOTAL SCORE");
                        outtextxy(125 + 350, 160 + 150, "=");
                        itoa(total, temp, 10);
                        outtextxy(125 + 390, 160 + 150, temp);
                        line(125 + 380, 160 + 160, 125 + 430, 160 + 160);

                        player[i].setScore(total);

                        //inserting into database
                        player[i].insert();
                        getch();
                        cleardevice();
                        setbkcolor(BLUE);
                }
                outtextxy(getmaxx()/2 - 140, getmaxy() - 15, "Press \'y\' to start the game again...");

/*
              new Collectible(150, 160)->render();
              new NegativePoint()->setCoordinates(150, 185)->render();
              new MegaPositivePoint()->setCoordinates(150, 210)->render();
              new MegaNegativePoint()->setCoordinates(150, 235)->render();
              new SuperMegaPositivePoint()->setCoordinates(150, 260)->render();
              new TeleportPickup()->setCoordinates(150, 285)->render();
 */
                settextstyle(11, 0, 3);
                continueGame = getch();
        } while(continueGame == 'y' || continueGame == 'Y');


        closegraph();

//saving
        TerrainsDatabase(terrain, Folder.LEVEL_START, Dat.TERRAIN).setRecords(num_terrains);
//VerticalTerrainsDatabase(mVerticalTerrain, Folder.LEVEL_START, Dat.VERTICAL_TERRAIN).setRecords(num_vertical_terrains);
        CollectiblesDatabase(collectible, Folder.LEVEL_START, Dat.COLLECTIBLE).setRecords(num_collectibles - 31);
}

void render(long mTemp) {
//setpage();
        mGraphicsEngine.setScreen();
        cleardevice();
//rendering the horizontal terrains
        for(int i = 0; i < num_terrains; i++) {
                terrain[i].render(mTemp);
        }

        for(i = 0; i < num_collectibles; i++) {
                collectible[i]->render(mTemp);
        }

//showpage();
        mGraphicsEngine.showScreen();
        delay(20);
}

void levelEditor(long mTemp) {
//level editor screen
        setbkcolor(WHITE);
        int mode = 0;
        int alreadyClicked = 0;
        int edit = -1;


//default tool is terrain
        int tool = 0;

        while(1) {
                if(kbhit()) {
                        char ch = getch();

//general case
                        if(ch == Key.BACKSPACE) {
                                mTemp = 0;
                        } else if(ch == 'c' || ch == 'C') {
                                tool = 1;
                        } else if(ch == 't' || ch == 'T') {
                                tool = 0;
                        } else if(ch == 'v' || ch == 'V') {
                                tool = 2;
                        } else if(ch == 'p' || ch == 'P') {
                                tool = 3; //points
                        } else if(ch == 'n' || ch == 'N') {
                                tool = 4; //points
                        } else if(ch == 's' || ch == 'S') {
                                tool = 5; //points
                        } else if(ch == 'm' || ch == 'M') {
                                tool = 6; //points
                        } else if(ch == ',') {
                                tool = 7; //points
                        } else if(ch == 'r' || ch == 'R') {
                                //refresh without moving (refresh)
                                render(mTemp);
                        } else if(ch == '2') {
                                //immediately switch back to game mode
                                ::mode = 1;
                                return;
                        } else {

                                if(mode == 1) {
                                        if(ch == 'd' || ch == 'D') {
                                                mTemp += speed;
                                                render(mTemp);
                                        } else if(ch == 'a' || ch == 'A') {
                                                mTemp -= speed;
                                                render(mTemp);
                                        } else if(ch == '0') {
                                                mode = 0;
                                                setbkcolor(WHITE);
                                        } else {
                                                return;
                                                //break;
                                        }
                                } else {
                                        if(ch == '1') {
                                                mode = 1;
                                                setbkcolor(BLACK); //better mouse visibility for editing
                                        } else {
                                                return;
                                                //break;
                                        }

                                }
                        }

                }


                if(mode == 0) {
                        mTemp += speed;
                        render(mTemp);
                } else if(mode == 1) {
//editor mode
//implementing mouse pointer on screen
                        Mouse mouse = Mouse();

                        int mouseX, mouseY, mouseButton;
                        mouse.getPos(mouseX, mouseY, mouseButton);

                        mouseX += mTemp;

                        if(mouseButton == 1) { //LMB
//cout << "mouseX: " << mouseX << " mouseY: " << mouseY << endl;
                                getch();
                                if(alreadyClicked == 0) {
                                        alreadyClicked = 1;

                                        if(tool == 0) {
//left to right only for now
                                                terrain[num_terrains] = Terrain(mouseX, mouseY, mouseX + 20, mouseY + 20);
                                                num_terrains++;
                                                edit = num_terrains - 1;
                                        } else if(tool == 1) {
//collisions small dp
                                                terrain[num_terrains] = Terrain(mouseX, mouseY, mouseX + 1, mouseY + 5);
                                                num_terrains++;
                                                edit = num_terrains - 1;
                                        } else if(tool == 2) {
                                                terrain[num_terrains] = Terrain(mouseX, mouseY, mouseX + 20, mouseY + 40);
                                                num_terrains++;
                                                edit = num_terrains - 1;
                                        } else if(tool == 3) {
                                                collectible[num_collectibles] = new Collectible()->setCoordinates(mouseX, mouseY);
                                                num_collectibles++;
                                        } else if(tool == 4) {
                                                collectible[num_collectibles] = new NegativePoint()->setCoordinates(mouseX, mouseY);
                                                num_collectibles++;
                                        } else if(tool == 5) {
                                                collectible[num_collectibles] = new SuperMegaPositivePoint()->setCoordinates(mouseX, mouseY);
                                                num_collectibles++;
                                        } else if(tool == 6) {
                                                collectible[num_collectibles] = new MegaNegativePoint()->setCoordinates(mouseX, mouseY);
                                                num_collectibles++;
                                        } else if(tool == 7) {
                                                collectible[num_collectibles] = new TeleportPickup()->setCoordinates(mouseX, mouseY);
                                                num_collectibles++;
                                        }


                                } else {
//save
                                        alreadyClicked = 0;
                                        delay(100);
                                }

                        } else if(mouseButton == 0) {
//preview
                                if(alreadyClicked == 1) {

                                        if(tool == 0) {
                                                //reinvoking constructor
                                                terrain[edit] = Terrain(terrain[edit].getFromX(), terrain[edit].getFromY(), mouseX, terrain[edit].getFromY() + 20);
                                        } else if(tool == 1) {
//collisions small dp
                                                terrain[edit] = Terrain(terrain[edit].getFromX(), terrain[edit].getFromY(), mouseX, mouseY);
                                        } else if(tool == 2) {
//vertical terrain update
                                                terrain[edit] = Terrain(terrain[edit].getFromX(), terrain[edit].getFromY(), terrain[edit].getFromX() + 20, mouseY);
                                        }

                                }
                        } else if(mouseButton == 2) { //RMB

//if not in preview mode
                                if(alreadyClicked == 0) {
                                        int check = 0;
                                        for(int i = 0; i < num_terrains; i++) {
                                                if(terrain[i].containsPoint(mouseX, mouseY)) {
                                                        for(int j = i; j < num_terrains - 1; j++) {
                                                                terrain[j] = terrain[j+1];
                                                        }
                                                        num_terrains--;
                                                        check = 1;
                                                        break;
                                                }
                                        }
                                        if(check) {
                                                cout << "Terrain removed!" << check;
                                                getch();
                                        }
                                }

                        } else if(mouseButton == 4) { //CMB || MMB

                                if(alreadyClicked == 0) {
                                        for(int i = 0; i < num_terrains; i++) {
                                                if(terrain[i].containsPoint(mouseX, mouseY)) {
                                                        alreadyClicked = 1;
                                                        edit = i;
                                                        cout << "Terrain selected, move to start editing!";
                                                        getch();
                                                        break;
                                                }
                                        }
                                }

                        }





                }



        }




}

/*
To get the updated version visit:
https://github.com/shiv-sankar/Gravity-Ball
*/
