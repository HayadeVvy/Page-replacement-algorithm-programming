/********************************************************************************
 * File: Main.cpp
 * Author: Anthony(Jinu) Lee
 * Modification history:
 *  1) modified by Anthony(Jinu) Lee
 *      a. Modified date:11-17-2021
 *      b. Modified description: Added Clock algorithm and main function to the program.
 *  2) modified by Anthony(Jinu) Lee
 *      a. Modified date:11-18-2021
 *      b. Modified description: Added LRU and FIFO algorithm to the program.
 *  3) modified by Anthony(Jinu) Lee
 *      a. Modified date:11-19-2021
 *      b. Modified description: Fixed issue with clock index increment.
 * Procedures:
 * Main: It initialize the experiment and traces of the pages, then it send data to 
 * three different algorithms to calculate page fault error and display on the screen.
 * lru: Uses Least recently used algorithm to calculate page fault errors. 
 * fifo: Uses first in first out algorithm to calculate page fault errors.
 * clock: Uses clock replacement algorithm to calculate page fault errors.
 * *****************************************************************************/
#include <stdio.h>
#include <random>
#include <queue>
#include <bits/stdc++.h>
#define limit 1000                                    //Defines a limit for number of experiment and pages
std::default_random_engine gene;                      //Defines a random number generator
std::normal_distribution<double> gen(10,2);           //Defines a normal distribution function with mean of 10 and variance of 2
int data_arr[limit];                                  //Defines the page array
int LRUResults[20];                                   //Defines each page fault arrays for LRU,FIFO and clock algorithm.
int FIFOResults[20];
int ClockResults[20];
using namespace std;
/***********************************************************
 * struct LRU_NODE
 * Author: Anthony(Jinu) Lee
 * Date: 11-18-2021
 * Description: This contains the page number and its waiting time inside of frame.
 * 
 * variable: 
 * number: The page number
 * Waiting: Its waiting time inside of the frame from last usage.
 * *********************************************************/
struct LRU_NODE                                       
{
    int number;
    int Waiting;
};
/***********************************************************
 * int LRU(int a, int b[])
 * Author: Anthony (Jinu) Lee
 * Date: 11-18-2021
 * Description: This function takes frame limit 'a', and page array 'b[]',
 *  then it uses leat recently used algorithm to calculate the page fault with given frame limit
 * Parameters:
 *  int a: Frame limit
 *  int b[]: Page array.
 * ********************************************************/
int LRU(int a, int b[])                      
{
    LRU_NODE target_Arr[a];          //This is a array for LRU struct which contains the waiting time of each page inside the frame.
    int counter =0;                  //Counter variable to control the limit of frame
    set <int> check;                 //Set variable to search the element inside the frame.
    int P_fault =0;                  //Page fault variable.
    for (int k=0; k < limit; k++)                    //Loops over all the pages.
    {
        if(check.find(b[k])==check.end())           //Check if the element is not found inside the frame.
        {
            if(check.size()< a && counter < a)      //Check if the frame can allow another page inside. 
            {
                check.insert(b[k]);                 //Insert page inside the frame.
                P_fault++;                          //increment page fault
                target_Arr[counter].number = b[k];  //Update the struct with the new information and reset the waiting time.
                target_Arr[counter].Waiting = 0;
            }
            else                                    //Perform function below when there is no space inside the frame.
            {
                int Max_int = 0;                    //Temporary variable for holding maximum waiting time.
                int position;                       //Temporary variable for getting the index of struct array.
                for (int q=0; q < a; q++)           //Loops over struct array to find the node with maximum waiting time.
                {
                    if(target_Arr[q].Waiting > Max_int)    //If found, retrieve the max waiting time and its position in array
                    {
                        Max_int = target_Arr[q].Waiting;
                        position = q;
                    }
                }
                int Change_target = target_Arr[position].number; //Replace the node with maximum waiting time with the new element.
                P_fault++;                                       //increment page fault
                check.erase(Change_target);                      //Replace the node with maximum waiting time with the new element.
                check.insert(b[k]);
                target_Arr[position].number = b[k];
                target_Arr[position].Waiting = 0;
            }
        }
        else                                                    //Do this if the element is present inside the array.
        {
            int found_val = b[k];                               //temporary variable to hold the value that has been found
            for (int n=0; n < a; n++)                           //Loop over the struct array to locate the element that has been found.
            {
                if(target_Arr[n].number == b[k])                //Reset the waiting time for that struct node.
                {
                    target_Arr[n].Waiting = 0;
                    break;
                }
            }
        }
        for (int control=0; control <a; control++)              //Gradually increment the waiting time of all node during execution.
        {
            target_Arr[control].Waiting++;
        }
    }
    return P_fault;                                             //Return the total page fault.
}
/***********************************************************
 * int FIFO(int a, int b[])
 * Author: Anthony (Jinu) Lee
 * Date: 11-18-2021
 * Description: This function takes frame limit 'a', and page array 'b[]',
 *  then it uses "first in first out" algorithm to calculate the page fault with given frame limit
 * Parameters:
 *  int a: Frame limit
 *  int b[]: Page array.
 * ********************************************************/
