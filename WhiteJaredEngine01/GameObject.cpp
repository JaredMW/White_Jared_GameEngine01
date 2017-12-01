// Jared White
// March 6, 2017
// WhiteJaredEngine01 GameObject.cpp


#include "GameObject.h"
#include "Engine.h"
#include <math.h>


// Create a default GameObject with no model
GameObject::GameObject()
	: Object()
{
	tag = "GameObject";
}

// Create a GameObject with a specified Model and default Transform
GameObject::GameObject(Model* model, std::string tag)
{
	// Attempt to create a new GameObject
	objectModel = Engine::modelManager.addModel(
		model->modelMesh.filename,
		model->modelTexture.filename,
		tag);

	this->tag = tag;						// Setup the GameObject's tag
	this->scripts = std::vector<Script*>();	// Setup the Scripts

	if (objectModel != nullptr)
	{
		Engine::addGameObject(this);
	}
}


// Create a GameObject with a specified Model and Transform
GameObject::GameObject(Model* model, Transform* transform, std::string tag)
{
	// Attempt to create a new GameObject
	objectModel = Engine::modelManager.addModel(model);

	this->transform = transform;			// Setup the custom Transform
	rigidbody = nullptr;					// Set a null Rigidbody - does not have a Rigidbody
	this->tag = tag;						// Set the GameObject tag
	this->scripts = std::vector<Script*>();	// Setup the scripts

	// Track this new GameObject if model is valid
	if (objectModel != nullptr) { Engine::addGameObject(this); }
}


// Create a GameObject with a specified Model, Transform, and Rigidbody
GameObject::GameObject(Model* model, Transform* transform, Rigidbody* rigidbody, std::string tag)
{
	// Attempt to create a new GameObject
	objectModel = Engine::modelManager.addModel(model);

	this->transform = transform;					// Setup the custom Transform
	this->rigidbody = rigidbody;					// Setup the Rigidbody
	this->rigidbody->transform = this->transform;	// Let Rigidbody point to this Transform
	this->tag = tag;								// Set the GameObject tag
	this->scripts = std::vector<Script*>();			// Setup the scripts

	// Track this new GameObject if model is valid
	if (objectModel != nullptr) { Engine::addGameObject(this); }
}


// Create a GameObject with a specified Model, Transform, Rigidbody, and Scripts
GameObject::GameObject(Model* model, Transform* transform, Rigidbody* rigidbody, std::vector<Script> scripts, std::string tag)
{
	// Attempt to create a new GameObject
	objectModel = Engine::modelManager.addModel(model);

	this->transform = transform;					// Setup the custom Transform
	this->rigidbody = rigidbody;					// Setup the Rigidbody
	this->rigidbody->transform = this->transform;	// Let Rigidbody point to this Transform
	this->tag = tag;								// Set the GameObject tag

	// Setup the Scripts for this GameObject
	this->scripts = std::vector<Script*>();
	for (unsigned int i = 0; i < scripts.size(); i++)
	{
		this->scripts[i]->transform = this->transform;		// Set each Script's GameObject to this
		this->scripts[i]->start();							// Start each Script
	}

	//this->scripts = scripts;						// Set the Scripts for the GameObject
	//for (unsigned int i = 0; i < scripts.size(); i++)
	//{
	//	scripts[i].transform = this->transform;		// Set each Script's GameObject to this
	//	scripts[i].start();							// Start each Script
	//}

	// Track this new GameObject if model is valid
	if (objectModel != nullptr) { Engine::addGameObject(this); }
}


// Create a GameObject with a specified Model and specified Transform
GameObject::GameObject(Model* model,
	glm::vec3 location, glm::vec3 rotation, glm::vec3 scale,
	std::string tag)
	: GameObject(model, &Transform(location, rotation, scale), tag)
{
}


GameObject::~GameObject()
{

}


// Attempt to draw this GameObject to the screen
void GameObject::render() const
{
	// If rendering enabled
	if (renderObject)
	{
		// Upload this GameObject's Transform's local-to-world matrix
		glUniformMatrix4fv(
			Engine::shader.shaderObjWorldMatLoc,
			1,
			GL_FALSE,
			&transform->getLocalWorldMat()[0][0]);

		// Attempt to draw the model
		objectModel->render();
	}

}


// Update the GameObject's Transform, Rigidbody, Scripts and Model
void GameObject::update()
{
	// Update the GameObject's RigidBody (if it has one)
	if (rigidbody != nullptr) { rigidbody->update(); }

	// Update the GameObject's scripts
	for (unsigned int i = 0; i < scripts.size(); i++)
	{
		if (scripts[i]->isActive) { scripts[i]->update(); }
	}

	// Update the GameObject's Transform
	transform->update();
}

