#include <WIZ/wiz.h>

#include <WIZ/nodes2d.h>

#include "collision.h"

class GameMenuState : public IGameState {

private:
	SpriteBatch *batch;
	Sprite *background;
	Sprite *target;
	Sprite *ball;
	Sprite *ball2;
	Sprite *ballInner;

	PhysicWorld2d world;

public:
	GameMenuState(SpriteBatch *batch);

	void update(float delta);

	void draw();

};