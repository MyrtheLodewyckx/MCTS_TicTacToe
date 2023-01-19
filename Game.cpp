#include "pch.h"
#include <iostream>
#include "Game.h"
#include "utils.h"
#include "TicTacToe.h"
#include "Mcts.h"

Game::Game( const Window& window )
	:m_Window{ window }
{	 
	Initialize( );
	std::srand((unsigned int)time(NULL));
}

Game::~Game( )
{
	Cleanup( );
	delete m_pTicTacToe;
	m_pTicTacToe = nullptr;

	delete m_pMctsNode;
	m_pMctsNode = nullptr;
}

void Game::Initialize( )
{
	m_pTicTacToe = new TicTacToe(200, 200, 3, 3, { (m_Window.width - 600) / 2.f,(m_Window.height - 600) / 2.f });
}

void Game::Cleanup( )
{

}

void Game::Update( float elapsedSec )
{
	if (m_pTicTacToe->GetIsEnemyTurn() && !m_pTicTacToe->GetIsDone())
	{
		if (m_pMctsNode)
			delete m_pMctsNode;

		m_pMctsNode = new Mcts_node(m_pTicTacToe,false, nullptr, m_pTicTacToe->GetGameState(), -1);

		for (int i{ 0 }; i < 1000; ++i)
		{
			m_pMctsNode->Explore();
		}

		auto node = m_pMctsNode->GetNext();

		if(node)
		m_pTicTacToe->TakeAction(node->GetActionIdx());
	}

}

void Game::Draw( ) const
{
	utils::ClearBackground();
	m_pTicTacToe->Render(m_Window);
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_UP:
		break;
	case SDLK_RIGHT:
		break;
	case SDLK_1:
	case SDLK_KP_1:
		//std::cout << "Key 1 released\n";
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{

}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
	{
		//std::cout << "Left mouse button released\n";
			Vector2f mousePos{ float(e.x), float(m_Window.height - e.y) };
			m_pTicTacToe->HandleMouseInput(mousePos);
		break;
	}
	case SDL_BUTTON_RIGHT:
		//std::cout << "Right mouse button released\n";
		break;
	case SDL_BUTTON_MIDDLE:
		//std::cout << "Middle mouse button released\n";
		break;
	}
}


