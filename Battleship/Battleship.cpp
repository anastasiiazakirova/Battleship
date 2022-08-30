// Battleship.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is the console version of the game Battleship with the computer.

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "FBattleshipGame.h"

void PrintIntro();
void PlayGame();
void FillField();
bool AskToPlayAgain();
bool PlayerShot();
bool CompuerShot();
void GameResult(FBattleshipGame*, FBattleshipGame*);

bool bIsBattle;

FBattleshipGame Player;
FBattleshipGame Compuer;

int main()
{
	bool bPlayAgain = false;
	do {
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);
	system("pause");
	return 0;
}

void PlayGame()
{
	bool bGameLose = false;
	bIsBattle = false;
	Player.Reset();
	Compuer.Reset();
	PrintIntro();
	FillField();
	bIsBattle = true;
	do {
		bGameLose = PlayerShot();

		if (!bGameLose) {
			bGameLose = CompuerShot();
		}

	} while (!bGameLose);
}

void PrintIntro() {
	std::cout << "\n";
	std::cout << "                                  /|\n";
	std::cout << "                                  \\|\n";
	std::cout << "                                  _|_\n";
	std::cout << "                             ___ |   | __\n";
	std::cout << "                           _|   ||   ||  |______/|\n";
	std::cout << "                   ___()__/____________________/ |\n";
	std::cout << "                   \\           _____            /\n";
	std::cout << "                    \\    121  (_   _)   _||    /\n";
	std::cout << "                     \\__________| |____(   )__/\n";
	std::cout << "                              /     \\   / /     \n";
	std::cout << "                             |       \\ / /     \n";
	std::cout << "                             | |    | / /   \n";
	std::cout << "                             | |    |\\/   \n";
	std::cout << "\n";
	std::cout << "                           Welcome to the game.\n";
	std::cout << "You need to save as much of your territory as possible and not lose your army.\n";
	std::cout << "             To do this, you must quickly destroy the enemy.\n\n";
	return;
}

void FillField()
{
	
	std::string FillOption = "";
	std::string Name = "";
	bool bIsFieldFill = false;
	bIsBattle = false;

	std::cout << "Please enter your name: ";
	std::getline(std::cin, Name);
	Player.SetName(Name);
	//Filling the field for the Player
	do {
		std::cout << "Please choose the option you want to place your military facilities\n";
		std::cout << "Enter 1 to deploy troops yourself.\nEnter 2 to place troops randomly.\n";
		std::getline(std::cin, FillOption);
		if (FillOption[0] == '1') {
			std::cout << "\n       YOUR FIELD\n";
			bIsFieldFill = Player.FillFieldFromScreen();
		}
		else if (FillOption[0] == '2') {
			std::cout << "\n       YOUR FIELD\n";
			bIsFieldFill = Player.RandFillField();
			Player.PrintField(bIsBattle);
		}
		else {
			std::cout << "Incorrect input!\n";
		}
	} while (!bIsFieldFill);
	system("pause");
	system("CLS");
	std::cout << "Congratulations! The field is full. Let's start the game.\n";

	//Filling the field for the Compuer
	Compuer.RandFillField();
	Compuer.SetName("Compuer");
}

bool PlayerShot() {
	bool bIsShotHappen;
	bool bIsSuccessfulShot;
	bool bGameLose;
	do
	{
		do {
			//Shot and check if Player have already fired at this coordinate
			std::cout << Player.GatName() << " takes a shot\n";
			Player.TakeShotFromScreen();
			//If the shot has already taken place at this coordinate, shoot again
			bIsShotHappen = Compuer.IsShotHappen(Player.Shot);
		} while (!bIsShotHappen);

		bIsSuccessfulShot = Compuer.IsShipwrecked(Player.Shot); //If the shot hits the ship, Player fires again
		bGameLose = Compuer.IsGameLose(); //Checking if the Player has won
	} while (bIsSuccessfulShot && !bGameLose);

	if (bGameLose) {
		GameResult(&Player, &Compuer); //Displaying the winner and loser on the screen
	}
	return bGameLose;
}

bool CompuerShot() {
	bool bIsShotHappen;
	bool bIsSuccessfulShot;
	bool bGameLose;
	do
	{
		do {
			//Shot and check if Compuer have already fired at this coordinate
			//If the shot has already taken place at this coordinate, shoot again
			std::cout << Compuer.GatName()<< " takes a shot";

			std::this_thread::sleep_for(std::chrono::nanoseconds(10));
			std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(2));
			std::cout << ".";
			std::this_thread::sleep_for(std::chrono::nanoseconds(10));
			std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(2));
			std::cout << ".";
			std::this_thread::sleep_for(std::chrono::nanoseconds(10));
			std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(2));
			std::cout << ".";

			Compuer.TakeRandShot();
			bIsShotHappen = Player.IsShotHappen(Compuer.Shot);
		} while (!bIsShotHappen);

		bIsSuccessfulShot = Player.IsShipwrecked(Compuer.Shot); //If the shot hits the ship, Compuer fires again
		bGameLose = Player.IsGameLose(); //Checking if the Compuer has won
	} while (bIsSuccessfulShot && !bGameLose);

	if (bGameLose) {
		GameResult(&Compuer, &Player); //Displaying the winner and loser on the screen
	}

	return bGameLose;
}


void GameResult(FBattleshipGame *Winner, FBattleshipGame *Loser)
{
	std::cout << "\nCongratulations! "<< (*Winner).GatName() <<" won the game.\n";
	std::cout << (*Winner).GatName() << " destroyed the entire fleet of "<< (*Loser).GatName() <<" and ";
	std::cout << (*Loser).GetDestruction() <<"% of non-military infrastructure\n";
	
	std::cout << (*Loser).GatName() << " destroyed "<< (*Winner).GetWreckedShips() <<"% of " << (*Winner).GatName() <<" fleet and ";
	std::cout << (*Winner).GetDestruction() << "% of non-military infrastructure.\n";
}


bool AskToPlayAgain()
{
	std::string Response = "";
	std::cout << "Do you want to play again?(y/n)\n";
	std::getline(std::cin, Response);
	return (Response[0] == 'y' || Response[0] == 'Y');
}
