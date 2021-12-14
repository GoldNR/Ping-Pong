#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(0,0), "Ping Pong", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);

    sf::CircleShape ball(10.0f);
    sf::RectangleShape player1 (sf::Vector2f(137.0f, 10.0f));
    sf::RectangleShape player2 (sf::Vector2f(137.0f, 10.0f));

    sf::SoundBuffer buffer1;
    sf::SoundBuffer buffer2;
    sf::SoundBuffer buffer3;

    sf::Sound soundPaddle;
    sf::Sound soundWall;
    sf::Sound soundScore;

    sf::Font font;
    sf::Text scorePlayer1;
    sf::Text scorePlayer2;

    sf::Clock clock;

    buffer1.loadFromFile("sound_paddle.wav");
    buffer2.loadFromFile("sound_wall.wav");
    buffer3.loadFromFile("sound_score.wav");

    soundPaddle.setBuffer(buffer1);
    soundWall.setBuffer(buffer2);
    soundScore.setBuffer(buffer3);

    ball.setFillColor(sf::Color::White);
    player1.setFillColor(sf::Color::White);
    player2.setFillColor(sf::Color::White);

    ball.setPosition(678, 379);
    player1.setPosition(620, 758);
    player2.setPosition(620, 0);

    font.loadFromFile("Emulogic-zrEw.ttf");
    scorePlayer1.setFont(font);
    scorePlayer2.setFont(font);
    scorePlayer1.setCharacterSize(24);
    scorePlayer2.setCharacterSize(24);
    scorePlayer1.setFillColor(sf::Color::White);
    scorePlayer2.setFillColor(sf::Color::White);
    scorePlayer1.setPosition(100, 512);
    scorePlayer2.setPosition(100, 256);

    float ballVelocityX = 10.0f, ballVelocityY = 10.0f;

    int player1score = 0, player2score = 0;

    while (window.isOpen())
    {
        // Score system setup
        std::string p1score = std::to_string(player1score), p2score = std::to_string(player2score);
        scorePlayer1.setString(p1score);
        scorePlayer2.setString(p2score);

        // Time delay before ball moves
        sf::Time time = clock.getElapsedTime();
        if (time.asSeconds() >= 1) {
            ball.move(ballVelocityX, ballVelocityY);
        }

        float ballX = ball.getPosition().x, ballY = ball.getPosition().y;

        // if ball hits wall
        if (ballX >= 1341 || ballX <= 0) {
            ballVelocityX = -ballVelocityX;
            soundWall.play();
        }

        // if ball goes out of bounds
        if (ballY >= 778.0f || ballY <= -20.0f) {
            ball.setPosition(678, 379);
            ballVelocityY = -ballVelocityY;
            soundScore.play();
            clock.restart();
            if (ballY <= -20.0f) { player1score++; }
            else if (ballY >= 778.0f) { player2score++; }
        }

        // Player controls
        // Player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player1.move(-15.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player1.move(15.0f, 0.0f);
        }

        // Player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player2.move(-15.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player2.move(15.0f, 0.0f);
        }

        // Collision detection; if ball hits paddle
        if (player1.getGlobalBounds().intersects(ball.getGlobalBounds()) || player2.getGlobalBounds().intersects(ball.getGlobalBounds())) {
            ballVelocityY = -ballVelocityY;
            soundPaddle.play();
        }

        window.clear();
        window.draw(ball);
        window.draw(player1);
        window.draw(player2);
        window.draw(scorePlayer1);
        window.draw(scorePlayer2);
        window.display();
        window.setFramerateLimit(60);
    }

    return 0;
}