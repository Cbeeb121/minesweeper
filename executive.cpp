#include "executive.h"
#include <cstdlib>

executive::executive()
{
	m_game_board = nullptr;
	gameover=false;
	m_show_board = nullptr;
}


executive::~executive()
{
}


void executive::StartFilesForVBA()
{
	std::ofstream outFile;
	outFile.open("map.txt");
	for (int i = 0; i < m_row_size; i++)
	{
		for (int j = 0; j < m_row_size; j++)
		{
			outFile << std::to_string( m_game_board[i][j].Holding());
		}
	}
	outFile.close();
	outFile.open("board.txt");
	for (int i = 0; i < m_row_size; i++)
	{
		for (int j = 0; j < m_row_size; j++)
		{
			outFile << "H ";
		}
		outFile << "\n";
	}
}

void executive::Run()
{
	std::ofstream resetFiles;
	resetFiles.open("you_lose.txt", std::ofstream::out | std::ofstream::trunc);
	resetFiles.close();
	int x=0, y=0;
	CreateBoard();
	StartFilesForVBA();
	while(!gameover)
	{
		std:: cout << "Where would you like to check?\n" << "Please enter row you would like to check: ";
		std:: cin >> x;
		//this is a key that VBA can use to end the C++ application.
		//we felt this was better than allowing the VBA application administrator privileges.
		if (x == -9999) return;
		std:: cout << "\n Please enter column you would ike to check: ";
		std:: cin >> y;
		Read(x,y);
	}

}



void executive::CreateBoard()
{
	srand(time(NULL));
	//This GUI will be interacted with "behind the scenes"
	//by a windows forms application.
	std::cout << "------------------------------------\n";
	std::cout << "Terminal GUI\n";
	std::cout << "------------------------------------\n";
	std::cout << "Input m_game_board size: ";
	std::cin >> m_row_size;
	std::cout << "Input m_mine_number: ";
	std::cin >> m_mine_number;
	
	m_game_board = new square*[m_row_size];
	for (int i = 0; i < m_row_size;i++)
	{
		m_game_board[i] = new square[m_row_size];
	}
	//now to randomize mine location
	int result = 0;
	while (m_mine_number > 0)
	{
		for (int i = 0; i < m_row_size;i++)
		{
			for (int j = 0; j < m_row_size;j++)
			{
				result = rand() % 10;
				if (result == MINE && m_mine_number > 0 && m_game_board[i][j].Holding()!=MINE)
				{
					//place a mine
					m_game_board[i][j].Holding(MINE);
					m_mine_number--;
				}
				else if(m_game_board[i][j].Holding() != MINE)
				{
					//make a blank
					m_game_board[i][j].Holding(NONE);
				}
			}//end of j for
		}//end of i for
	
	}

	//for testing purposes.
	Print();
	UpdateAdjacents();



	m_show_board = new char*[m_row_size];
	//occupy the show array
	for (int i = 0; i < m_row_size; i++)
	{
		m_show_board[i] = new char[m_row_size];
	}
	for (int i = 0; i < m_row_size; i++)
	{
		for (int j = 0; j < m_row_size; j++)
		{
			m_show_board[i][j] = 'H';
		}
	}
}

void executive::UpdateAdjacents()
{
	for (int i = 0; i < m_row_size; i++)
	{
		for (int j = 0; j < m_row_size; j++)
		{

			int counter = 0;
			if (m_game_board[i][j].Holding() == NONE)
			{
				//check up-right
				if (((i + 1) < m_row_size && (j + 1) < m_row_size))
				{
					if (m_game_board[i + 1][j + 1].Holding() == MINE)
					{
						counter++;
					}
				}


				//check right
				if (((i + 1) < m_row_size && (j) < m_row_size))
				{
					if (m_game_board[i + 1][j].Holding() == MINE)
					{
						counter++;
					}
				}

				//check down-right
				if ((i + 1) < m_row_size && (j - 1) >=0)
				{
					if (m_game_board[i + 1][j - 1].Holding() == MINE)
					{
						counter++;
					}
				}

				//check up
				if (((i)<m_row_size && (j + 1)<m_row_size))
				{
					if (m_game_board[i][j + 1].Holding() == MINE)
					{
						counter++;
					}
				}

				//check down
				if (((i)<m_row_size && (j - 1)>=0))
				{
					if (m_game_board[i][j - 1].Holding() == MINE)
					{
						counter++;
					}
				}

				//check left
				if (((i - 1) >= 0 && (j)<m_row_size))
				{
					if (m_game_board[i - 1][j].Holding() == MINE)
					{
						counter++;
					}
				}

				//check up-left
				if (((i - 1)>=0 && (j + 1)<m_row_size))
				{
					if (m_game_board[i - 1][j + 1].Holding() == MINE)
					{
						counter++;
					}
				}

				//check down-left
				if ((i - 1)>=0 && (j - 1)>=0)
				{
					if (m_game_board[i - 1][j - 1].Holding() == MINE)
					{
						counter++;
					}
				}

				//update the adjacency number
				m_game_board[i][j].AdjacentMines(counter);
				if (counter > 0)
				{
					m_game_board[i][j].Holding(ADJACENT);
				}
			}
		}

		}

	//for testing purposes
	Print();


	}





