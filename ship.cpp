// Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
#include "ship.h"
using namespace std;

Ship::Ship() : container('s'), output_mode('M'), level(0), dimension(0) , no_sol(false){}

void Ship::discover(){
    
    Info &tail = space_station[end.level][end.row][end.column];
    while(tail.dir == ' '){
        //check for no solution
        if(srch_cont.empty()){
            no_sol = true;
            return;
        }

        //BFS with queue mode
        if(container == 'q'){
            curr_loc = srch_cont.front();
            srch_cont.pop_front();
        }
        //DFS with stack mode
        else{
            curr_loc = srch_cont.back();
            srch_cont.pop_back();
        }
            
        investigate(curr_loc);
    }
}

void Ship::investigate(Coordinate &c){
    
    //first, investigate the same floor in the sequence of n,e,s,w
    int row = c.row;
    int col = c.column;
    int lev = c.level;
    char type = space_station[lev][row][col].type;

    //n
    if(row - 1 > 0 || row - 1 == 0){
        Coordinate north = {lev, row - 1, col};
        if(reachable(north)){
            mark(north, 'n');
        } 
    }
    
    //e
    if(col + 1 < dimension - 1 || col + 1 == dimension - 1){
        Coordinate east = {lev, row, col + 1};
        if(reachable(east)){
            mark(east, 'e');
        } 
    }
    
    //s
    if(row + 1 < dimension - 1 || row + 1 == dimension - 1){
        Coordinate south = {lev, row + 1, col};
        if(reachable(south)){
            mark(south, 's');
        } 
    }

    //w
    if(col - 1 > 0 || col - 1 == 0){
        Coordinate west = {lev, row, col - 1};
        if(reachable(west)){
            mark(west, 'w');
        } 
    }
    
    //if standing on elevator, check every other floor
    if(type == 'E'){
        for(int i = 0; i < level; ++i){
            Coordinate elevator = {i, row, col};
            if(space_station[i][row][col].type == 'E' && reachable(elevator))
                mark(elevator, static_cast<char>(lev));
        }
    }
}

bool Ship::reachable(Coordinate &c){

    //first check whether it is a valid coordinate
    Info &here = space_station[c.level][c.row][c.column];
    if(here.type == ' ') return false;

    //valid coordinate but cannot be reached 
    if(here.type == '#') return false;
    if(here.dir != ' ') return false;

    return true;
}

void Ship::mark(Coordinate &c, char dir){
    
    Info &here = space_station[c.level][c.row][c.column];
    if(dir != 'n' && dir != 'e' && dir != 's' && dir != 'w')
           here.dir = static_cast<char>(dir + 48);
    else   here.dir = dir;
    srch_cont.push_back(c);
}

void Ship::backtracing(){

    //start from the hanger
    char curr_dir = ' ';
    char pred_dir = 'H';
    int prev_lev = end.level;
    Coordinate me = end;

    while(true){
        //get Info of me
        Info &here = space_station[me.level][me.row][me.column];

        //record my current direction
        curr_dir = here.dir;

        //update my type/direction to the direction of predecessor
        here.type = pred_dir;
        here.dir = pred_dir;

        //add me to the front of the backtracing route
        footstep.push_front(me);

        //stop backtracing once reaching the start
        Coordinate &lhs = footstep.front();
        if(lhs.level == start.level && lhs.row == start.row && lhs.column == start.column){
            break;
        }
        //use my direction to find predecessor
        else me = update(me, curr_dir);

        //if level is changed, update pred_dir to the level that I come from, update level
        int curr_lev = me.level;
        if(prev_lev != curr_lev){
           pred_dir = static_cast<char>(prev_lev + 48);
           prev_lev = curr_lev; 
        }
        //update pred_dir
        else pred_dir = curr_dir;
    }
    //remove the coordinate of hanger
    footstep.pop_back();
}

Ship::Coordinate Ship::update(Coordinate &c, char curr){

    int lev = c.level;
    int row = c.row;
    int col = c.column;
    if(curr == 'n')       c = Coordinate{lev, row + 1, col};
    else if(curr == 'e')  c = Coordinate{lev, row, col - 1};
    else if(curr == 's')  c = Coordinate{lev, row - 1, col};
    else if(curr == 'w')  c = Coordinate{lev, row, col + 1};
    else{
        c = Coordinate{static_cast<int>(curr - 48), row, col};
    }               
    return c;

}

void Ship::print_deque(char mode){
    if(mode == 's'){
        for(auto iter = srch_cont.begin(); iter != srch_cont.end(); ++iter){
            Coordinate &c = *iter;
            Info &here = space_station[c.level][c.row][c.column];
            if(here.dir == ' '){
                cout << "(" << c.level << "," << c.row << ","
                << c.column << "," << here.type << ")" << "\n";
            }
            else{
                cout << "(" << c.level << "," << c.row << ","
                << c.column << "," << here.dir << ")" << "\n";
            }
            
        }
    }
    else if(mode == 'f'){
        for(auto iter = footstep.begin(); iter != footstep.end(); ++iter){
            Coordinate &c = *iter;
            Info &here = space_station[c.level][c.row][c.column];
            cout << "(" << c.level << "," << c.row << ","
                 << c.column << "," << here.dir << ")" << "\n";
        }
    }
    else{
        cerr << "Invalid output mode specified\n";
        exit(0);
    }
    
}

bool Ship::no_solution(){
    return no_sol;
}

Ship::Info Ship::get_info(Coordinate &c){
    return Ship::space_station[c.level][c.row][c.column];
}

bool operator==(const Ship::Coordinate &lhs, const Ship::Coordinate &rhs){
    if(lhs.level == rhs.level && lhs.row == rhs.row && lhs.column == rhs.column){
        return true;
    }
    else return false;
}   

