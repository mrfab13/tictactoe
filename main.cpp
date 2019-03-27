#include <iostream>
#include <ctime>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <conio.h>
#include <array>
#include <vector>

using namespace std;

//sets the cursor to a specified XY position
void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

//the main menu
int menu()
{
	char opt = 0;
	cout << "w e l c o m  t o  t i c t a c t o e" << endl;
	cout << "1) Human VS Human" << endl;
	cout << "2) Ai (Expert) VS Human " << endl;
	cout << "3) Ai (Easy) VS Human" << endl;
	cout << "4) Exit" << endl;
	cin >> opt;
	return(opt);
}

//draws a board in the console to play on
void setup()
{
	system("CLS");
	cout << "   |   |   " << endl;
	cout << "   |   |   " << endl;
	cout << "___|___|___" << endl;
	cout << "   |   |   " << endl;
	cout << "   |   |   " << endl;
	cout << "___|___|___" << endl;
	cout << "   |   |   " << endl;
	cout << "   |   |   " << endl;
	cout << "   |   |   " << endl;
	cout << endl << endl << "WASD or wasd to move and enter to place";
}

//returns O or X for display purposes
char turn(int player)
{
	char person;
	if (player == 0)
	{
		person = 'O';
	}
	else if (player == 1)
	{
		person = 'X';
	}
	else if (player == -1)
	{
		person = 'X';
	}
	return(person);
}

//for the player to wasd the curser aroound and place the XO depeding on the player
int placement(vector<int>vec, int player)
{
	int x = 1;
	int y = 1;
	int ch = 0;
	int ans = 0;
	int horM = 0;
	int verM = 0;

	while (ch != 13)
	{
		gotoxy(15, 0);
		cout << "player \""<<  turn((player % 2) - 1) << "\" turn" << endl;
		gotoxy(x, y);
		ch = _getch();
		if (ch == 119 || ch == 87) //w key pressed
		{
			if(verM > 0) //if can move up do so and update position
			{
				y -= 3;
				verM--;
			}
		}
		else if (ch == 97 || ch == 65) //a key pressed
		{
			if (horM > 0) // if can move left do so and update position
			{
				x -= 4;
				horM--;
			}

		}
		else if (ch == 115 || ch == 83) //s key pressed
		{
			if (verM < 2)// if can move down do so and update position
			{
				y += 3;
				verM++;
			}

		}
		else if (ch == 100 || ch == 68) //d key pressed
		{
			if (horM < 2)// if can move right do so and update position
			{
				x += 4;
				horM++;
			}

		}
		else if (ch == 13) //enter key pressed
		{
			ans = ((verM * 3) + 1) + (horM) - 1; //translate from a 1-3 x and 1-3 y position to a 0-8 number to go into vector position 
			if (vec.at(ans) != 3) //if the space isnt blank continue looping 
			{
				ch = NULL;

			}
		}
	}
	//draws depending on plaer
	if (player %2 == 0)
	{
		cout << "\b X";
	}
	else if (player%2 !=  0)
	{
		cout << "\b O";
	}
	return (ans);
}

//checks if the player passes through has won the game or if the boardstatehas tied
int checkwin(vector<int> vec, int player)
{
	int check = 1;
	int tie = 1;
	for (int i = 0; i < 9; i++)
	{
		if (vec.at(i) == 3) //if a blank space on the board it cant be a tie
		{
			tie = 0; 
		}
	}
	//sets value beforehand so if its a tie on the winning move its still a win
	if (tie == 1)
	{
		check = -1; 
	}
	//all possible win conditions
	if (vec.at(0) == player && vec.at(3) == player && vec.at(6) == player)
	{
		check = 0;
	}
	if (vec.at(1) == player && vec.at(4) == player && vec.at(7) == player)
	{
		check = 0;
	}
	if (vec.at(2) == player && vec.at(5) == player && vec.at(8) == player)
	{
		check = 0;
	}
	if (vec.at(0) == player && vec.at(1) == player && vec.at(2) == player)
	{
		check = 0;
	}
	if (vec.at(3) == player && vec.at(4) == player && vec.at(5) == player)
	{
		check = 0;
	}
	if (vec.at(6) == player && vec.at(7) == player && vec.at(8) == player)
	{
		check = 0;
	}
	if (vec.at(0) == player && vec.at(4) == player && vec.at(8) == player)
	{
		check = 0;
	}
	if (vec.at(2) == player && vec.at(4) == player && vec.at(6) == player)
	{
		check = 0;
	}
	return (check);
}

//PVP mode
void humanGame()
{
	//initialsing values
	vector<int> vec(9, 3);
	int pos;
	int playing = 1;
	int player = 1;
	while (playing == 1) //place, apply to vector, ceck for win, next player, loop
	{
		pos = placement(vec, player);
		vec.at(pos) = player%2;
		playing = checkwin(vec, player%2);
		player++;
	}
	//prints results
	gotoxy(0, 13);
	if (playing == -1)
	{
		cout << "its a tie" << endl;
	}
	else if (playing == 0)
	{
		cout << "game over player \"" << turn(player % 2) << "\" wins " << endl;
	}
	system("pause");
}

