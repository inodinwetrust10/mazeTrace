// Maze PathFinding algorithm using Depth First Search

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <chrono>
#include <thread>
using namespace std;

// Defining hash Function for pair<T1, T2>

vector<vector<char>> generateMaze(int size) {
    vector<vector<char>> maze(size, vector<char>(size, 'O'));

    // Place start and end points
    maze[0][0] = 'S';
    maze[size - 1][size - 1] = 'E';

    // Randomly place barriers
    srand(time(0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (maze[i][j] != 'S' && maze[i][j] != 'E') {
                if (rand() % 3 == 0) { // 1 in 3 chance to place a barrier
                    maze[i][j] = 'X';
                }
            }
        }
    }
    return maze;
}


void printMaze(vector<vector<char>>& maze) {
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

void printPath(vector<vector<char>>& maze, stack<pair<int, int>>& path) {
    if (path.empty() || maze[path.top().first][path.top().second] != 'E') {
        cout << "No way found from start to end!\n";
    }else{
    vector<vector<char>> tempMaze = maze;
    while (!path.empty()) {
        pair<int, int> cell = path.top();
        path.pop();
        tempMaze[cell.first][cell.second] = '*'; // '*' represents the path
    }
    printMaze(tempMaze);
    }
}

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};
class MazePathFinding{
    public:
        vector<vector<char>> maze;
        unordered_map<pair<int, int>, bool, hash_pair> visited;

        // Defining some constants
        const string DOWN = "down";
        const string RIGHT = "right";
        const string UP = "up";
        const string LEFT = "left";
        const char START = 'S';
        const char END = 'E';
        const char BLOCK = 'O';

        MazePathFinding(vector<vector<char>> maze){
            this->maze = maze;
        }

        // finding the start index
        pair<int, int> find_start_point(vector<vector<char>>& maze) {
            pair<int, int> start_index; 
            for (int i = 0; i < maze.size(); i++) {
                for (int j = 0; j < maze[i].size(); j++) {
                    if (maze[i][j] == START) 
                        start_index = {i, j};
                }
            }
            return start_index;
        }

        // Utility Function to check of "O" or "E" ('O' means BLOCK and 'E' means END)
        bool check_for_O_or_E(vector<vector<char>> &maze, string next_move, pair<int, int> current_index){
            bool isPossible = false;
            if(next_move == DOWN){
                if(maze[current_index.first + 1][current_index.second] == BLOCK || maze[current_index.first + 1][current_index.second] == END)
                    isPossible = true;
            }
            else if(next_move == RIGHT){
                if(maze[current_index.first][current_index.second + 1] == BLOCK || maze[current_index.first][current_index.second + 1] == END)
                    isPossible = true;
            }
            else if(next_move == UP){
                if(maze[current_index.first - 1][current_index.second] == BLOCK || maze[current_index.first - 1][current_index.second] == END)
                    isPossible = true;
            }
            else if(next_move == LEFT){
                if(maze[current_index.first][current_index.second - 1] == BLOCK || maze[current_index.first][current_index.second - 1] == END)
                    isPossible = true;
            }
            return isPossible;
        }
        // Utility function to check in the hash table if a particular BLOCK is visited or not
        bool isVisited(pair<int, int> current_index, string next_move){
            bool isPossible = false;
            if(next_move == DOWN){
                if(visited[{current_index.first + 1, current_index.second}])
                    isPossible = true;
            }
            else if(next_move == RIGHT){
                if(visited[{current_index.first, current_index.second + 1}])
                    isPossible = true;
            }
            else if(next_move == UP){
                if(visited[{current_index.first - 1, current_index.second}])
                    isPossible = true;
            }
            else if(next_move == LEFT){
                if(visited[{current_index.first, current_index.second - 1}])
                    isPossible = true;
            }
            return isPossible;
        }

        // Utility function for above 2 utility functions (combining above two conditions)
        bool combined_check(vector<vector<char>> &maze, pair<int, int> current_index, string next_move){
            bool isPossible = false;
            if(next_move == DOWN){
                if(current_index.first + 1 < maze.size() && check_for_O_or_E(maze, DOWN, current_index) && !isVisited(current_index, DOWN))
                    isPossible = true;
            }
            else if(next_move == RIGHT){
                if(current_index.second + 1 < maze[0].size() && check_for_O_or_E(maze, RIGHT, current_index) && !isVisited(current_index, RIGHT))
                    isPossible = true;
            }
            else if(next_move == UP){
                if(current_index.first - 1 >= 0 && check_for_O_or_E(maze, UP, current_index) && !isVisited(current_index, UP))
                    isPossible = true;
            }
            else if(next_move == LEFT){
                if(current_index.second - 1 >= 0 && check_for_O_or_E(maze, LEFT, current_index) && !isVisited(current_index, LEFT))
                    isPossible = true;
            }
            return isPossible;
        }

