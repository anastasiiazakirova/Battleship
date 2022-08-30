#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include "FBattleshipGame.h"


FBattleshipGame::FBattleshipGame() { Reset(); }

void FBattleshipGame::Reset()
{
	MyDestruction = 0;
	CellsOfShip = 0;
	Name = "";

	srand(time(NULL));

	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			MyField[i][j] = EStatusCell::EMPTY_CELL;
		}
	}
	
	int NumberOfShips = MAX_DECKS;
	int DecksSips = 1;
	//Counting the number of ships in total.
	for (int DecksSips = 1; DecksSips <= MAX_DECKS; DecksSips++) {
		CellsOfShip += (NumberOfShips * DecksSips);
		NumberOfShips--;
	}
	TotalCellsShips = CellsOfShip;
	return;
}

void FBattleshipGame::SetName(std::string SrtingName)
{
	if (std::size(SrtingName) == NULL) {
		Name = "Player";
	}
	else {
		Name = SrtingName;
	}
}

void FBattleshipGame::PrintField(bool bIsBattle) const
{
	std::cout << "   a b c d e f g h i j\n";
	for (int Y = 0; Y < FIELD_SIZE; Y++) {
		std::cout << Y << "  ";
		for (int X = 0; X < FIELD_SIZE; X++) {
			std::cout << CellPrint(Y, X, bIsBattle) << ' ';
		}
		std::cout << std::endl;
	}
}

char FBattleshipGame::CellPrint(int Y, int X, bool bIsBattle) const
{
	if (MyField[Y][X] == EStatusCell::EMPTY_CELL) {
		return '.';
	}
	if (bIsBattle) {
		//The results of the shots will be displayed (when the game itself takes place)
		if (MyField[Y][X] > EStatusCell::EMPTY_CELL) {
			return '.';
		} else if (MyField[Y][X] == EStatusCell::WOUND_SHIP) {
			return 'X';
		} else if (MyField[Y][X] == EStatusCell::NOT_WOUND) {
			return 'o';
		}
	}
	else { 
		//All ships on the field will be displayed (when filled)
		if (MyField[Y][X] > EStatusCell::EMPTY_CELL) {
			return '+';
		}
	}

}

bool FBattleshipGame::IsCorrectCoordinate(Coordinate CheckingCoordinates) const
{
	switch (CheckingCoordinates.status)
	{
	case EStatusCoordinate::OK:
		return true;
		break;
	case EStatusCoordinate::EMPTY_STRING:
		std::cout << "Errors! The coordinate is not entered\n";
		break;
	case EStatusCoordinate::INCORRECT_STRING:
		std::cout << "Errors! The coordinate entered is incorrect\n";
		break;
	case EStatusCoordinate::NOT_IN_FIELD_RANGE:
		std::cout << "Errors! The coordinate is not in the range of the field\n";
		break;
	case EStatusCoordinate::ALREADY_SHOT_HERE:
		std::cout << "A shot was already fired at this coordinate.\n";
		break;
	default:
		std::cout << "Unidentified error\n";
		break;
	}
	return false;
}

FBattleshipGame::Coordinate FBattleshipGame::SetCoordinate(std::string LineOfCoordinat)
{
	const int CHAR_a_CODE = 97;
	const int CHAR_0_CODE = 48;
	Coordinate CurrentCoordinate;

	if (std::size(LineOfCoordinat) != NULL) {
		if (isalpha(LineOfCoordinat[0]) > 0  && isdigit(LineOfCoordinat[1]) > 0) //If the first letter and the second digit
		{
			CurrentCoordinate.y = (int)LineOfCoordinat[1] - CHAR_0_CODE;
			CurrentCoordinate.x = (int)tolower(LineOfCoordinat[0]) - CHAR_a_CODE;
		}
		else if (isalpha(LineOfCoordinat[1]) > 0 && isdigit(LineOfCoordinat[0]) > 0) //If the second letter and the first digit
		{
			CurrentCoordinate.y = (int)LineOfCoordinat[0] - CHAR_0_CODE;
			CurrentCoordinate.x = (int)tolower(LineOfCoordinat[1]) - CHAR_a_CODE;
		}else {
			CurrentCoordinate.status = EStatusCoordinate::INCORRECT_STRING;
			return CurrentCoordinate;
		}

		//Check whether the coordinate is within the field
		if (CurrentCoordinate.x >= 0 && CurrentCoordinate.x < FIELD_SIZE
			&& CurrentCoordinate.y >= 0 && CurrentCoordinate.y < FIELD_SIZE) {
			CurrentCoordinate.status = EStatusCoordinate::OK;
		}
		else {
			CurrentCoordinate.status = EStatusCoordinate::NOT_IN_FIELD_RANGE;
		}
	} else {
		CurrentCoordinate.status = EStatusCoordinate::EMPTY_STRING;
	}

	return CurrentCoordinate;
}

