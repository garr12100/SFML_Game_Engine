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
#include "Utility/MathUtility.h"
#include <iomanip>      // std::setprecision
#include <thread>

using namespace Engine;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	

	

	CCollider::debug = true;


	std::shared_ptr<GameObject> go1 = std::make_shared<GameObject>();
	go1->name = "Num 1";
	go1->GetTransform()->SetLocalPosition(sf::Vector2f(0.f, 90.f));
	go1->GetTransform()->SetLocalScale(sf::Vector2f(4.f, 4.f));
	go1->GetTransform()->SetLocalRotation(0.f);
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
	//phys1->SetStatic(true);
	phys1->SetMass(10.f);
	phys1->SetVelocity(sf::Vector2f(0.f, 100.f));
	go1->AddComponent<CPhysics>(phys1);


	std::shared_ptr<GameObject> go2= std::make_shared<GameObject>();
	go2->name = "Num 2";
	go2->GetTransform()->SetLocalPosition(sf::Vector2f(211.f, 206.5f));
	go2->GetTransform()->SetLocalRotation(180.f);
	go2->GetTransform()->SetLocalScale(sf::Vector2f(3.f, 3.f));
	std::shared_ptr<ShapeComponent> sc2 = std::make_shared<ShapeComponent>();;
	sc2->SetSize(sf::Vector2f(6.f, 5.f));
	sc2->SetColor(sf::Color::Red);
	//sc2->move = true;
	sc2->spin = false;
	go2->AddComponent<ShapeComponent>(sc2);
	std::shared_ptr<CCollider> col2 = std::make_shared<CCollider>();
	col2->sizeOffset = sc2->GetShape().getSize();
	//col2->isTrigger = true;
	go2->AddComponent<CCollider>(col2);
	std::shared_ptr<CPhysics> phys2 = std::make_shared<CPhysics>();
	phys2->SetVelocity(sf::Vector2f(10.f, 19.6f));
	phys2->SetHasGravity(false);
	phys2->SetStatic(false);

	go2->AddComponent<CPhysics>(phys2);

	//go2->GetTransform()->SetParent(go1->GetTransform());

	std::shared_ptr<GameObject> go3 = std::make_shared<GameObject>();
	go3->name = "Num 3";
	go3->GetTransform()->SetLocalPosition(sf::Vector2f(0, 375.f));
	go3->GetTransform()->SetLocalScale(sf::Vector2f(2.f, 1.f));
	go3->GetTransform()->SetLocalRotation(0.f);
	std::shared_ptr<ShapeComponent> sc3 = std::make_shared<ShapeComponent>();
	//sc3->SetSize(sf::Vector2f(8.f, 3.f));

	sc3->SetColor(sf::Color::Yellow);
	//sc3->move = true;
	//sc3->spin = true;
	go3->AddComponent<ShapeComponent>(sc3);
	std::shared_ptr<CCollider> col3 = std::make_shared<CCollider>();
	col3->sizeOffset = sc3->GetShape().getSize();
	go3->AddComponent<CCollider>(col3);
	//col3->isTrigger = true;
	std::shared_ptr<CPhysics> phys3 = std::make_shared<CPhysics>();
	phys3->SetStatic(false);
	phys3->SetMass(1.f);
	//phys3->SetVelocity(sf::Vector2f(0.f, -100.f));
	//phys3->SetHasGravity(false);
	phys3->SetHasDrag(false);
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
	//GameObject::Instantiate(go2);
	GameObject::Instantiate(go3);

	sf::Transform worldTransform;
	worldTransform.scale(sf::Vector2f(1.f, -1.f));
	worldTransform.translate(sf::Vector2f(200.f, -(float)window.getSize().y));

	sf::Clock clock;
	float t = 0.f;
	float timeScale = 1.f;
	float fixedDeltaTime = 0.f;
	float physicsFrameRate = 60.f;
	// run the program as long as the window is open
	while (window.isOpen())
	{
		//std::this_thread::sleep_for(
		float deltaTimeOriginal = clock.restart().asSeconds();
		float deltaTime = deltaTimeOriginal * timeScale;
		t += deltaTimeOriginal;
		fixedDeltaTime += deltaTimeOriginal;

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

		//Then calculate physics/collision, using only Fixed Delta Time
		if (fixedDeltaTime >= 1.f / physicsFrameRate)
		{
			//fixedDeltaTime = 1.f / physicsFrameRate;
			for (std::shared_ptr<GameObject> go : gameObjects)
			{
				std::vector<std::shared_ptr<CPhysics>> physicsComps = go->GetComponentsOfType<CPhysics>();
				for (std::shared_ptr<CPhysics> physicsComp : physicsComps)
				{
					physicsComp->PhysicsUpdate(fixedDeltaTime);
				}

			}

			for (std::shared_ptr<GameObject> go : gameObjects)
			{

				std::vector<std::shared_ptr<CCollider>> colliders = go->GetComponentsOfType<CCollider>();
				for (std::shared_ptr<CCollider> collider : colliders)
				{
					collider->CheckForCollisions(fixedDeltaTime);
				}
			}

			for (std::shared_ptr<GameObject> go : gameObjects)
			{

				std::vector<std::shared_ptr<CCollider>> colliders = go->GetComponentsOfType<CCollider>();
				for (std::shared_ptr<CCollider> collider : colliders)
				{
					collider->ResetPreviousTransform();
				}
			}

			fixedDeltaTime = 0.f;
		}


		// clear the window with black color
		window.clear(sf::Color::Black);

		//Then render objects, then render debug
		for (std::shared_ptr<GameObject> go : gameObjects)
		{
			std::vector<std::shared_ptr<ShapeComponent>> shapeComponents = go->GetComponentsOfType<ShapeComponent>();
			for (std::shared_ptr<ShapeComponent> sc : shapeComponents)
			{
				window.draw(sc->GetShape(),worldTransform * sc->GetGameObject()->GetTransform()->GetMatrix());
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
				Utility::TransformVertices(worldTransform, outline, 5);
				window.draw(outline, 5, sf::LineStrip);
				for (auto bestEdge : col->bestEdges)
				{ 
					bestEdge = Utility::TransformVertices(worldTransform, bestEdge);
					window.draw( &bestEdge[0], bestEdge.size(), sf::Lines);
				}
				if(col->contactVerts.size() > 0)
					window.draw(&Utility::TransformVertices(worldTransform, col->contactVerts)[0], col->contactVerts.size(), sf::Points);

			}

		}

		window.display();

	}

	return 0;
}