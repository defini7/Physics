// I used: https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/olcPixelGameEngine.h

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Mario : public olc::PixelGameEngine
{
public:
	Mario()
	{
		sAppName = "Mario";
	}

private:
	struct sRect
	{
		olc::vf2d pos;
		olc::vf2d size;
	};

	sRect player = { { 150.0f, 100.0f }, { 8.0f, 8.0f } };

	//olc::Sprite* sprTileset = nullptr;

	const float fSpeed = 70.0f;

	int nReversion = 1;
	bool bJumping = false;
	bool bJumpInProgress = false;
	float fJumpStartedY = 0.0f;

	enum class SIDE
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	bool IsPointOverlapRect(const olc::vf2d& p, const sRect& r)
	{
		return (p.x + 1 >= r.pos.x + 1 && p.y + 1 >= r.pos.y + 1 && p.x < r.pos.x + 1 + r.size.x && p.y < r.pos.y + 1 + r.size.y);
	}

	bool IsRectOverlapRect(const sRect& r1, const sRect& r2, SIDE& side)
	{
		// side of player
		if (round(r2.pos.y) == r1.pos.y + r1.size.y + 1)
			side = SIDE::DOWN;
		else if (r1.pos.y == round(r2.pos.y) + r2.size.y)
			side = SIDE::UP;
		else if (r1.pos.x == round(r2.pos.x) + r2.size.x)
			side = SIDE::LEFT;
		else
			side = SIDE::RIGHT;

		return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x - 1 &&
			r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y - 1);
	}

protected:
	bool OnUserCreate() override
	{
		//sprTileset = new olc::Sprite("NES-Super-Mario-Bros-Tileset.png");

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY() };

		olc::vf2d vLastPos = player.pos;

		sRect ground = { { 0.0f, (float)ScreenHeight() - 16.0f }, { (float)ScreenWidth() - 1.0f, 16.0f } };

		if (GetKey(olc::LEFT).bHeld)
			player.pos.x -= fSpeed * fElapsedTime * nReversion;

		if (GetKey(olc::RIGHT).bHeld)
			player.pos.x += fSpeed * fElapsedTime * nReversion;

		if (GetKey(olc::UP).bHeld && !bJumpInProgress)
		{
			fJumpStartedY = player.pos.y;
			bJumping = true;
		}

		Clear(olc::DARK_BLUE);

		SIDE side;

		if (bJumping)
		{
			player.pos.y -= fSpeed / 2.0f * fElapsedTime;
			nReversion = 1;

			bJumpInProgress = true;

			if (fJumpStartedY - player.pos.y > 30.0f)
			{
				nReversion = -1;
				bJumping = false;
			}
		}
		else
		{
			if (IsRectOverlapRect(ground, player, side))
			{
				nReversion = -1;
				player.pos = vLastPos + (vLastPos - player.pos);
				bJumpInProgress = false;
			}
			else
			{
				nReversion = 1;
				player.pos.y += 2.0f * fSpeed * fElapsedTime;
			}
		}

		DrawRect(ground.pos, ground.size, olc::WHITE);
		DrawRect(player.pos - olc::vf2d(0, 1), player.size, olc::WHITE);

		return true;
	}
};

int main()
{
	Mario game;

	if (game.Construct(240, 200, 4, 4))
		game.Start();

	return 0;
}
