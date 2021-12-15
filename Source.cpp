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
vector <Object> objects;

Object sun("sun.png");
Object player("banan.png");

void update() {
	int g = 10; float t = 0.007;
	bool onground = false;
	if (objects[1].mass != 0) {
		if (objects[1].y >= window_height - objects[1].width / 2)
		{
			objects[1].y = window_height - objects[1].width / 2;
			onground = true;
		}
			
		else {
			objects[1].velocity.y = objects[1].velocity.y + g * t;
			objects[1].y = objects[1].y + objects[1].velocity.y * t;
			objects[1].Move(objects[1].x, objects[1].y);
		}
	}
	else {
		objects[1].y = objects[1].y;
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && objects[1].x > objects[1].height) {
		player.setImage("banan.png");
		objects[1].x -= objects[1].velocity.x;
		objects[1].Move(objects[1].x - 1, objects[1].y);
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) && objects[1].x < window_width - (objects[1].height)) {
		player.setImage("bananreverse.png");
		objects[1].x += objects[1].velocity.x;
		objects[1].Move(objects[1].x + 1, objects[1].y);
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
	objects[1].width = 50;
	objects[1].height = 50;
	float dx=0, dy=0;
	objects[1].Move(objects[1].width , objects[1].height / 2);
	while (window.isOpen())
	{
		
		Event event;
		Vector2i pixelpos = Mouse::getPosition(window);
		cout << pixelpos.x << ' ' << pixelpos.y << endl;
		while (window.pollEvent(event))
		{
			{
				if (event.type == Event::Closed||event.key.code == Keyboard::Escape)
					window.close();
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					cout << "mouse" << endl;
					if (objects[1].x == pixelpos.x && objects[1].y ==pixelpos.y) {
						dx = pixelpos.x - objects[1].x;
						dy = pixelpos.y - objects[1].y;
						objects[1].moving=true;
					}
				}
			}
			if (event.type == Event::MouseButtonReleased)
				if (event.key.code == Mouse::Left)
					objects[1].moving = false;

		}
		if (objects[1].moving == true) {
			objects[1].x = pixelpos.x - dx;
			objects[1].y = pixelpos.y - dy;
		}

		
		// call update every tick
		update();
		window.clear();
		// draw all objects in vector
		for (int i = 0; i < int(objects.size()); i++) {
			window.draw(objects[i].image);
		}
		
		window.display();
	}
	return 0;
}
