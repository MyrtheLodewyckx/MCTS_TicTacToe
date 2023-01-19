//https://medium.com/@_michelangelo_/monte-carlo-tree-search-mcts-algorithm-for-dummies-74b2bae53bfa
#pragma once
#include <math.h>
#include <vector>
#include "Actions.h"

class Mcts_node
{
	std::vector<Mcts_node*> m_pChildren;

	int m_Wins{};
	int m_TimesVisited{ 0 };

	MCTGame* m_pGame = nullptr;
	GameState* m_pGameState = nullptr;

	bool m_Done{ false };

	Mcts_node* m_pParent = nullptr;

	int m_ActionIndex{};
	float m_UTC{ 0 };

	float CalculateUCT();
	void CreateChild();
	int Rollout();

public:

	 Mcts_node(MCTGame* game, bool done, Mcts_node* parent,GameState* pState, int actionIdx);
	~Mcts_node();

	void Explore();
	
	int GetActionIdx() { return m_ActionIndex; }

	Mcts_node* GetNext();
};

static const int g_Iterations{ 1000 };
static std::vector<Mcts_node*> g_AllNodes{};
static std::vector<GameState*> g_AllStates{};
static bool g_HasDestructorBeenCalled{false};