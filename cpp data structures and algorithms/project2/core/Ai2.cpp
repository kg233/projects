//Ai2.cpp

#include <ics46/factory/DynamicFactory.hpp>
#include "Ai2.hpp"


ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, kuixir::Ai2, "Shallow Dummy (Required)");

std::pair<int, int> kuixir::Ai2::chooseMove(const OthelloGameState& state){
	// std::clock_t timer_start = std::clock();

	OthelloCell role = set_role(state);
	std::stack<std::pair<int, int>> possible_moves = get_all_moves(state);
	std::pair<int,int> current_best{NULL, NULL};

	const OthelloBoard& board = state.board();

	if (state.isValidMove(0, 0)){
		return std::pair<int, int>{0,0};
	}
	if(state.isValidMove(0, board.height()-1)){
		return std::pair<int, int>{0, board.height()-1};
	}
	if(state.isValidMove(board.width()-1, 0)){
		return std::pair<int, int>{board.width()-1, 0};
	}
	if(state.isValidMove(board.width()-1, board.height()-1)){
		return std::pair<int, int>{board.width()-1, board.height()-1};
	}


	int bestAlpha = INT_MIN;
	int bestBeta = INT_MAX;
	while (not possible_moves.empty()){
		std::unique_ptr<OthelloGameState> test_state = state.clone();

		test_state -> makeMove(possible_moves.top().first, possible_moves.top().second);

		int score = search(*test_state, 4, bestAlpha, bestBeta, role);
		if (score > bestAlpha){
			bestAlpha = score;
			current_best = possible_moves.top();
		}
		possible_moves.pop();
	}
	// std::clock_t timer_end = std::clock();

	// std::cout << static_cast<float>(timer_end - timer_start) / CLOCKS_PER_SEC << std::endl;

	// if (static_cast<float>(timer_end - timer_start) / CLOCKS_PER_SEC > 3.0) {
	// 	std::cout << "EXCEEDED 3 SECONDS: "<< static_cast<float>(timer_end - timer_start) / CLOCKS_PER_SEC << std::endl;
	// }


	return current_best;

}

int kuixir::Ai2::search(OthelloGameState& state, unsigned int depth, int bestAlpha, int bestBeta, OthelloCell role){

	if (depth == 0){
		return evaluate(state, role);
	}
	std::stack<std::pair<int, int>> possible_moves = get_all_moves(state);
	if (possible_moves.empty()){
		return evaluate(state, role);
	}
	else{

		OthelloCell turn = OthelloCell::black;
		if (state.isWhiteTurn()){
			turn = OthelloCell::white;
		}
		if (turn == role){
			int myValue = bestAlpha;


			std::vector<int> result;
			while (not possible_moves.empty()){
				std::unique_ptr<OthelloGameState> test_state = state.clone();
				test_state->makeMove(possible_moves.top().first, possible_moves.top().second);
				possible_moves.pop();

				int temp = search(*test_state, depth-1, myValue, bestBeta, role);

				if (temp > myValue){
					myValue = temp;
				}
				result.push_back(temp);
				if (not max_is_it_worth_keep_looking(bestBeta, myValue)){
					break;
				}

			}
			
			return *max_element(result.begin(), result.end());
		}

		else{
			int myValue = bestBeta;
			std::vector<int> result;

			while (not possible_moves.empty()){
				std::unique_ptr<OthelloGameState> test_state = state.clone();
				test_state->makeMove(possible_moves.top().first, possible_moves.top().second);
				possible_moves.pop();

				int temp = search(*test_state, depth-1, bestAlpha, myValue, role);

				if (temp < myValue){
					myValue = temp;
				}
	
				result.push_back(temp);

				if (not min_is_it_worth_keep_looking(bestAlpha, myValue)){
					break;
				}
			}

			return *min_element(result.begin(), result.end());
		}

	}

}

