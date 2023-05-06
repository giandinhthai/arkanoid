#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace sf;
RenderWindow window(VideoMode(600,400), "ARKANOID");
CircleShape ball;
const float speedBall=1;
const float fastSpeedBall=1.5;
const float speedVaus=2;
Vector2f ballDirMove(-speedBall,-speedBall);
RectangleShape vaus;
const int wallRow=5;
const int wallCol=10;
RectangleShape wall[wallRow][wallCol];
enum BlockSide {
    None,
    Top,
    Left,
    Right,
    Bottom
};

BlockSide checkCollision(const CircleShape& ball, const RectangleShape& block)
{
    FloatRect ballBounds = ball.getGlobalBounds();
    FloatRect blockBounds = block.getGlobalBounds();

    if (blockBounds.intersects(ballBounds)) {
        if (ball.getPosition().y + ball.getRadius() > block.getPosition().y - block.getSize().y / 2) {
            return BlockSide::Bottom;
        }
        else if (ball.getPosition().x + ball.getRadius() > block.getPosition().x + block.getSize().x / 2) {
            return BlockSide::Right;
        }
        else if (ball.getPosition().x - ball.getRadius() < block.getPosition().x - block.getSize().x / 2) {
            return BlockSide::Left;
        }
        else if (ball.getPosition().y - ball.getRadius() < block.getPosition().y + block.getSize().y/2) {
            return BlockSide::Top;
        }
        else {
            return BlockSide::None;
        }
    }

    return BlockSide::None;
}

bool collisionCenterBlock(const CircleShape& ball, const RectangleShape& block)
{
    if (abs(ball.getPosition().x - block.getPosition().x) < block.getSize().x / 4) {
        return true;
    }
    else {
        return false;
    }

    return false;
}

void loseGame(){
    Font font;
    font.loadFromFile("fonts/Sigmar-Regular.ttf"); // replace with your own font file

    Text text("You lose the game!", font, 50);
    text.setPosition(window.getSize().x / 2 - text.getLocalBounds().width / 2, window.getSize().y / 2 - text.getLocalBounds().height / 2);
    text.setFillColor(Color::Green);
    window.clear();
    window.draw(text);
    window.display();
    sleep(seconds(3));
}
void winGame(){
    Font font;
    font.loadFromFile("fonts/Sigmar-Regular.ttf"); // replace with your own font file

    Text text("You win!", font, 50);
    text.setPosition(window.getSize().x / 2 - text.getLocalBounds().width / 2, window.getSize().y / 2 - text.getLocalBounds().height / 2);
    text.setFillColor(Color::Green);
    window.clear();
    window.draw(text);
    window.display();
    sleep(seconds(3));
}

int main()
{
    //window
    
    window.setFramerateLimit(200);
    //ball
    ball=CircleShape(5.f);
    ball.setOrigin(2.5f,2.5f);
    ball.setFillColor(Color::Magenta);
    ball.setPosition(window.getSize().x/2,window.getSize().y/2);
    //vaus
    vaus=RectangleShape(Vector2f(window.getSize().x/3,50)); 
    vaus.setFillColor(Color::Cyan);
    vaus.setPosition(window.getSize().x/2,window.getSize().y*0.85);
    vaus.setOrigin(vaus.getSize().x/2,vaus.getSize().y/2);
    //wall
    bool appear[wallRow][wallCol];
    for (int row = 0; row < wallRow; row++) {
        for (int col = 0; col < wallCol; col++) {
            RectangleShape rect(Vector2f(window.getSize().x / wallCol, window.getSize().y / 3/wallRow));
            rect.setFillColor(Color::Blue);
            rect.setOutlineThickness(1.f);
            rect.setOutlineColor(Color::Cyan);
            rect.setOrigin(rect.getSize() / 2.f);
            rect.setPosition((col + 0.5f) * rect.getSize().x, (row + 0.5f) * rect.getSize().y);
            wall[row][col] = rect;
            appear[row][col]=true;
        }
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if(event.type == Event::Closed){
                window.close();
            }
        }
        if(ball.getPosition().x>=window.getSize().x || ball.getPosition().x <=0){
            ballDirMove.x*=-1;
        }
        if(ball.getPosition().y<=0 ){
            ballDirMove.y*=-1;
        }
        if(ball.getPosition().y>=window.getSize().y){
            loseGame();
            return 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)){
            if (vaus.getPosition().x<window.getSize().x-vaus.getSize().x/2){
                vaus.move(speedVaus,0);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)){
            if (vaus.getPosition().x>=vaus.getSize().x/2){
                vaus.move(-speedVaus,0);
            }
        }
        for(int row=0;row<wallRow;row++){
            for(int col=0;col<wallCol;col++){
                if(appear[row][col]){
                    if (checkCollision(ball,wall[row][col])) appear[row][col]=false;
                    switch (checkCollision(ball,wall[row][col])){
                        case BlockSide::Top:
                            if (ballDirMove.y>0) ballDirMove.y*=-1;
                            break;
                        case BlockSide::Left:
                            if (ballDirMove.x>0) ballDirMove.x*=-1;
                            break;
                        case BlockSide::Right:
                            // handle right collision
                            if (ballDirMove.x<0) ballDirMove.x*=-1;
                            break;
                        case BlockSide::Bottom:
                            if (ballDirMove.y<0) ballDirMove.y*=-1;
                            break;
                        case BlockSide::None:
                            // handle no collision
                            break;
                    }
                }
            }
        }
        switch (checkCollision(ball,vaus)){
            case BlockSide::Top:
                if (collisionCenterBlock(ball,vaus)){
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*speedBall;
                    ballDirMove.y=-speedBall;
                }
                else{
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*fastSpeedBall;
                    ballDirMove.y=-fastSpeedBall;
                }
                break;
            case BlockSide::Left:
                // handle left collision
                if (collisionCenterBlock(ball,vaus)){
                    ballDirMove.x=-speedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*speedBall;
                }
                else{
                    ballDirMove.x=-fastSpeedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*fastSpeedBall;
                }
                break;
            case BlockSide::Right:
                // handle right collision
                if (collisionCenterBlock(ball,vaus)){
                    ballDirMove.x=speedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*speedBall;
                }
                else{
                    ballDirMove.x=fastSpeedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*fastSpeedBall;
                }
                break;
            case BlockSide::Bottom:
            case BlockSide::None:
                // handle no collision
                break;
        }
        ball.move(ballDirMove.x,ballDirMove.y);
        window.clear();
        window.draw(ball);
        window.draw(vaus);
        bool win=true;
        for(int row=0;row<wallRow;row++){
            for(int col=0;col<wallCol;col++){
                if (appear[row][col]) {
                    window.draw(wall[row][col]);
                    win=false;
                }
            }
        }
        if (win){
            winGame();
            sleep(seconds(3));
            return 0;
        }
        window.display();
        
    }

    return 0;
}
