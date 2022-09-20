#include "stdafx.h"
#include <tge/engine.h>
#include "Game.h"

#include <functional>
#include <tge/error/ErrorManager.h>


#include "SR/ReflectHelper.h"

using namespace std::placeholders;

#ifdef _DEBUG
#pragma comment(lib,"Engine_Debug.lib")
#pragma comment(lib,"External_Debug.lib")
std::wstring BUILD_NAME = L"Debug";
#else
#pragma comment(lib,"Engine_Release.lib")
#pragma comment(lib,"External_Release.lib")
std::wstring BUILD_NAME = L"Release";
#endif // DEBUG


Game::Game()
{
}


Game::~Game()
{

}

LRESULT Game::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Tga::Engine::GetInstance()->OnCustomWinProcCallback(hWnd, message, wParam, lParam);

	lParam;
	wParam;
	hWnd;
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	}

	return 0;
}


bool Game::Init(const std::wstring& aVersion, HWND /*aHWND*/, std::function<void(GameWorld*)> anOnUpdateCallback)
{
	Tga::EngineCreateParameters createParameters;

	createParameters.myInitFunctionToCall = [this] { InitCallBack(); };
	createParameters.myWinProcCallback = [this](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			// this message is read when the window is closed
		case WM_DESTROY:
		{
			Reflect::SerializeMembers<GameWorld>(&myGameWorld, "propertyMemberData.json");
		}
		}
		return WinProc(hWnd, message, wParam, lParam);
	};
	createParameters.myUpdateFunctionToCall = [&anOnUpdateCallback, this]
	{
		if (anOnUpdateCallback)
			anOnUpdateCallback(&myGameWorld);

		UpdateCallBack();
	};
	createParameters.myApplicationName = L"TGA 2D " + BUILD_NAME + L"[" + aVersion + L"] ";
	//createParameters.myPreferedMultiSamplingQuality = Tga::EMultiSamplingQuality::High;
	createParameters.myActivateDebugSystems = Tga::DebugFeature::Fps |
		Tga::DebugFeature::Mem |
		Tga::DebugFeature::Drawcalls |
		Tga::DebugFeature::Cpu |
		Tga::DebugFeature::Filewatcher |
		Tga::DebugFeature::OptimizeWarnings;

	if (!Tga::Engine::Start(createParameters))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return false;
	}

	// End of program
	return true;
}

void Game::InitCallBack()
{
	Reflect::DeserializeMembers<GameWorld>(&myGameWorld, "propertyMemberData.json");

	myGameWorld.Init();
}

void Game::UpdateCallBack()
{
	myGameWorld.Update(Tga::Engine::GetInstance()->GetDeltaTime());
	myGameWorld.Render();

}
