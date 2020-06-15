#include <iostream>
#include "Player.h"
#include "Arena.h"
#include "globals.h"
#include <string>
#include <stdlib.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}



string Player::takeComputerChosenTurn()
{
    if(m_arena->nRobotsAt(m_row,m_col+1)==0 && m_arena->nRobotsAt(m_row,m_col-1)==0 && m_arena->nRobotsAt(m_row+1,m_col)== 0 && m_arena->nRobotsAt(m_row-1,m_col)==0) // check surrounded
    {
        ///////SHOOT////////
        for(int i=0; i<=MAXSHOTLEN; i++){    //check all the direction and find the best direction for a shoot
            if(m_arena->nRobotsAt(m_row, m_col+i)>0){               //shoot to the right
                if (shoot(RIGHT))
                    return "Shot and hit!";
                else
                    return "Shot and missed!";
            }
            else if (m_arena->nRobotsAt(m_row, m_col-i)>0){         //shoot to the lefr
                if(shoot(LEFT))
                    return "Shot and hit!";
                else
                    return "Shot and missed!";
            }
            else if (m_arena->nRobotsAt(m_row+i, m_col)>0){    //shoot to the down
                if(shoot(DOWN))
                    return "Shot and hit!";
                else
                    return "Shot and missed!";
            }
            else if (m_arena->nRobotsAt(m_row-i, m_col)>0){   //shoot to the up
                if(shoot(UP))
                    return "Shot and hit!";
                else
                    return "Shot and missed!";
            }
        }
    }
    ////////MOVE////////
    else if (m_arena->nRobotsAt(m_row, m_col+1)==0){   //move to the right
        move(RIGHT);
        return "Moved";
    }
    else if (m_arena->nRobotsAt(m_row, m_col-1)==0){   //move to the left
        move(LEFT);
        return "Moved";
    }
    else if (m_arena->nRobotsAt(m_row+1, m_col)==0){  //move to the down
        move(DOWN);
        return "Moved";
    }
    else if (m_arena->nRobotsAt(m_row-1, m_col)==0){  //move to the up
        move(UP);
        return "Moved";
    }

stand();
return "stood";
}


void Player::stand()
{
    m_age++;
}

void Player::move(int dir)
{
    switch (dir)
    {
        case UP:
            if (m_row > 1)
            {
                m_row--;
                m_age++;
            }
            break;
        case DOWN:
            if (m_row < m_arena->rows())
            {
                m_row++;
                m_age++;
            }
            break;
        case LEFT:
            if (m_col > 1)
            {
                m_col--;
                m_age++;
            }
            break;
        case RIGHT:
            if ( m_col < m_arena->cols())
            {
                m_col++;
                m_age++;
            }
            break;
    }
}




bool Player::shoot(int dir){
    m_age++;
    
    if (rand() % 3 == 0) // miss with 1/3 probability
    {
        return false;
    }
    else if (dir == UP){
        for (int i = 0; i < MAXSHOTLEN; i++){
            if (m_arena->nRobotsAt(m_row-i, m_col)>0 && m_row-i > 0){
                m_arena->damageRobotAt(m_row - i, m_col);
                return true;
            }
        }
    }
    else if (dir == DOWN){
        for (int i = 0; i < MAXSHOTLEN; i++){
            if (m_arena->nRobotsAt(m_row+i, m_col)>0 && m_row+i > 0){
                m_arena->damageRobotAt(m_row + i, m_col);
                return true;
            }
        }
    }
    else if (dir == LEFT){
        for (int i = 0; i < MAXSHOTLEN; i++){
            if (m_arena->nRobotsAt(m_row, m_col-i)>0 && m_col-i > 0){
                m_arena->damageRobotAt(m_row, m_col - i);
                return true;
            }
        }
    }
    else if (dir == RIGHT){
        for (int i = 0; i < MAXSHOTLEN; i++){
            if (m_arena->nRobotsAt(m_row, m_col+i)>0 && m_col+i > 0){
                m_arena->damageRobotAt(m_row, m_col + i);
                return true;
            }
        }
    }
    return false;
}


bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}
