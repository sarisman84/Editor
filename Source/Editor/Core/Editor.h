#pragma once

class GameWorld;

class Editor
{
public:
	void Update(GameWorld* aGameWorld);
private:
	void BeginDocker();
	void EndDocker();
};

