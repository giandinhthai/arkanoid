#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
using namespace sf;

//ball
#define speedBall 1
#define fastSpeedBall 1.5
//vaus
#define speedVaus 2
//walls
#define numRow 3
#define numCol 5
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
        shape.setOrigin(radius,radius);
        shape.setFillColor(Color::Magenta);
        shape.setPosition(window.getSize().x/2,window.getSize().y/2);
        dirMove=Vector2f(-speedBall,-speedBall);
    }
    void draw(RenderWindow& window){
        window.draw(shape);
    }
    bool outWindow(RenderWindow& window){
        return (shape.getPosition().y + shape.getRadius()>=window.getSize().y);
            
    }
    void move(RenderWindow& window){
        if(shape.getPosition().x + shape.getRadius() >= window.getSize().x 
        || shape.getPosition().x - shape.getRadius() <=0){
            dirMove.x*=-1;
        }
        if(shape.getPosition().y - shape.getRadius() <= 0  ){
            dirMove.y*=-1;
        }
        shape.move(dirMove);
    }
    void normalSpeed(int positiveX,int positiveY){
        dirMove=Vector2f(positiveX*speedBall,positiveY*speedBall);
    }
    void fastSpeed(int positiveX,int positiveY){
        dirMove=Vector2f(positiveX*fastSpeedBall,positiveY*fastSpeedBall);
    }
    void changeDir(int positiveX,int positiveY){
        dirMove.x=positiveX*abs(dirMove.x);
        dirMove.y=positiveY*abs(dirMove.y);
    }
    Vector2f getDirMove(){
        return dirMove;
    }
    CircleShape* getShape(){
        return &(this->shape);
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
    bool draw(RenderWindow& window){
        if (appear){
            window.draw(shape);
            return true;
        }
        return false;
    }
    BlockSide checkCollision(Ball& ball,bool& center){
        CircleShape* ballShape=ball.getShape();
        FloatRect ballBounds = ballShape->getGlobalBounds();
        FloatRect blockBounds = shape.getGlobalBounds();
        center=false;
        if (ballBounds.intersects(blockBounds)) {
            center=(abs(ballShape->getPosition().x - shape.getPosition().x) < shape.getSize().x / 4);
            if (ballShape->getPosition().y  > shape.getPosition().y+shape.getSize().y/2) {
                return BlockSide::Bottom;
            }
            else if (ballShape->getPosition().x  > shape.getPosition().x+shape.getSize().x/2 ) {
                return BlockSide::Right;
            }
            else if (ballShape->getPosition().x < shape.getPosition().x-shape.getSize().x/2) {
                return BlockSide::Left;
            }
            else if (ballShape->getPosition().y  < shape.getPosition().y - shape.getSize().y/2) {
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
    void moveLeft(RenderWindow& window){
        if (shape.getPosition().x-speedVaus>=shape.getSize().x/2){
            shape.move(-speedVaus,0);
        }
    }
    void moveRight(RenderWindow& window){
        if (shape.getPosition().x+speedVaus<window.getSize().x-shape.getSize().x/2){
            shape.move(speedVaus,0);
        }
    }
};
class Wall:public Block{
public:
    Wall(Vector2f size,Vector2f position): Block(size,Color::Blue){
        shape.setOutlineColor(Color::Green);
        shape.setOutlineThickness(2);
        shape.setPosition(position);
    }
    BlockSide checkCollision(Ball& ball,bool& center) {
        if (!appear){
            return BlockSide::None;
        }
        BlockSide ans=Block::checkCollision(ball,center);
        if( ans!= BlockSide::None){
            appear=false;
        }
        return ans;
    }
};
class Walls{
private:
   vector<vector<Wall>> walls;
public:
    Walls(RenderWindow& window) {
        
        for (int row = 0; row < numRow; row++) {
            vector<Wall> crRow;
            for (int col = 0; col < numCol; col++) {
                Vector2f size = Vector2f(window.getSize().x / numCol, window.getSize().y / 3 / numRow);
                Vector2f position = Vector2f((col + 0.5f) * size.x, (row + 0.5f) * size.y);
                Wall wall(size, position);
                crRow.push_back(wall);
            }
            walls.push_back(crRow);
        }
    }
    bool draw(RenderWindow& window){
        bool draw=false;
        for (int row = 0; row < numRow; row++) {
            for (int col = 0; col < numCol; col++) {
                if (walls[row][col].draw(window)){
                    draw=true;
                }
            
            }
        }
        return draw;
    }
    void checkCollision(Ball& ball){
        for (int row = 0; row < numRow; row++) {
            for (int col = 0; col < numCol; col++) {
                bool center=false;
                
                BlockSide collisionWall=walls[row][col].checkCollision(ball,center);
                if (collisionWall!=BlockSide::None){
                    handleCollisionWall(collisionWall,ball);
                }
            }
        }
    }
    void handleCollisionWall(BlockSide collision,Ball& ball){
        int positiveX= (ball.getDirMove().x>0)? 1:-1;
        int positiveY= (ball.getDirMove().y>0)? 1:-1;
        switch (collision){
            case BlockSide::Top:
                if (positiveY==1) ball.changeDir(positiveX,-1);
                break;
            case BlockSide::Left:
                if (positiveX==1) ball.changeDir(-1,positiveY);
                break;
            case BlockSide::Right:
                if (positiveX==-1) ball.changeDir(1,positiveY);
                break;
            case BlockSide::Bottom:
                if (positiveY==-1) ball.changeDir(positiveX,1);
                break;
            case BlockSide::None:
                break;
        }
    }
};
class Game{
private:
    RenderWindow window;
    Ball ball;
    Vaus vaus;
    Walls walls;
public:
    Game(): window(VideoMode(600, 400), "ARKANOID"),
    ball(5.f,window),
    vaus(Vector2f(window.getSize().x/6,10),window),
    walls(window) {
        window.setFramerateLimit(100);
    }
    void draw(){
        window.clear();
        ball.draw(window);
        vaus.draw(window);
        if(!walls.draw(window)){
            notifi("you win!");
        }
        window.display();
    }
    void run(){
        while (window.isOpen())
        {
            
            if (ball.outWindow(window)){
                notifi("you lose");
                return;
            }
            Event event;
            while (window.pollEvent(event))
            {
                if(event.type == Event::Closed){
                    window.close();
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Right)){
                vaus.moveRight(window);
            }
            if (Keyboard::isKeyPressed(Keyboard::Left)){
                vaus.moveLeft(window);
            }
            bool center=false;
            BlockSide collisionVaus = vaus.checkCollision(ball,center);
            if (collisionVaus!=BlockSide::None&&collisionVaus!=BlockSide::Bottom){
                handleCollisionVaus(collisionVaus,center,ball);
            }
            walls.checkCollision(ball);
            ball.move(window);
            draw();
        }
    }
    
    void handleCollisionVaus(BlockSide collision,bool center,Ball& ball){
        int positiveX= (ball.getDirMove().x>0)? 1:-1;
        int positiveY= (ball.getDirMove().y>0)? 1:-1;
        switch(collision){
            case BlockSide::Top:
                if (center){
                    ball.normalSpeed(positiveX,-1);
                }
                else{
                    ball.fastSpeed(positiveX,-1);
                }
                break;
            case BlockSide::Left:
                if (center){
                    ball.normalSpeed(-1,positiveY);
                }
                else{
                    ball.fastSpeed(-1,positiveY);
                }
                break;
            case BlockSide::Right:
                if (center){
                    ball.normalSpeed(1,positiveY);
                }
                else{
                    ball.fastSpeed(1,positiveY);
                }
                break;
            case BlockSide::Bottom:
            case BlockSide::None:
                break;
        }
    }
    void notifi(string mess){
        Font font;
        font.loadFromFile("fonts/Sigmar-Regular.ttf");

        Text text(mess, font, 50);
        text.setPosition(window.getSize().x / 2 - text.getLocalBounds().width / 2, window.getSize().y / 2 - text.getLocalBounds().height / 2);
        text.setFillColor(Color::Green);
        window.clear();
        window.draw(text);
        window.display();
        sleep(seconds(3));
    }
};
int main(){
    
    Game game;
    game.run();
    return 0;
}