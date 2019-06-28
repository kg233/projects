//ai3.cpp

#include <ics46/factory/DynamicFactory.hpp>
#include "Ai3.hpp"


ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, kuixir::ai3, "Dummy");

std::pair<int, int> kuixir::ai3::chooseMove(const OthelloGameState& state){
	std::clock_t timer_start = std::clock();

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
	std::cout<<"[";
	while (not possible_moves.empty()){
		std::unique_ptr<OthelloGameState> test_state = state.clone();

		test_state -> makeMove(possible_moves.top().first, possible_moves.top().second);

		int score = search(*test_state, 5, bestAlpha, bestBeta, role);

		std::cout << "(" << possible_moves.top().first << ", " << possible_moves.top().second << ": " << score<< "); ";

		if (score > bestAlpha){
			bestAlpha = score;
			current_best = possible_moves.top();
		}
		possible_moves.pop();
	}
	std::clock_t timer_end = std::clock();

	std::cout << static_cast<float>(timer_end - timer_start) / CLOCKS_PER_SEC << std::endl;

	if (static_cast<float>(timer_end - timer_start) / CLOCKS_PER_SEC > 3.0) {
		std::cout << "EXCEEDED 3 SECONDS: "<< static_cast<float>(timer_end - timer_start) / CLOCKS_PER_SEC << std::endl;
	}

	std::cout<<"]; "<< "chose: " << current_best.first<< ", " << current_best.second << std::endl;
	return current_best;

}

