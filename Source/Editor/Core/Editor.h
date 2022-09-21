#pragma once
#include <tge/math/color.h>
#include "SR/ReflectHelper.h"
class GameWorld;



class Editor
{
public:
	void Update(GameWorld* aGameWorld);
	Tga::Color myClearColor;
private:
	void BeginDocker();
	void EndDocker();
	
};

template<>
inline auto Reflect::RegisterElement<Editor>() {
	return Class("Editor",
		Member("Background Color", &Editor::myClearColor)
		);
}