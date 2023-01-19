#include "pch.h"
#include "TicTacToe.h"
#include "utils.h"

TicTacToe::TicTacToe(float width, float height, int rows, int cols, Vector2f bottomLeftPos)
	:m_CellWidth(width),
	m_CellHeight(height),
	m_BottomLeftPos(bottomLeftPos),
	m_Cols(cols),
	m_Rows(rows)
{

	m_pCrossTexture = new Texture("Resources/Cross.png");
	m_pCircleTexture = new Texture("Resources/Circle.png");
	m_pLoseTexture = new Texture("Resources/Lose.png");
	m_pWinTexture = new Texture("Resources/Win.png");
	m_pDrawTexture = new Texture("Resources/Draw.png");
	m_pRetryButton = new Texture("Resources/Retry.png");


	m_pState = new TicTacToeState();
	m_pState->m_Cells.reserve(rows * cols);
	for (float y{ bottomLeftPos.y + (rows-1) * height }; y > bottomLeftPos.y - height; y -= height)
	{
		for (float x{ bottomLeftPos.x }; x < bottomLeftPos.x + width * cols; x += width)
		{
			m_pState->m_Cells.push_back({ CellState::empty,{x,y} });
		}
	}
}

TicTacToe::~TicTacToe()
{
	delete m_pCircleTexture;
	m_pCircleTexture = nullptr;

	delete m_pCrossTexture;
	m_pCrossTexture = nullptr;

	delete m_pDrawTexture;
	m_pDrawTexture = nullptr;

	delete m_pLoseTexture;
	m_pLoseTexture = nullptr;

	delete m_pState;
	m_pState = nullptr;

	delete m_pWinTexture;
	m_pWinTexture = nullptr;
}

void TicTacToe::Render(const Window& window)
{
	
	for (int i{}; i < (int)m_pState->m_Cells.size(); ++i)
	{
		utils::SetColor({ 1,1,1,1 });
		utils::DrawRect(m_pState->m_Cells[i].m_Pos, m_CellWidth, m_CellWidth, 2.f);

		switch (m_pState->m_Cells[i].m_CellState)
		{
		case CellState::circle:
			m_pCircleTexture->Draw(Rectf{ m_pState->m_Cells[i].m_Pos.x,m_pState->m_Cells[i].m_Pos.y, m_CellWidth,m_CellHeight }, 
				{ 0,m_pCircleTexture->GetHeight(),m_pCircleTexture->GetWidth(),m_pCircleTexture->GetHeight() });
			break;
		case CellState::cross:
			m_pCrossTexture->Draw(Rectf{ m_pState->m_Cells[i].m_Pos.x,m_pState->m_Cells[i].m_Pos.y, m_CellWidth,m_CellHeight },
				{ 0,m_pCrossTexture->GetHeight(),m_pCrossTexture->GetWidth(),m_pCrossTexture->GetHeight() });
			break;
		}
	}

	if (m_pState->m_IsDone)
	{
		if (m_pState->GetValue() == -1)
			m_pWinTexture->Draw(Rectf{ m_pState->m_Cells[0].m_Pos.x,m_pState->m_Cells[0].m_Pos.y - ((m_Rows - 1) * m_CellHeight),m_Cols * m_CellWidth,m_Rows * m_CellHeight },
				Rectf{ 0, m_pWinTexture->GetHeight(),m_pWinTexture->GetWidth(),m_pWinTexture->GetHeight() });
		else if(m_pState->GetValue() == 1)
			m_pLoseTexture->Draw(Rectf{ m_pState->m_Cells[0].m_Pos.x,m_pState->m_Cells[0].m_Pos.y - ((m_Rows - 1) * m_CellHeight),m_Cols * m_CellWidth,m_Rows * m_CellHeight },
			Rectf{ 0, m_pLoseTexture->GetHeight(),m_pLoseTexture->GetWidth(),m_pLoseTexture->GetHeight() });
		else
			m_pDrawTexture->Draw(Rectf{ m_pState->m_Cells[0].m_Pos.x,m_pState->m_Cells[0].m_Pos.y - ((m_Rows - 1) * m_CellHeight),m_Cols * m_CellWidth,m_Rows * m_CellHeight },
				Rectf{ 0, m_pDrawTexture->GetHeight(),m_pDrawTexture->GetWidth(),m_pDrawTexture->GetHeight() });
	

		const float buttonHeight{ 37.f };
		const float buttonWidth{ 125.f };
		m_ResetButtonPos ={ window.width / 2.f - buttonWidth / 2.f,20.f };
		m_pRetryButton->Draw(Rectf{ m_ResetButtonPos.x,m_ResetButtonPos.y, buttonWidth, buttonHeight }, { 0,m_pRetryButton->GetHeight(),m_pRetryButton->GetWidth(),m_pRetryButton->GetHeight() });
	}
}

