#include "collision.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

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


CircleShape::CircleShape(Body *node, float radius) {
	this->type = CollisionShape::CIRCLE;
	this->body = node;
	this->radius = radius;
}

BoxShape::BoxShape(Body *body, float hw, float hh) {
	this->type = CollisionShape::BOX;
	this->body = body;
	halfHeight = hh;
	halfWidth = hw;
}


PlaneShape::PlaneShape(Vector2 position, Vector2 normal) {
	this->type = CollisionShape::PLANE;
	this->normal.set(normal.x, normal.y);
	this->position.set(position.x, position.y);
}
	

void Collision::resolve() {

	float tov1l = body1->velocity.dot(normal);
	float cv = -tov1l;

	if (body2 != nullptr)
		cv += body2->velocity.dot(normal);

	float finalVelocity = cv + cv;

	float tIm = body1->inverseMass;
	
	if (body2 != nullptr)
		tIm += body2->inverseMass;

	float imp1 = body1->inverseMass / tIm;
	body1->node->position.add(normal.x * imp1, normal.y * imp1);
	body1->velocity.add(finalVelocity * normal.x * imp1, finalVelocity * normal.y * imp1);

	if (body2 != nullptr) {
		float imp2 = -body2->inverseMass / tIm;
		body2->node->position.add(normal.x * imp2, normal.y * imp2);
		body2->velocity.add(finalVelocity * normal.x * imp2, finalVelocity * normal.y * imp2);
	}
	
}


void DefaultCollisionGenerator::swap(CollisionShape *&shape1, CollisionShape *&shape2) {
	CollisionShape *temp = shape1;
	shape1 = shape2;
	shape2 = temp; 
}


int DefaultCollisionGenerator::generate(Collision **collisions, int index, int available, CollisionShape *cshape1, CollisionShape *cshape2) {

	if (cshape1->type != CollisionShape::CIRCLE && cshape2->type == CollisionShape::CIRCLE)
		swap(cshape1, cshape2);

	// CIRCLE
	if (cshape1->type == CollisionShape::CIRCLE) {
		CircleShape *circle1 = (CircleShape*) cshape1;

		// CIRCLE - CIRCLE
		if (cshape2->type == CollisionShape::CIRCLE) {
			CircleShape *circle2 = (CircleShape*) cshape2;
			int tradius = circle1->radius + circle2->radius;
			if (circle1->body->node->position.distSquare(circle2->body->node->position) < tradius * tradius) {
				collisions[index]->body1 = circle1->body;
				collisions[index]->body2 = circle2->body;
				collisions[index]->deep = tradius - circle1->body->node->position.dist(circle2->body->node->position);
				Vector2 normal = circle1->body->node->position - circle2->body->node->position;
				normal.normalize();
				collisions[index]->normal.set(normal.x, normal.y);
				return 1;	
			}
			return 0;
		}

		// CIRCLE - BOX
		if (cshape2->type == CollisionShape::BOX) {
			BoxShape *box = (BoxShape*) cshape2;
			Vector2 cp = circle1->body->node->position - box->body->node->position;
			cp.rotate(-box->body->node->rotation);
			cp.x = clamp(cp.x, -box->halfWidth, box->halfWidth);
			cp.y = clamp(cp.y, -box->halfHeight, box->halfHeight);
			cp.rotate(box->body->node->rotation);
			cp.add(box->body->node->position);	

			Vector2 direction = circle1->body->node->position - cp;

			if (direction.getLenght2() < circle1->radius * circle1->radius) {
				collisions[index]->body1 = circle1->body;
				collisions[index]->body2 = box->body;
				collisions[index]->deep = circle1->radius - direction.getLenght();
				direction.normalize();
				collisions[index]->normal.set(direction.x, direction.y);
				return 1;
			}
			return 0;
		}

		// CIRCLE - PLANE
		if (cshape2->type == CollisionShape::PLANE) {
			PlaneShape *plane = (PlaneShape*) cshape2;

			Vector2 cp = circle1->body->node->position - plane->position;
			float dist = cp.dot(plane->normal);
			if (dist < circle1->radius) {
				collisions[index]->body1 = circle1->body;
				collisions[index]->body2 = nullptr;
				collisions[index]->deep = circle1->radius - dist;
				collisions[index]->normal.set(plane->normal.x, plane->normal.y);
				return 1;
			}
			return 0;
		}

	}
	if (cshape1->type == CollisionShape::BOX) {
		return 0;
	}

	if (cshape1->type == CollisionShape::PLANE) {
		return 0;
	}

	throw std::runtime_error("Not implemented! generate collision on DefaultCollisionGenerator! types " + std::to_string(cshape1->type) + " and " + std::to_string(cshape2->type));
}


void DefaultCollisionResolver::resolveCollisions(Collision **collisions, int count) {
	for (int i = 0; i < count; i++)
		collisions[i]->resolve();
}



PhysicWorld2d::PhysicWorld2d(int maxCollisionCount) {
	this->maxCollisionCount = maxCollisionCount;
	this->collisions = new Collision*[maxCollisionCount];
	for (int i = 0; i < maxCollisionCount; i++)
		collisions[i] = new Collision();
	collisionGenerator = new DefaultCollisionGenerator();
	collisionResolver = new DefaultCollisionResolver();
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
		if ((*it)->body != nullptr)
			(*it)->body->update(delta);
	}

	// generate contacts!
	int count = 0;
	for (unsigned int i = 0; i < shapes.size() && count < maxCollisionCount; i++) 
		for (unsigned int j = i+1; j < shapes.size() && count < maxCollisionCount; j++) {
			count += collisionGenerator->generate(collisions, count, maxCollisionCount - count, shapes[i], shapes[j]);
	}
	
	collisionResolver->resolveCollisions(collisions, count);
}