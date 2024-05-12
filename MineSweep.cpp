#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//Declare a struct for ease of convienence of handling location.
struct Location{
  int row;
  int column;
  string icon;
};

//Declare all the symbols that are going to be used.
string bomb(){
    return "\u001b[48;5;15m\U0001F4A3\u001b[0m";
}
string closed_sq(){
    return "\u001b[48;5;15m\U0001F532\u001b[0m";
}
string safe0(){
    return "\u001b[48;5;15m\U0001F7E9\u001b[0m";
}
string one(){
    return "\u001b[48;5;25m\u001b[38;5;0m\u00301\u001b[0m";
}
string two(){
    return "\u001b[48;5;15m\u001b[38;5;0m\u00302\u001b[0m";
}
string three(){
    return "\u001b[48;5;9m\u001b[38;5;0m\u00303\u001b[0m";
}
string four(){
    return "\u001b[48;5;207m\u001b[38;5;0m\u00304\u001b[0m";
}
string five(){
    return "\u001b[48;5;202m\u001b[38;5;0m\u00305\u001b[0m";
}
string six(){
    return "\u001b[48;5;14m\u001b[38;5;0m\u00306\u001b[0m";
}
string seven(){
    return "\u001b[48;5;130m\u001b[38;5;0m\u00307\u001b[0m";
}
string eight(){
    return "\u001b[48;5;93m\u001b[38;5;0m\u00308\u001b[0m";
}

