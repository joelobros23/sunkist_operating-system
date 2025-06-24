#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <deque>

// --- Configuration Constants ---
const unsigned int WINDOW_WIDTH = 1600;
const unsigned int WINDOW_HEIGHT = 900;
const float PI = 3.14159265359f;

// --- Utility Functions ---

// Function to get current time as a string
std::string getCurrentTime() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    std::stringstream ss;
    ss << std::put_time(&ltm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Generates a random float between two values
float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// --- Particle System Class ---
class ParticleSystem {
public:
    struct Particle {
        sf::CircleShape shape;
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    ParticleSystem(unsigned int count) {
        m_particles.resize(count);
        m_vertices.setPrimitiveType(sf::Points);
        m_vertices.resize(count);
    }

    void update(sf::Time elapsed, sf::Vector2i mousePos) {
        for (std::size_t i = 0; i < m_particles.size(); ++i) {
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            if (p.lifetime <= sf::Time::Zero) {
                resetParticle(i);
            }
            
            // Mouse interaction - repel particles
            sf::Vector2f mouseF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            float dist = std::sqrt(std::pow(p.shape.getPosition().x - mouseF.x, 2) + std::pow(p.shape.getPosition().y - mouseF.y, 2));
            if (dist < 150.f) {
                sf::Vector2f direction = p.shape.getPosition() - mouseF;
                p.velocity += direction * (1.f / (dist * dist + 1.f)) * 2500.f; // Repel force
            }

            // Apply friction/drag
            p.velocity *= 0.99f;


            p.shape.setPosition(p.shape.getPosition() + p.velocity * elapsed.asSeconds());

            // Update the vertex array
            m_vertices[i].position = p.shape.getPosition();

            // Update color based on lifetime
            float ratio = p.lifetime.asSeconds() / 5.0f;
            sf::Color color = sf::Color(100, 100, 200, static_cast<sf::Uint8>(ratio * 150));
            m_vertices[i].color = color;
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(m_vertices, states);
    }

private:
    void resetParticle(std::size_t index) {
        float angle = randomFloat(0, 2 * PI);
        float speed = randomFloat(20, 50);
        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        m_particles[index].shape.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
        m_particles[index].lifetime = sf::seconds(randomFloat(3, 7));
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
};

// --- UI Element Classes ---

// A simple struct for placeholder icon buttons
class IconButton {
public:
    sf::RectangleShape shape;
    sf::CircleShape icon; // Using a circle as a placeholder icon

    IconButton(sf::Vector2f position, float size) {
        shape.setSize(sf::Vector2f(size, size));
        shape.setPosition(position);
        shape.setFillColor(sf::Color(40, 40, 60, 180));
        shape.setOutlineThickness(1);
        shape.setOutlineColor(sf::Color(80, 80, 120));

        icon.setRadius(size / 4.f);
        icon.setFillColor(sf::Color(150, 150, 180));
        icon.setOrigin(icon.getRadius(), icon.getRadius());
        icon.setPosition(position.x + size / 2.f, position.y + size / 2.f);
    }

    void update(const sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            shape.setFillColor(sf::Color(60, 60, 90, 220));
        } else {
            shape.setFillColor(sf::Color(40, 40, 60, 180));
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(icon);
    }
};

// Class to manage the mock terminal log
class LogDisplay {
public:
    LogDisplay(unsigned int max_lines, sf::Font& font, sf::Vector2f position, sf::Vector2f size)
        : m_max_lines(max_lines), m_font(font), m_position(position), m_size(size)
    {
        m_background.setPosition(position);
        m_background.setSize(size);
        m_background.setFillColor(sf::Color(0, 0, 0, 128));
        m_background.setOutlineColor(sf::Color(0, 255, 0, 100));
        m_background.setOutlineThickness(1.f);
    }

    void addMessage(const std::string& message) {
        m_log_messages.push_back(message);
        if (m_log_messages.size() > m_max_lines) {
            m_log_messages.pop_front();
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(m_background);
        float y_pos = m_position.y + 5.f;
        for (const auto& msg : m_log_messages) {
            sf::Text text(msg, m_font, 14);
            text.setPosition(m_position.x + 10.f, y_pos);
            text.setFillColor(sf::Color(0, 255, 100)); // Hacker green
            window.draw(text);
            y_pos += 20.f;
        }
    }

private:
    unsigned int m_max_lines;
    sf::Font& m_font;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::RectangleShape m_background;
    std::deque<std::string> m_log_messages;
};


// --- Main Application Entry ---
int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // --- Window and Clock Setup ---
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sunkist OS - GUI Placeholder");
    window.setFramerateLimit(60);
    sf::Clock deltaClock;
    sf::Clock shaderClock;
    sf::Clock logClock;

    // --- Font Loading ---
    sf::Font mainFont;
    if (!mainFont.loadFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font.ttf." << std::endl;
        return -1;
    }

    // --- UI Text Elements Setup ---
    sf::Text titleText("Sunkist OS", mainFont, 80);
    titleText.setStyle(sf::Text::Bold);
    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, 100.f));

    sf::Text clockText("", mainFont, 18);
    clockText.setPosition(WINDOW_WIDTH - 220.f, 10.f);

    sf::Text statusText("System Status: Nominal", mainFont, 16);
    statusText.setFillColor(sf::Color(200, 200, 220));
    statusText.setPosition(20.f, WINDOW_HEIGHT - 40.f);


    // --- Core Components Initialization ---
    ParticleSystem particleSystem(300);
    LogDisplay logDisplay(15, mainFont, sf::Vector2f(WINDOW_WIDTH - 420, 50), sf::Vector2f(400, 340));
    std::vector<IconButton> sidebarButtons;

    for (int i = 0; i < 5; ++i) {
        sidebarButtons.emplace_back(IconButton(sf::Vector2f(20.f, 100.f + i * 80.f), 60.f));
    }


    // --- Main Application Loop ---
    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();

        // --- Event Processing ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // --- Update Logic ---
        clockText.setString(getCurrentTime());
        particleSystem.update(dt, sf::Mouse::getPosition(window));

        for (auto& button : sidebarButtons) {
            button.update(window);
        }

        // Add a new log message periodically
        if (logClock.getElapsedTime().asSeconds() > 1.5f) {
            std::stringstream ss;
            ss << "SYS_INFO: Core " << (rand() % 4) << " temp " << randomFloat(45.f, 60.f) << " C";
            logDisplay.addMessage(ss.str());
            logClock.restart();
        }

        // --- Drawing Logic ---
        window.clear(sf::Color(15, 15, 25));

        // Draw dynamic background gradient
        sf::VertexArray background(sf::Quads, 4);
        float time = shaderClock.getElapsedTime().asSeconds();
        sf::Color topColor(static_cast<sf::Uint8>(25 + 10 * sin(time * 0.4f)), 20, 35);
        sf::Color bottomColor(20, static_cast<sf::Uint8>(25 + 10 * cos(time * 0.4f)), 45);
        background[0].position = sf::Vector2f(0, 0);
        background[0].color = topColor;
        background[1].position = sf::Vector2f(WINDOW_WIDTH, 0);
        background[1].color = topColor;
        background[2].position = sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        background[2].color = bottomColor;
        background[3].position = sf::Vector2f(0, WINDOW_HEIGHT);
        background[3].color = bottomColor;
        window.draw(background);

        // Draw particles
        particleSystem.draw(window, sf::RenderStates::Default);

        // Draw UI Elements
        window.draw(titleText);
        window.draw(clockText);
        window.draw(statusText);
        
        for (auto& button : sidebarButtons) {
            button.draw(window);
        }

        logDisplay.draw(window);

        // Display the final rendered frame
        window.display();
    }

    return 0;
}
