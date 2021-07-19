#include "Game.h"

//private functions
void Game::initVariables()
{
	this->window = nullptr;
	this->points = 0;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new sf::RenderWindow(this->videoMode, "Space Shooter", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initEnemy()
{
	this->enemy.setPosition(10.f,10.f);
	this->enemy.setSize(sf::Vector2f(100.f,100.f));
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Cyan);
	//this->enemy.setOutlineColor(sf::Color::Green);
	//this->enemy.setOutlineThickness(1.f);
}

//Constructor/Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initEnemy();
}

Game::~Game()
{
	delete this->window;
}

//Accessor
const bool Game::running() const
{
	return this->window->isOpen();
}


//Functions
void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	this->enemy.setFillColor(sf::Color::Green);
	this->enemies.push_back(this->enemy);
}

void Game::pollEvent()
{
	//Event polling
	while (this->window->pollEvent(this->ev)) {

		switch (this->ev.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{	
	//returns the position of the mouse of the windows.
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	if(this->enemies.size()< this->maxEnemies){

		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
	
		else {
			this->enemySpawnTimer += 1.f;
		}
	}

	//moving and updating enemies
	for (int i = 0; i < this->enemies.size();i++) {
		this->enemies[i].move(0.f,1.f);
		bool deleted = false;

		//checked it clicked upon
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
				deleted = true;

				this->points += 10.f;
			}
		}

		//if enemy is past the bottom of the screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			deleted = true;
		}

		if (deleted) {
			this->enemies.erase(this->enemies.begin() + i);
		}
	}
}

void Game::update()
{
	this->pollEvent();
	this->updateMousePositions();
	this->updateEnemies();
}

void Game::renderEnemies()
{
	for (auto &e : this->enemies) {
		this->window->draw(e);
	}
}

void Game::render()
{
	this->window->clear();
	//draw
	this->renderEnemies();
	this->window->display();
}

