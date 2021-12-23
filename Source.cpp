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
bool flagclose = false;//попытка качественного закрытия окна
int playerdirection;
vector <Object> objects;

Object sun("sun.png");
Object player("banan.png");
Object wall("wall.png");
Object platform("platform.png");
RectangleShape healthbar(Vector2f(100, 20));
int healthpoint = 100;

void update(float time) {
	int g = 10;
	if (gravitation == true) {
		if (objects[1].mass != 0) {
			if (objects[1].y >=window_height - objects[1].height/4)
			{
				objects[1].y = window_height - objects[1].height/4;
				onground = true;
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


}

int main()
{
	RenderWindow window(VideoMode(window_width, window_height), "SFML works!");
	// add sun to the vector of objects
	objects.push_back(sun);
	// move sun to the center of the screen
	objects[0].Move(window_width / 2, sun.height / 2);
	objects.push_back(player);
	objects[1].mass = 1;
	Clock clock;
	int max = 0;
	objects[1].Move(window_width/2+objects[1].height*2, objects[1].height / 2);
	objects.push_back(wall);//objects[2] это стена 
	objects.push_back(platform);//objects[3] это платформа
	objects[2].Move(objects[2].width/2, window_height);
	healthbar.setFillColor(Color::Red);
	healthbar.setPosition(0, 380);
	objects[3].Move(window_width-700 , window_height - objects[3].height/4);
	objects[2].x = objects[2].width;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 80000;
		Vector2i pos = Mouse::getPosition(window);//забираем коорд курсора
		Event event;
		while (window.pollEvent(event))
		{
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (objects[1].image.getGlobalBounds().contains(pos.x, pos.y))
					{
						objects[1].moving = true;//можем двигать
						gravitation = false;
					}
				}
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) flagclose=true; // 2 task 
			if (event.type == Event::MouseButtonReleased) // 3 task
			{//если отпустили клавишу
				if (event.key.code == Mouse::Left)
				{ //а именно левую
					objects[1].moving = false; //то не можем двигать 
					gravitation = true;
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if ((event.key.code == sf::Keyboard::Up|| event.key.code == sf::Keyboard::W) && (onground))
				{
					objects[1].velocity.y = 1;
					objects[1].y -= 100;
					onground = false;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::E)&&objects[1].x<=objects[2].x+50) {
				
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
		FloatRect playerbounds = objects[1].image.getGlobalBounds();//координаты персонажа
		FloatRect rectanglebounds = objects[2].image.getGlobalBounds();//координаты стены в виде прямоугольника
		if (playerbounds.intersects(rectanglebounds)) {
			if (playerdirection == 0) {
				objects[1].Move(objects[1].x + 1, objects[1].y);
			}
		}
		// 2 task
		if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && objects[1].x > objects[1].width/2) {
			player.setImage("banan.png");
			objects[1].x -= objects[1].velocity.x;
			objects[1].Move(objects[1].x - 1, objects[1].y);
			playerdirection = 0;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && objects[1].x < window_width - (objects[1].width/2)) {
			player.setImage("bananreverse.png");
			objects[1].x += objects[1].velocity.x;
			objects[1].Move(objects[1].x + 1, objects[1].y);
	
		}
		if (objects[1].moving) {//если можем двигать; 3 task
			objects[1].x = pos.x;//двигаем  по Х
			objects[1].y = pos.y ;//двигаем по Y
			objects[1].Move(objects[1].x, objects[1].y);
		}
		if(objects[1].x)

		update(time);
		window.clear();
		// draw all objects in vector
		for (int i = 0; i < int(objects.size()); i++)
		{
			window.draw(objects[i].image);
			
		}
		window.draw(healthbar);
		window.display();
		if (flagclose) {
			window.close();
			break;
		}
	}
	return 0;
}