std::vector<int> TicTacToe::GetAvailableActionIdxs(GameState* pState)
{
	TicTacToeState* state = static_cast<TicTacToeState*>(pState);

	std::vector<int> m_Actions{};
	for (int i{ 0 }; i < (int)state->m_Cells.size(); ++i)
	{
		if (state->m_Cells[i].m_CellState == CellState::empty)
		{
			m_Actions.push_back(i);
		}
	}

	return m_Actions;
}

std::vector<int> TicTacToe::GetAvailableActionIdxs()
{
	return GetAvailableActionIdxs(m_pState);
}

void TicTacToe::TakeAction(int cellIdx)
{
	cell& cell = m_pState->m_Cells[cellIdx];
	if (cell.m_CellState != CellState::empty || m_pState->m_IsDone)
		return;

	if (m_pState->m_IsEnemyTurn)
	{
		cell.m_CellState = CellState::cross;
		m_pCrossTexture->Draw(Rectf{ cell.m_Pos.x,cell.m_Pos.y, m_CellWidth,m_CellHeight }, { 0,m_pCrossTexture->GetHeight(),m_pCrossTexture->GetWidth(),m_pCrossTexture->GetHeight() });
		CheckIfDone(cellIdx);
		m_pState->m_IsEnemyTurn = false;
		return;
	}

	cell.m_CellState = CellState::circle;
	m_pCircleTexture->Draw(Rectf{ cell.m_Pos.x,cell.m_Pos.y, m_CellWidth,m_CellHeight }, { 0,m_pCircleTexture->GetHeight(),m_pCircleTexture->GetWidth(),m_pCircleTexture->GetHeight() });
	CheckIfDone(cellIdx);
	m_pState->m_IsEnemyTurn = true;

}

GameState* TicTacToe::SampleAction(int actionIdx, GameState* pState)
{
	auto pTicTacToeState = static_cast<TicTacToeState*>(pState);
	TicTacToeState* s = new TicTacToeState();
	s->m_Cells = pTicTacToeState->m_Cells;
	s->m_IsEnemyTurn = pTicTacToeState->m_IsEnemyTurn;

	cell& cell = s->m_Cells[actionIdx];

	if (cell.m_CellState != CellState::empty)
		return s;

	if (s->m_IsEnemyTurn)
	{
		cell.m_CellState = CellState::cross;
		CheckIfDone(actionIdx, s);
		s->m_IsEnemyTurn = false;
		return s;
	}

	cell.m_CellState = CellState::circle;
	CheckIfDone(actionIdx, s);
	s->m_IsEnemyTurn = true;
	return s;
}

GameState* TicTacToe::GetGameState()
{
	return m_pState;
}

int TicTacToe::PosToCellIdx(Vector2f pos)
{
	int result{-1};
	for (int i{ 0 }; i < m_Cols; ++i)
	{
		if (pos.x < (m_pState->m_Cells[0].m_Pos.x + (i+1) * m_CellWidth))
		{
			result = i;
			break;
		}
	}

	for (int i{ 0 }; i < m_Rows; ++i)
	{
		if (pos.y > m_pState->m_Cells[0].m_Pos.y - i * m_CellWidth)
		{
			result += i * m_Cols;
			break;
		}
	}

	return result;
}

void TicTacToe::CheckIfDone(int idx)
{
	CheckIfDone(idx, m_pState);
}

