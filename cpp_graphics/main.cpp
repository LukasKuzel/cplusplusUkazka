#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <vector>
#include <random>

sf::RenderWindow createWindow(){
    sf::VideoMode::getDesktopMode();
    return {
            sf::VideoMode {1200, 800},
            "Hello",
            sf::Style::Titlebar | sf::Style::Close
    };
}

sf::Texture loadTexture(const std::string & filename){

    sf::Texture tex;
    if (not tex.loadFromFile(filename)){
        std::cerr << "Chyba" << std::endl;
        std::exit(-1);
    }
    return tex;
}

double toRad(const double degree){
    return degree * (M_PI / 180.0);
}
double toDeg(const double rad){
    return rad * (180.0/M_PI);

}


bool detectColision(const sf::RectangleShape & rect, const sf::Vector2i mouse) {

    const auto dx = mouse.x - rect.getPosition().x;
    const auto dy = (mouse.y - rect.getPosition().y) * -1;
    const auto d = std::sqrt(dx*dx + dy*dy);

    const auto cos = (double) dx / d;
    auto phi = std::acos(cos);

    if(dy < 0 or (dy == 0 and dx < 0)) {
        phi = 2 * M_PI - phi;
    }
    const auto rectRot = toRad(rect.getRotation());
    sf::RectangleShape r2 = rect;
    r2.setRotation(0.0);


    const auto phi2 = phi + rectRot;
    const auto newX = rect.getPosition().x + std::cos(phi2) *d;
    const auto nexY = rect.getPosition().y + std::sin(phi2) *d;
    return r2.getGlobalBounds().contains(newX,nexY);
}

int main() {
    auto win = createWindow();

    auto texture = loadTexture("tuk.png");

    texture.setSmooth(true);

    sf::Sprite sprite { texture };

    constexpr float spriteSize = 256.f;

    constexpr float g = 500.f;
    constexpr float jump = -500.f;

    float spriteVelocity = 0.f;

    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(spriteSize / texture.getSize().x, spriteSize / texture.getSize().y);
    sprite.setPosition(600,400);

    sf::RectangleShape rect({ 100, 100 });
    rect.setFillColor(sf::Color::Red);
    rect.setOrigin(50,50);
    rect.setPosition(600,400);

    std::vector <sf::RectangleShape> rects;

    rects.emplace_back(sf::RectangleShape( { 80, 80 } ));
    rects.back().setFillColor(sf::Color::White);
    rects.back().setOrigin(25, 25);
    rects.back().setPosition(300, 150);
    rects.back().setRotation(15.f);

    rects.emplace_back(sf::RectangleShape( { 80, 80 } ));
    rects.back().setFillColor(sf::Color::White);
    rects.back().setOrigin(25, 25);
    rects.back().setPosition(800, 400);
    rects.back().setRotation(45.f);

    rects.emplace_back(sf::RectangleShape( { 80, 80 } ));
    rects.back().setFillColor(sf::Color::White);
    rects.back().setOrigin(25, 25);
    rects.back().setPosition(650, 400);
    rects.back().setRotation(25.f);

    constexpr float rectVelocity = 250.f;

    sf::Clock clock;


    sf::CircleShape circle(200);
    circle.setOrigin(200,200);
    circle.setPosition(400,400);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color(128,128,128));
    circle.setOutlineThickness(3.f);

    std::vector<sf::CircleShape> circles;
    std::random_device random;
    std::uniform_real_distribution angleDist(0.0, 2 * M_PI);
    std::uniform_real_distribution radiusDist(0.0, (double)circle.getRadius()- 30) ;

    for(int i= 0; i < 10; i++){
        const auto angle = angleDist(random);
        const auto distance = radiusDist(random);

        sf::CircleShape c(20);
        c.setOrigin(20,20);
        c.setFillColor(sf::Color::Yellow);
        c.setOutlineThickness(3.f);

        const auto x= circle.getPosition().x + std::cos(angle) * distance;
        const auto y= circle.getPosition().y + std::sin(angle) * distance;
        c.setPosition(x,y);

        circles.emplace_back(c);
    }

    while (win.isOpen()){

        sf::Event event {};

        auto dt = clock.restart().asSeconds();

        int x = 0;
        int y = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            y-=1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            y+=1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            x-=1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            x+=1;
        }

        if (not x or (not y)){
            rect.move(x * rectVelocity * dt, y * rectVelocity * dt);
        } else {
            rect.move(
                    x * rectVelocity * dt * (1/std::sqrt(2)),
                    y * rectVelocity * dt * (1/std::sqrt(2))
            );
        }

        spriteVelocity += dt * g;

        sprite.move(0.f, spriteVelocity * dt);

        const auto bounds = sprite.getGlobalBounds();

        if (bounds.top + bounds.height > win.getSize().y) {
            const auto diff = bounds.top + bounds.height - win.getSize().y;
            sprite.move(0.0, diff * -1);
            spriteVelocity = 0.f;
        }

        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
            }
            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Q) {
                win.close();
            }
            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Space) {
                spriteVelocity = jump;
            }
        }

        for (auto & rect : rects) {
            rect.setFillColor(sf::Color::White);

            const auto pos = sf::Mouse::getPosition(win);



            if (detectColision(rect, pos)) {
                rect.setFillColor(sf::Color::Cyan);
            }

        }


        win.clear(sf::Color::Black);
        win.draw(rect);
        win.draw(sprite);
        win.draw(circle);


        for (const auto & c: circles){
            win.draw(c);
        }

        for (const auto & rect : rects) {
            win.draw(rect);
        }


        win.display();

    }
}
