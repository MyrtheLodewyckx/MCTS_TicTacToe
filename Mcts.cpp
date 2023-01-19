#include "pch.h"
#include "Mcts.h"
#include <algorithm>
#include <iostream>


Mcts_node::Mcts_node(MCTGame* game, bool done, Mcts_node* parent, GameState* pState, int actionIdx)
	:m_pGame(game),
	m_Done(done),
	m_pParent(parent),
	m_ActionIndex(actionIdx),
	m_pGameState(pState)
{
	if (g_AllStates.capacity() != g_Iterations)
		g_AllStates.reserve(g_Iterations);

	if (g_AllNodes.capacity() != g_Iterations)
	g_AllNodes.reserve(g_Iterations);
	g_AllNodes.push_back(this);
}

Mcts_node::~Mcts_node()
{

	if (g_HasDestructorBeenCalled)
		return;

	g_HasDestructorBeenCalled = true;

	if (g_AllNodes.size() != 0)
	{
		for (int i{}; i < g_AllNodes.size(); ++i)
		{
			if (!g_AllNodes[i])
				continue;
			if (g_AllNodes[i] == this)
				continue;

			delete g_AllNodes[i];
			g_AllNodes[i] = nullptr;
		}
		g_AllNodes.clear();
	}
	if (g_AllStates.size() != 0)
	{
		for (int i{}; i < g_AllStates.size(); ++i)
		{
			delete g_AllStates[i];
			g_AllStates[i] = nullptr;
		}
		g_AllStates.clear();
		g_HasDestructorBeenCalled = false;
	}

}

float Mcts_node::CalculateUCT()
{
	if (m_TimesVisited == 0)
	{
		m_UTC = INFINITY;
		return m_UTC;
	}

	float c = 1.41421356f; //constand which is usually sqrt(2);

	Mcts_node* top = this;
	if (m_pParent)
		top = m_pParent;

	m_UTC = m_Wins / (float)m_TimesVisited + c * (float)sqrt((float)log(top->m_TimesVisited) / m_TimesVisited);

	return m_UTC;
}

void Mcts_node::CreateChild()
{
	if (m_Done || m_pGameState->IsTerminal())
		return;

	//Get an action that has not been taken yet
	std::vector<int> actions{ m_pGame->GetAvailableActionIdxs(m_pGameState)};
	std::vector<int> actionsNotTaken{};
	bool hasActionBeenTaken{ false };
	if (actions.size() == 0)
		return;

	for (int i{ 0 }; i < actions.size(); ++i)
	{
		for (int j{ 0 }; j < m_pChildren.size(); ++j)
		{
			if (actions[i] == m_pChildren[j]->GetActionIdx())
				hasActionBeenTaken = true;
		}

		if (!hasActionBeenTaken)
		{
			auto pState = m_pGame->SampleAction(actions[i], m_pGameState);
			g_AllStates.push_back(pState);
			m_pChildren.push_back(new Mcts_node(m_pGame, m_Done, this, pState, actions[i]));
			if (actions.size() - m_pChildren.size() == 0)
				m_Done = true;

			return;
		}
		hasActionBeenTaken = false;
	}

	
}


void Mcts_node::Explore()
{

	//Selection
	Mcts_node* current = this;

	while (current->m_Done)
	{
		auto l = [](Mcts_node* n1, Mcts_node* n2)->bool { return(n1->CalculateUCT() < n2->CalculateUCT()); };
		auto r = std::max_element(current->m_pChildren.begin(), current->m_pChildren.end(), l);

		float maxUCT = current->m_pChildren[std::distance(current->m_pChildren.begin(), r)]->m_UTC;

		std::vector<int> maxElements{};

		for (int i{ 0 }; i < (int)current->m_pChildren.size(); ++i)
		{
			if (current->m_pChildren[i]->m_UTC == maxUCT)
				maxElements.push_back(i);
		}

		if (maxElements.size() != 0)
			current = current->m_pChildren[maxElements[std::rand() % (int)maxElements.size()]];

		else current = current->m_pChildren[std::distance(current->m_pChildren.begin(), r)];
	}

	//Expansion
	current->CreateChild();
	if (current->m_pChildren.size() != 0)
		current = current->m_pChildren[(int)current->m_pChildren.size()-1];


	//Simulation
	current->m_Wins += current->Rollout();
	++current->m_TimesVisited;
	current->CalculateUCT();


	//Back propagate
	auto parent = current;
	if(parent)
	while (parent->m_pParent)
	{
		parent = parent->m_pParent;
		++parent->m_TimesVisited;
		parent->m_Wins += current->m_Wins;

		for (int i{ 0 }; i <parent->m_pChildren.size(); ++i)
		{
			parent->m_pChildren[i]->CalculateUCT();
		}
	}
}

int Mcts_node::Rollout()
{
	if (m_pGameState->IsTerminal())
		return m_pGameState->GetValue();

	GameState* state{};
	int actionIdx{};

	auto actions = m_pGame->GetAvailableActionIdxs(m_pGameState);
	actionIdx = actions[rand() % actions.size()];
	state = m_pGame->SampleAction(actionIdx, m_pGameState);

	while (!state->IsTerminal())
	{
		auto actions = m_pGame->GetAvailableActionIdxs(state);
		actionIdx = actions[rand() % actions.size()];
		state = m_pGame->SampleAction(actionIdx, state,true);
	}

	g_AllStates.push_back(state);
	return state->GetValue();
}

Mcts_node* Mcts_node::GetNext()
{
	if (m_pChildren.size() == 0)
	{
		std::cout << "Tree root has no children!";
		return nullptr;
	}

	for (int i{}; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i]->m_TimesVisited != 0 && m_pChildren[i]->m_TimesVisited == m_pChildren[i]->m_Wins)
			return m_pChildren[i];
	}

	auto l = [](Mcts_node* n1, Mcts_node* n2)->bool { return(n1->m_UTC < n2->m_UTC); };

	auto r = std::max_element(m_pChildren.begin(), m_pChildren.end(), l);

	int i = (int)std::distance(m_pChildren.begin(), r);
	return m_pChildren[i];
}
