#pragma once

#include <string>
#include <vector>


class FBattleshipGame {

public:
	
	bool bIsBattle;
	enum class EStatusCoordinate {
		EMPTY_STRING,
		INCORRECT_STRING,
		NOT_IN_FIELD_RANGE,
		ALREADY_SHOT_HERE,
		OK
	};

	struct Coordinate {
		int x;
		int y;
		EStatusCoordinate status;
	};

	Coordinate Shot;
	FBattleshipGame();
	void Reset();

	
	char CellPrint(int, int, bool) const;
	void PrintField(bool) const;
	bool IsCorrectCoordinate(Coordinate) const;
	bool IsGameLose() const;
	std::string GatName() const;

	void SetName(std::string);

	bool IsShotHitShip(Coordinate);

	bool FillFieldFromScreen();
	bool RandFillField();
	Coordinate SetCoordinate(std::string); //To transform the coordinates of the line in two int
	

	bool IsShotHappen(Coordinate) const;

	int GetDestruction() const;
	int GetWreckedShips() const;

	bool GetShotCoordinate(Coordinate);

	void TakeShotFromScreen();
	void TakeRandShot();


	bool IsShipwrecked(Coordinate);
private:
	static const int FIELD_SIZE = 10;
	static const int MAX_DECKS = 4;
	int TotalCellsShips;
	int MyField[FIELD_SIZE][FIELD_SIZE];
	int MyDestruction;
	int CellsOfShip;
	std::string Name;
	enum EStatusCell {
		NOT_WOUND = -2,
		WOUND_SHIP = -1,
		EMPTY_CELL = 0,
		ONE_DECK_SHIP = 1,
		TWO_DECK_SHIP = 2,
		THREE_DECK_SHIP = 3,
		FOUR_DECK_SHIP = 4,
	};

		
	bool PutShip(int, Coordinate, Coordinate);
	
	
};