void executive::Print()
{
	std::cout << "\n\n";
	//print our 2D array
	for (int i = 0; i < m_row_size;i++)
	{
		for (int j = 0; j < m_row_size;j++)
		{
			std::cout << m_game_board[i][j].Holding() << " ";
		}
		std::cout << "\n";
	}
}


//Used to read in coordinate values and decide which button path to choose.
void executive::Read(int x, int y)
{
	if (m_game_board[x][y].Holding() == MINE)
	{
		//std::cout << "BOMB";
		BombReveal();
	}
	if (m_game_board[x][y].Holding() == NONE)
	{
		//call recursive reveal function 'NoneReveal'
		NoneRevealMaster(x, y);
	}
	if (m_game_board[x][y].Holding() == ADJACENT)
	{
		AdjacentReveal(x,y);
	}
}

void executive::AdjacentReveal(int x, int y)
{
	m_show_board[x][y] = std::to_string(m_game_board[x][y].AdjacentMines()).at(0);

	//update the text file
	std::ofstream outFile;
	outFile.open("board.txt", std::ofstream::out | std::ofstream::trunc);
	outFile.close();
	outFile.open("board.txt");
	for (int i = 0; i < m_row_size; i++)
	{
		for (int j = 0; j < m_row_size; j++)
		{
			outFile << m_show_board[i][j] << " ";
		}
		outFile << "\n";
	}
	outFile.close();
}

void executive::BombReveal()
{
	
	std::string newFile = "you_lose.txt";
	std::ofstream outFile;
	outFile.open(newFile);
	outFile << "L\n";
	outFile.close();

}

void executive::NoneReveal(int x, int y)
{

	if (m_game_board[x][y].Holding() == ADJACENT)
	{
		return;
	}
	//recurse up-right
	if (((x + 1) < m_row_size && (y + 1) < m_row_size))
	{

			recReveal(x + 1, y + 1);
	}
	//recurse right
	if (((x + 1) < m_row_size && (y) < m_row_size))
	{
		recReveal(x + 1, y);
	}
	//recurse down-right
	if ((x + 1) < m_row_size && (y - 1) >= 0)
	{
		recReveal(x + 1, y - 1);
	}
	//recurse up
	if (((x) < m_row_size && (y + 1) < m_row_size))
	{
		recReveal(x, y + 1);
	}
	//recurse down
	if (((x) < m_row_size && (y - 1) >= 0))
	{
		recReveal(x, y - 1);
	}
	//recurse left
	if (((x - 1) >= 0 && (y) < m_row_size))
	{
		recReveal(x-1, y);
	}
	//recurse up-left
	if (((x - 1) >= 0 && (y + 1) < m_row_size))
	{
		recReveal(x - 1, y+1);
	}
	//recurse down-left
	if ((x - 1) >= 0 && (y - 1) >= 0)
	{
		recReveal(x - 1, y - 1);
	}
}

void executive::NoneRevealMaster(int x, int y)
{
	NoneReveal(x, y);
	//update a file;
	std::ofstream outFile;
	outFile.open("board.txt", std::ofstream::out | std::ofstream::trunc);
	outFile.close();
	outFile.open("board.txt");
	for (int i = 0; i < m_row_size; i++)
	{
		for (int j = 0; j < m_row_size; j++)
		{
			outFile << m_show_board[i][j] << " ";
		}
		outFile << "\n";
	}
	//reset the recursive checks.
	for (int i = 0; i < m_row_size; i++)
	{
		for (int j = 0; j < m_row_size; j++)
		{
			m_game_board[i][j].CheckedRecursively(false);
		}
	}
	outFile.close();
}

void executive::recReveal(int x, int y)
{
	if (m_game_board[x][y].CheckedRecursively() == true)
	{
		return;
	}
	if (m_game_board[x][y].Holding() == ADJACENT)
	{
		m_show_board[x][y] = std::to_string(m_game_board[x][y].AdjacentMines()).at(0) ;
	}
	else if (m_game_board[x][y].Holding() == NONE)
	{
		m_show_board[x][y] = '-';
		m_game_board[x][y].CheckedRecursively(true);
		NoneReveal(x, y);
	}
}