//the recursively called inside itself to run through every possoble position and does alpha beta pruning
int minimax(vector<int>vec, int depth, int turn, int alpha, int beta)
{
	int score; 
	int tempturn;
	if (turn == 1)
	{
		tempturn = 0;
	}
	else
	{
		tempturn = 1;
	}
	score = checkwin(vec, tempturn);
	if (score == 0) //if sombody won assign varying points depening on depth and who won
	{
		if (turn == 0)
		{
			score = -100 + depth;
		}
		else
		{
			score = 100 - depth;
		}
		return (score);
	}
	if (score == -1) //tie
	{
		return (0); 
	}
	if (turn == 0)//simulate ai turn
	{
		int best = -1000;
		for (int i = 0; i < 9; i++) //for all 9 placements
		{
			if (vec.at(i) == 3) //if tested space is blank
			{
				vec.at(i) = 0; //set
				best = max(best, minimax(vec, depth + 1, 1, alpha, beta)); //recursvly calls the fncton and takes best option 
				alpha = max(alpha, best); //prune
				if (beta <= alpha)
				{
					break;
				}
				vec.at(i) = 3; //set back
			}
		}
		return(best); //returns the best option
	}
	else //the same as above but simulating the player turn
	{
		int best = 1000;
		for (int i = 0; i < 9; i++)
		{
			if (vec.at(i) == 3)
			{
				vec.at(i) = 1;
				best = min(best, minimax(vec, depth + 1, 0, alpha, beta)); 
				beta = min(beta, best);
				if (beta <= alpha)
				{
					break;
				}
				vec.at(i) = 3;
			}
		}
		return (best);
	}
}

//initates the minimax function 
int compute(vector<int>vec)
{
	int pos = 0;
	int best = -1000;
	int moveval = 0;
	for (int j = 0; j < 9; j++)//checks if it can win immeditly to save computing time and adds a fail-safe
	{
		if (vec.at(j) == 3)
		{
			vec.at(j) = 0;
			if (checkwin(vec, 0) == 0)
			{
				pos = j;
				return (pos);
			}
			vec.at(j) = 3;
		}
	}
	for (int j = 0; j < 9; j++) //calls minmax 9 times for each poisition taking the move with the best value
	{
		if (vec.at(j) == 3)
		{
			vec.at(j) = 0;
			moveval = minimax(vec, 0, 1, -1000, 1000); //-1000,1000
			vec.at(j) = 3;
			if (moveval > best)
			{
				pos = j;
				best = moveval;
			}
		}

	}
	return (pos); //returns the positon of the best value
}

//shell for the AI turn
int Aiturn(vector<int> vec, int ez)
{
	int pos = 0;
	int x;
	int y;

	if (ez == 0)//hard mode (depth search max with pruning)
	{
		pos = compute(vec);
	}
	else if (ez == 1) //ez mode (depth search 2 equilivant)
	{
		pos = rand() % (8);
		while (vec.at(pos) != 3)
		{
			pos = rand() % (8); //lowest priority = rand
		}
		for (int j = 0; j < 9; j++) //mid priorty making sure it doesnt lose (overwriting rand if it occurs)
		{
			if (vec.at(j) == 3)
			{
				vec.at(j) = 1;
				if (checkwin(vec, 1) == 0)
				{
					pos = j;
				}
				vec.at(j) = 3;
			}
		}
		for (int j = 0; j < 9; j++) //highest priorty is immediatly winning (overwriting rand and not losing)
		{
			if (vec.at(j) == 3)
			{
				vec.at(j) = 0;
				if (checkwin(vec, 0) == 0)
				{
					pos = j;
				}
				vec.at(j) = 3;
			}
		}
	}
	//uses the returned pos value and converts it to x y positions and draws it in
	if (pos == 0 || pos == 3 || pos == 6)
	{
		x = 1;
	}
	if (pos == 1 || pos == 4 || pos == 7)
	{
		x = 5;
	}
	if (pos == 2 || pos == 5 || pos == 8)
	{
		x = 9;
	}
	if (pos == 0 || pos == 1 || pos == 2)
	{
		y = 1;
	}
	if (pos == 3 || pos == 4 || pos == 5)
	{
		y = 4;
	}
	if (pos == 6 || pos == 7 || pos == 8)
	{
		y = 7;
	}
	gotoxy(x, y);
	cout << "\b X";
	return(pos);
}

//structure gameplay fo the AI vs HUMAN
void AiGame(int ez)
{
	vector<int> vec(9, 3);
	int pos;
	int playing = 1;
	int player = 1;
	while (playing == 1)
	{
		if (playing == 1 && player % 2 == 1)  //player turn 
		{
			pos = placement(vec, player);
			vec.at(pos) = player % 2;
			playing = checkwin(vec, player % 2);
			player++;
		}
		else if (playing == 1 && player % 2 == 0)  //ai turn 
		{
			gotoxy(15, 0);
			cout << "player \"" << turn((player % 2) - 1) << "\" turn" << endl; //sayint that ai is playing as this is done under the placement function for the player that the ai doesnt use
			pos = Aiturn(vec , ez);
			vec.at(pos) = player % 2;
			playing = checkwin(vec, player % 2);
			player++;
		}
	}
	//prints results
	gotoxy(0, 13);
	if (playing == -1)
	{
		cout << "its a tie" << endl;
	}
	else if (playing == 0)
	{
		cout << "game over player \"" << turn(player % 2) << "\" wins " << endl;
	}
	system("pause");
}
//player = 1 (odd) ai = 0 (even);
//calls the menu and lets the player select and play any game mode and returns them to the menu when that games over
int main()
{
	int choice;
	while (true)
	{
		system("CLS");
		choice = menu(); //choice is stored as a char so letters dont break the code 49-52 is equal to 1-4
		if (choice == 49)
		{
			setup();
			humanGame(); //pvp
		}
		else if (choice == 50)
		{
			setup();
			AiGame(0); //easy mode off
		}
		else if (choice == 51)
		{
			setup();
			AiGame(1); //easy mode on 
		}
		else if (choice == 52)
		{
			return(0); //quits the game 
		}
		else
		{
			choice = 0; //non valide input
		}
	}
	return(0);
}