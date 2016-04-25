#ifndef SCENELOAD_H
#define SCENELOAD_H

#pragma once
#include "GameStateManager.h"
#include "NetworkManager.h"
#include "MenuState.h"
#include "JoinState.h"
#include "HostState.h"
#include "PlayState.h"

class SceneLoad
{
public:
	static void LoadMainMenu()
	{
		GameStateManager::Instance().PushState(new MenuState());

	}
	static void LoadJoinMenu()
	{
		GameStateManager::Instance().PushState(new JoinState());
	}
	static void LoadHostMenu()
	{
		GameStateManager::Instance().PushState(new HostState());
	}
	static void LoadPlayState()
	{
		GameStateManager::Instance().PushState(new PlayState());
	}

	static void StartGame()
	{
		HostState *hostState = (HostState*)GameStateManager::Instance().CurrentState();
		if (hostState)
		{
			if(hostState->NumOfReady >= NetworkManager::Instance().GetNumOfConnections() - 1)
				NetworkManager::Instance().SignalEvent(STARTGAME, true);
		}
	}

	static void Ready()
	{
		NetworkManager::Instance().SignalEvent(STARTGAME, true);
	}
	static void Pop()
	{
		GameStateManager::Instance().Pop();
	}
};

#endif