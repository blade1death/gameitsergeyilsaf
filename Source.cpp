

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class Object {
public:
	sf::Texture texture;
	sf::Sprite image;
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
	float mass = 0;
	sf::Vector2f velocity;
	bool moving = false;
	bool can_collide = false;

	Object(string filename) {
		texture.loadFromFile(filename);
		image.setTexture(texture);
		width = texture.getSize().x;
		height = texture.getSize().y;
		image.setOrigin(width / 2, height / 2);

		x = image.getPosition().x;
		y = image.getPosition().y;

		velocity = sf::Vector2f();
		velocity.x = 0.;
		velocity.y = 0.;
	};
	~Object() {};
	void Move(float new_x, float new_y) {
		image.setPosition(new_x, new_y);
		x = new_x;
		y = new_y;
	};
	void setImage(string filename) {
		texture.loadFromFile(filename);
		image.setTexture(texture);
	};
	void setScale(float x_scale, float y_scale) {
		image.setScale(x_scale, y_scale);
		width *= x_scale;
		height *= y_scale;
	};

};

int window_width = 1200;
int window_height = 800;
bool onground = false;
bool gravitation = true;//не могу прыгать если я двигаю персонажа левой кнопкой мыши
int flagclose = 0;
int playerdirection;
int frd = 0;
int last_frd = 0;
int number_of_menu;
bool jump = false;
vector <Object> objects;

Object sun("sun.png");
Object player("banan.png");
Object wall("wall.png");
Object platform("platform2.png");
Object background("start.png");
Object buttonstart("mousebuttonstart.png");
Object buttonexit("mousebuttonexit.png");
Object platform2("block.png");


RectangleShape healthbar(Vector2f(100, 20));
RectangleShape healtpalyer(Vector2f(50, 10));

int healthpoint = 100;

void update(float time, int& frd, int& last_frd) {
	int g = 10;
	FloatRect playerbounds = objects[1].image.getGlobalBounds();//координаты персонажа
	FloatRect rectanglebounds = objects[2].image.getGlobalBounds();//координаты стены в виде прямоугольника
	FloatRect rectangle2Bounds = objects[7].image.getGlobalBounds();
	if (gravitation == true) {
		if (objects[1].mass != 0) {
			if (objects[1].y >= window_height - objects[3].height - 5)
			{
				objects[1].y = window_height - objects[3].height - 5;
				onground = true;
				objects[1].velocity.y = 0;
			}
			else if (objects[1].y>= window_height - objects[3].height - 42+5 &&objects[1].x>objects[7].x) {
				objects[1].y = window_height - objects[3].height - 42+5;
				onground = true;
				objects[1].velocity.y = 0;
			}
			else {
				objects[1].velocity.y = objects[1].velocity.y + g * time;
				objects[1].y = objects[1].y + objects[1].velocity.y * time;
				objects[1].Move(objects[1].x, objects[1].y);
				healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
			}
		}
		else {
			objects[1].y = objects[1].y;
		}
	}
	if (frd == 1 && objects[1].x > objects[1].width / 2) {
		if (playerbounds.intersects(rectanglebounds) == false) {
			player.setImage("banan.png");
			objects[1].velocity.x = -1;
			objects[1].x += (objects[1].velocity.x);
			objects[1].Move(objects[1].x, objects[1].y);
			healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
			objects[1].velocity.x = 0;
			frd = 0;
		}
	}
	if (frd == 2 && objects[1].x < window_width - (objects[1].width / 2)) {
			player.setImage("bananreverse.png");
			objects[1].velocity.x = 1;
			objects[1].x += (objects[1].velocity.x);
			objects[1].Move(objects[1].x, objects[1].y);
			healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
			objects[1].velocity.x = 0;
			frd = 0;
	}
	if (last_frd == 3 && onground) {
		objects[1].velocity.y = 1;
		objects[1].y -= 100;
		healtpalyer.setPosition(objects[1].x-24 , objects[1].y - 64);
		onground = false;
		last_frd = 0;
	}
	if (objects[1].x + 20 == objects[7].x&&objects[1].y>=742) {
		objects[1].Move(objects[1].x - 1, objects[1].y);
	}
}

