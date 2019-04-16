#include "SFML/Graphics.hpp"
#include <memory>
#include <vector>
#include <iostream>
#include "GameObject.h"
#include "Transform.h"
#include "Components/Component.h"
#include "Components/ShapeComponent.h"
#include "Components/CCollider.h"
#include "Components/CPhysics.h"
#include <iomanip>      // std::setprecision

using namespace Engine;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	

	std::shared_ptr<Transform> t1 = std::make_shared<Transform>();
	std::shared_ptr<Transform> t2 = std::make_shared<Transform>();

	CCollider::debug = true;


	std::shared_ptr<GameObject> go1 = std::make_shared<GameObject>();
	go1->name = "Num 1";
	go1->GetTransform()->SetLocalPosition(sf::Vector2f(400, 300));
	go1->GetTransform()->SetLocalScale(sf::Vector2f(4.f, 4.f));
	std::shared_ptr<ShapeComponent> sc1 = std::make_shared<ShapeComponent>();
	sc1->spin = false;
	sc1->SetColor(sf::Color::Magenta);
	go1->AddComponent<ShapeComponent>(sc1);
	std::shared_ptr<CCollider> col = std::make_shared<CCollider>();
	col->sizeOffset = sc1->GetShape().getSize();
	//col->isTrigger = true;
	go1->AddComponent<CCollider>(col);
	std::shared_ptr<CPhysics> phys1 = std::make_shared<CPhysics>();
	phys1->SetHasGravity(false);
	phys1->SetStatic(true);
	go1->AddComponent<CPhysics>(phys1);

	std::shared_ptr<GameObject> go2= std::make_shared<GameObject>();
	go2->name = "Num 2";
	go2->GetTransform()->SetLocalPosition(sf::Vector2f(200.f, 300.f));
	go2->GetTransform()->SetLocalRotation(0.f);
	go2->GetTransform()->SetLocalScale(sf::Vector2f(2.f, 1.f));
	std::shared_ptr<ShapeComponent> sc2 = std::make_shared<ShapeComponent>();;
	sc2->SetColor(sf::Color::Red);
	//sc2->move = true;
	sc2->spin = false;
	go2->AddComponent<ShapeComponent>(sc2);
	std::shared_ptr<CCollider> col2 = std::make_shared<CCollider>();
	col2->sizeOffset = sc2->GetShape().getSize();
	col2->isTrigger = true;
	go2->AddComponent<CCollider>(col2);
	std::shared_ptr<CPhysics> phys2 = std::make_shared<CPhysics>();
	phys2->SetVelocity(sf::Vector2f(50.f, 0.f));
	phys2->SetHasGravity(false);
	//go2->AddComponent<CPhysics>(phys2);

	//go2->GetTransform()->SetParent(go1->GetTransform());

	std::shared_ptr<GameObject> go3 = std::make_shared<GameObject>();
	go3->name = "Num 3";
	go3->GetTransform()->SetLocalScale(go2->GetTransform()->GetScale());
	go3->GetTransform()->SetLocalRotation(go2->GetTransform()->GetRotation());
	go3->GetTransform()->SetLocalPosition(sf::Vector2f(525.f, 500.f));
	std::shared_ptr<ShapeComponent> sc3 = std::make_shared<ShapeComponent>();;
	sc3->SetColor(sf::Color::Transparent);
	//sc3->move = true;
	//sc3->spin = true;
	go3->AddComponent<ShapeComponent>(sc3);
	std::shared_ptr<CCollider> col3 = std::make_shared<CCollider>();
	col3->sizeOffset = sc3->GetShape().getSize();
	go3->AddComponent<CCollider>(col3);
	std::shared_ptr<CPhysics> phys3 = std::make_shared<CPhysics>();
	phys3->SetVelocity(sf::Vector2f(0.f, 19.6f));
	go3->AddComponent<CPhysics>(phys3);

	//go1->GetTransform()->SetLocalRotation(100.f);


	//go3->GetTransform()->SetParent(go1->GetTransform());

	const float* m = go3->GetTransform()->GetMatrix().getMatrix();
	for (int i = 0; i < 16; i++)
	{
		std::cout << std::fixed << std::setprecision(4) << m[i] << "   ";
		if ((i + 1) % 4 == 0)
			std::cout << std::endl;
	}

	std::cout << go3->GetTransform()->GetRotation() << std::endl;
	std::cout << go3->GetTransform()->GetScale().x << "    " << go1->GetTransform()->GetScale().y;


	GameObject::Instantiate(go1);
	GameObject::Instantiate(go2);
	GameObject::Instantiate(go3);

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

		//Update all objects,
		std::vector<std::shared_ptr<GameObject>> gameObjects = GameObject::GetAllGameObjects();
		for (std::shared_ptr<GameObject> go : gameObjects)
		{
			go->Update(deltaTime);

		}
		//phys2->SetVelocity(sf::Vector2f(50.f, phys2->GetVelocity().y));

		//Then calculate physics/collision
		for (std::shared_ptr<GameObject> go : gameObjects)
		{
			std::vector<std::shared_ptr<CPhysics>> physicsComps = go->GetComponentsOfType<CPhysics>();
			for (std::shared_ptr<CPhysics> physicsComp : physicsComps)
			{
				physicsComp->PhysicsUpdate(deltaTime);
			}

			std::vector<std::shared_ptr<CCollider>> colliders = go->GetComponentsOfType<CCollider>();
			for (std::shared_ptr<CCollider> collider : colliders)
			{
				collider->CheckForCollisions(deltaTime);
			}
		}


		// clear the window with black color
		window.clear(sf::Color::Black);

		//Then render objects, then render debug
		for (std::shared_ptr<GameObject> go : gameObjects)
		{
			std::vector<std::shared_ptr<ShapeComponent>> shapeComponents = go->GetComponentsOfType<ShapeComponent>();
			for (std::shared_ptr<ShapeComponent> sc : shapeComponents)
			{
				window.draw(sc->GetShape(), sc->GetGameObject()->GetTransform()->GetMatrix());
			}


		}

		//Then render debug
		for (std::shared_ptr<GameObject> go : gameObjects)
		{

			std::vector<std::shared_ptr<CCollider>> colliders = go->GetComponentsOfType<CCollider>();
			for (std::shared_ptr<CCollider> col : colliders)
			{
				sf::Vertex outline[5];
				col->GetDebugOutline(outline);
				window.draw(outline, 5, sf::LineStrip);
			}

		}

		window.display();

	}

	return 0;
}