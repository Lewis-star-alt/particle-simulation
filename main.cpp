#include <SFML/Graphics.hpp>
#include <vector>



class Particle
{
public:
    sf::CircleShape shape; // Графическое представление
    sf::Vector2f velocity; // Вектор скорости (vx, vy)
    sf::Vector2f acceleration; // Вектор ускорения (ax, ay)

    Particle(float radius, sf::Color color, sf::Vector2f position)
    {
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setPosition(position);


        velocity = sf::Vector2f(0.f, 0.f);
        acceleration = sf::Vector2f(0.f, 0.f);
    }

    void update(float dt)
    {
        velocity += acceleration * dt;
        shape.move(velocity * dt);
        acceleration = sf::Vector2f(0.f, 0.f);
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "Particle Simulator");
    window.setFramerateLimit(60); // Ограничиваем FPS для стабильности

    std::vector<Particle> particles;
    sf::Clock clock;
    const sf::Vector2f gravity(0.f, 500.f);


    // Главный цикл
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                float radius = rand() % 20 + 5;
                sf::Color color(rand() % 256, rand() % 256, rand() % 256);

                // Создаём частицу в позиции курсора мыши
                particles.push_back(Particle(radius, color, sf::Vector2f(event.mouseButton.x, event.mouseButton.y)));
            }
        }
        for (auto& p : particles)
        {
            // Применяем правила (силы) к частице
            p.acceleration = gravity;

            // Обновляем состояние частицы на основе прошедшего времени
            p.update(deltaTime);


            float groundLevel = 800.f; // Уровень земли (низ окна)
            float particleBottom = p.shape.getPosition().y + p.shape.getRadius() * 2;

            if (particleBottom > groundLevel)
            {
                p.shape.setPosition(p.shape.getPosition().x, groundLevel - p.shape.getRadius() * 2);
                p.velocity.y = -p.velocity.y * 0.8f; // "затухание"
            }
        }

        window.clear(sf::Color::Black); // Очищаем экран

        // Рисуем все частицы
        for (auto& p : particles) {
            window.draw(p.shape);
        }

        window.display(); // Показываем нарисованное
    }


    return 0;
}
