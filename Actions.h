#pragma once
#include <vector>

class Action
{
public:
	virtual void Execute() = 0;
	virtual ~Action() {}
};

class GameState
{
public:
	virtual ~GameState() {}
	virtual bool IsTerminal() = 0;
	int GetValue() { return m_Value; }
	void SetValue(int v) { m_Value = v; }
protected:
	int m_Value{};
};

class MCTGame
{
public:
	virtual std::vector<int> GetAvailableActionIdxs(GameState* pState) = 0;
	virtual std::vector<int> GetAvailableActionIdxs() = 0;
	virtual GameState* SampleAction(int actionIdx, GameState* pState, bool deleteOldState = false) = 0;

	virtual GameState* GetGameState() = 0;
	virtual ~MCTGame() = default;
};
