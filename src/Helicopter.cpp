#include "Helicopter.h"


#include "Game.h"
#include "Util.h"

Helicopter::Helicopter()
{
	TextureManager::Instance()->load("../Assets/textures/hellie.png", "Helicopter");

	auto size = TextureManager::Instance()->getTextureSize("Helicopter");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(HELICOPTER);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);
}

Helicopter::~Helicopter()
= default;

void Helicopter::draw()
{
	TextureManager::Instance()->draw("Helicopter", 
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 60.0f) );
}

void Helicopter::update()
{
	m_Move();
}

void Helicopter::clean()
{
}

void Helicopter::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void Helicopter::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

glm::vec2 Helicopter::getOrientation() const
{
	return m_orientation;
}

float Helicopter::getTurnRate() const
{
	return m_turnRate;
}

void Helicopter::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float Helicopter::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Helicopter::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void Helicopter::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}

void Helicopter::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
}

float Helicopter::getRotation() const
{
	return m_rotationAngle;
}

void Helicopter::m_Move()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();

	// direction with magnitude
	m_targetDirection = m_destination - getTransform()->position;
	
	// normalized direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if(abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
	}
	
	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	// using the formula pf = pi + vi*t + 0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);

	getTransform()->position += getRigidBody()->velocity;
}
