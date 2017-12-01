// Jared White
// March 6, 2017
// WhiteJaredEngine01 GameObject.h


#pragma once
#include "Object.h"
#include "ModelManager.h"
#include "Rigidbody.h"
#include "Script.h"

#include <vector>


// Tpye of collision on this object
enum Collider {
	none = 0,		// No Collider
	//point = 1,	// Point Collider
	sphere = 1,//2,	// Sphere Collider
	aabb = 2//3		// AABB-Collider
};

// A 3-dimensional Object with a Model and Transform.
class GameObject :
	public Object
{
private:

public:
	#pragma region DATA
	Model* objectModel;		// The Model contained within this GameObject
	Rigidbody* rigidbody;	// The Rigidbody of this GameObject
	Collider collider = none;
	std::vector<Script*> scripts/* = std::vector<Script&>()*/;	// All scripts attached to the GameObject

	bool renderObject = true;
	#pragma endregion


	#pragma region CONSTRUCTORS
	// Create a default GameObject with no components to it
	GameObject();

	// Create a GameObject with a Model, default transform, and a tag to refer by
	GameObject(Model* model, std::string tag = "GameObject");

	// Create a GameObject with a Model, Transform, and tag to refer by
	GameObject(Model* model,
		Transform* transform,
		std::string tag = "GameObject");

	// Create a GameObject with a Model, Transform, Rigidbody, and tag to refer by
	GameObject(Model* model,
		Transform* transform,
		Rigidbody* rigidbody,
		std::string tag = "GameObject");

	// Create a GameObject with a Model, Transform, Rigidbody, a set of Scripts, and atag to refer by
	GameObject(Model* model,
		Transform* transform,
		Rigidbody* rigidbody,
		std::vector<Script> scripts,
		std::string tag = "GameObject");

	// Create a GameObject
	GameObject(Model* model,
		glm::vec3 location, glm::vec3 rotation, glm::vec3 scale,
		std::string tag = "GameObject");
	~GameObject();
	#pragma endregion
	

	#pragma region METHODS
	// Attempt to render this GameObject to the screen
	void render() const;

	// Update this GameObject (update the Transform's matrix & the Rigidbody if it exists)
	void update();

	// Add a Script to this GameObject and execute its start function
	void addScript(Script* script);

	// Check to see if this GameObject is colliding with another GameObject
	bool isColliding(const GameObject* otherObject) const;
	#pragma endregion
};