int FIFO(int a, int b[])
{
    int size_queue = a;                     //Temporary variable to indicate the max size of the frame.
    queue<int> test;                        //Queue that will hold the pages.
    set <int> check;                        //initiallize the set to perform search inside the frame.
    int P_fault =0;                         //Page fault variable.
    for (int k =0; k < limit; k++)          //Loop over all pages. 
    {
       if(check.find(b[k])== check.end())   //check if element is not found.
        {
             if (test.size() < size_queue)  //Check if frame can allow another element into frame.
             {
                 check.insert(b[k]);        //Insert element inside the frame and increment page fault.
                 P_fault++;
                 test.push(b[k]);
             }  
             else                           //Perform this when frame is at max capacity
             {
                 int temp = test.front();   //Get first in variable from the queue and pop it from the queue
                 test.pop();
                 check.erase(temp);         //Erase popped variable from the set.
                 check.insert(b[k]);        //Insert new variable into set and queue.
                 test.push(b[k]);
                 P_fault++;                 //increment page fault.

             }
        }
    }
    return P_fault;                         //return total page fault.
}
/***********************************************************
 * int Clock(int a, int b[])
 * Author: Anthony (Jinu) Lee
 * Date: 11-18-2021
 * Description: This function takes frame limit 'a', and page array 'b[]',
 *  then it uses leat recently used algorithm to calculate the page fault with given frame limit
 * Parameters:
 *  int a: Frame limit
 *  int b[]: Page array.
 * ********************************************************/
int Clock(int a, int b[])
{
    int Cir_array[a];           //Declares circular array with its max size.
    int use_arr[a];             //Declares another array indicating use bits.
    int P_fault =0;             //Variable for page fault.
    int index = 0;              //Variable for index for circular array
    for(int k = 0; k < a; k++)  //Loop over use bit array to set every use bit to 0.
    {
        use_arr[k] = 0;
    }
    for(int i= 0; i < limit; i++)   //Loop over all the pages.
    {
        bool found = false;         //initially set found as false.
        int temp = b[i];            //Temporary variable for current page.
       
        for(int j=0; j < a; j++)    //loop over circular array.
        {
            if (Cir_array[j]==temp) //check if the element is present inside the circular array.
            {
                found = true;
                index = j;
                break;
            }
        }
        if(found)                   //if found, set its use bit to 1 and continue execution.
        {
            use_arr[index] = 1;
            continue;
        }
        else                        //Do this when element is not present.
        {
            while (true)
            {
                if (use_arr[index] == 0)    //check if the user bit array of current index is 0
                {
                    Cir_array[index] = temp;    //Replace the current element with the incoming page.
                    use_arr[index] = 1;         //Set use bit to 1.
                    P_fault++;                  //increment page fault.
                    break;                      //break the roof.
                }
                else                            //Else loop around circular array to find page with use bit 0
                {
                    use_arr[index] = 0;         //set the current index's use bit to 0.
                    index = (index+1) % a;      //increment the index.
                }
            }
        }
    }
    return P_fault;                         //Return total page fault.
}


int main(int argc, char *argv[])                //Driver code for testing algorithms.
{
    for(int i = 0; i < limit; i++)              //intiallize experiment with given limit
    {
        for(int j =0; j < limit; j++)           //Initialize page array with given limit
        {
            double number = gen(gene);          //Generate random number with mean of 10 and variance of 2.
            data_arr[j] = (10*((int)(j/100))) + number; //Assign the random page number into page array.
        }
        for(int wss = 4; wss <= 20; wss++)      //Perform experiment with 3 different algorithm and frame with size of 4 to 20.
        {
            LRUResults[wss] += LRU(wss,data_arr);
            FIFOResults[wss] += FIFO(wss,data_arr);
            ClockResults[wss] += Clock(wss,data_arr);
        }
    }
    for (int wss =4; wss <= 20; wss++)          //Print out result for each algorithm with varying frame size.
    {
        cout << "LRU for " << wss << " was " <<LRUResults[wss] << endl;
    }
    for (int wss =4; wss <= 20; wss++)
    {
        cout << "FIFO for " << wss << " was " <<FIFOResults[wss] << endl;
    }
    for (int wss =4; wss <= 20; wss++)
    {
        cout << "Clock for " << wss << " was " <<ClockResults[wss] << endl;
    }
}