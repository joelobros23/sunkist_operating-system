#ifndef SUNKIST_COMPONENTS_HPP
#define SUNKIST_COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <deque>

// --- Configuration Constants ---
// We can define them here to be used across files
const unsigned int WINDOW_WIDTH = 1600;
const unsigned int WINDOW_HEIGHT = 900;
const float PI = 3.14159265359f;


// --- Utility Function Declarations ---
std::string getCurrentTime();
float randomFloat(float min, float max);


// --- Particle System Class Definition ---
/**
 * @class ParticleSystem
 * @brief Manages a dynamic background of interactive particles.
 *
 * This class handles the creation, update, and rendering of a large number
 * of particles. It includes logic for particle lifetime, movement, and
 * interaction with the user's mouse cursor.
 */
class ParticleSystem {
public:
    /**
     * @brief Constructs the particle system.
     * @param count The number of particles to manage.
     */
    ParticleSystem(unsigned int count);

    /**
     * @brief Updates the state of all particles.
     * @param elapsed Time elapsed since the last frame.
     * @param mousePos Current position of the mouse cursor.
     */
    void update(sf::Time elapsed, sf::Vector2i mousePos);

    /**
     * @brief Draws the particles to a render target.
     * @param target The window or texture to draw to.
     * @param states Render states to apply (e.g., transforms, blend mode).
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    // Internal struct to hold particle data
    struct Particle {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    /**
     * @brief Resets a particle to a new state after its lifetime expires.
     * @param index The index of the particle to reset.
     */
    void resetParticle(std::size_t index);

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
};


// --- Icon Button Class Definition ---
/**
 * @class IconButton
 * @brief Represents a simple, clickable icon button for the UI.
 *
 * Creates a rectangular button with a placeholder icon that responds to
 * mouse hover events.
 */
class IconButton {
public:
    /**
     * @brief Constructs the icon button.
     * @param position The screen position of the button.
     * @param size The width and height of the button.
     * @param iconType A character representing the icon (e.g., 'S' for settings).
     * @param font The font to use for the icon character.
     */
    IconButton(sf::Vector2f position, float size, const std::string& label, sf::Font& font);

    /**
     * @brief Updates the button's state (e.g., hover effect).
     * @param window The render window to get mouse position from.
     */
    void update(const sf::RenderWindow& window);

    /**
     * @brief Draws the button to the render window.
     * @param window The window to draw to.
     */
    void draw(sf::RenderWindow& window) const;

private:
    sf::RectangleShape m_shape;
    sf::Text m_iconText;
};


// --- Log Display Class Definition ---
/**
 * @class LogDisplay
 * @brief A mock terminal window that displays scrolling system messages.
 *
 * Simulates a system log, periodically adding new messages and scrolling
 * old ones off the screen.
 */
class LogDisplay {
public:
    // Enum to classify log message types for color-coding
    enum MessageType {
        INFO,
        WARNING,
        CRITICAL_ERROR
    };

    /**
     * @brief Constructs the log display panel.
     * @param max_lines The maximum number of lines to show at once.
     * @param font The font to use for log text.
     * @param position The screen position of the log panel.
     * @param size The dimensions of the log panel.
     */
    LogDisplay(unsigned int max_lines, sf::Font& font, sf::Vector2f position, sf::Vector2f size);

    /**
     * @brief Adds a new message to the log.
     * @param message The string content of the message.
     * @param type The type of the message (for color).
     */
    void addMessage(const std::string& message, MessageType type);

    /**
     * @brief Draws the log panel and its text to the window.
     * @param window The window to draw to.
     */
    void draw(sf::RenderWindow& window) const;

private:
    // Internal struct to hold message data//
    struct LogMessage {
        std::string text;
        MessageType type;
    };

    unsigned int m_max_lines;
    sf::Font& m_font;
    sf::RectangleShape m_background;
    std::deque<LogMessage> m_log_messages;
};

#endif // SUNKIST_COMPONENTS_HPP
