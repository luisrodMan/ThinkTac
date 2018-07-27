#include <WIZ/math.h>
#include <WIZ/nodes2d.h>

class Body {

public:
	Vector2 velocity;
	Vector2 acceleration;
	float inverseMass = 0;
	Node2d *node=nullptr;

	Body(Node2d *node);

	void update(float delta);

};

class Collision {

public:
	Vector2 normal;
	float deep;
	Body *body1 = nullptr;
	Body *body2 = nullptr;

	void resolve();

};

class CollisionShape {

public:
	static const int BOX = 1;
	static const int CIRCLE = 2;
	static const int PLANE = 3;

	int type = 0;

	Body *body = nullptr;
};

class BoxShape : public CollisionShape {
public:
	BoxShape(Body *body, float hw, float hh);

	float halfWidth = 0;
	float halfHeight = 0;
};

class CircleShape : public CollisionShape {
public:
	CircleShape(Body *node, float radius);
	float radius = 0;
};
class PlaneShape : public CollisionShape {
public:
	PlaneShape(Vector2 position, Vector2 normal);
	Vector2 position, normal;
};

class ICollisionGenerator {

public:
	virtual ~ICollisionGenerator(){}
	virtual int generate(Collision **collisions, int index, int available, CollisionShape *cshape1, CollisionShape *cshape2) = 0;

};

class ICollisionResolver {

public:
	virtual ~ICollisionResolver(){}
	virtual void resolveCollisions(Collision **collisions, int count) = 0;

};

class DefaultCollisionGenerator : public ICollisionGenerator {

private:
	int currentIndex = 0;
	void swap(CollisionShape *&shape1, CollisionShape *&shape2);
	int generate(Collision **collisions, int index, int available, CollisionShape *cshape1, CollisionShape *cshape2);

public:

	int generate(Collision **collisions, int start, int max, CollisionShape **shapes, int shapesCount);

};

class DefaultCollisionResolver : public ICollisionResolver {
public:
	void resolveCollisions(Collision **collisions, int count);	
};

class PhysicWorld2d {

private:
	std::vector<CollisionShape*> shapes;
	ICollisionGenerator *collisionGenerator;
	ICollisionResolver *collisionResolver;

	Collision **collisions;
	int maxCollisionCount = 0;

public:

	PhysicWorld2d(int maxCollisionCount=20);
	~PhysicWorld2d();

	void add(CollisionShape *shape);
	
	void remove(CollisionShape *shape);

	void update(float delta);

};