        // Moving the current_index to the next Fesable Block
pair<int, int> next_fesable_move(vector<vector<char>>& maze, pair<int, int> current_index) {
    int rows = maze.size();
    int columns = maze[0].size();
    if (combined_check(maze, current_index, DOWN)) {
        current_index.first += 1;
    } else if (combined_check(maze, current_index, RIGHT)) {
        current_index.second += 1;
    } else if (combined_check(maze, current_index, UP)) {
        current_index.first -= 1;
    } else if (combined_check(maze, current_index, LEFT)) {
        current_index.second -= 1;
    }
    return current_index;
}


        // Utility Fuction to check if the next move is Fesable or not
        bool ifNextMovePossible(vector<vector<char>>& maze, pair<int, int> current_index) {
            bool is_fesable = false;
            
            if (combined_check(maze,current_index,  DOWN)) {
                    is_fesable = true;
            }
            else if (combined_check(maze,current_index,  RIGHT)) {
                
                    is_fesable = true;
            }
            else if (combined_check(maze,current_index,  UP)) {
                
                    is_fesable = true;
            }
            else if (combined_check(maze,current_index,  LEFT)) {
            
                    is_fesable = true;
            }
            else
                is_fesable = false;
            return is_fesable;
        }

        // Depth First Search Function combining all the above utility function 
        // to return a stack of possible moves
    stack<pair<int, int>> depth_first_search() {
    pair<int, int> start_index = find_start_point(maze);
    pair<int, int> current_index = start_index;
    stack<pair<int, int>> st;
    st.push(start_index);
    visited[start_index] = true;

    while (!st.empty() && maze[current_index.first][current_index.second] != 'E') {
        if (ifNextMovePossible(maze, current_index)) {
            current_index = next_fesable_move(maze, current_index);
            visited[current_index] = true;
            st.push(current_index);
        } else {
            st.pop();
            if (!st.empty()) {
                current_index = st.top();
            }
        }
    }

    // Check if the stack contains the end point, if not, return an empty stack
    if (st.empty() || maze[current_index.first][current_index.second] != 'E') {
        return stack<pair<int, int>>();
    }

    return st;
}

};





void clearConsole() {
    cout << "\033[H\033[J";  // ANSI escape code to clear the console screen
}

void printMazeWithVisualization(vector<vector<char>>& maze, stack<pair<int, int>>& path) {
    clearConsole();
    vector<vector<char>> tempMaze = maze;
    if(path.empty()){
        cout<<"Path not found!"<<endl;
    }else{
        
    

    while (!path.empty()) {
        pair<int, int> cell = path.top();
        path.pop();
        tempMaze[cell.first][cell.second] = '*';
        clearConsole();  // Clear the console before printing the updated maze
        printMaze(tempMaze);
        cout<<endl;
        this_thread::sleep_for(chrono::milliseconds(1000));  // Pause briefly for visualization
    }
    }

}














int main() {
    int size;
    char choice;

    cout << "Choose maze generation option: (R) Random or (M) Manual: ";
    cin >> choice;
    
    cout<<"'O' means empty space (BLOCK)"<<endl;
    cout<<"'X' means barrier"<<endl;
    cout<<"'S' means start point"<<endl;
    cout<<"'E' means end point"<<endl;


    if (choice == 'R' || choice == 'r') {
        cout << "Enter the size of the maze: ";
        cin >> size;
        vector<vector<char>> maze = generateMaze(size);
        MazePathFinding mpf(maze);
        cout << "Original Maze:\n";
        printMaze(maze);

        stack<pair<int, int>> path = mpf.depth_first_search();

        cout << "\nPath from start to end:\n";
        
         printMazeWithVisualization(maze, path);
       
    } else if (choice == 'M' || choice == 'm') {
        cout << "Enter the size of the maze: ";
        cin >> size;
        vector<vector<char>> maze(size, vector<char>(size, 'O'));

        cout << "Enter the maze:" << endl;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cin >> maze[i][j];
            }
        }

        MazePathFinding mpf(maze);


        stack<pair<int, int>> path = mpf.depth_first_search();

        cout << "\nPath from start to end:\n";
        printMazeWithVisualization(maze, path);
        
    } else {
       
        cout << "Invalid choice. Please choose (R) for Random or (M) for Manual maze generation.";
    }

    return 0;
}