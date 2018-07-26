#include "collision.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

Body::Body(Node2d *node) {
	this->node = node;
}

void Body::update(float delta) {
	velocity.x += acceleration.x * delta;	
	velocity.y += acceleration.y * delta;

	float damping = 0.99f;
	node->position.x += velocity.x * delta * damping;	
	node->position.y += velocity.y * delta * damping;

	float min = 0.001f;
	if (std::abs(velocity.x) < min)
		velocity.x = 0;
	if (std::abs(velocity.y) < min)
		velocity.y = 0;	
}



void Collision::resolve() {

	float tov1l = body1->velocity.dot(normal);
	float cv = body2->velocity.dot(normal) - tov1l;

	float finalVelocity = cv + cv;

	float tIm = body1->inverseMass + body1->inverseMass;

	float imp1 = body1->inverseMass / tIm;
	float imp2 = -body2->inverseMass / tIm;

	body1->node->position.add(normal.x * imp1, normal.y * imp1);
	body2->node->position.add(normal.x * imp2, normal.y * imp2);

	body1->velocity.add(finalVelocity * normal.x * imp1, finalVelocity * normal.y * imp1);
	body2->velocity.add(finalVelocity * normal.x * imp2, finalVelocity * normal.y * imp2);

	std::cout << "Collision resolved! " << std::endl;
}


BoxShape::BoxShape(Body *body, float hw, float hh){
	this->type = CollisionShape::BOX;
	this->body = body;
	halfWidth = hw;
	halfHeight = hh;
}

CircleShape::CircleShape(Body *node, float radius) {
	this->type = CollisionShape::CIRCLE;
	this->body = node;
	this->radius = radius;
}





void DefaultCollisionResolver::resolveCollisions(Collision **collisions, int count) {
	for (int i = 0; i < count; i++)
		collisions[i]->resolve();
}



int DefaultCollisionGenerator::generate(Collision **collisions, int index, int available, CollisionShape *cshape1, CollisionShape *cshape2) {

	bool checked = false; 
	if (cshape1->type == CollisionShape::CIRCLE) {
		CircleShape *circle1 = (CircleShape*) cshape1;

		// circle - circle
		if (cshape2->type == CollisionShape::CIRCLE) {
			checked = true;

			CircleShape *circle2 = (CircleShape*) cshape2;
			int tradius = circle1->radius + circle2->radius;
			if (circle1->body->node->position.distSquare(circle2->body->node->position) < tradius * tradius) {
				collisions[index]->deep = tradius - circle1->body->node->position.dist(circle2->body->node->position);
				collisions[index]->body1 = circle1->body;
				collisions[index]->body2 = circle2->body;
				Vector2 normal = circle1->body->node->position - circle2->body->node->position;
				normal.normalize();
				collisions[index]->normal.set(normal.x, normal.y);

				std::cout << "Collision generated: at index "<< std::to_string(index) << " deep " << std::to_string(collisions[index]->deep) <<
					" normal " << std::to_string(collisions[index]->normal.x) << ","<<std::to_string(collisions[index]->normal.y) << std::endl;

				return 1;
			}
		}
	}
	if (!checked)
		throw std::runtime_error("Not implemented! generate collision on DefaultCollisionGenerator! types " + std::to_string(cshape1->type) + " and " + std::to_string(cshape2->type));
	return 0;
}


PhysicWorld2d::PhysicWorld2d() : PhysicWorld2d(20) {
}

PhysicWorld2d::PhysicWorld2d(int maxCollisionCount) {
	collisionGenerator = new DefaultCollisionGenerator();
	collisionResolver = new DefaultCollisionResolver();

	this->maxCollisionCount = maxCollisionCount;
	this->collisions = new Collision*[maxCollisionCount];
	for (int i = 0; i < maxCollisionCount; i++)
		collisions[i] = new Collision();
}

PhysicWorld2d::~PhysicWorld2d() {
	delete collisionGenerator;
	delete collisionResolver;
	for (auto it = shapes.begin(); it != shapes.end(); it++) {
		delete *it;
	}
	shapes.clear();

	for (int i = 0; i < maxCollisionCount; i++) {
		delete collisions[i];
	}

	delete collisions;
}

void PhysicWorld2d::add(CollisionShape *shape) {
	shapes.push_back(shape);
}

void PhysicWorld2d::remove(CollisionShape *shape) {
	for (auto it = shapes.begin(); it != shapes.end(); it++) {
		if (*it == shape) {
			shapes.erase(it);
			return;
		}
	}
	throw std::runtime_error("Shape not found on remove(CollisionShape) PhysicWorld2d!"); 
}

void PhysicWorld2d::update(float delta) {
	// update bodies
	for (auto it = shapes.begin(); it != shapes.end(); it++) {
		(*it)->body->update(delta);
	}

	// use space partisioning here!!!


	int count = 0;
	for (unsigned int i = 0; i < shapes.size() && count < maxCollisionCount; i++) 
		for (unsigned int j = i+1; j < shapes.size() && count < maxCollisionCount; j++) {
			count += collisionGenerator->generate(collisions, count, maxCollisionCount - count, shapes[i], shapes[j]);
		}

	collisionResolver->resolveCollisions(collisions, count);

}