bool GameObject::isColliding(const GameObject* otherObject) const
{
	// No collision if either object does not have a collider
	if (collider == Collider::none || otherObject->collider == Collider::none)
	{
		return false;
	}


	// If this collider is a sphere...
	else if (collider == Collider::sphere)
	{
		///	(SPHERE-SPHERE)
		// If other collider is a sphere
		if (otherObject->collider == Collider::sphere)
		{
			// If the square distance between the two spheres is smaller
			//		than the sum of their square radii, they are colliding.
			float sqrDist = powf(otherObject->transform->location.x - transform->location.x, 2)
				+ powf(otherObject->transform->location.y - transform->location.y, 2)
				+ powf(otherObject->transform->location.z - transform->location.z, 2);

			if (sqrDist < powf(
				((transform->scale.x * objectModel->modelMesh.size.x	// extents = scale * size
					+ transform->scale.y * objectModel->modelMesh.size.y
					+ transform->scale.z * objectModel->modelMesh.size.z)
				+ (otherObject->transform->scale.x * otherObject->objectModel->modelMesh.size.x
					+ otherObject->transform->scale.y * otherObject->objectModel->modelMesh.size.y
					+ otherObject->transform->scale.z * otherObject->objectModel->modelMesh.size.z)) / 3.f,
				2))
			{
				return true;
			}

			return false;
		}


		///	(Sphere-AABB)
		// If the other collider is an AABB box
		else if (otherObject->collider == Collider::aabb)
		{
			// Get the distance between sphere and AABB box
			glm::vec3 dist = transform->location - otherObject->transform->location;
			glm::vec3 extents = otherObject->transform->scale * otherObject->objectModel->modelMesh.size;	// Obtain the extents

													// Get the point on the AABB closest to the sphere
			glm::vec3 point = otherObject->transform->location + glm::clamp(dist, -extents, extents);

			// Get the distance from the sphere and the point & square its magnitude
			glm::vec3 pointDist = transform->location - point;
			float sqrPointDist =
				powf(pointDist.x, 2)
				+ powf(pointDist.y, 2)
				+ powf(pointDist.z, 2);

			// If the square distance between the point and the sphere is less than
			//		the radius of the sphere, then collision
			if (sqrPointDist <
				pow((transform->scale.x * objectModel->modelMesh.size.x
					+ transform->scale.y * objectModel->modelMesh.size.y
					+ transform->scale.z * objectModel->modelMesh.size.z) / 3.f, 2.f))
			{
				return true;
			}

			return false;
		}

		return false;
	}


	// If this collider is an AABB box...
	else if (collider == Collider::aabb)
	{
		/// (AABB-Sphere)
		// If the other object has a sphere collider
		if (otherObject->collider == Collider::sphere)
		{
			// Get the distance between sphere and AABB box
			glm::vec3 dist = otherObject->transform->location - transform->location;
			glm::vec3 extents = transform->scale * objectModel->modelMesh.size;	// Obtain the extents

			// Get the point on the AABB closest to the sphere
			glm::vec3 point = transform->location + glm::clamp(dist, -extents, extents);

			// Get the distance from the sphere and the point & square its magnitude
			glm::vec3 pointDist = otherObject->transform->location - point;
			float sqrPointDist =
				powf(pointDist.x, 2)
				+ powf(pointDist.y, 2)
				+ powf(pointDist.z, 2);

			// If the square distance between the point and the sphere is less than
			//		the radius of the sphere, then collision
			if (sqrPointDist <
				pow((otherObject->transform->scale.x * otherObject->objectModel->modelMesh.size.x
					+ otherObject->transform->scale.y * otherObject->objectModel->modelMesh.size.y
					+ otherObject->transform->scale.z * otherObject->objectModel->modelMesh.size.z) / 3.f, 2.f))
			{
				return true;
			}

			return false;
		}


		/// (AABB-AABB)
		// If they are both AABB boxes
		else if (otherObject->collider == Collider::aabb)
		{
			glm::vec3 dist = otherObject->transform->location - transform->location;

			// If minB < maxA...
			// If maxB > minA...
			/*if (otherObject->transform->location.x + (otherObject->objectModel->modelMesh.minExtent.x * otherObject->transform->scale.x)
					< transform->location.x + (objectModel->modelMesh.maxExtent.x * transform->scale.x)
				&& otherObject->transform->location.x + (otherObject->objectModel->modelMesh.maxExtent.x * otherObject->transform->scale.x)
					> transform->location.x + (objectModel->modelMesh.minExtent.x * transform->scale.x)
				&& otherObject->transform->location.x + (otherObject->objectModel->modelMesh.minExtent.y * otherObject->transform->scale.y)
					< transform->location.y + (objectModel->modelMesh.maxExtent.y * transform->scale.y)
				&& otherObject->transform->location.y + (otherObject->objectModel->modelMesh.maxExtent.y * otherObject->transform->scale.y)
					> transform->location.y + (objectModel->modelMesh.minExtent.y * transform->scale.y)
				&& otherObject->transform->location.z + (otherObject->objectModel->modelMesh.minExtent.z * otherObject->transform->scale.z)
					< transform->location.z + (objectModel->modelMesh.maxExtent.z * transform->scale.z)
				&& otherObject->transform->location.z + (otherObject->objectModel->modelMesh.maxExtent.z * otherObject->transform->scale.z)
					> transform->location.z + (objectModel->modelMesh.minExtent.z * transform->scale.z))
			{
				return true;
			}

			return false;*/

			if (abs(dist.x) > transform->scale.x// * objectModel->modelMesh.size.x
					+ otherObject->transform->scale.x// * otherObject->objectModel->modelMesh.size.x
				|| abs(dist.y) > transform->scale.y// * objectModel->modelMesh.size.y
					+ otherObject->transform->scale.y// * otherObject->objectModel->modelMesh.size.y
				|| abs(dist.z) > transform->scale.z// * objectModel->modelMesh.size.z
					+ otherObject->transform->scale.z/* * otherObject->objectModel->modelMesh.size.z*/)
			{
				return false;
			}

			return true;
		}

		return false;
	}

	return false;
}


// Add a Script to the GameObject and execute its start function
void GameObject::addScript(Script* script)
{
	script->transform = transform;
	scripts.push_back(script);
	script->start();
}