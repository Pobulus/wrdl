#include <ncurses.h>
#include <ctime>
#include <cstdlib>

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <random>

#include <vector>

std::string loadDict ( std::string mapname,  std::unordered_map<std::string, int> &gamemap) {
    std::vector<std::string> wordList;

    std::ifstream mapStream ( mapname );
    if ( mapStream ) {
        std::string line;
        
        while ( std::getline( mapStream, line) ){
            gamemap[line] = 1;
            wordList.push_back(line);

        }
        mapStream.close();
    } else {
        std::cerr << "Błąd w otwieraniu pliku "<<mapname << std::endl;
    }
    return wordList[rand()%(int)wordList.size()/5];
}

bool verify(char f[6][5], int row, std::string password, int colors[6][5]){
	int count = 0;
	for(int x = 0; x < 5; x++){
		if(password.find(f[row][x])!=std::string::npos) {
			if(f[row][x]==password.at(x)){
				colors[row][x] = 20;
//				attron(COLOR_PAIR(20));
//				mvprintw(2+(2*row), 1+(2*x), "%c", f[row][x]);
				count++;
			}else{
				colors[row][x] = 30;
//				attron(COLOR_PAIR(30));
//				mvprintw(2+(2*row), 1+(2*x), "%c", f[row][x]);
			}
		}else{
			colors[row][x] = 70;
		}
		
	}
	
	
	if(count == 5) return false;
	else return true;
}
void display(char f[6][5], int clr[6][5]){
	for(int y = 0; y < 6; y++){
		for(int x = 0; x< 5; x++){
			if(clr[y][x]==0) attron(COLOR_PAIR(70));
			else attron(COLOR_PAIR(clr[y][x]));
			
			char to_print = f[y][x];
			if(to_print != ' '){
				to_print += ('A'-'a');
			}
			mvprintw(2+(2*y), 1+(2*x), "%c", to_print);
			//mvprintw(2+(2*y), 1+(2*x)+20, "%d", clr[y][x]);
		}
	}
	return;
}

int main(int argc, char* argv[]){
	srand((unsigned int)time(NULL));
	char field[6][5] = {' '};
	int colors[6][5] = {0};
	setlocale(LC_ALL, "");
	int row=0, col=0;
	std::string frame = 
	"┏━┳━┳━┳━┳━┓\n"
	"┃ ┃ ┃ ┃ ┃ ┃\n"
	"┣━╋━╋━╋━╋━┫\n"
	"┃ ┃ ┃ ┃ ┃ ┃\n"
	"┣━╋━╋━╋━╋━┫\n"
	"┃ ┃ ┃ ┃ ┃ ┃\n"
	"┣━╋━╋━╋━╋━┫\n"
	"┃ ┃ ┃ ┃ ┃ ┃\n"
	"┣━╋━╋━╋━╋━┫\n"
	"┃ ┃ ┃ ┃ ┃ ┃\n"
	"┣━╋━╋━╋━╋━┫\n"
	"┃ ┃ ┃ ┃ ┃ ┃\n"
	"┗━┻━┻━┻━┻━┛\n";
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	curs_set(0);
	printw("LOADING WORDS");
	std::unordered_map<std::string, int> dictionary;
	std::string password = loadDict("dict.txt", dictionary);
	mvprintw(0, 0, "RANDOM PASSWORD");	
	if(argc>1){
		std::string pword (argv[1]);
		if(dictionary[pword]){password = pword;mvprintw(0, 0, "CUSTOM PASSWORD");}
		else mvprintw(0,0, "Custom password not in dictionary");
		}
	//mvprintw(0,0, password.c_str());
	mvprintw(1, 0, frame.c_str());
	
	if(!has_colors()){
		endwin();
		printf("NO COLORS IN YOUR TERMINAL");
		exit(1);
		return 0;
	}
    
	//define color pairs
	//think of the numbers as mixing the colors
	//black 0
	//red 1
	//green 2
	//yellow 3
	//blue 4
	//magenta 5
	//cyan 6       
	//white 7
	start_color();
	
	init_pair(10, COLOR_RED, COLOR_BLACK);//red
	init_pair(20, COLOR_BLACK, COLOR_GREEN);//green
	init_pair(30, COLOR_BLACK, COLOR_YELLOW);//yellow
	init_pair(40, COLOR_BLUE, COLOR_BLACK);//blue
	init_pair(50, COLOR_MAGENTA, COLOR_BLACK);//blue
	init_pair(60, COLOR_CYAN, COLOR_BLACK);//green
	init_pair(70, COLOR_WHITE, COLOR_BLACK);//white
	bool mainloop = true;
	while(mainloop){
		if(row == 6){
			endwin();
			std::cout << "The password was: "<< password << std::endl;
			return 0;
		}
		int a = getch();
		if(a == '\n'&&field[row][4] > ' '){
			std::string s(field[row]);
			if(dictionary[s]){
				if(verify(field, row, password, colors)){
					row++;
					col = 0;
				}else{
					endwin();
					std::cout <<"you win."<<std::endl;
					return 0;}
			}else{
				mvprintw(15, 0, "%s is not in dictionary.", field[row]);
			}
			
		}
		if(a == 8){
			if(col==4)field[row][col] = ' ';
			if(col >0) col --;
			field[row][col] = ' ';
			}
		if(a>'a'-1 && a < 'z'+1){

			field[row][col] = a;
			if(col < 4)col++;
			
		}
		//mvprintw(19, 0, "%d", a);
		attron(COLOR_PAIR(70));
		display(field, colors);
		
		//mvprintw(0, 5, "%d:%d", row, col);
		refresh();
	}
	endwin();
	
}
