#pragma once
#include <vector>
#include "Actions.h"
#include "Texture.h"

enum class CellState
{
	empty,
	circle,
	cross
};

struct cell
{
	CellState m_CellState{};
	Point2f m_Pos{};
};

class TicTacToeState: public GameState
{
public:
	std::vector<cell> m_Cells{};
	bool m_IsEnemyTurn{ true };
	virtual bool IsTerminal() override { return m_IsDone; }

	~TicTacToeState(){}

	bool m_IsDone{};
};


class TicTacToe : public MCTGame
{
	int m_Cols{};
	int m_Rows{};

	float m_CellWidth{};
	float m_CellHeight{};

	Vector2f m_BottomLeftPos{};

	TicTacToeState* m_pState{};

	Texture* m_pCrossTexture{};
	Texture* m_pCircleTexture{};
	Texture* m_pLoseTexture{};
	Texture* m_pWinTexture{};
	Texture* m_pDrawTexture{};
	Texture* m_pRetryButton{};

	void CheckIfDone(int idx);
	void CheckIfDone(int idx, TicTacToeState* pState);

	void Reset();
	int PosToCellIdx(Vector2f pos);

	Vector2f m_ResetButtonPos{};


public:
	TicTacToe(float width, float height, int rows, int cols, Vector2f bottomLeftPos);
	~TicTacToe();

	void Render(const Window& window);

	virtual std::vector<int> GetAvailableActionIdxs(GameState* pState) override;
	virtual std::vector<int> GetAvailableActionIdxs() override;

	virtual GameState* SampleAction(int actionIdx, GameState* pState) override;
	virtual GameState* GetGameState() override;
	void TakeAction(int cellIdx);

	

	bool GetIsDone();
	bool GetIsEnemyTurn();

	void HandleMouseInput(Vector2f mousePos);

};

