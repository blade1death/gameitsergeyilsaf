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
int frd=0;
int last_frd = 0;
int number_of_menu;
vector <Object> objects;

Object sun("sun.png");
Object player("banan.png");
Object wall("wall.png");
Object platform("platform2.png");
Object background("start.png");
Object buttonstart("mousebuttonstart.png");
Object buttonexit("mousebuttonexit.png");


RectangleShape healthbar(Vector2f(100, 20));

int healthpoint = 100;

void update(float time, int &frd,int &last_frd ) {
	int g = 10;
	if (gravitation == true) {
		if (objects[1].mass != 0) {
			if (objects[1].y >=window_height - objects[3].height-5)
			{
				objects[1].y = window_height - objects[3].height-5;
				onground = true;
				objects[1].velocity.y = 0;
			}

			else  {
				objects[1].velocity.y = objects[1].velocity.y + g * time;
				objects[1].y = objects[1].y + objects[1].velocity.y * time;
				objects[1].Move(objects[1].x, objects[1].y);
			}
		}
		else {
			objects[1].y = objects[1].y;
		}
	}
	if (frd == 1 && objects[1].x > objects[1].width / 2) {
		playerdirection = 0;
		player.setImage("banan.png");
		objects[1].velocity.x = -1;
		objects[1].x += (objects[1].velocity.x);
		objects[1].Move(objects[1].x, objects[1].y);
		objects[1].velocity.x = 0;
		frd = 0;
	}
	if (frd == 2 && objects[1].x < window_width - (objects[1].width / 2)) {
		playerdirection = 1;
		player.setImage("bananreverse.png");
		objects[1].velocity.x =1;
		objects[1].x += (objects[1].velocity.x);
		objects[1].Move(objects[1].x, objects[1].y);
		objects[1].velocity.x = 0;
		frd = 0;
	}
	if (last_frd == 3&&onground) {
		objects[1].velocity.y = 1;
		objects[1].y -= 100;
		onground = false;
		last_frd = 0;
	}


}

int main()
{
	RenderWindow window(VideoMode(window_width, window_height), "SFML works!");
	Clock clock;
	int max = 0;

	objects.push_back(sun);
	objects.push_back(player);
	objects.push_back(wall);//objects[2] это стена 
	objects.push_back(platform);//objects[3] это платформа
	objects.push_back(background);//оbkects[4] это фон меню
	objects.push_back(buttonstart);//objects[5] это кнопка старта в меню
	objects.push_back(buttonexit);//objects[6] это кнопка выхода из игры


	objects[1].mass = 1;
	
	objects[1].Move(window_width/2+objects[1].height*2, objects[1].height / 2);
	objects[2].Move(objects[2].width/2, window_height-objects[3].height*3);
	objects[3].Move(window_width/2 , window_height - objects[3].height/4);
	objects[0].Move(window_width / 2, sun.height / 2);
	objects[4].Move(window_width/2 , window_height/2);
	objects[5].Move(window_width / 2 - 256, window_height / 2);
	objects[6].Move(window_width / 2 + 256, window_height / 2);

	objects[2].x = objects[2].width;

	healthbar.setFillColor(Color::Red);
	healthbar.setPosition(0, window_height - (objects[2].height + 25));
	


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
		if (flagclose==1) {
			while (window.pollEvent(event))
			{
				if (Keyboard::isKeyPressed(Keyboard::W)) {
					last_frd = 3;
				}
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (objects[1].image.getGlobalBounds().contains(pos.x, pos.y))
					{
						objects[1].moving = true;
						gravitation = false;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape)) flagclose =0; 
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
					if (event.key.code == sf::Keyboard::E && (objects[1].x <= objects[2].x + 45) && (playerdirection == 0)) {
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
				}

			}

			FloatRect playerbounds = objects[1].image.getGlobalBounds();//координаты персонажа
			FloatRect rectanglebounds = objects[2].image.getGlobalBounds();//координаты стены в виде прямоугольника
			FloatRect platformbounds = objects[3].image.getGlobalBounds();
			if (playerbounds.intersects(rectanglebounds)) {
				if (playerdirection == 0) {
					objects[1].Move(objects[1].x + 1, objects[1].y);
				}
				if (playerdirection == 1) {
					objects[1].Move(objects[1].x - 1, objects[1].y);
				}
			}
			if (objects[1].moving) {
				objects[1].x = pos.x;
				objects[1].y = pos.y;
				objects[1].Move(objects[1].x, objects[1].y);
			}
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
				frd = 1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
				frd = 2;
			}

			update(time, frd, last_frd);
			window.clear();
			for (int i = 0; i < int(objects.size()); i++)
			{
				if(i!=4&&i!=5&&i!=6)
					window.draw(objects[i].image);

			}
			window.draw(healthbar);
			window.display();
		}
		else if (flagclose == 0) {
			while (window.pollEvent(event)) {
				if (objects[5].image.getGlobalBounds().contains(pos.x, pos.y)) {
					if (Mouse::isButtonPressed(Mouse::Left))
						flagclose = 1;
				}if (objects[6].image.getGlobalBounds().contains(pos.x, pos.y)&& Mouse::isButtonPressed(Mouse::Left)) {
					window.close();
					break;
				}
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Escape) {
						flagclose = 0;
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
