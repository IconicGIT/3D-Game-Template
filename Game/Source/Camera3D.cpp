#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Window.h"
#include "Camera3D.h"

#include "Defs.h"
#include "Globals.h"

Camera3D::Camera3D() : Module()
{
	name.Create("camera 3D");

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 0.0f);
}

// Destructor
Camera3D::~Camera3D()
{}

// Called before render is available
bool Camera3D::Awake()
{
	LOG("Loading Camera3D");
	bool ret = true;




	return ret;
}

// Called before the first frame
bool Camera3D::Start()
{

	return true;
}

// Called each loop iteration
bool Camera3D::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Camera3D::Update(float dt)
{

	//Position Movement
	vec3 newPos(0, 0, 0);
	float speed = 20.0f * dt;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) 
		newPos.y += speed;

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) 
		newPos.y -= speed;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
		newPos -= Z * speed;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT
		&& !(app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_DOWN))
		newPos += Z * speed;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
		newPos -= X * speed;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
		newPos += X * speed;

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		LookAt(vec3(0, 0, 0));

	}

	Position += newPos;

	//Mouse Movement
	if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT/* && app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT*/)
	{
		int dx = -app->input->GetMouseXMotion();
		int dy = -app->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= currentReference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}

		}
		Position = currentReference + Z * length(Position);

	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT/* && app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT*/)
	{
		int dx = -app->input->GetMouseXMotion();
		int dy = -app->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

	}

	LOG("position: %f %f %f", Position.x, Position.y, Position.z);
	LOG("direction x: %f %f %f", X.x, X.y, X.z);
	LOG("direction y: %f %f %f", Y.x, Y.y, Y.z);
	LOG("direction z: %f %f %f", Z.x, Z.y, Z.z);

	return true;
}

// Called each loop iteration
bool Camera3D::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Camera3D::CleanUp()
{
	LOG("Freeing camera 3D");

	return true;
}

void Camera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void Camera3D::Look(const vec3& Position, const vec3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->currentReference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->currentReference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void Camera3D::LookAt(const vec3& Spot)
{
	currentReference = Spot;

	Z = normalize(Position - currentReference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	Position = (Spot.x - 10, Spot.y + 2, Spot.z + 10);

	CalculateViewMatrix();
}