bool FBattleshipGame::PutShip(int DecksShip, Coordinate InitialCoordinate, Coordinate LastCoordinate)
{
	if (((abs(InitialCoordinate.x - LastCoordinate.x) + 1) == DecksShip) ||
		((abs(InitialCoordinate.y - LastCoordinate.y) + 1) == DecksShip)) {

		// Checking if there is an empty place for the ship
		int MinX = std::min(InitialCoordinate.x, LastCoordinate.x);
		int MinY = std::min(InitialCoordinate.y, LastCoordinate.y);
		MinX = (MinX > 0) ? (MinX - 1) : MinX;
		MinY = (MinY > 0) ? (MinY - 1) : MinY;

		int MaxX = std::max(InitialCoordinate.x, LastCoordinate.x);
		int MaxY = std::max(InitialCoordinate.y, LastCoordinate.y);
		MaxX = (MaxX < 9) ? (MaxX + 1) : MaxX;
		MaxY = (MaxY < 9) ? (MaxY + 1) : MaxY;

		for (int y = MinY; y <= MaxY; y++) {
			for (int x = MinX; x <= MaxX; x++) {
				if (MyField[y][x] != 0) {
					return false;
				}
			}
		}

		//Put the ship
		for (int y = std::min(InitialCoordinate.y, LastCoordinate.y); y <= std::max(InitialCoordinate.y, LastCoordinate.y); y++) {
			for (int x = std::min(InitialCoordinate.x, LastCoordinate.x); x <= std::max(InitialCoordinate.x, LastCoordinate.x); x++) {
				MyField[y][x] = DecksShip;
			}
		}
		return true;
	} 	
	else {
		return false;
	}
}



bool FBattleshipGame::FillFieldFromScreen()
{
	std::string LineOfFirstCoordinat = "";
	std::string LineOfLastCoordinat = "";
	int DecksShip = MAX_DECKS;
	bool bIsShipPut;
	bool bIsBattle = false;
	Coordinate First, Second;

	PrintField(bIsBattle);
	do {
		for (int CounShip = MAX_DECKS - DecksShip; CounShip >= 0; CounShip--) {
			do {
				std::cout << "Place " << CounShip + 1 << ' ' << DecksShip << "-deck ship.\n";
				std::cout << "Ship can be located strictly horizontally or vertically!\n";
				do {
					std::cout << "Initial coordinate the ship: ";
					std::getline(std::cin, LineOfFirstCoordinat);
					First = SetCoordinate(LineOfFirstCoordinat);
				} while (!IsCorrectCoordinate(First));

				if (DecksShip > 1) {
					do {
						std::cout << "The last coordinate of: ";
						std::getline(std::cin, LineOfLastCoordinat);
						Second = SetCoordinate(LineOfLastCoordinat);
					} while (!IsCorrectCoordinate(Second));
				}
				else if (DecksShip == 1)
				{
					Second = First;
				}
				bIsShipPut = PutShip(DecksShip, First, Second);

				if (!bIsShipPut) {
					std::cout << "Error! The ship cannot be put in these coordinates. Try again.\n";
				}
			} while (!bIsShipPut);
			PrintField(bIsBattle);
		}
		DecksShip--;

	} while (DecksShip > NULL);

	return true;

}

