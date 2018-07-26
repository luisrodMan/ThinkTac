#include "GameMenuState.h"
#include "config.h"

GameMenuState::GameMenuState(SpriteBatch *batch) {
	this->batch = batch;
	background = new Sprite(WIZ::getTextureManager().loadTexture("resources/background_1.png"));
	target = new Sprite(WIZ::getTextureManager().loadTexture("resources/target_1.png"));
	ball = new Sprite(WIZ::getTextureManager().loadTexture("resources/ball_1.png"));
	ballInner = new Sprite(WIZ::getTextureManager().loadTexture("resources/ball_1.png"));
	ballInner->scale.set(0.5f, 0.5f);

	ball->position.set(100, 100);
	ballInner->position.set(100, 100);

	background->origin.set(0, 0);
	target->position.set(400, 800);
	target->rotation = -90;


	ball2 = new Sprite(WIZ::getTextureManager().loadTexture("resources/ball_1.png"));	

	CircleShape *circle1 = new CircleShape(new Body(ball), ball->getWidth()*0.5f);
	circle1->body->velocity.set(20, 0);
	circle1->body->inverseMass = 1 / 10.0f;

	CircleShape *circle2 = new CircleShape(new Body(ball2), ball2->getWidth()*0.5f);
	circle2->body->velocity.set(-20, 0);
	circle2->body->node->position.set(300, 100);
	circle2->body->inverseMass = 1 / 10.0f;

	world.add(circle1);
	world.add(circle2);
}

void GameMenuState::update(float delta) {
	target->rotation += 50 * delta;

	ball->rotation += 50 * delta;
	ballInner->rotation -= 50 * delta;

	world.update(delta);
}

void GameMenuState::draw() {
	batch->begin();

	background->drawNode(*batch);

	target->drawNode(*batch);

	ball->drawNode(*batch);
	ball2->drawNode(*batch);
	ballInner->drawNode(*batch);

	batch->end();
}