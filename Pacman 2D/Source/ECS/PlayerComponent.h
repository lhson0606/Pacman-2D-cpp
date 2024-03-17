#pragma once
class PlayerComponent
{
public:
	enum MoveDirection
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		NONE
	};

	PlayerComponent();
	PlayerComponent(int initialLife);
	~PlayerComponent();
	int GetLife();
	void DecreaseLife();
	void IncreaseLife();
	void ResetLife();
	bool IsEnergized();
	void SetEnergized(int duration);
	int GetAnimIdx();
	void SetAnimIdx(int idx);
	bool HasEnteredNewTile();
	void SetEnteredNewTile(bool val);
	MoveDirection GetInputDirection();
	void SetInputDirection(MoveDirection dir);
private:
	int life;
	int pelletCount = 0;
	bool isDead = false;
	bool isEnergized = false;
	//#todo: use Sprite class instead
	int animIdx = 0;
	bool hasEnteredNewTile = false;
	MoveDirection inpDir = NONE;
};
