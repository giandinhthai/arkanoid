#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace sf;

//ball
#define speedBall 1
#define fastSpeedBall 1.5
//vaus
#define speedVaus 2
enum BlockSide {
    None,
    Top,
    Left,
    Right,
    Bottom
};

class Ball{
private:
    CircleShape shape;
    Vector2f dirMove;
public:
    Ball(float radius,const RenderWindow& window){
        shape=CircleShape(radius);
        shape.setOrigin(radius/2,radius/2);
        shape.setFillColor(Color::Magenta);
        shape.setPosition(window.getSize().x/2,window.getSize().y/2);
        dirMove=Vector2f(-speedBall,-speedBall);
    }
    void draw(RenderWindow& window){
        window.draw(shape);
    }
    void move(){
        shape.move(dirMove);
    }
};
class Block{
protected:
    RectangleShape shape;
    bool appear;
public:
    Block(Vector2f size, Color color,bool appear=true){
        shape=RectangleShape(size);
        shape.setFillColor(color);
        shape.setOrigin(shape.getSize().x/2,shape.getSize().y/2);
        this->appear=appear;
    }
    void draw(RenderWindow& window){
        if (appear){
            window.draw(shape);
        }
    }
    BlockSide checkCollision(const CircleShape& ball){
        FloatRect ballBounds = ball.getGlobalBounds();
        FloatRect blockBounds = shape.getGlobalBounds();

        if (blockBounds.intersects(ballBounds)) {
            if (ball.getPosition().y + ball.getRadius() > shape.getPosition().y - shape.getSize().y / 2) {
                return BlockSide::Bottom;
            }
            else if (ball.getPosition().x + ball.getRadius() > shape.getPosition().x + shape.getSize().x / 2) {
                return BlockSide::Right;
            }
            else if (ball.getPosition().x - ball.getRadius() < shape.getPosition().x - shape.getSize().x / 2) {
                return BlockSide::Left;
            }
            else if (ball.getPosition().y - ball.getRadius() < shape.getPosition().y + shape.getSize().y/2) {
                return BlockSide::Top;
            }
            else {
                return BlockSide::None;
            }
        }

        return BlockSide::None;
    }
};
class Vaus: public Block{
public:
    Vaus(Vector2f size,RenderWindow& window):Block(size,Color::Cyan){
        shape.setPosition(window.getSize().x/2,window.getSize().y*0.85);
    }
    void moveLeft(){
        shape.move(-speedVaus,0);
    }
    void moveRight(){
        shape.move(speedVaus,0);
    }
};
class Wall:public Block{
public:
    Wall(Vector2f size): Block(size,Color::Blue){
        shape.setOutlineColor(Color::Green);
        shape.setOutlineThickness(2);
    }
    BlockSide checkCollision()
};
class Game{
private:
    RenderWindow window;
    Ball ball;
    Vaus vaus;
public:
    Game(): window(VideoMode(600, 400), "ARKANOID"),
    ball(5.f,window),
    vaus(Vector2f(window.getSize().x/6,10),window) {
        // initialize ball and vaus objects here, if needed
        window.setFramerateLimit(100);
    }
    void draw(){
        window.clear();
        ball.draw(window);
        vaus.draw(window);
        window.display();
    }
    void run(){
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if(event.type == Event::Closed){
                    window.close();
                }
            }
            draw();
        }
    }
};
int main(){
    
    Game game;
    game.run();
    return 0;
}