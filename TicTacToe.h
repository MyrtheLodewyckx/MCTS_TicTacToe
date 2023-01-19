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

	TicTacToeState() = default;
	~TicTacToeState() {}

	TicTacToeState& operator=(const TicTacToeState& t);
	TicTacToeState(const TicTacToeState& t);
	TicTacToeState& operator=(TicTacToeState&& t) noexcept;

	std::vector<cell> m_Cells{};
	bool m_IsEnemyTurn{ true };
	virtual bool IsTerminal() override { return m_IsDone; }

	bool m_IsDone{};
};


class TicTacToe : public MCTGame
{
	int m_Cols{};
	int m_Rows{};

	float m_CellWidth{};
	float m_CellHeight{};

	Vector2f m_BottomLeftPos{};

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
	TicTacToe& operator=(const TicTacToe& t);
	TicTacToe(const TicTacToe& t);
	TicTacToe& operator=(TicTacToe&& t) noexcept;

	TicTacToeState* m_pState{};


	void Render(const Window& window);

	virtual std::vector<int> GetAvailableActionIdxs(GameState* pState) override;
	virtual std::vector<int> GetAvailableActionIdxs() override;

	virtual GameState* SampleAction(int actionIdx, GameState* pState, bool deleteOldState = false) override;
	virtual GameState* GetGameState() override;

	void TakeAction(int cellIdx);	

	bool GetIsDone();
	bool GetIsEnemyTurn();

	void HandleMouseInput(Vector2f mousePos);

};

