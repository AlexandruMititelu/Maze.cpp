#include <iostream>
#include <vector>
#include <string>
#include <utility> 
#include <cstdlib>
#include <ctime>

class Cell
{   
    public:
        bool isAvailable = true;
        bool isAvailablePathFinding = true;
        bool hasNorth = true;
        bool hasWest  = true;
        bool hasEast  = true;
        bool hasSouth = true;
        bool isOnTheExitPath = false;
        
        Cell() = default;
};

class Maze
{
    public:
        Maze() = default;
        bool isInBounds(int rows, int columns);
        bool isAvailable(int rows, int columns);
        bool isAvailablePathFinding(int rows, int columns, int position);
        
        void getInput(int argc, char **argv);
        void startMaze(int argc, char **argv);
        void generateMazeRecursively(int x, int y, int wallsTornDown);
        bool findPath(int x, int y);
        void markPath();
        void printTheMaze();

    private:
        int _rows, _columns;
        std::vector<std::vector<Cell>> mazeMatrix;
        int mazeSize;
        std::vector<int> endPos;

        int EAST  = 0;
        int SOUTH = 1;
        int NORTH = 2;
        int WEST  = 3;
        
        std::vector<std::pair<int, int>> neighborIndexes = 
        {
            std::make_pair(0, 1),   std::make_pair(1, 0), 
            std::make_pair(- 1, 0), std::make_pair(0, - 1)
        };
        std::vector<std::pair<int, int>> pathToExit;
        std::vector<std::pair<int, int>> availableNeighbourCells;
};

bool Maze::isInBounds(int rows, int columns)
{
    return rows < _rows && columns < _columns && rows >= 0 && columns >= 0;
}

bool Maze::isAvailable(int rows, int columns)  
 {
    if(isInBounds(rows, columns))
    {
        return mazeMatrix.at(rows).at(columns).isAvailable;
    }
    return false;
 }

bool Maze::isAvailablePathFinding(int rows, int columns, int position)  
 {
    if(isInBounds(rows, columns))
    {
        if(mazeMatrix.at(rows).at(columns).isAvailablePathFinding)
        {
            if(position == EAST)
            {
                return !mazeMatrix.at(rows).at(columns).hasWest;
            }
            if(position == WEST)
            {
                return !mazeMatrix.at(rows).at(columns).hasEast;
            }
            if(position == NORTH)
            {
                return !mazeMatrix.at(rows).at(columns).hasSouth;
            }
            if(position == SOUTH)
            {
                return !mazeMatrix.at(rows).at(columns).hasNorth;
            }
            return false;
        }               
    }
    return false;
 }
void Maze::getInput(int argc, char **argv)
{   
    if( argc == 3 || argc == 4)
    {
        _rows = std::stoi(argv[1]);
        _columns = std::stoi(argv[2]);

        if(argc == 4)   
        { 
            srand(std::stoi(argv[3]));
        }
        else
        {
            srand(time(NULL));
        }
    }
    else
    {
        throw std::exception();
    }
}

void Maze::startMaze(int argc, char **argv)
{
    getInput(argc, argv);

    endPos   = {_rows - 1, _columns - 1};
    mazeSize = _rows * _columns;
	mazeMatrix.resize(_rows, std::vector<Cell>(_columns));

	generateMazeRecursively(0, 0, 0);
    
	 if(findPath(0, 0))
     {
	     markPath();
	     printTheMaze();
      }
}