int kuixir::ai3::search(OthelloGameState& state, unsigned int depth, int bestAlpha, int bestBeta, OthelloCell role){

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

int kuixir::ai3::evaluate(const OthelloGameState& state, OthelloCell role){
	if (state.isGameOver()){
		return 10000*Heu_utility(state, role);
	}
	int gameSum = state.blackScore() + state.whiteScore();

	if (gameSum < 20){
		return 2000*Heu_corners(state, role) 
			+ -200*Heu_x(state, role) 
			+ -200*Heu_c(state, role)
			+ 200*Heu_potentialMob(state, role)
			+ -1000*(Heu_numDIff(state, role));
	}

	else if (gameSum < 55){

		return 2000*Heu_corners(state, role) 
			+ -1000*Heu_x(state, role)
			+ -1000*Heu_c(state, role)
			+ 150*Heu_potentialMob(state, role)
			+ 100*(Heu_numDIff(state, role))
			+ 500* wall(state, role)
			+ 800 * Heu_fullLines(state, role);


	}

	else{
		return 10000*Heu_corners(state, role) 
			+ 4000*(Heu_numDIff(state, role))
			+ 2000 * Heu_fullLines(state, role);
	}

}


OthelloCell kuixir::ai3::set_role(const OthelloGameState& state){
	if (state.isBlackTurn()){
		return OthelloCell::black;
	}
	else{
		return OthelloCell::white;
	}
}

std::stack<std::pair<int, int>> kuixir::ai3::get_all_moves(const OthelloGameState& state){
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


int kuixir::ai3::get_corner_score(const OthelloBoard& board, OthelloCell color){
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


int kuixir::ai3::get_x_score(const OthelloBoard& board, OthelloCell color){
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
bool kuixir::ai3::max_is_it_worth_keep_looking(int bestBeta, int myValue){
	return bestBeta >= myValue;
}

bool kuixir::ai3::min_is_it_worth_keep_looking(int bestAlpha, int myValue){
	return bestAlpha <= myValue;
}

int kuixir::ai3::get_c_score(const OthelloBoard& board, OthelloCell color){
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
OthelloCell kuixir::ai3::negate(OthelloCell color){
	switch (color){
		case OthelloCell::white:
			return OthelloCell::black;
		case OthelloCell::black:
			return OthelloCell::white;
		case OthelloCell::empty:
			return OthelloCell::empty;

	}
}

int kuixir::ai3::Heu_numDIff(const OthelloGameState& state, OthelloCell role){
	if (role == OthelloCell::black){
		return 100 * (state.blackScore() - state.whiteScore())/(state.blackScore() + state.whiteScore());
	}
	else{
		return 100 * (state.whiteScore() - state.blackScore())/(state.whiteScore() + state.blackScore());
	}
}

int kuixir::ai3::Heu_utility(const OthelloGameState& state, OthelloCell role){
	if (role == OthelloCell::black){
		return state.blackScore() - state.whiteScore();
	}
	else{
		return state.whiteScore() - state.blackScore();
	}
}

int kuixir::ai3::Heu_corners(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& b = state.board();
	int myScore = get_corner_score(b, role);
	int oppScore = get_corner_score(b, negate(role));
	if (myScore + oppScore > 0){
		return 100*(myScore - oppScore)/(myScore+oppScore);
	}
	return 0;
}

int kuixir::ai3::Heu_x(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& b = state.board();
	int myScore = get_x_score(b, role);
	int oppScore = get_x_score(b, negate(role));
	if (myScore + oppScore > 0){
		return 100*(myScore - oppScore)/(myScore+oppScore);
	}
	return 0;
}

int kuixir::ai3::Heu_c(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& b = state.board();
	int myScore = get_c_score(b, role);
	int oppScore = get_c_score(b, negate(role));
	if (myScore + oppScore > 0){
		return 100*(myScore - oppScore)/(myScore+oppScore);
	}
	return 0;
}

// int kuixir::ai3::Heu_mobility(const OthelloGameState& state){
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

int kuixir::ai3::Heu_potentialMob(const OthelloGameState& state, OthelloCell role){
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

int kuixir::ai3::empty_adj_cell(const OthelloBoard& board, int coord_x, int coord_y){
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

int kuixir::ai3::Heu_fullLines(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& board = state.board();
	int score = 0;


	for (int rowLevel = 0; rowLevel < board.height(); rowLevel++){
		int temp = checkRow(board, rowLevel, role);
		if (temp == 0){
			break;
		}
		score += temp;
		// std::cout << "detected Row top"<<std::endl;
	}
	for (int rowLevel = board.height()-1; rowLevel >= 0; rowLevel--){
		int temp = checkRow(board, rowLevel, role);
		if (temp == 0){
			break;
		}
		score += temp;
		// std::cout << "detected Row bot"<<std::endl;
	}

	for (int colLevel = 0; colLevel < board.width(); colLevel++){
		int temp = checkCol(board, colLevel, role);
		if (temp == 0){
			break;
		}
		score += temp;
		// std::cout << "detected col left"<<std::endl;
	}

	for (int colLevel = board.width()-1; colLevel >= 0; colLevel--){
		int temp = checkCol(board, colLevel, role);
		if (temp == 0){
			break;
		}
		score += temp;
		// std::cout << "detected col right"<<std::endl;
	}
	if (board.cellAt(0,0) == role){
		// std::cout << "detected diag top left"<<std::endl;
		for (int x = 1; x < board.width()-1; x++){  //top left
			int temp = checkRightDiag(board, x, 0, role);
			if (temp == 0){
				break;
			}
			score += temp;
			
		}
	}

	if (board.cellAt(board.width()-1, 0) == role){
		// std::cout << "detected diag top right"<<std::endl;
		for (int x = board.width()-2; x >= 0; x--){  //top right
			int temp = checkLeftDiag(board, x, 0, role);
			if (temp == 0){
				break;
			}
			score += temp;
			
		}
	}

	if (board.cellAt(0, board.height()-1) == role){
		// std::cout << "detected diag bot left"<<std::endl;
		for (int x = 1; x < board.width()-1; x++){ //bottom left
			int temp = checkLeftDiag(board, x, board.height()-1, role);
			if (temp == 0){
				break;
			}
			score += temp;
			
		}
	}

	if (board.cellAt(board.width()-1, board.height()-1) == role){
		// std::cout << "detected diag bot right"<<std::endl;
		for (int x = board.width()-2; x >= 0; x--){ //bottom right
			int temp = checkRightDiag(board, x, board.height()-1, role);
			if (temp == 0){
				break;
			}
			score += temp;

		}
	}
	
	return 100*score/(score + 8);
}


int kuixir::ai3::checkRow(const OthelloBoard& board, int row, OthelloCell role){
	bool emptyThreshold = false;
	for (int x = 0; x < board.width(); x++){
		if (board.cellAt(x, row) == OthelloCell::empty ){
			if (emptyThreshold){
				return 0;
			}
			emptyThreshold = true;
		}
		else if (board.cellAt(x,row) != role){
			return 0;
		}
	}
	return 1;
}

int kuixir::ai3::checkCol(const OthelloBoard& board, int col, OthelloCell role){
	bool emptyThreshold = false;
	for (int y = 0; y < board.height(); y++){
		if (board.cellAt(col, y) == OthelloCell::empty ){
			if (emptyThreshold){
				return 0;
			}
			emptyThreshold = true;
		}
		else if (board.cellAt(col, y) != role){
			return 0;
		}
	}
	return 1;
}

int kuixir::ai3::checkLeftDiag(const OthelloBoard& board, int x, int y, OthelloCell role){
	bool emptyThreshold = false;

	while (x < board.width()-1 and y < board.height()-1){
		x += 1;
		y += 1;
		if (board.cellAt(x, y) == OthelloCell::empty ){
			if (emptyThreshold){
				return 0;
			}
			emptyThreshold = true;
		}
		else if (board.cellAt(x,y) != role){
			return 0;
		}
	}
	while (x > 0 and y > 0){
		x -= 1;
		y -= 1;
		if (board.cellAt(x, y) == OthelloCell::empty ){
			if (emptyThreshold){
				return 0;
			}
			emptyThreshold = true;
		}
		else if (board.cellAt(x,y) != role){
			return 0;
		}
	}
	return 1;
}

int kuixir::ai3::checkRightDiag(const OthelloBoard& board, int x, int y, OthelloCell role){
	bool emptyThreshold = false;

	while (x > 0 and y < board.height()-1){
		x -= 1;
		y += 1;
		if (board.cellAt(x, y) == OthelloCell::empty ){
			if (emptyThreshold){
				return 0;
			}
			emptyThreshold = true;
		}
		else if (board.cellAt(x,y) != role){
			return 0;
		}
	}
	while (x < board.width()-1 and y > 0){
		x += 1;
		y -= 1;
		if (board.cellAt(x, y) == OthelloCell::empty ){
			if (emptyThreshold){
				return 0;
			}
			emptyThreshold = true;
		}
		else if (board.cellAt(x,y) != role){
			return 0;
		}
	}
	return 1;
}

int kuixir::ai3::wall(const OthelloGameState& state, OthelloCell role){
	const OthelloBoard& board = state.board();
	int myScore = 0;
	myScore += up_down_wall_count(board, role);
	myScore += left_right_wall_count(board, role);
	return 100*myScore/((board.height()-1)*2 + (board.width()-1)*2 );

}

int kuixir::ai3::up_down_wall_count(const OthelloBoard& board, OthelloCell role){
	int score = 0;
	for (int x = 2; x < board.width()-2;x++){
		if (board.cellAt(x, 0) == role){
			score += 1;
		}
		if (board.cellAt(x, board.height()-1) == role){
			score += 1;
		}
	}
	return score;
}

int kuixir::ai3::left_right_wall_count(const OthelloBoard& board, OthelloCell role){
	int score = 0;
	for (int y = 2; y < board.height()-2;y++){
		if (board.cellAt(0, y) == role){
			score += 1;
		}
		if (board.cellAt(board.width()-1, y) == role){
			score += 1;
		}

	}
	return score;
}