// Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
#include <getopt.h>
#include <string>
#include <cstring>
#include "ship.h"
using namespace std;

void Ship::get_mode(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;

    //check more or less than one -stack -queue
    int num_mode = 0;
    for(int i = 0; i < argc; ++i){
        if(strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "-s") == 0 
        || strcmp(argv[i], "--stack") == 0  || strcmp(argv[i], "--queue") == 0 )
            ++num_mode;
    }
    if(num_mode > 1){
        cerr << "Multiple routing modes specified";
        exit(1);
    }
    if(num_mode == 0){
        cerr << "No routing mode specified";
        exit(1);
    }

    struct option longOpts[] = {{ "stack", no_argument, nullptr, 's' },
                                { "queue", no_argument, nullptr, 'q' },
                                { "output", required_argument, nullptr, 'o' },
                                { "help", no_argument, nullptr, 'h' },
                                { nullptr, 0, nullptr, '\0' }};
    

    while ((option = getopt_long(argc, argv, "sqo:h", longOpts, &option_index)) != -1) {
        switch (option) {
            case 's':
                container = 's';
                break;
        
            case 'q':
                container = 'q';
                break;

            case 'o': {
                string arg{optarg};
                if(arg != "L" && arg != "M"){
                    cerr << "Invalid output mode specified\n" ;
                    exit(1);
                }

                if(arg == "L") output_mode = 'L';
                else           output_mode = 'M';
                break;
            }  

            case 'h':     
                cout << "Usage: " << argv[0] << " --stack|-s" << " --queue|-q" << " [-o M|L] | -h\n";
                exit(0);

            default:
                cerr << "Error: invalid option\n";
                exit(1);
    }
  }
}  // get_mode()

void Ship::read_list(){
    //Read in coordinates
    char next;
    string instruction;

    //read in the coordinates from list
    while(cin >> next){       //(
            if(next == '/') getline(cin, instruction);
            else{
                int lev;
                int row;
                int col;
                char tp;

                cin >> lev;   //lev
                cin >> next;  //,
                //check invalid lev
                if(lev > level - 1 || lev < 0){
                    cerr << "Invalid map level\n";
                    exit(1);
                }

                cin >> row;   //row
                cin >> next;  //,
                //check invalid row
                if(row > dimension - 1 || row < 0){
                    cerr << "Invalid map row\n";
                    exit(1);
                }

                cin >> col;   //col
                cin >> next;  //,
                
                //check invalid column
                if(col > dimension - 1 || col < 0){
                    cerr << "Invalid map column\n";
                    exit(1);
                }

                cin >> tp;   //tp
                cin >> next; //)

                //check invalid type
                if(tp != '.' && tp != '#' && tp != 'S' && tp != 'H' && tp != 'E'){
                    cerr << "Invalid map character\n";
                    exit(1);
                }

                //store coordinates into space station
                space_station[lev][row][col] = Info{tp};
                if(tp == 'S'){
                    start = Coordinate{lev, row, col};
                    srch_cont.push_back(start);
                }
                
                if(tp == 'H'){
                    end = Coordinate{lev, row, col};
                }   

            }
        }
    
} //read_list()

void Ship::read_map(){
    //Read in coordinates
    char next;
    string instruction;

    for(int i = 0; i < level; ++i){
        for(int j = 0; j < dimension; ++j){
            for(int k = 0; k < dimension; ++k){
                cin >> next;
                //skipping comments
                while(next == '/'){
                    getline(cin, instruction);
                    cin >> next;
                }
                //check invalid type
                if(next != '.' && next != '#' && next != 'S' && next != 'H' && next != 'E'){
                    cerr << "Invalid map character\n";
                    exit(1);
                }
                space_station[i][j][k] = Info{next};

                if(next == 'S'){
                    start = Coordinate{i, j, k};
                    srch_cont.push_back(start);
                }
                
                if(next == 'H'){
                    end = Coordinate{i, j, k};
                }   
            }
        }
    }
}//read_map()

void Ship::read_input(){
    //determine input mode
    char mode;
    cin >> mode;
    if(mode != 'L' && mode != 'M'){
        cerr << "Invalid output mode specified\n";
        exit(1);
    }
    
    //resize space station
    cin >> level >> dimension;
    if(level < 1 || level > 10){
        cerr << "Invalid map level\n";
        exit(1);
    }

    space_station.resize(level, vector<vector<Info>>(dimension, vector<Info>(dimension)));
    
    //list mode
    if(mode == 'L') read_list();
    //map mode
    else read_map();

}//read_input()

void Ship::write(){
    if(output_mode == 'L'){
        cout << "//path taken\n";
        print_deque('f');
    }
    else{
        cout << "Start in level " << start.level << ", row " 
        << start.row << ", column " << start.column << "\n";
        for(int i = 0; i < level; ++i){
            cout << "//level " << i << "\n";
            for(int j = 0; j < dimension; ++j){
                for(int k = 0; k < dimension; ++k){
                    Info &temp = space_station[i][j][k];
                    cout << temp.type;
                }
                cout << "\n";
            }
        }
    }
}//write()


int main(int argc, char** argv){

    ios_base::sync_with_stdio(false); //turn off sync
    Ship ship;
    ship.get_mode(argc, argv);
    ship.read_input();
    ship.discover();    
    if(!ship.no_solution())
        ship.backtracing();
    ship.write();
}