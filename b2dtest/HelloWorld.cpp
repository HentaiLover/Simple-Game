/*
* Copyright (c) 2006-2007 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <Box2D/Box2D.h>
#include <cmath>
#include <SFML/Graphics.hpp>

#define SCREEN_X 1280.f
#define SCREEN_Y 720.f

#define SCALE 30.f

void createObject(b2World &world, float X, float Y, float width, float height, b2BodyType type)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / 30.f, Y / 30.f);
	BodyDef.type = type;
	b2Body* Body = world.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((width / 2) / SCALE, (height/ 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.3f;  
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void createGround(b2World &world, float X, float Y, float width, float height)
{
	createObject(world, X, Y, width, height, b2_kinematicBody);
}

void createBox(b2World &world, float X, float Y, float width, float height)
{
	createObject(world, X, Y, width, height, b2_dynamicBody);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "test");
	window.setFramerateLimit(60);

	b2World world(b2Vec2(0.0, 10.0), 1);

	createGround(world, SCREEN_X/2, SCREEN_Y-20, SCREEN_X, 16.f);
	
	sf::RectangleShape rect;
	rect.setOutlineThickness(3);
	rect.setOutlineColor(sf::Color::Magenta);
	rect.setSize(sf::Vector2f(32, 32));

	sf::RectangleShape ground;
	
	ground.setFillColor(sf::Color::White);
	ground.setSize(sf::Vector2f(SCREEN_X, 16));
	ground.setOrigin(SCREEN_X/2, 8.f);
	ground.setPosition(SCREEN_X/2, SCREEN_Y-20);
	//ground.setRotation(iter->GetAngle() * 180 / b2_pi);

	sf::Vertex line1[] =
	{
		sf::Vertex(sf::Vector2f(0, SCREEN_Y/2)),
		sf::Vertex(sf::Vector2f(SCREEN_X, SCREEN_Y / 2))
	};

	sf::Vertex line2[] =
	{
		sf::Vertex(sf::Vector2f(SCREEN_X/2, 0)),
		sf::Vertex(sf::Vector2f(SCREEN_X/2, SCREEN_Y))
	};

	while (window.isOpen())
	{
		world.SetGravity(b2Vec2(0.0, 10.0));
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;
			createBox(world, x , y, 32, 32);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			b2Vec2 P00((SCREEN_X / 2)/SCALE, (SCREEN_Y / 2)/SCALE);

			int x = sf::Mouse::getPosition(window).x;
			int y = sf::Mouse::getPosition(window).y;
			b2Vec2 Mpos(x / SCALE, y / SCALE);

			world.SetGravity((Mpos-P00));
		}

		world.Step(1/60.f, 8, 3); 
		window.clear(sf::Color::Black);

		for (b2Body *iter = world.GetBodyList(); iter != 0; iter=iter->GetNext())
		{
			if (iter->GetType() == b2_dynamicBody)
			{
				rect.setFillColor(sf::Color::White);
				rect.setOrigin(16.f, 16.f);
				rect.setPosition(SCALE*iter->GetPosition().x, SCALE*iter->GetPosition().y);
				rect.setRotation(iter->GetAngle() * 180 / b2_pi);
				window.draw(rect);
			}
			
		}
		window.draw(ground);
		window.draw(line1, 2, sf::Lines);
		window.draw(line2, 2, sf::Lines);
		window.display();
	}
	return 0;
}