bool FBattleshipGame::RandFillField()
{
	int RandOrientation;
	int x, y;
	bool BIs²hipPut = false;
	Coordinate FirstCoordinate, SecondCoordinate;
	enum EOrientation
	{
		VERTICAL,
		HORIZONTAL
	};

	int DecksShip = MAX_DECKS;
	do {
		for (int CounShip = MAX_DECKS - DecksShip; CounShip >= 0; CounShip--) {
			do {
				RandOrientation = rand() % 2;
				if (RandOrientation == VERTICAL) {
					x = rand() % FIELD_SIZE;
					y = rand() % (FIELD_SIZE - DecksShip);
					FirstCoordinate = { y, x, EStatusCoordinate::OK};
					SecondCoordinate = { y + DecksShip - 1, x, EStatusCoordinate::OK };
					BIs²hipPut = PutShip(DecksShip, FirstCoordinate, SecondCoordinate);
				}
				else if (RandOrientation == HORIZONTAL) {
					x = rand() % (FIELD_SIZE - DecksShip);
					y = rand() % FIELD_SIZE;
					FirstCoordinate = { y, x, EStatusCoordinate::OK };
					SecondCoordinate = { y, x + DecksShip - 1, EStatusCoordinate::OK };
					BIs²hipPut = PutShip(DecksShip, FirstCoordinate, SecondCoordinate);
				}
			} while (!BIs²hipPut);
		}
		DecksShip--;
	} while (DecksShip > NULL);

	return true;
}


void FBattleshipGame::TakeShotFromScreen()
{
	std::string LineOfShotCoordinate = "";
	do {
		std::cout << "Shot coordinate: ";
		std::getline(std::cin, LineOfShotCoordinate);
		Shot = SetCoordinate(LineOfShotCoordinate);
	} while (!IsCorrectCoordinate(Shot));

	return;
}

void FBattleshipGame::TakeRandShot()
{
	Shot.x = rand() % FIELD_SIZE;
	Shot.y = rand() % FIELD_SIZE;
	Shot.status = EStatusCoordinate::OK;
	return;
}




bool FBattleshipGame::IsShotHappen(Coordinate ShotCoordinate) const
{
	if (MyField[ShotCoordinate.y][ShotCoordinate.x] == WOUND_SHIP ||
		MyField[ShotCoordinate.y][ShotCoordinate.x] == NOT_WOUND) {
		ShotCoordinate.status = EStatusCoordinate::ALREADY_SHOT_HERE;
	}
	return IsCorrectCoordinate(ShotCoordinate);
}



bool FBattleshipGame::IsShipwrecked(Coordinate ShotCoordinate)
{
	bIsBattle = true;
	if (MyField[ShotCoordinate.y][ShotCoordinate.x] == EMPTY_CELL) {
		//The shot did not hit ship
		MyField[ShotCoordinate.y][ShotCoordinate.x] = NOT_WOUND;
		std::cout << "The shot did not hit the ship.\n";
		std::cout << "    " << Name <<" field\n";
		PrintField(bIsBattle);
		MyDestruction++;
		return false;
	}
	else if (MyField[ShotCoordinate.y][ShotCoordinate.x] > EMPTY_CELL) {
		//The shot hits ship.
		MyField[ShotCoordinate.y][ShotCoordinate.x] = WOUND_SHIP;
		std::cout << "The shot hit the ship. Shoot again.\n";
		std::cout << "    " << Name << " field\n";
		PrintField(bIsBattle);
		CellsOfShip--;
		return true;
	}
}



bool FBattleshipGame::IsGameLose() const
{
	return (CellsOfShip == 0);
}

std::string FBattleshipGame::GatName() const
{
	return Name;
}

int FBattleshipGame::GetDestruction() const
{
	//Returns the percentage of shots fired into empty cells
	return (100 - ((FIELD_SIZE * FIELD_SIZE - TotalCellsShips - MyDestruction) * 100)/(FIELD_SIZE * FIELD_SIZE - TotalCellsShips));
}

int FBattleshipGame::GetWreckedShips() const
{
	//Returns the value in percentage of how many ship decks were destroyed.
	return ((TotalCellsShips - CellsOfShip) * 100 / TotalCellsShips);
}

