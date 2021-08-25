#include "Topograph.h"

///**************Part 1********************************
///Constructor.  Open a ArcGIS ASCII Grid file
///read the header to assign height and width
///resize v and read elevation data
Topograph::Topograph(string fileName)
{
    //creating input file stream -ifs-
    ifstream ifs;

    //opening the fileName
    ifs.open(fileName);
    //checking if the fileName was opened succesfully
    if(!ifs) error("Could not open the file");

    //variable for taking trash headers which is no use, 
    string ncols, nrows, xllcorner, yllcorner, cellsize;

    //trashRead >> ncols then  extracting width >> 1321
    ifs >> ncols >> width;
    //trashRead >> ncrows then extracting height >> 481
    ifs >> nrows >> height;

    //ingoring white spaces after height
    ifs.ignore();
    //reading the next three lines, xllcorner, yllcorner and cellsize
    getline(ifs, xllcorner);
    getline(ifs, yllcorner);
    getline(ifs, cellsize);

    //resizing the vector
    v.resize(height);

    //variable for reading the integers after header
    int nextData = -1;

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            //reading the data one by one after the header and pushing it to the vector
            ifs >> nextData;
            v[i].push_back(nextData);
        }
    }    
}

void Topograph::findMin()
{
    int minValue;
    //creating a local variable and initialzing it with first element in vector
    minValue = v[0][0];

    //going thru height
    for(int i = 0; i < height; i++)
    {
        //going thru width
        for(int j = 0; j < width; j++)
        {
            //going thru vector and finding the minimum value
            if(v[i][j] < minValue)
                //setting the smallest value in the vector to minValue
                minValue = v[i][j];
        }
    }
    //then setting the smallest value in our vector to min - the private memeber in topograph
    min = minValue;
}

void Topograph::findMax()
{
    int maxValue;
    //creating a local variable and initialzing it with first element in vector
    maxValue = v[0][0];

    //going thru height
    for(int i = 0; i < height; i ++)
    {
        //going thru width
        for(int j = 0; j < width; j++)
        {
            //going thru vector and finding the largest value
            if(v[i][j] > maxValue)
                //setting the largest value in the vector to maxValue
                maxValue = v[i][j];
        }
    }
    //then setting the largest value in our vector to max - private member in topograph
    max = maxValue;
}

///scale n from [fromLow:fromHigh] to [toLow:toHigh]
///e.x -> n = 45 [30:50] [70:120] 
///-> {(45 - 30) / (50 - 30)} * {(120 - 70) + 70}
int Topograph::mapRange(int n, int fromLow, int fromHigh, int toLow, int toHigh)
{
    return (((double)n - fromLow) / (fromHigh - fromLow)) * (toHigh - toLow) + toLow;
}

///find min and max elevation values
///call mapRange to convert each elevation to grayscale
///set each bmp(i,j) to its grayscale value
void Topograph::drawMap(Bmp& bmp)
{
    //calling min and max to get the elevation values
    findMin();
    findMax();
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            //calling mapRange and setting it to our vector
            //v[i][j] is the 2D vector of rgb values
            //in mapRange v[i][j] is the tranfomable value
            //min and max is the range for the map
            //0 - 255 is going to set its resolution
            unsigned char vector  = (unsigned char)mapRange(v[i][j] , min , max , 0 , 255);
            
            //assigning each grayscale value to its elevation values of rbg
            //by object of type Bmp
            
            bmp(i,j).r = vector;
            bmp(i,j).g = vector;
            bmp(i,j).b = vector;
            
        }
    }

}

///**************Part 2********************************
///Draw one greedy path
///call moveForward to advance (i,j)
///set each bmp(i,j) to color
///returns the total sum of the elevation change
int Topograph::drawGreedyPath(Bmp& bmp, int startingRow, rgb color)
{
    int evelationValue, sum = 0, i = startingRow, j = 0;

    bmp(i, j) = color;
    //going thru path in width
    for (j = 0; j < (width - 1);)
    {
        //first saving my previous position
        evelationValue = v[i][j];

        //to advance (i,j) calling move forward
        moveForward(i,j);
        //setting bmp to its color
        bmp(i, j).r = color.r;
        bmp(i, j).g = color.g;
        bmp(i, j).b = color.b;
        
        //substracting previous postion from advanced position 
        // taking its absolute value and adding it to our running sum
        sum = sum + fabs(evelationValue - v[i][j]);
    }
    //return the running sum
    return sum;
}

///call drawGreedyPath for each startingRow, color red
///store the index of the path with lowest elevation change
///call drawGreedyPath for the lowest index, color blue
void Topograph::drawBestPath(Bmp& bmp)
{
    //setting up minValue with red and startingRow by calling the function
    int minValue = drawGreedyPath(bmp, 0, RED_PIXEL);
    int lowestIndex;

    for (int i = 0; i < height; i++)
    {
        //storing the lowest elevation
        int minIndex = drawGreedyPath(bmp, i,RED_PIXEL);
        
        //finding the lowest min value and lowest index
        if(minIndex < minValue)
        {
            minValue = minIndex;
            lowestIndex = i;
        }
    }
    //calling greedyPath with lowest index and blue color
    drawGreedyPath(bmp, lowestIndex, BLUE_PIXEL);
    cout << "Min value is     " << minValue << endl;
    cout << "The best path is " << drawGreedyPath(bmp, lowestIndex, BLUE_PIXEL) << endl;
}

///Advance (i,j) along its greedy path
///Choose the right side adjacent index with the lowest elevation change
///For a tie, mid has highest priority, then down, then up
///i + 1 is down, i - 1 is up
///j + 1 is forward
///Be careful if i is on the upper or lower edge
void Topograph::moveForward(int& i, int& j)
{
   int fwd_down, fwd, fwd_up;

    if(i == 0)
    {
        fwd_down = fabs(v[i][j] - v[i+1][j+1]);
        fwd = fabs(v[i][j] - v[i][j+1]);
        if(fwd <= fwd_down) 
        {
            j++;
        }

        i++;
        j++;
    }
    else if(i == (height - 1))
    {
        fwd = fabs(v[i][j] - v[i][j+1]);
        fwd_up = fabs(v[i][j] - v[i-1][j+1]);

        if(fwd <= fwd_up) 
        {
            j++;
        }

        i--;
        j++;
    
    }
    else
    {
        fwd_down = fabs(v[i][j] - v[i+1][j+1]);
        fwd      = fabs(v[i][j] - v[i][j+1]);
        fwd_up   = fabs(v[i][j] - v[i-1][j+1]);

        

        if (fwd_up < fwd && fwd_up < fwd_down) 
        {   
            i--; 
            j++; 
        }
        
        else if(fwd_down <= fwd_up && fwd_down < fwd) 
        {
            i++; 
            j++; 
        }
        
        else
        {
            j++; 
        }
        
    }
    

    
}












