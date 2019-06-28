//ai3.hpp
#ifndef ai3_HPP
#define ai3_HPP


#include "OthelloAI.hpp"
#include <stack>
#include <vector>
#include <algorithm>


#include <iostream>
#include <ctime>


namespace kuixir{

	class ai3: public OthelloAI
	{
	public:
		virtual std::pair<int, int> chooseMove(const OthelloGameState& state);

		int search(OthelloGameState& state, unsigned int depth, int bestAlpha, int bestBeta, OthelloCell role);

		int evaluate(const OthelloGameState& state, OthelloCell role);

		OthelloCell set_role(const OthelloGameState& state);

		std::stack<std::pair<int, int>> get_all_moves(const OthelloGameState& state);
		std::vector<int> get_score_list(const OthelloGameState& state, std::stack<std::pair<int, int>> moves);

		// bool is_my_cell_at(const OthelloBoard& board, int x, int y);

		int get_corner_score(const OthelloBoard& board, OthelloCell color);

		// int get_wall_score(const OthelloBoard& board, int offset);

		int get_x_score(const OthelloBoard& board, OthelloCell color);

		bool max_is_it_worth_keep_looking(int bestBeta, int myValue);

		bool min_is_it_worth_keep_looking(int bestAlpha, int myValue);

		int get_c_score(const OthelloBoard& board, OthelloCell color);

		int Heu_numDIff(const OthelloGameState& state, OthelloCell color);

		int Heu_mobility(const OthelloGameState& state, OthelloCell color);

		int Heu_utility(const OthelloGameState& state, OthelloCell role);

		OthelloCell negate(OthelloCell color);

		int Heu_corners(const OthelloGameState& state, OthelloCell role);

		int Heu_x(const OthelloGameState& state, OthelloCell role);
		int Heu_c(const OthelloGameState& state, OthelloCell role);
		int Heu_potentialMob(const OthelloGameState& state, OthelloCell role);
		int empty_adj_cell(const OthelloBoard& board, int coord_x, int coord_y);

		int Heu_fullLines(const OthelloGameState& state, OthelloCell role);
		int checkRow(const OthelloBoard& board, int row, OthelloCell role);
		int checkCol(const OthelloBoard& board, int col, OthelloCell role);
		int checkLeftDiag(const OthelloBoard& board, int x, int y, OthelloCell role);
		int checkRightDiag(const OthelloBoard& board, int x, int y, OthelloCell role);

		int wall(const OthelloGameState& state, OthelloCell role);
		int up_down_wall_count(const OthelloBoard& board, OthelloCell role);
		int left_right_wall_count(const OthelloBoard& board, OthelloCell role);

	};

}

#endif