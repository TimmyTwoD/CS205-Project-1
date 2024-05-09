#include <bits/stdc++.h>

using namespace std;

/* key:
   a = 97
   n = 110
   g = 103
   e = 101
   l = 108
   i = 105
   c = 99
   blank = 48
*/
vector<vector<char>> failed;  //placeholder for failed state

vector<vector<char>> goalState = {{97, 110, 103},  //already solved angelica puzzle
                                  {101, 108, 105},
                                  {99, 97, 48}};
vector<vector<char>> hard =      {{48, 99, 110}, //depth 24 puzzle similar to 8 puzzle depth  
                                  {101, 105, 97},
                                  {103, 108, 97}};
vector<vector<char>> medium =    {{97, 103, 105}, //depth 12
                                  {108, 48, 99},
                                  {101, 97, 110}};
vector<vector<char>> easy =      {{97, 110, 103}, //depth 4 
                                  {108, 48, 105},
                                  {101, 99, 97}};
                                  
struct Node
{
    vector<vector<char>> puzzle; //current config of the puzzle
    int PositionOfZeroRow;
    int PositionOfZeroColumn;
    vector<string> solutionPath; // holds the set of moves needed to reach goal state
    int heuristic; //the heuristic score
};

struct Heuristic
{
    bool operator()(const Node &a, const Node &b)
    {
        return ((a.heuristic + a.solutionPath.size()) > (b.heuristic+ b.solutionPath.size())); 
    }
};

void createPuzzle(vector<vector<char>>& puzz) 
{
    char input2, input3, input4; 
    cout << "Enter the configuration of the puzzle, enter a \"0\" for the blank, and enter only valid puzzles.\n";
    cout << "Enter 3 lowercase letters at a time separating them with a space.\n";
    cout << "First Row:  ";
    cin >> input2 >> input3 >> input4;

    puzz[0][0] = input2;
    puzz[0][1] = input3;
    puzz[0][2] = input4;

    cout << "Second Row: ";
    cin >> input2 >> input3 >> input4;

    puzz[1][0] = input2;
    puzz[1][1] = input3;
    puzz[1][2] = input4;

    cout << "Third Row:  ";
    cin >> input2 >> input3 >> input4;

    puzz[2][0] = input2;
    puzz[2][1] = input3;
    puzz[2][2] = input4;
}

void outputPuzzle(vector<vector<char>>& puzz)
{
    for(int i = 0; i < puzz.size(); i++)
    {
        cout<<endl;
        for(int j = 0; j < puzz[i].size(); j++)
        {
            cout << puzz[i][j] << " ";
        }
    }
}

int getHeuristic(int whichHeur, vector<vector<char>> &temp)
{
    int heuristic = 0;

    if (whichHeur == 1) //uniform cost
    {
        return 0;
    }

    if (whichHeur == 2) //number of misplaced tiles
    {
        for(int i = 0; i < temp.size(); i++)
        {
            for(int j = 0; j < temp.size(); j++)
            {
                if (temp[i][j] != goalState[i][j])
                { 
                    heuristic++;
                }
            }
        }
    }

    if (whichHeur == 3)//Manhattan Dist
    {
        for(int i = 0; i < temp.size(); i++)
        {
            for(int j = 0; j < temp.size(); j++)
            {
                if (temp[i][j] != goalState[i][j])
                { 
                    if (temp[i][j] != 48)//gets the distance of each misplaced tile to the goal state and adds it to h(n), since there is two a's take the shorter distance
                    {
                        if (temp[i][j] == 97) //a
                        {
                            heuristic += min(i+j, abs(i-2) + abs(j-1));
                        }
                        if (temp[i][j] == 110) //n
                        {
                            heuristic += i + abs(j-1);
                        }
                        if (temp[i][j] == 103) //g
                        {
                            heuristic += i + abs(j-2);
                        }
                        if (temp[i][j] == 101) //e
                        {
                            heuristic += abs(i-1) + j;
                        }
                        if (temp[i][j] == 108) //l
                        {
                            heuristic += abs(i-1) + abs(j-1);
                        }
                        if (temp[i][j] == 105) //i
                        {
                            heuristic += abs(i-1) + abs(j-2);
                        }
                        if (temp[i][j] == 99) //c
                        {
                            heuristic += abs(i-2) + j;
                        }
                    }
                }
            }
        }
    }

    return heuristic;
}

Node BlankSpot(vector<vector<char>> &child) // finds the position of the "0" and updates the nodes with it
{
    Node temp;
    temp.puzzle = child; 
    
    for(int i = 0; i < temp.puzzle.size(); i++){
        for(int j = 0; j < temp.puzzle.size(); j++){
            if(temp.puzzle[i][j] == 48){
                temp.PositionOfZeroRow = i;
                temp.PositionOfZeroColumn = j;
                break;
            }
        }
    }
    
    return temp;
}

