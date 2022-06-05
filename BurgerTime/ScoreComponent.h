#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class ScoreComponent final : public dae::BaseComponent, public dae::Observer
{
public:
	ScoreComponent();
	~ScoreComponent();

	void Notify(std::shared_ptr<dae::GameObject> gameObject, ObserverEvent event);
private:
	enum class Points
	{
		DropBurger			= 50,
		KilledHotDog		= 100,
		KilledPickle		= 200,
		KilledEgg			= 300,
		DropBurgerEnemy1	= 500,
		DropBurgerEnemy2	= 1000,
		DropBurgerEnemy3	= 2000,
		DropBurgerEnemy4	= 4000,
		DropBurgerEnemy5	= 8000,
		DropBurgerEnemy6	= 16000,
	};
};