int kuixir::Ai2::evaluate(const OthelloGameState& state, OthelloCell role){
	if (state.isGameOver()){
		return 10000*Heu_utility(state, role);
	}
	int gameSum = state.blackScore() + state.whiteScore();

	if (gameSum < 20){
		return 200*Heu_corners(state, role) 
			+ -100*Heu_x(state, role) 
			+ -100*Heu_c(state, role)
			+ 20*Heu_potentialMob(state, role)
			+ -10*(Heu_numDIff(state, role));
	}

	else if (gameSum < 55){
		return 1000*Heu_corners(state, role) 
			+ -300*Heu_x(state, role)
			+ -300*Heu_c(state, role)
			+ 10*Heu_potentialMob(state, role)
			+ 50*(Heu_numDIff(state, role));
	}

	else{
		return 1000*Heu_corners(state, role) 
			+ 100*(Heu_numDIff(state, role));
	}

}


OthelloCell kuixir::Ai2::set_role(const OthelloGameState& state){
	if (state.isBlackTurn()){
		return OthelloCell::black;
	}
	else{
		return OthelloCell::white;
	}
}

std::stack<std::pair<int, int>> kuixir::Ai2::get_all_moves(const OthelloGameState& state){
	std::stack<std::pair<int, int>> result;

	for (int x = 0; x < state.board().width(); x++){
		for (int y = 0; y < state.board().height(); y++){
			if (state.board().cellAt(x, y) == OthelloCell::empty){
				if (state.isValidMove(x, y)){
					result.push(std::pair<int,int>(x, y));
				}
			}
		}
	}
	return result;
}


int kuixir::Ai2::get_corner_score(const OthelloBoard& board, OthelloCell color){
	int score = 0;
	if (board.cellAt(0, 0) == color){
		score += 1;
	}
	if(board.cellAt(0, board.height()-1) == color){
		score += 1;
	}
	if(board.cellAt(board.width()-1, 0) == color){
		score += 1;
	}
	if(board.cellAt(board.width()-1, board.height()-1) == color){
		score += 1;
	}

	return score;
}


int kuixir::Ai2::get_x_score(const OthelloBoard& board, OthelloCell color){
	int score = 0;

	if(board.cellAt(1, 1) == color){
		score += 1;
	}
	if(board.cellAt(1, board.height()-2) == color){
		score += 1;
	}
	if(board.cellAt(board.width()-2, 1) == color){
		score += 1;
	}
	if(board.cellAt(board.width()-2, board.height()-2) == color){
		score += 1;
	}
	return score;
}
bool kuixir::Ai2::max_is_it_worth_keep_looking(int bestBeta, int myValue){
	return bestBeta >= myValue;
}

bool kuixir::Ai2::min_is_it_worth_keep_looking(int bestAlpha, int myValue){
	return bestAlpha <= myValue;
}

int kuixir::Ai2::get_c_score(const OthelloBoard& board, OthelloCell color){
	int score = 0;

	if((board.cellAt(1, 0) == color or board.cellAt(0, 1) == color) and not (board.cellAt(0, 0) == color)){
		score += 1;
	}
	if((board.cellAt(1, board.height()-1) == color or board.cellAt(0, board.height()-2) == color) and not (board.cellAt(0, board.height()-1) == color)){
		score += 1;
	}
	if((board.cellAt(board.width()-2, 0) == color or board.cellAt(board.width()-1, 1) == color) and not (board.cellAt(board.width()-1, 0) == color)){
		score += 1;
	}
	if((board.cellAt(board.width()-1, board.height()-2) == color or board.cellAt(board.width()-2, board.height()-1) == color) and not (board.cellAt(board.width()-1, board.height()-1) == color)){
		score += 1;
	}
	return score;
}

//===============================================
OthelloCell kuixir::Ai2::negate(OthelloCell color){
	switch (color){
		case OthelloCell::white:
			return OthelloCell::black;
		case OthelloCell::black:
			return OthelloCell::white;
		case OthelloCell::empty:
			return OthelloCell::empty;

	}
}