Node search(vector<vector<char>> problem, int whichHeuristic)//search function
{   
    clock_t tStart = clock();
    priority_queue<Node, vector<Node>, Heuristic> priorQ;
    set<vector<vector<char>>> trackDupes;

    Node currstate;
    Node temp;

    Node fail;
    fail.puzzle = failed; 

    int maxQueueSize = priorQ.size();
    int numNodesExpanded = 0;
    int tempNum = 0;

    temp = (BlankSpot(problem));
    temp.heuristic = getHeuristic(whichHeuristic, problem);
    priorQ.push(temp);
    trackDupes.insert(problem);

    while (!priorQ.empty()) //queue
    {
        tempNum = priorQ.size();
        maxQueueSize = max(maxQueueSize, tempNum);

        currstate = priorQ.top();
        trackDupes.insert(currstate.puzzle); 

        priorQ.pop();
        numNodesExpanded++;

        if (currstate.puzzle == goalState)
        {
            cout << "\n\nGoal state!\n\n";
            cout << "Solution depth was " << currstate.solutionPath.size() << endl;
            cout << "Number of nodes expanded: " << numNodesExpanded - 1 << endl;
            cout << "Max queue size: " << maxQueueSize << endl;
            printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
            return currstate;
        }

        cout << "\nThe best state to expand with a g(n) = " << currstate.solutionPath.size() << " and h(n) = " << currstate.heuristic << " is ";
        
        outputPuzzle(currstate.puzzle);
        
        if (currstate.PositionOfZeroColumn != 0) 
        {   
            temp = currstate;

            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn-1]; //swap left
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn-1] = 48; 
            temp.PositionOfZeroColumn--;

            tempNum = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (tempNum != trackDupes.size()) //if not duped, ready to push and update node's attributes
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Left");
                priorQ.push(temp); //enqueue new node
            }

        }

        if (currstate.PositionOfZeroColumn < currstate.puzzle.size()-1) 
        {
            temp = currstate;
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn+1]; //swap right
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn+1] = 48;
            temp.PositionOfZeroColumn++;

            tempNum = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (tempNum != trackDupes.size())
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Right");
                priorQ.push(temp);
            }

        }

        if (currstate.PositionOfZeroRow != 0) 
        {
            temp = currstate;
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow-1][temp.PositionOfZeroColumn]; //swap up
            temp.puzzle[temp.PositionOfZeroRow-1][temp.PositionOfZeroColumn] = 48; 
            temp.PositionOfZeroRow--;

            tempNum = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (tempNum != trackDupes.size())
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Up");
                priorQ.push(temp);
            }

        }

        if (currstate.PositionOfZeroRow < currstate.puzzle[currstate.PositionOfZeroRow].size()-1) 
        {   
            temp = currstate;
            temp.puzzle[temp.PositionOfZeroRow][temp.PositionOfZeroColumn] = temp.puzzle[temp.PositionOfZeroRow+1][temp.PositionOfZeroColumn]; //swap down
            temp.puzzle[temp.PositionOfZeroRow+1][temp.PositionOfZeroColumn] = 48; 
            temp.PositionOfZeroRow++;

            tempNum = trackDupes.size();
            trackDupes.insert(temp.puzzle);
            if (tempNum != trackDupes.size())
            {
                temp.heuristic = getHeuristic(whichHeuristic, temp.puzzle);
                temp.solutionPath.push_back("Down");
                priorQ.push(temp);
            }

        }

    }

    return fail; // reached here meaning all possible moves done but no goalstate
}

int main()
{
    int input;
    int heuristic;
    Node test;
    vector<vector<char>> puzz = goalState; //give it a temporary state that we will override anyways

    cout << "\nCS205 David Liu's Angelica Puzzle Solver\n";
    cout <<"----------------------------------------------------------------------\n";
    cout << "Enter \"1\" for a default puzzle or \"2\" to create a custom puzzle\n\n";

    cin >> input;

    if (input == 1)
    {
        cout<<"Choose a sample puzzle by entering the number:\n";
        cout<<"1. Easy\n";
        cout<<"2. Medium\n";
        cout<<"3. Hard\n";
        cout<<"4. Solved\n\n";

        cin >> input;

        if (input == 1)
        {
            puzz = easy;
        }
        else if (input == 2)
        {
            puzz = medium;
        }
        else if (input == 3)
        {
            puzz = hard;
        }
        else if (input == 4)
        {
            puzz = goalState;
        }
                    
    }
    else if (input == 2)
    {
        createPuzzle(puzz);
    }
    
    cout<<"\nCurrent Puzzle Configuration:";

    outputPuzzle(puzz);

    cout << "\n\nSelect the number of the algorithm you wish to search with: \n";
    cout << "1. Uniform Cost Search\n" << "2. Misplaced Tile Heuristic\n" << "3. Manhattan Distance Heuristic\n\n";

    cin >> heuristic;

    test = search(puzz, heuristic);

    if (test.puzzle == failed)
    {
        cout << "No Solution" << endl;
    }

    return 0;
}