int main()
{
	RenderWindow window(VideoMode(window_width, window_height), "banana adventure!");
	Clock clock;
	int max = 10;

	objects.push_back(sun);
	objects.push_back(player);
	objects.push_back(wall);//objects[2] это стена 
	objects.push_back(platform);//objects[3] это платформа
	objects.push_back(background);//оbkects[4] это фон меню
	objects.push_back(buttonstart);//objects[5] это кнопка старта в меню
	objects.push_back(buttonexit);//objects[6] это кнопка выхода из игры
	objects.push_back(platform2);


	objects[1].mass = 1;

	objects[1].Move(window_width / 2 + objects[1].height * 2, objects[1].height / 2);
	objects[2].Move(objects[2].width / 2, window_height - objects[3].height * 3);
	objects[3].Move(window_width / 2, window_height - objects[3].height / 4);
	objects[0].Move(window_width / 2, sun.height / 2);
	objects[4].Move(window_width / 2, window_height / 2);
	objects[5].Move(window_width / 2 - 256, window_height / 2);
	objects[6].Move(window_width / 2 + 256, window_height / 2);
	objects[7].Move(window_width - 100, window_height - objects[3].height-10);

	objects[2].x = objects[2].width;
	objects[7].y = window_height - objects[3].height - 42+14;
	objects[7].x = window_height - objects[7].width/2+300;
	healthbar.setFillColor(Color::Red);
	healthbar.setPosition(0, window_height - (objects[2].height + 25));

	healtpalyer.setFillColor(Color::Red);
	healtpalyer.setPosition(objects[1].x-48, objects[1].y - 64);


	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("atack.ogg");
	Sound shoot(shootBuffer);

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 80000;
		Vector2i pos = Mouse::getPosition(window);//забираем коорд курсора
		Event event;
		if (flagclose == 1) {
			while (window.pollEvent(event))
			{
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (objects[1].image.getGlobalBounds().contains(pos.x, pos.y))
					{
						objects[1].moving = true;
						gravitation = false;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape)) flagclose = 0;
				if (event.type == Event::MouseButtonReleased)
				{
					if (event.key.code == Mouse::Left)
					{
						objects[1].moving = false;
						gravitation = true;
					}
				}
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::E && (objects[1].x <= objects[2].x + 45) && (frd==1)) {
						shoot.play();
						if (healthpoint > 0) {
							healthbar.setSize(Vector2f(100 - max, 20));
							healthpoint -= 10;
							max += 10;
							if (max == 100) {
								healthbar.setSize(Vector2f(0, 0));
								objects[2].setScale(0, 0);
							}
						}
					}
					if (event.key.code == sf::Keyboard::W) {
						last_frd = 3;

					}
				}

			}
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
				frd = 1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
				frd = 2;
			}
			if (objects[1].moving) {
				objects[1].x = pos.x;
				objects[1].y = pos.y;
				objects[1].Move(objects[1].x, objects[1].y);
				healtpalyer.setPosition(objects[1].x - 24, objects[1].y - 64);
			}

			update(time, frd, last_frd);
			window.clear();
			for (int i = 0; i < int(objects.size()); i++)
			{
				if (i != 4 && i != 5 && i != 6)
					window.draw(objects[i].image);

			}
			window.draw(healtpalyer);
			window.draw(healthbar);
			window.display();
		}
		else if (flagclose == 0) {
			while (window.pollEvent(event)) {
				if (objects[5].image.getGlobalBounds().contains(pos.x, pos.y)) {
					if (Mouse::isButtonPressed(Mouse::Left))
						flagclose = 1;
				}if (objects[6].image.getGlobalBounds().contains(pos.x, pos.y) && Mouse::isButtonPressed(Mouse::Left)) {
					window.close();
					break;
				}
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Escape) {
						window.close();
					}
				}
			}
			window.clear();
			window.draw(objects[4].image);
			window.draw(objects[5].image);
			window.draw(objects[6].image);
			window.display();
		}
	}
	return 0;
}
