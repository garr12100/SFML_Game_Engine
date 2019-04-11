#include "SFML/Graphics.hpp"
#include <memory>
#include <vector>
#include <iostream>
#include "GameObject.h"
#include "Transform.h"
#include "Components/Component.h"
#include "Components/ShapeComponent.h"

using namespace Engine;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

	std::vector<std::shared_ptr<GameObject>> GOs;
	

	std::shared_ptr<Transform> t1 = std::make_shared<Transform>();
	std::shared_ptr<Transform> t2 = std::make_shared<Transform>();

	std::shared_ptr<GameObject> go1 = std::make_shared<GameObject>();
	go1->GetTransform()->localPosition = sf::Vector2f(400, 300);
	go1->GetTransform()->localRotation = 15.0f;
	go1->GetTransform()->localScale = sf::Vector2f(5, 5);
	std::shared_ptr<ShapeComponent> sc1 = std::make_shared<ShapeComponent>();;
	sc1->spin = true;
	go1->AddComponent<ShapeComponent>(sc1);
	//const float* m = go1.GetTransform()->GetMatrix().getMatrix();
	//for (int i = 0; i < 16; i++)
	//{
	//	std::cout << m[i] << "   ";
	//	if ((i + 1) % 4 == 0)
	//		std::cout << std::endl;
	//}
	std::shared_ptr<GameObject> go2= std::make_shared<GameObject>();
	go2->GetTransform()->localScale = sf::Vector2f(0.5f, 0.5f);
	go2->GetTransform()->localPosition = sf::Vector2f(100.f, -100.f);
	std::shared_ptr<ShapeComponent> sc2 = std::make_shared<ShapeComponent>();;
	sc2->SetColor(sf::Color::Red);
	go2->AddComponent<ShapeComponent>(sc2);
	go2->GetTransform()->SetParent(go1->GetTransform());
	GOs.push_back(go1);
	GOs.push_back(go2);

	sf::Clock clock;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (std::shared_ptr<GameObject> go : GOs)
		{
			go->Update(deltaTime);
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		for (std::shared_ptr<GameObject> go : GOs)
		{
			std::vector<std::shared_ptr<ShapeComponent>> shapeComponents = go->GetComponentsOfType<ShapeComponent>();
			for (std::shared_ptr<ShapeComponent> sc : shapeComponents)
			{
				window.draw(sc->GetShape());
			}
		}

		window.display();

	}

	return 0;
}