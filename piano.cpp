#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <map>

class VirtualPiano {
public:
    VirtualPiano() {
        window.create(sf::VideoMode(800, 200), "Virtual Piano");
        loadSounds();
        createKeyboard();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    std::map<std::string, sf::SoundBuffer> soundBuffers;
    std::map<std::string, sf::Sound> sounds;
    std::vector<sf::RectangleShape> keys;

    void loadSounds() {
        const std::vector<std::string> notes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2"};

        for (const auto& note : notes) {
            sf::SoundBuffer buffer;
            if (!buffer.loadFromFile("sounds/" + note + ".wav")) {
                std::cerr << "Failed to load sound for note " << note << std::endl;
            }
            soundBuffers[note] = buffer;
            sounds[note].setBuffer(buffer);
        }
    }

    void createKeyboard() {
        const std::vector<std::string> notes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2"};

        for (const auto& note : notes) {
            sf::RectangleShape key;
            key.setSize(sf::Vector2f(50, 100));
            key.setOutlineThickness(1);
            key.setOutlineColor(sf::Color::Black);
            key.setFillColor(sf::Color::White);
            key.setOrigin(25, 0);
            key.setPosition(keys.size() * 50, 0);

            if (note.find("#") != std::string::npos) {
                key.setSize(sf::Vector2f(30, 70));
                key.setFillColor(sf::Color::Black);
                key.move(-15, 0);
            }

            keys.push_back(key);
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else {
                    handleKeyPress(event.key.code);
                }
            }
        }
    }

    void handleKeyPress(sf::Keyboard::Key key) {
        const std::vector<std::string> notes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C2"};

        for (size_t i = 0; i < notes.size(); ++i) {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::A + i))) {
                playKey(notes[i]);
            }
        }
    }

    void playKey(const std::string& note) {
        sounds[note].stop();
        sounds[note].play();
    }

    void update() {
        for (auto& key : keys) {
            key.setFillColor(sf::Color::White);
        }
    }

    void render() {
        window.clear(sf::Color::White);
        for (const auto& key : keys) {
            window.draw(key);
        }
        window.display();
    }
};

int main() {
    VirtualPiano app;
    app.run();

    return 0;
}
