#include <WIZ/wiz.h>
#include <WIZ/nodes2d.h>

class GameLevelState : public IGameState {

private:
	SpriteBatch *batch;
	Node2d rootNode;

public:
	GameLevelState(SpriteBatch *batch, int level);
	~GameLevelState();

	void update(float delta);

	void draw();
	
};