#include "GameMenuState.h"
#include "config.h"

#include <WIZ/xml.h>
#include <iostream>


GameMenuState::GameMenuState(SpriteBatch *batch) {

	std::cout << "bien!" << std::endl;
	XmlDocument doc;
	doc.load("resources/text.txt");

	if (doc.getRoot() != nullptr) {
		std::cout << "tag1: " + doc.getRoot()->getTag() << std::endl;
	} else
		std::cout << " null root!" << std::endl;
	
	this->batch = batch;
	background = new Sprite(WIZ::getTextureManager().loadTexture("resources/background_1.png"));
	target = new Sprite(WIZ::getTextureManager().loadTexture("resources/target_1.png"));
	ball = new Sprite(WIZ::getTextureManager().loadTexture("resources/ball_1.png"));
	ballInner = new Sprite(WIZ::getTextureManager().loadTexture("resources/ball_1.png"));
	ballInner->scale.set(0.5f, 0.5f);


	block1 = new Sprite(WIZ::getTextureManager().loadTexture("resources/block_2.png"));
	block1->position.set(400, 900);
	block2 = new Sprite(WIZ::getTextureManager().loadTexture("resources/block_2.png"));
	block2->position.set(400, 400);

	ball->position.set(100, 100);
	ballInner->position.set(100, 100);

	background->origin.set(0, 0);
	target->position.set(400, 800);
	target->rotation = -90;

	ball2 = new Sprite(WIZ::getTextureManager().loadTexture("resources/ball_1.png"));	

	CircleShape *circle1 = new CircleShape(new Body(ball), ball->getWidth()*0.5f);
	circle1->body->velocity.set(200, 0);
	circle1->body->inverseMass = 1 / 10.0f;

	CircleShape *circle2 = new CircleShape(new Body(ball2), ball2->getWidth()*0.5f);
	circle2->body->velocity.set(0, 300);
	circle2->body->node->position.set(300, 100);
	circle2->body->inverseMass = 1 / 10.0f;

	world.add(circle1);
	world.add(circle2);

	BoxShape *box1 = new BoxShape(new Body(block1), block2->getWidth()*0.5f, block2->getHeight()*0.5f);
	BoxShape *box2= new BoxShape(new Body(block2), block2->getWidth()*0.5f, block2->getHeight()*0.5f);
	box2->body->inverseMass = 0;
	box1->body->inverseMass = 0;

	world.add(box1);
	world.add(box2);

	// play are bounds
	world.add(new PlaneShape(Vector2(0,0), Vector2(1, 0)));
	world.add(new PlaneShape(Vector2(WIDTH,0), Vector2(-1, 0)));
	world.add(new PlaneShape(Vector2(0,0), Vector2(0, 1)));
	world.add(new PlaneShape(Vector2(0,HEIGHT), Vector2(0, -1)));
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

	block1->drawNode(*batch);
	block2->drawNode(*batch);

	ball->drawNode(*batch);
	ball2->drawNode(*batch);
	ballInner->drawNode(*batch);

	batch->end();
}