//Make the grid.
void set_grid(vector <vector <string> > &grid,vector <int> &size){
    int row = size.at(0);
    int col = size.at(1);
    for(int i=0;i<row;++i){
        vector <string>blank_square;
        for(int j=0;j<col;++j){
            blank_square.push_back(closed_sq());
        }
        grid.push_back(blank_square);
    }

}
//Display the Grid.
void show_grid(const vector <vector <string> > &grid){
    cout<<"  ";
    for(int j=0;j<grid.at(0).size();++j){
        cout<< char(65+j) << " ";
    }
    cout<<endl;
    for(int i=0;i<grid.size();++i){
        cout << i + 1 << " ";
        for(int x=0;x < grid.at(i).size();++x){
            cout << grid.at(i).at(x);
        }
        cout<<endl;
    }
    cout<<endl;
}
//Randomly Place 10 Bombs in the grid.
void Set_bombs(vector <Location> &bomb_loc){
    vector< pair<int, int> > selectedCoordinates;
    Location data;
    while(selectedCoordinates.size()<10){
        int random_row = rand() % 9;
        int random_col = rand() % 9 ;
        for(int x=0;x<selectedCoordinates.size();++x){
            if((random_row==selectedCoordinates.at(x).first) && (random_col==selectedCoordinates.at(x).second)){
                selectedCoordinates.erase(selectedCoordinates.begin()+x);
            }else{
                continue;
            }
        }
        selectedCoordinates.push_back({random_row,random_col});
    }
    for(int x=0;x<selectedCoordinates.size();++x){
        data.row = selectedCoordinates.at(x).first;
        data.column = selectedCoordinates.at(x).second;
        data.icon = bomb();
        bomb_loc.push_back(data);
    }
}
//Get guesses from user.
void get_guess(const vector <vector <string> > &grid,Location &guess,vector <string> &guesses){
    bool validguess = false;
    while(!validguess){
        cout << "Which location do you want to check? (e.g. C5) >> " ;
        string answer;
        cin >> answer;
        
        // check to see if they put a row and a column
        if(answer.size()<2){
            cout << "That's not a spot in the Grid, please enter your guess again. " << endl ;
            continue;
        }
        // To make sure guessed squares are accounted for.
        for(int x=0;x<guesses.size();++x){
            if(guesses.at(x)==answer){
                cout << "You have already guessed this spot on the Grid. Please Enter another spot." << endl ;
                continue;
            }
        }
        // convert their answer into a Location variable
            guess.column = answer.at(0) - 'A'; // character "math" to get from ASCII to an index value
            guess.row = answer.at(1) - '1';    // character "math" to get from ASCII to an index value
            guesses.push_back(answer);
            
        // check if what they entered converts to a valid indices
        if((((0 <= guess.column) && (guess.column < grid.at(0).size()))) && (((0 <= guess.row) && (guess.row < grid.size())))){
            validguess = true;
        } else {
            cout << "That's not a spot in the Grid, please enter your guess again. " << endl ;
        }
    }
}
//Convert Number to emoji
string ConvNtoE(int &Num){
    string emoji;
    if(Num==0){
        emoji= safe0();
    }
    else if(Num==1){
        emoji= one();
    }
    else if(Num==2){
        emoji= two();
    }
    else if(Num==3){
        emoji= three();
    }
    else if(Num==4){
        emoji= four();
    }
    else if(Num==5){
        emoji= five();
    }
    else if(Num==6){
        emoji= six();
    }
    else if(Num==7){
        emoji= seven();
    }
    else if(Num==8){
        emoji= eight();
    }else{
        emoji="";
    }
    return emoji;
}
//Function to find the number of bombs immediately adjacent to the guessed square.
int adjacent_bomb(vector<vector<string>>& grid,const vector<Location>& bomb_loc,int row,int column){
    int count=0;
    int num_row = grid.size();
    int num_col = grid.at(0).size();
    for(int i= max(0,row-1);i<min(row+2,num_row);++i){
        for(int j= max(0,column-1);j<min(column+2,num_col);++j){
            for(int k=0;k<bomb_loc.size();++k){
                if((bomb_loc.at(k).row==i)&&(bomb_loc.at(k).column==j)){
                    ++count;
                }
            }
        }
    }
    return count;
}
//Update and publish new grid.
void update_grid(vector <vector <string> > &grid,const vector <Location> &bomb_loc,const Location &guess, bool &isbomb){
    int numofbombs =0 ;
    numofbombs=adjacent_bomb(grid,bomb_loc,guess.row,guess.column);
    //cout<<numofbombs<<endl;
    grid.at(guess.row).at(guess.column)=ConvNtoE(numofbombs);
    for(int i=0;i<bomb_loc.size();++i){
        if((guess.row==bomb_loc.at(i).row) && (guess.column==bomb_loc.at(i).column)){
            for(int j=0;j<bomb_loc.size();++j){
                grid.at(bomb_loc.at(j).row).at(bomb_loc.at(j).column)=bomb();
            }
            isbomb = true;
            cout<< "\u001b[1m\u001b[48;5;97m OPPS!! You found the bombs! \u001b[0m" << endl;
            cout<< "\u001b[1m\u001b[48;5;97m GAME OVER!! \u001b[0m"<<endl;
            break;
        }
    }
    show_grid(grid);
}
int main(){
    /* initialize random seed:*/
    srand (time(NULL));
    //Initialize New game.
    cout << "\u001b[1m\u001b[7m Minesweeper Game \u001b[0m" << endl; 
    cout << "\u001b[1m\u001b[7m The objective of the game is to select all the squares without bombs. \u001b[0m"<<endl;
    cout << "\u001b[1m\u001b[7m There are 10 bombs randomly placed. \u001b[0m" << endl;
    cout << "\u001b[1m\u001b[7m If you select the square with a bomb you lose! \u001b[0m" << endl;
    cout << "\u001b[1m\u001b[7m For each Square you select, it will showcase the exact number of bombs immediately adjacent to it. \u001b[0m" << endl;
    cout << "\u001b[1m\u001b[7m For Example \u001b[0m"<< safe0()<<"\u001b[1m\u001b[7m represents 0 bombs adjacent to it. \u001b[0m" << endl;
    cout << "\u001b[1m\u001b[7m Good Luck! \u001b[0m" << endl;
    cout<<endl;
    //Initialize Grid and size of grid using vectors.
    vector <vector <string> > grid;
    vector <int> size = {9,9};
    //Setup and show the Playing grid.
    set_grid(grid,size);
    show_grid(grid);
    //Setup Bomb Locations
    vector <Location> bomb_loc; 
    Set_bombs(bomb_loc);

    bool playing = true;
    int numGuesses = 0;

    //Initializing a vector to store all the answers guessed.
    vector <string> guesses;

    while(playing){
        //Initalize vector to get guesses from user
        Location guess;
        bool isbomb=false;
        get_guess(grid,guess,guesses);
        update_grid(grid,bomb_loc,guess,isbomb);
        ++numGuesses;
        if(numGuesses==71){
            cout << "\u001b[1m\u001b[48;5;90m Your score is: 71 \u001b[0m"<< endl;
            cout << "\u001b[1m\u001b[48;5;90m Congratulations! You Beat the Game! \u001b[0m" << endl;
            break;//End the game.
        }
        if(isbomb){
            cout << "Your score is: " << numGuesses-1 << endl;
            break;//End the game.
        }
    }
}