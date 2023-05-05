#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace sf;
RenderWindow window(VideoMode(600,400), "ARKANOID");
CircleShape ball;
const float speedBall=3;
const float fastSpeedBall=4.5;
const float speedVaus=4;
Vector2f ballDirMove(-speedBall,-speedBall);
RectangleShape vaus;

enum BlockSide {
    None,
    Top,
    Left,
    Right
};

BlockSide checkCollision(const CircleShape& ball, const RectangleShape& block)
{
    FloatRect ballBounds = ball.getGlobalBounds();
    FloatRect blockBounds = block.getGlobalBounds();

    if (ballBounds.intersects(blockBounds)) {
        if (ball.getPosition().y + ball.getRadius() < block.getPosition().y) {
            return BlockSide::Top;
        }
        else if (ball.getPosition().x + ball.getRadius() > block.getPosition().x + block.getSize().x / 2) {
            return BlockSide::Right;
        }
        else if (ball.getPosition().x - ball.getRadius() < block.getPosition().x - block.getSize().x / 2) {
            return BlockSide::Left;
        }
        else {
            return BlockSide::None;
        }
    }

    return BlockSide::None;
}
bool collisionCenterBlock(const CircleShape& ball, const RectangleShape& block)
{
    if (abs(ball.getPosition().x - vaus.getPosition().x) < vaus.getSize().x / 2) {
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

int main()
{
    //window
    
    window.setFramerateLimit(60);
    //ball
    ball=CircleShape(5.f);
    ball.setOrigin(2.5f,2.5f);
    ball.setFillColor(Color::Magenta);
    ball.setPosition(window.getSize().x/2,window.getSize().y/2);
    //vaus
    vaus=RectangleShape(Vector2f(window.getSize().x/9,10)); 
    vaus.setFillColor(Color::Cyan);
    vaus.setPosition(window.getSize().x/2,window.getSize().y*0.85);
    vaus.setOrigin(vaus.getSize().x/2,vaus.getSize().y/2);

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
        switch (checkCollision(ball,vaus)){
            case BlockSide::Top:
                if (collisionCenterBlock(ball,vaus)){
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*speedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*-speedBall;
                }
                else{
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*fastSpeedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*-fastSpeedBall;
                }
                break;
            case BlockSide::Left:
                // handle left collision
                if (collisionCenterBlock(ball,vaus)){
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*-speedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*speedBall;
                }
                else{
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*-fastSpeedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*fastSpeedBall;
                }
                break;
            case BlockSide::Right:
                // handle right collision
                if (collisionCenterBlock(ball,vaus)){
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*-speedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*speedBall;
                }
                else{
                    ballDirMove.x=ballDirMove.x/abs(ballDirMove.x)*-fastSpeedBall;
                    ballDirMove.y=ballDirMove.y/abs(ballDirMove.y)*fastSpeedBall;
                }
                break;
            case BlockSide::None:
                // handle no collision
                break;
        }
        ball.move(ballDirMove.x,ballDirMove.y);
        window.clear();
        window.draw(ball);
        window.draw(vaus);
        window.display();
        
    }

    return 0;
}
