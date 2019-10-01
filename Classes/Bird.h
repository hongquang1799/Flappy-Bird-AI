#ifndef BIRD_H
#define BIRD_H

#include "CollideableSprite.h"
#include "neural-network/Sequential.h"
#include "neural-network/matrix/Matrix.h"

class Bird : public CollideableSprite
{
public:
	static Bird * createWithFileName(const std::string& fileName);

	static Bird * createWithFrameName(const std::vector<std::string>& frameNames);

	static size_t aliveBirdCount;

	Bird();
	virtual ~Bird();

	void update(float dt) override;

	virtual void onBeginContactEvent(CollideableSprite * other);

	virtual void mutate(std::shared_ptr<mc::Sequential> target, float mutationRate, float mutationRange);

	virtual void randomizeBrain();

	virtual void flap(float acc);

	virtual void execute(mc::Matrix& input);

	virtual void setGravity(float g);

	inline bool isAlive() const
	{
		return alive;
	}

	inline float getLinearSpeed() const
	{
		return linearSpeed;
	}

	inline float getAngularSpeed() const
	{
		return linearSpeed;
	}

	inline std::shared_ptr<mc::Sequential> getBrain() const
	{
		return brain;
	}

	inline float getFitness() const
	{
		return static_cast<float>(score);
	}

	inline void setDistanceXToClosestPipe(float dist)
	{
		distanceXToClosestPipe = dist;
	}

	inline float getDistanceXToClosestPipe() const
	{
		return distanceXToClosestPipe;
	}

	inline size_t getScore() const
	{
		return score;
	}

	inline void increaseScore()
	{
		score++;
	}
protected:
	virtual bool initWithFileName(const std::string& fileName);

	virtual bool initWithFrameName(const std::vector<std::string>& frameNames);

	void mutateMatrix(mc::Matrix dst, mc::Matrix target, float mutationRate, float mutationRange);

	bool alive;
	size_t score;
	float distanceXToClosestPipe;

	float linearSpeed;
	float angularSpeed;
	float gravity;

	std::shared_ptr<mc::Sequential> brain;
};

#endif // __HELLOWORLD_SCENE_H__
