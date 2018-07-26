#include <WIZ/desktop/desktop.h>

#include "config.h"
#include "GameMenuState.h"

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
		WIZ::getGameStateManager().push(new GameMenuState(batch));
	}

	void sizeChange(int w, int h) {
		Matrix projection;
		int x=0;
		int y = 0;
		WIZ::getGraphics().getViewport(x, y, w, h);
		projection.setToOrtho(0, w, 0, h, 10, -10);
		batch->setProjectionMatrix(projection);
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