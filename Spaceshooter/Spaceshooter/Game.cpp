#include "Game.h"

//private functions
void Game::initVariables()
{
	this->window = nullptr;
	this->points = 0;
	this->health = 10;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
	this->endGame = false;
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

void Game::initFonts()
{
	if (this->font.loadFromFile("Fonts/Pixeboy-z8XGD.ttf")) {
		std::cout << "Game::initFonts::Failed to load the font." << std::endl;
	}
}

void Game::initText()
{
	this->uitext.setFont(this->font);
	this->uitext.setCharacterSize(24);
	this->uitext.setFillColor(sf::Color::White);
	this->uitext.setString("NONE");
}

//Constructor/Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initEnemy();
	this->initFonts();
	this->initText();
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

const bool Game::getEndGame() const
{
	return this->endGame;
}


//Functions
void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	//Randomize the enemy type
	int type= rand() % 5;
	switch (type) {
		case 0:
			this->enemy.setSize(sf::Vector2f(25.f, 25.f));
			this->enemy.setFillColor(sf::Color::Magenta);
			break;
		case 1:
			this->enemy.setSize(sf::Vector2f(30.f, 30.f));
			this->enemy.setFillColor(sf::Color::Blue);
			break;
		case 2:
			this->enemy.setSize(sf::Vector2f(50.f, 50.f));
			this->enemy.setFillColor(sf::Color::Cyan);
			break;
		case 3:
			this->enemy.setSize(sf::Vector2f(70.f, 70.f));
			this->enemy.setFillColor(sf::Color::Red);
			break;
		case 4:
			this->enemy.setSize(sf::Vector2f(90.f, 90.f));
			this->enemy.setFillColor(sf::Color::Green);
			break;
		default:
			this->enemy.setSize(sf::Vector2f(80.f, 80.f));
			this->enemy.setFillColor(sf::Color::Green);
	}
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

		//deletes the enemy out of the bound.
		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << this->health << std::endl;
		}
	}

	//checked it clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		
		if (this->mouseHeld == false) {
			this->mouseHeld = true;
			bool deleted = false;
			
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++) {
				
				//erases the enemy if pointed to.
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
					if (this->enemies[i].getFillColor() == sf::Color::Magenta) {
						this->points += 10;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
						this->points += 7;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan) {
						this->points += 5;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Red) {
						this->points += 3;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Green) {
						this->points += 1;
					}
					std::cout << "Points: " << this->points << std::endl;

					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else {
			this->mouseHeld = false;
		}
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n";
	this->uitext.setString(ss.str());
}

void Game::update()
{
	this->pollEvent();
	if (this->endGame == false) {
		this->updateMousePositions();
		this->updateText();
		this->updateEnemies();
	}
	if (this->health <= 0) {
		this->endGame = true;
	}
}

void Game::renderEnemies(sf::RenderTarget &target)
{
	for (auto &e : this->enemies) {
		target.draw(e);
	}
}

void Game::renderText(sf::RenderTarget &target)
{
	target.draw(this->uitext);
}

void Game::render()
{
	this->window->clear();
	//draw
	this->renderEnemies(*this->window);
	this->renderText(*this->window);
	this->window->display();
}

