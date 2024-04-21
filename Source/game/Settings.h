/*
  ==============================================================================

    Rules.h
    Created: 28 Nov 2023 9:25:04pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

enum class Topology { Boarders, Torus };

class Settings
{
public:
    Settings()
    {
        topology = Topology::Torus;
    };
    
    /** Set topology configuration */
    void setTopology(Topology newTopology)
    {
        topology = newTopology;
    };
    
    /** Get topology configuration */
    Topology getTopology()
    {
        return topology;
    };
    
    /** Returns true if a cell survives or becomes alive */
    bool getNeighbourAffect(bool cellIsAlive, int numberOfNeighbours)
    {
        if (cellIsAlive)
        {
            if (numberOfNeighbours < 2 || numberOfNeighbours > 3)
            {
                return false; // Cell was alive and dies
            }
            else
            {
                return true; // Cell was alive and survives
            }
        }
        else
        {
            if (numberOfNeighbours == 3)
            {
                return true; // Cell was dead and becomes a live cell
            }
            else
            {
                return false; // Cell was dead and stays dead
            }
        }
    };
private:
    Topology topology;
};
