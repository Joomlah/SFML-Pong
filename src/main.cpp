#include <iostream>
#include <sstream>
#include "SFML/Graphics.hpp"

const unsigned short int WIDTH = 800;
const unsigned short int HEIGHT = 600;


const unsigned short int BALL_X_SIZE = 25;
const unsigned short int BALL_Y_SIZE = 25;

const float BALL_MOVE_SPEED = 2.5f;

unsigned short int leftScore = 0;
unsigned short int rightScore = 0;


sf::Vector2f moveBall(sf::Sprite b);

template <typename T>
std::string NumberToString ( T Number );


sf::FloatRect leftPadBoundingBox;
sf::FloatRect rightPadBoundingBox;
sf::FloatRect ballBoundingBox;

std::string leftScoreStr;
std::string rightScoreStr;
std::ostringstream convert;

std::string outputText = "SCORE " + leftScoreStr + " - " + rightScoreStr;

int main()
{
    const unsigned short int PADDLE_X_SIZE = 10;
    const unsigned short int PADDLE_Y_SIZE = 100;

    const unsigned short int PADDLE_TOP = 50;

    const unsigned short int SCREEN_OFFSET = 15;


    float leftPlayerXPos = SCREEN_OFFSET;

    float rightPlayerXPos = WIDTH - SCREEN_OFFSET * 2;

    const float MOVE_SPEED = 5.0f;


    sf::Texture paddleTexture;

    sf::Font bitter;

    sf::Sprite leftPlayerPaddle;
    sf::Sprite rightPlayerPaddle;
    sf::Sprite ball;



    paddleTexture.create(1,1);

    sf::Uint8* pixels = new sf::Uint8[4];
    paddleTexture.update(pixels);

    leftPlayerPaddle.setTexture(paddleTexture);
    leftPlayerPaddle.setTextureRect(sf::IntRect(0, 0, PADDLE_X_SIZE, PADDLE_Y_SIZE));
    leftPlayerPaddle.setColor(sf::Color(255, 255, 255, 255));
    leftPlayerPaddle.setOrigin(0, PADDLE_Y_SIZE / 2);
    leftPlayerPaddle.setPosition(leftPlayerXPos, HEIGHT / 2);

    sf::Vector2f leftPadPos = leftPlayerPaddle.getPosition();


    rightPlayerPaddle.setTexture(paddleTexture);
    rightPlayerPaddle.setTextureRect(sf::IntRect(0, 0, PADDLE_X_SIZE, PADDLE_Y_SIZE));
    rightPlayerPaddle.setColor(sf::Color(255, 255, 255, 255));
    rightPlayerPaddle.setOrigin(0, PADDLE_Y_SIZE / 2);
    rightPlayerPaddle.setPosition(rightPlayerXPos, HEIGHT / 2);

    sf::Vector2f rightPadPos = rightPlayerPaddle.getPosition();


    ball.setTexture(paddleTexture);
    ball.setTextureRect(sf::IntRect(0, 0, BALL_X_SIZE, BALL_Y_SIZE));
    ball.setColor(sf::Color(255, 255, 255, 255));
    ball.setOrigin(BALL_X_SIZE / 2, BALL_Y_SIZE / 2);
    ball.setPosition(WIDTH / 2, HEIGHT / 2);


    bitter.loadFromFile("Bitter-Regular.ttf");

    sf::Text text;

    text.setFont(bitter);

    leftScoreStr = NumberToString(leftScore);
    rightScoreStr = NumberToString(rightScore);

    std::cout << leftScore << " - " << rightScore << '\n';
    std::cout << leftScoreStr << " - " << rightScoreStr << '\n';

    outputText = "SCORE " + leftScoreStr + " - " + rightScoreStr;

    text.setString(outputText);
    text.setCharacterSize(24);
    text.setColor(sf::Color(255, 255, 255, 255));
    text.setPosition((WIDTH / 2) - 75, 15);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong");

    window.setFramerateLimit(120);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            leftPadPos.y -= MOVE_SPEED;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            leftPadPos.y += MOVE_SPEED;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            rightPadPos.y -= MOVE_SPEED;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            rightPadPos.y += MOVE_SPEED;
        }



        if (leftPadPos.y - PADDLE_TOP <= 0)
        {
            leftPadPos.y = PADDLE_TOP;
        }
        else if (leftPadPos.y + PADDLE_TOP >= HEIGHT)
        {
            leftPadPos.y = HEIGHT - PADDLE_TOP;
        }

        if (rightPadPos.y - PADDLE_TOP <= 0)
        {
            rightPadPos.y = PADDLE_TOP;
        }
        else if (rightPadPos.y + PADDLE_TOP >= HEIGHT)
        {
            rightPadPos.y = HEIGHT - PADDLE_TOP;
        }


        leftPadBoundingBox = leftPlayerPaddle.getGlobalBounds();
        rightPadBoundingBox = rightPlayerPaddle.getGlobalBounds();
        ballBoundingBox = ball.getGlobalBounds();

        leftPlayerPaddle.setPosition(leftPadPos.x, leftPadPos.y);
        rightPlayerPaddle.setPosition(rightPadPos.x, rightPadPos.y);

        ball.setPosition(moveBall(ball));

        text.setString(outputText);

        window.clear();
        window.draw(leftPlayerPaddle);
        window.draw(rightPlayerPaddle);
        window.draw(ball);
        window.draw(text);
        window.display();
    }

    return 0;
}


float xDir = 1;
float yDir = 1;

sf::Vector2f moveBall(sf::Sprite b)
{
    sf::Vector2f bPos = b.getPosition();

    if (bPos.x > WIDTH || bPos.x < 0)
    {
        // Ball hit the X edge of the screen. Reset from middle.
        if (bPos.x > WIDTH)
        {
            // Left player scores!
            std::cout << "LEFT SCORES!" << '\n';
            leftScore += 1;
            leftScoreStr = NumberToString(leftScore);
        }
        else if (bPos.x < 0)
        {
            // Right player scores!
            std::cout << "RIGHT SCORES!" << '\n';
            rightScore += 1;
            rightScoreStr = NumberToString(rightScore);
        }
        outputText = "SCORE " + leftScoreStr + " - " + rightScoreStr;

        bPos.x = WIDTH / 2;
        bPos.y = HEIGHT / 2;

        xDir *= -1;
    }
    else if (bPos.y > HEIGHT || bPos.y < 0)
    {
        yDir *= -1;
    }

    if (ballBoundingBox.intersects(leftPadBoundingBox) || ballBoundingBox.intersects(rightPadBoundingBox))
    {
        xDir *= -1;
    }

    bPos.x += BALL_MOVE_SPEED * xDir;
    bPos.y += BALL_MOVE_SPEED * yDir;

    return bPos;
}


template <typename T>
    std::string NumberToString ( T Number )
    {
        std::ostringstream ss;
        ss << Number;
        return ss.str();
    }
