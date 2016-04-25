#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"

class MenuState : public GameState
{
public:
	MenuState();
	~MenuState();

	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void FixedUpdate(float dt) override;
	virtual void ShutDown() override;

protected:
	class SceneNode* Text;
	class Camera* camera;
};

#endif // ! MENUSTATE_H
