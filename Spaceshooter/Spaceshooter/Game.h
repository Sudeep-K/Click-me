#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

/*
	Class that acts as a game engine
	Wrapper class.
*/

class Game
{
private:
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Resources
	sf::Font font;

	//text
	sf::Text uitext;

	//Game logic
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;
	bool endGame;

	//Mouse position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Game object
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	//private functions
	void initVariables();
	void initWindow();
	void initEnemy();
	void initFonts();
	void initText();

public:
	//Constructors/Destructors
	Game();
	virtual ~Game();

	//Acccessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();

	void pollEvent();
	void updateMousePositions();
	void updateEnemies();
	void updateText();
	void update();

	void renderEnemies(sf::RenderTarget &target);
	void renderText(sf::RenderTarget &target);
	void render();
};