void TicTacToe::CheckIfDone(int idx, TicTacToeState* pState)
{
	//THIS CODE IS A HOT MESS AND I AM NOT PROUD OF IT >_<
	//but it works for now and I can't think of a different solution, soooooooo have fun reading this ^-^

	int count{ 1 };
	int idxToCheck{ idx - 1 - m_Cols };

	//check diagonally
	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x > pState->m_Cells[idxToCheck].m_Pos.x)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck - 1 - m_Cols;

			if (idxToCheck >= 0 && idxToCheck <pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x > pState->m_Cells[idxToCheck].m_Pos.x)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}
	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}

	idxToCheck = idx + 1 + m_Cols;
	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x < pState->m_Cells[idxToCheck].m_Pos.x)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck + 1 + m_Cols;

			if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x < pState->m_Cells[idxToCheck].m_Pos.x)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}

	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}

	//check diagonally
	count = 1;
	idxToCheck = idx + 1 - m_Cols;

	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x < pState->m_Cells[idxToCheck].m_Pos.x)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck + 1 - m_Cols;

			if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x < pState->m_Cells[idxToCheck].m_Pos.x)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}
	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}

	idxToCheck = idx - 1 + m_Cols;
	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x > pState->m_Cells[idxToCheck].m_Pos.x)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck - 1 + m_Cols;

			if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x > pState->m_Cells[idxToCheck].m_Pos.x)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}

	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}

	//Check horizontally

	count = 1;
	idxToCheck = idx - 1;

	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x > pState->m_Cells[idxToCheck].m_Pos.x)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck - 1;

			if (idxToCheck >= 0 && idxToCheck <pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x > pState->m_Cells[idxToCheck].m_Pos.x)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}
	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}

	idxToCheck = idx + 1;
	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x < pState->m_Cells[idxToCheck].m_Pos.x)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck + 1;

			if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.x < pState->m_Cells[idxToCheck].m_Pos.x)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}

	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}

	//check vertically

	count = 1;
	idxToCheck = idx - m_Cols;

	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.y < pState->m_Cells[idxToCheck].m_Pos.y)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck - m_Cols;

			if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.y < pState->m_Cells[idxToCheck].m_Pos.y)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}
	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}

	idxToCheck = idx + m_Cols;
	if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.y > pState->m_Cells[idxToCheck].m_Pos.y)
		if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
		{
			++count;
			idxToCheck = idxToCheck + m_Cols;

			if (idxToCheck >= 0 && idxToCheck < pState->m_Cells.size() && pState->m_Cells[idx].m_Pos.y > pState->m_Cells[idxToCheck].m_Pos.y)
				if (pState->m_Cells[idx].m_CellState == pState->m_Cells[idxToCheck].m_CellState)
				{
					++count;
				}
		}

	if (count == 3)
	{
		pState->m_IsDone = true;
		if (pState->m_IsEnemyTurn)
			pState->SetValue(1);
		else pState->SetValue(-1);
		return;
	}


	//Check if draw

	bool isDraw{ true };
	for (int i{ 0 }; i < pState->m_Cells.size(); ++i)
	{
		if (pState->m_Cells[i].m_CellState == CellState::empty)
		{
			isDraw = false;
			break;
		}
	}

	if (isDraw)
		pState->m_IsDone = true;
}

bool TicTacToe::GetIsDone()
{
	return m_pState->m_IsDone;
}

bool TicTacToe::GetIsEnemyTurn()
{
	return m_pState->m_IsEnemyTurn;
}

void TicTacToe::HandleMouseInput(Vector2f mousePos)
{
	if (m_pState->m_IsDone)
	{
		if (utils::IsPointInRect({ mousePos.x,mousePos.y }, { m_ResetButtonPos.x,m_ResetButtonPos.y,125.f, 37.f }))
			Reset();

	}
	else
	{
		if (!m_pState->m_IsEnemyTurn)
		{
			int idx{ PosToCellIdx(mousePos) };
			TakeAction(idx);
		}
	}
}

void TicTacToe::Reset()
{
	for (int i{}; i < m_pState->m_Cells.size(); ++i)
	{
		m_pState->m_Cells[i].m_CellState = CellState::empty;
	}
	m_pState->SetValue(0);
	m_pState->m_IsDone = false;
	m_pState->m_IsEnemyTurn = true;
}
