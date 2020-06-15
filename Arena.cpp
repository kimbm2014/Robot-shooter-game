#include <iostream>
#include "Arena.h"
#include "Player.h"
#include "Robot.h"


using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Arena implementations
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRobots = 0;
}

Arena::~Arena()
{
    //delete robots
    for(int i = 0; i < m_nRobots; i++){
        delete m_robots[i];
    }
    delete m_player;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::robotCount() const
{
    return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const
{
    int counter = 0;
    
    for (int i = 0; i != m_nRobots; i++)
    {
        if (m_robots[i]->row() == r && m_robots[i]->col() == c)    //if robots are on same position, then increment of # of robots.
        {
            counter++;
        }
    }
    return counter;
}

void Arena::display(string msg) const
{
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';
    
    // Indicate each robot's position
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            switch(nRobotsAt(r+1,c+1)){
                case 1:
                    grid[r][c] = 'R';
                    break;
                case 2:
                    grid[r][c] = '2';
                    break;
                case 3:
                    grid[r][c] = '3';
                    break;
                case 4:
                    grid[r][c] = '4';
                    break;
                case 5:
                    grid[r][c] = '5';
                    break;
                case 6:
                    grid[r][c] = '6';
                    break;
                case 7:
                    grid[r][c] = '7';
                    break;
                case 8:
                    grid[r][c] = '8';
                    break;
                case 0:
                    break;
                default:
                    grid[r][c] = '9';
                    break;
            }
    
    // Indicate player's position
    if (m_player != nullptr)
    {
        // Set the char to '@', unless there's also a robot there,
        // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*'; //WHEN DEAD/ ROBOT ON TOP
    }
    
    // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
    
    // Write message, robot, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c)
{
    // If MAXROBOTS have already been added, return false.  Otherwise,
    // dynamically allocate a new robot at coordinates (r,c).  Save the
    // pointer to the newly allocated robot and return true.

    if (m_nRobots == MAXROBOTS)
    {
        cout << "Too many robots have been added" << endl;
        return false;
    }
    else
    {
        m_robots[m_nRobots] = new Robot(this, r, c);
        m_nRobots++;
        return true;
    }
}

bool Arena::addPlayer(int r, int c)
{
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

void Arena::damageRobotAt(int r, int c) ////////////////////TO DO///////////////
{
    // TODO:  Damage one robot at row r, column c if at least one is there.
    // If the robot does not survive the damage, destroy it.
        for(int i=0; i<m_nRobots; i++){
            Robot *temp = m_robots[i];
        if (m_robots[i]->row()==r && m_robots[i]->col()==c){
            if(!m_robots[i]->takeDamageAndLive()){
                temp=m_robots[i];
                do
                {
                    m_robots[i]=m_robots[i+1];
                    i++;
                }while(i<m_nRobots);
                m_robots[m_nRobots]=nullptr;
                m_nRobots--;
                delete temp;
            }
            return;
         }
        }
    }


bool Arena::moveRobots()
{
    // TODO:  Have the k-th robot in the arena make one move.
    //        If that move results in that robot being in the same
    //        position as the player, the player dies.
    for (int k = 0; k < m_nRobots; k++)
    {
        m_robots[k]->move();
        
        if (m_robots[k]->row() == m_player->row() && m_robots[k]->col() == m_player->col())
        {
            m_player->setDead();
        }
    }
    return !m_player->isDead();     //when return true, player is alive, otherwise false
}

