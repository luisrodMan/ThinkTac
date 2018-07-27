#include <WIZ/desktop/desktop.h>

#include "config.h"
#include "GameMenuState.h"
#include "GameLevelState.h"

class TickTackApplication : public DefaultApplication {

private:
	SpriteBatch *batch;

public:

	void initialize() {
		WIZ::getGraphics().setGameDimension(WIDTH, HEIGHT);
		Matrix projection;
		projection.setToOrtho(0, WIDTH, 0, HEIGHT, 10, -10);
		batch = new SpriteBatch();
		batch->setProjectionMatrix(projection);
		WIZ::getGameStateManager().push(new GameLevelState(batch, 1));
	}
 
	void sizeChange(int w, int h) {
		// Matrix transform;
		// int x=0; int y = 0;
		// WIZ::getGraphics().getViewport(x, y, w, h);
		// float scale = w / (float) WIZ::getGraphics().getGameWidth();
		// transform.M11 = transform.M22 = scale;
		// batch->setTransformMatrix(transform);
	}

	~TickTackApplication() {
		delete batch;
	}

};


int main() {

	TickTackApplication app;
	DesktopApplicationRunner applicationRunner(&app, "Tick Tack", WIDTH/1.9f, HEIGHT/1.95f);
	applicationRunner.run();

}