void Maze::generateMazeRecursively(int x, int y, int wallsTornDown)
{
    int position, xNext, yNext;
    
    std::vector<std::pair<int, int>> availableNeighbourCells;

    if(wallsTornDown < mazeSize - 1)
    {   
        for (int i = 0; i < neighborIndexes.size(); i++) 
        {
            if (isAvailable(x + neighborIndexes.at(i).first, y + neighborIndexes.at(i).second)) 
            {
                availableNeighbourCells.push_back(std::make_pair(neighborIndexes.at(i).first, neighborIndexes.at(i).second));
            }
        }
    
        if (availableNeighbourCells.size() == 0 && wallsTornDown != mazeSize - 1) 
        {
            return;
        }
        
        position = rand() % availableNeighbourCells.size();
        xNext = x + availableNeighbourCells.at(position).first;
        yNext = y + availableNeighbourCells.at(position).second;

        if(isAvailable(xNext, yNext))
        {   
            mazeMatrix.at(xNext).at(yNext).isAvailable = false;

            if(availableNeighbourCells.at(position).first  == 0
            && availableNeighbourCells.at(position).second == 1)
            {                   
                mazeMatrix.at(x).at(y).hasEast = false;
                mazeMatrix.at(xNext).at(yNext).hasWest = false;
            }
            if(availableNeighbourCells.at(position).first  == 1
            && availableNeighbourCells.at(position).second == 0)
            { 
                mazeMatrix.at(x).at(y).hasSouth = false;
                mazeMatrix.at(xNext).at(yNext).hasNorth = false;
            }
            if(availableNeighbourCells.at(position).first  == - 1
            && availableNeighbourCells.at(position).second == 0)
            {
                mazeMatrix.at(x).at(y).hasNorth = false;
                mazeMatrix.at(xNext).at(yNext).hasSouth = false;
            }
            if(availableNeighbourCells.at(position).first  == 0
            && availableNeighbourCells.at(position).second == - 1)     
            {        
                mazeMatrix.at(x).at(y).hasWest = false;
                mazeMatrix.at(xNext).at(yNext).hasEast = false;
            }

            wallsTornDown++;
            generateMazeRecursively(xNext, yNext, wallsTornDown);
        }
            generateMazeRecursively(x, y, wallsTornDown);
    }
    return;
}

bool Maze::findPath(int x, int y)
{
    int xNext, yNext;

    mazeMatrix.at(x).at(y).isAvailablePathFinding = false;

    if(x == endPos.at(0) && y == endPos.at(1))
    {
        return true;
    }

    if(x == 0 && y == 0)
    {
        pathToExit.push_back(std::make_pair(0, 0));
    }
    
    for(int position = 0; position < neighborIndexes.size() ; position++)
    {
        xNext = x + neighborIndexes.at(position).first;
        yNext = y + neighborIndexes.at(position).second;

        if(isAvailablePathFinding(xNext, yNext, position))
        {   
            pathToExit.push_back(std::make_pair(xNext, yNext));

            mazeMatrix.at(xNext).at(yNext).isAvailablePathFinding = false;
            mazeMatrix.at(xNext).at(yNext).isOnTheExitPath = true;

            if(findPath(xNext, yNext))
            {
                return true;
            }
            else
            {   
                mazeMatrix.at(xNext).at(yNext).isOnTheExitPath = false;

                pathToExit.pop_back();
                xNext = pathToExit.back().first;
                yNext = pathToExit.back().second;
            }
        }
    }
    mazeMatrix.at(xNext).at(yNext).isAvailablePathFinding = true;

    return false;
}

void Maze::markPath()
{   
    int x, y;
    
    for(int i = 0; i < pathToExit.size(); i++)
    {
        x = pathToExit.at(i).first;
        y = pathToExit.at(i).second;

        mazeMatrix.at(x).at(y).isOnTheExitPath = true;
    }
}

void Maze::printTheMaze()
{   
  for(int i = 0; i < _rows; i++)
  {
        std::cout << "+";
        for (int j = 0; j < _columns; j++)
        {
            if (mazeMatrix.at(i).at(j).hasNorth) 
            {
                std::cout << "---+";
            }
            else 
            {
                std::cout << "   +";
            }
        }

        std::cout << std::endl;

        std::cout << "|";
        for(int j = 0; j < _columns; j++)
        {
            if (mazeMatrix.at(i).at(j).hasEast)
            {
                if (!mazeMatrix.at(i).at(j).isOnTheExitPath) 
                {
                    std::cout << "   |";
                }
                else 
                {
                    std::cout << " . |";
                }
            }
            else 
            {
                if (!mazeMatrix.at(i).at(j).isOnTheExitPath) 
                {
                    std::cout << "    ";
                }
                else
                {
                    std::cout << " .  ";
                }
            }
        }
        std::cout << std::endl;
    }

    std::cout << "+";
    for (int j = 0; j < _columns; j++)
    {
        std::cout << "---+";
    } 

    std::cout << std::endl;
}

int main(int argc, char **argv)
{   
    try
    {
        Maze maze;
        maze.startMaze(argc, argv);
    }
    catch(std::exception & e)
    {
        std::cerr << "Invalid decision, invalid CLT command."
                  << std::endl;
    }           
    
    return 0;
}