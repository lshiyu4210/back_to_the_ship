// Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
#ifndef SHIP_H
#define SHIP_H
#include <iostream>
#include <deque>
#include <vector>
#include <cassert>
#include <algorithm>
using namespace std;

class Ship{
public:

    struct Coordinate{
        int level;
        int row;
        int column;
    };

    struct Info{
        char type = '.';       //type:.,#,S,H,E 
        char dir = ' ';        //directions: n,e,s,w
    };
    
    //default constructor
    Ship();

    //Functions implemented in ship_driver.cpp
    //Read command line options 
    void get_mode(int argc, char** argv);

    //Read input files
    void read_input();

    void read_list();

    void read_map();

    //Write output files
    void write();

    //Functions implemented in ship.cpp
    //BFS for queue, DFS for stack
    void discover();
    
    //find reachable locations around certain coordinate and add them
    //to the search container
    void investigate(Coordinate &c);
    
    //determine whether one location is reachable or not
    bool reachable(Coordinate &c);

    //mark one function as discovered and push onto search container
    void mark(Coordinate &c, char dir);

    //find the route from the hanger to the start
    void backtracing();

    //update the standing point for backtracing
    Coordinate update(Coordinate &c, char curr);

    //print every element from srch_cont
    void print_deque(char mode);
    
    //return no_sol
    bool no_solution();

    //get info from space station base on coordinate
    Info get_info(Coordinate &c);

private:
    
    //layout of the entire searching space
    vector<vector<vector<Info>>> space_station;  

    //search container for both stack and queue mode
    deque<Coordinate> srch_cont;

    //recording the path from start to hanger
    deque<Coordinate> footstep;

    //standing point for investigation
    Coordinate curr_loc;

    //Start and end positions
    Coordinate start;

    Coordinate end;

    //search container and output mode
    char container;

    char output_mode;
    
    //space station dimensions
    int level;

    int dimension;

    //check if there's no solution
    bool no_sol;

};

#endif