#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(0,0), "Ping Pong", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);

    const unsigned int &width = sf::VideoMode::getDesktopMode().width;
    const unsigned int &height = sf::VideoMode::getDesktopMode().height;

    sf::CircleShape ball(10.0f);
    sf::RectangleShape player1(sf::Vector2f(width / 10, 10.0f)), player2(sf::Vector2f(width / 10, 10.0f)); // Paddle width is 1/10th of user's monitor size
    sf::SoundBuffer buffer1, buffer2, buffer3;
    sf::Sound soundPaddle, soundWall, soundScore;
    sf::Font font;
    sf::Text showP1Score, showP2Score;
    sf::Clock ballClock, hitClock;

    try {
        buffer1.loadFromFile("sound_paddle.wav");
        buffer2.loadFromFile("sound_wall.wav");
        buffer3.loadFromFile("sound_score.wav");
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl; // Writes about the error on the console, if any
    }
    
    soundPaddle.setBuffer(buffer1);
    soundWall.setBuffer(buffer2);
    soundScore.setBuffer(buffer3);

    ball.setFillColor(sf::Color::White);
    player1.setFillColor(sf::Color::White);
    player2.setFillColor(sf::Color::White);

    ball.setPosition((width / 2) - 10, (height / 2) - 10);
    player1.setPosition((width / 2) - (player1.getGlobalBounds().width / 2), height - 10);
    player2.setPosition((width / 2) - (player1.getGlobalBounds().width / 2), 0);

    font.loadFromFile("Emulogic-zrEw.ttf");
    showP1Score.setFont(font);
    showP2Score.setFont(font);
    showP1Score.setCharacterSize(24);
    showP2Score.setCharacterSize(24);
    showP1Score.setFillColor(sf::Color::White);
    showP2Score.setFillColor(sf::Color::White);
    showP1Score.setPosition(100, height * 2 / 3 - 10);  // 2/3rds of screen height
    showP2Score.setPosition(100, height / 3 - 10);  // 1/3rd of screen height

    float ballVelocityX = 10.0f, ballVelocityY = 10.0f;

    int P1Score = 0, P2Score = 0;

    while (window.isOpen())
    {
        // Score system setup
        std::string P1ScoreTS = std::to_string(P1Score), P2ScoreTS = std::to_string(P2Score);
        showP1Score.setString(P1ScoreTS);
        showP2Score.setString(P2ScoreTS);

        // Time delay before ball moves
        sf::Time ballTime = ballClock.getElapsedTime();
        if (ballTime.asSeconds() >= 1) {
            ball.move(ballVelocityX, ballVelocityY);
        }

        float ballX = ball.getPosition().x, ballY = ball.getPosition().y;

        // if ball hits wall
        if (ballX >= width - 20 || ballX <= 0) {
            ballVelocityX = -ballVelocityX;
            soundWall.play();
        }

        // if ball goes out of bounds
        if (ballY >= height || ballY <= -20) {
            ball.setPosition((width / 2) - 10, (height / 2) - 10);
            ballVelocityY = -ballVelocityY;
            soundScore.play();
            ballClock.restart();
            if (ballY <= -20.0f) { P1Score++; }
            else if (ballY >= height) { P2Score++; }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // Player controls:
        // Player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.getPosition().x > 0) {
            player1.move(-15.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.getPosition().x < (width - player1.getGlobalBounds().width)) {
            player1.move(15.0f, 0.0f);
        }

        // Player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player2.getPosition().x > 0) {
            player2.move(-15.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player2.getPosition().x < (width - player2.getGlobalBounds().width)) {
            player2.move(15.0f, 0.0f);
        }

        // Collision detection; if ball hits paddle
        sf::Time hitTime = hitClock.getElapsedTime();
        if (hitTime.asMilliseconds() >= 100) {
            if (player1.getGlobalBounds().intersects(ball.getGlobalBounds()) || player2.getGlobalBounds().intersects(ball.getGlobalBounds())) {
                ballVelocityY = -ballVelocityY;
                soundPaddle.play();
                hitClock.restart();
            }
        }

        window.clear();
        window.draw(ball);
        window.draw(player1);
        window.draw(player2);
        window.draw(showP1Score);
        window.draw(showP2Score);
        window.display();
        window.setFramerateLimit(60);
    }

    return 0;
}