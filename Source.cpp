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

void update(float time) {
	int g = 10;
	bool onground = false;
	if (objects[1].mass != 0) {
		if (objects[1].y >= window_height - objects[1].width / 2)
		{
			objects[1].y = window_height - objects[1].width / 2;
			onground = true;
		}

		else {
			objects[1].velocity.y = objects[1].velocity.y + g * time;
			objects[1].y = objects[1].y + objects[1].velocity.y * time;
			objects[1].Move(objects[1].x, objects[1].y);
		}
	}
	else {
		objects[1].y = objects[1].y;
	}
	if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && objects[1].x > objects[1].height) {
		player.setImage("banan.png");
		objects[1].x -= objects[1].velocity.x;
		objects[1].Move(objects[1].x - 1, objects[1].y);
	}
	if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && objects[1].x < window_width - (objects[1].height)) {
		player.setImage("bananreverse.png");
		objects[1].x += objects[1].velocity.x;
		objects[1].Move(objects[1].x + 1, objects[1].y);
	}
	if ((Keyboard::isKeyPressed(Keyboard::W)) && (onground))
	{
		objects[1].velocity.y = 1;
		objects[1].y -= 100;
		onground = false;
	}
	//if (!onground) { objects[1].y += 0.0015 * time; } 

}

int main()
{
	RenderWindow window(VideoMode(window_width, window_height), "SFML works!");
	// add sun to the vector of objects
	objects.push_back(sun);
	// move sun to the center of the screen
	objects[0].Move(window_width / 2, sun.height / 2);
	objects.push_back(player);
	bool isMove = false;//переменная для щелчка мыши по спрайту
	objects[1].mass = 1;
	Clock clock;
	objects[1].width = 50;
	objects[1].height = 50;
	float dx = 0; //корректировка нажатия по х
	float dy = 0; //по у
	objects[1].Move(objects[1].width, objects[1].height / 2);
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 80000;
		Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		//std::cout << pixelPos.x << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
		//std::cout << pos.x << "\n";//смотрим на Х,которая преобразовалась в мировые координаты
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonPressed)
			{ //если нажата клавиша мыши
				if (event.key.code == Mouse::Left)
				{ //а именно левая
					if (objects[1].x == pixelPos.x && objects[1].y == pixelPos.y)//и при этом координата курсора попадает в спрайт; objects[1].getGlobalBounds().contains(pos.x, pos.y)
					{
						//std::cout << "isClicked!\n";//выводим в консоль сообщение об этом
						dx = pos.x - objects[1].x;//делаем разность между позицией курсора и спрайта.для корректировки нажатия; pos.x - objects[1].getPosition().x
						dy = pos.y - objects[1].y;//тоже самое по игреку; pos.y - objects[1].getPosition().y
						isMove = true;//можем двигать спрайт							
					}
				}
			}
			if (event.type == Event::Closed || event.key.code == Keyboard::Escape) window.close();			
			/*if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					//cout << "mouse" << endl;
					if (objects[1].x == pixelPos.x && objects[1].y == pixelPos.y) {
						dx = pixelPos.x - objects[1].x;
						dy = pixelPos.y - objects[1].y;
						objects[1].moving = true;
					}
				}*/
			if (event.type == Event::MouseButtonReleased)
			{//если отпустили клавишу
				if (event.key.code == Mouse::Left)
				{ //а именно левую
					isMove = false; //то не можем двигать спрайт
				}
			}
			if (isMove) {//если можем двигать				
				objects[1].x = pos.x - dx;//двигаем спрайт по Х
				objects[1].y = pos.y - dy;//двигаем по Y
				//p.sprite.setPosition(pos.x - dX, pos.y - dY);//можно и так написать,если у вас нету х и у в классе игрока
			}
		}
		// call update every tick
		update(time);
		window.clear();
		// draw all objects in vector
		for (int i = 0; i < int(objects.size()); i++) 
		{
			window.draw(objects[i].image);
		}
		window.display();
	}
	return 0;
}
