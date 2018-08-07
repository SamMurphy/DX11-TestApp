#include "Camera.h"
#include "Globals.h"
#include <glm/gtx/rotate_vector.hpp>


Camera::Camera() : _forward(0, 0, 1), _up(0, 1, 0), _fov(glm::radians(60.0f)), _boost(false), _pan(0,0,0), _tilt(0,0,0), _position(0,0,0), _rotation(0,0,0)
{
	// 2D
	_orthographicMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 0.1f, 1.1f);
	_view2dMatrix = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
	// 3D
	_projectionMatrix = glm::perspective(_fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.0f);
	_viewMatrix = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));

	_moveSpeed = 1.0f;
	_rotationSpeed = 0.002f;
}


Camera::~Camera()
{
}

/*
*  Updates camera rotation and view
*
*  @param deltaTime Time passed since last frame
*/
void Camera::Update(double deltaTime)
{
	ImGui::SliderFloat("Move Speed", &_moveSpeed, 1.0f, 100.0f);
	ImGui::SliderFloat("Rotation Speed", &_rotationSpeed, 0.001f, 100.0f);

	float multiplier = 1.0f;
	if (_boost) multiplier *= 2.0f;

	
	// Set Camera Rotations
	_rotation += _tilt * _rotationSpeed;
	if (_rotation.x > glm::radians(88.0f))
		_rotation.x = glm::radians(88.0f);
	if (_rotation.x < glm::radians(-88.0f))
		_rotation.x = glm::radians(-88.0f);

	const glm::mat4 lookAt = glm::rotate(GetRotation().z, glm::vec3(0, 0, 1)) *
		glm::rotate(GetRotation().y, glm::vec3(0, 1, 0)) *
		glm::rotate(GetRotation().x, glm::vec3(1, 0, 0));

	glm::vec3 rotation = glm::vec3(lookAt * glm::vec4(GetForward(), 1));



	glm::vec3 rotatedPan = glm::vec3(lookAt * glm::vec4(_pan, 1));
	_position += rotatedPan * multiplier * _moveSpeed;

	glm::mat4 view = glm::lookAt(GetPosition(), GetPosition() + rotation, GetUp());

	//Set View Matrix
	SetViewMatrix(view);
}

bool Camera::OnKeyPress(int key, bool down)
{
	bool used = false;
	// PAN
	if (key == 87) // W
	{
		used = true;
		_pan.z = down ? 1.0f : 0.0f;
	}

	if (key == 83) // S
	{
		used = true;
		_pan.z = down ? -1.0f : 0.0f;
	}

	if (key == 65) // A
	{
		used = true;
		_pan.x = down ? 1.0f : 0.0f;
	}

	if (key == 68) // D
	{
		used = true;
		_pan.x = down ? -1.0f : 0.0f;
	}

	if (key == 32) // Space
	{
		used = true;
		_pan.y = down ? 1.0f : 0.0f;
	}

	if (key == 17) // Left-Control
	{
		used = true;
		_pan.y = down ? -1.0f : 0.0f;
	}

	// Tilt
	if (key == 38) // Up
	{
		used = true;
		_tilt.x = down ? 1.0f : 0.0f;
	}

	if (key == 40) // Down
	{
		used = true;
		_tilt.x = down ? -1.0f : 0.0f;
	}

	if (key == 37) // Left
	{
		used = true;
		_tilt.y = down ? 1.0f : 0.0f;
	}

	if (key == 39) // Right
	{
		used = true;
		_tilt.y = down ? -1.0f : 0.0f;
	}

	if (key == 16) // Shift
	{
		used = true;
		_boost = down;
	}

	return used;
}