int kuixir::Ai2::Heu_numDIff(const OthelloGameState& state, OthelloCell role){
	if (role == OthelloCell::black){
		return 100 * (state.blackScore() - state.whiteScore())/(state.blackScore() + state.whiteScore());
	}
	else{
		return 100 * (state.whiteScore() - state.blackScore())/(state.whiteScore() + state.blackScore());
	}
}

int kuixir::Ai2::Heu_utility(const OthelloGameState& state, OthelloCell role){
	if (role == OthelloCell::black){
		return state.blackScore() - state.whiteScore();
	}
	else{
		return state.whiteScore() - state.blackScore();
	}
}

int kuixir::Ai2::Heu_corners(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& b = state.board();
	int myScore = get_corner_score(b, role);
	int oppScore = get_corner_score(b, negate(role));
	if (myScore + oppScore > 0){
		return 100*(myScore - oppScore)/(myScore+oppScore);
	}
	return 0;
}

int kuixir::Ai2::Heu_x(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& b = state.board();
	int myScore = get_x_score(b, role);
	int oppScore = get_x_score(b, negate(role));
	if (myScore + oppScore > 0){
		return 100*(myScore - oppScore)/(myScore+oppScore);
	}
	return 0;
}

int kuixir::Ai2::Heu_c(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& b = state.board();
	int myScore = get_c_score(b, role);
	int oppScore = get_c_score(b, negate(role));
	if (myScore + oppScore > 0){
		return 100*(myScore - oppScore)/(myScore+oppScore);
	}
	return 0;
}

// int kuixir::Ai2::Heu_mobility(const OthelloGameState& state){
// 	OthelloCell turn = OthelloCell::white;
// 	if (not state.isWhiteTurn()){
// 		turn = negate(turn);
// 	}
// 	int moves = get_all_moves(state).size();
// 	if (turn == role){
// 		return moves;
// 	}
// 	else{
// 		return -moves;
// 	}
// }

int kuixir::Ai2::Heu_potentialMob(const OthelloGameState& state, OthelloCell role){
	int score = 0;
	const OthelloBoard& board = state.board();
	for (int x = 0; x< board.width(); x++){
		for (int y = 0; y < board.height(); y++){
			if (board.cellAt(x, y) == negate(role)){
				score += empty_adj_cell(board, x, y);
			}
		}
	}
	return 100*score/32;

}

int kuixir::Ai2::empty_adj_cell(const OthelloBoard& board, int coord_x, int coord_y){
	int score = 0;

	if (not (coord_x + 1 >= board.width()) and not (coord_x == 0)){
		if (board.cellAt(coord_x + 1, coord_y) == OthelloCell::empty){
			score += 1;
		}
		if (board.cellAt(coord_x-1, coord_y) == OthelloCell::empty){
			score += 1;
		}
	}

	if (not(coord_y + 1 >= board.height()) and not (coord_y == 0)){
		if (board.cellAt(coord_x, coord_y+1) == OthelloCell::empty){
			score += 1;
		}
		if (board.cellAt(coord_x, coord_y-1) == OthelloCell::empty){
			score += 1;
		}
	}

	if (coord_x != board.width()-1 and coord_x != 0 and coord_y != 0 and coord_y != board.height()-1){
		if (board.cellAt(coord_x -1, coord_y + 1) == OthelloCell::empty){
			score+= 1;
		}
		if (board.cellAt(coord_x -1, coord_y - 1) == OthelloCell::empty){
			score+= 1;
		}
		if (board.cellAt(coord_x +1, coord_y - 1) == OthelloCell::empty){
			score+= 1;
		}
		if (board.cellAt(coord_x +1, coord_y + 1) == OthelloCell::empty){
			score+= 1;
		}

	}
	return score;
}

