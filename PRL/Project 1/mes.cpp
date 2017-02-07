/*
 * Algorithm: minimum extraction sort
 * Author: Anna Ostroukh <xostro00>
 */

#include <mpi.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>

 using namespace std;

 #define TAG 0
 #define EMPTY -1

/* Function reads file with random numbers and returns them
*/
deque <int> getInputValues () {
    char input[] = "numbers";                       //name of the file we should parse  
    int number = 0;                                 //value from file
    deque <int> inputValues;
    fstream fin;                                    
    fin.open(input, ios::in);

    while (fin.good()) {                            //read values from file and put it into the list
        number = fin.get();
        if (!fin.good())
            break;
        inputValues.push_front(number);
    }
    fin.close();

    return inputValues;
}

int main(int argc, char *argv[]) {
    int numprocs, lproc;				     //total number of processors, number of list processors
    int rank;                                //rank of processor
    
    int procVal;              	             //value of processor
    double start, end;                       //start time and end time for measurements
    
    MPI_Status stat;                
 	
    //MPI INIT
    MPI_Init(&argc,&argv);                              // MPI inicialization
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);           // total amount of processes running 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);               // id of a particular process

    //If we have just one processor with one value on input - read value and send to itself
    if (numprocs == 1) {
        deque <int> inputValues = getInputValues();

        cout<<inputValues[0]<<endl;

        MPI_Send(&inputValues[0], 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        MPI_Recv(&procVal, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);

        cout<<procVal<<endl;

        MPI_Finalize(); 
        return 0;
    }

/* Processor with rank 0 reads values from file and send them to other processors;
*  Processors with EMPTY value (nonleafs) get -1 value
*/
    lproc = (numprocs + 1) / 2;                         //count the number of leaf processors
   
    if(rank == 0) {	                                    // processor 0 gets values from file and send it to other processors
        deque <int> inputValues = getInputValues();
        int invar = 0;
        int n = 2;

        for(int i = 0; i < inputValues.size(); i++)     //outputs the sequence 
            cout<<inputValues[i]<<" ";	
            cout<<"\n";	

        while (n < inputValues.size())                  //increase the number of values if it isn't !=2^k
        n = n * 2;

        if (inputValues.size() < n) {
            while (inputValues.size() < n) {
                inputValues.push_front(EMPTY);          //add EMPTY values to the list
            }
        }					                   
        
        if (inputValues.size() < numprocs) {            //sends EMPTY value to non-leaf processors first (ranks from 0 to lproc-1)
            procVal = EMPTY;
            while (invar < lproc-1) {
                MPI_Send(&procVal, 1, MPI_INT, invar, TAG, MPI_COMM_WORLD);
                invar++;
            }
        }
    
        if (invar < lproc) {                             // sends values to leaf processors
           for (int i = 0; i < lproc; i++) {
                MPI_Send(&inputValues[i], 1, MPI_INT, invar, TAG, MPI_COMM_WORLD); 
                invar++;
            }
        }
    } 

    //All processors reads one value (random or EMPTY = -1)
    MPI_Recv(&procVal, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &stat);
    //cout<<"Processor "<<rank<<" my number is: "<<procVal<<endl;  
    
    int steps = 2 * lproc + log2 (lproc) -1;            //estimated steps of algorithm
                                 
    vector <int> outputNumbers;                         //output buffer with sorted values
    
    int lchildValue;                                    //value of left child
    int rchildValue;                                    //value of right child
    
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();                                //starting time of measurements

    for (int i = 0; i < steps; i++) { 
        if (rank >= lproc-1) {                          //all leaf processors send their values to parents (from 0 to 1 level of a binary tree)
            if (rank%2 == 0) {       //if it's right child
                MPI_Send(&procVal, 1, MPI_INT, rank/2-1, TAG, MPI_COMM_WORLD);   //right child send value to parent
                MPI_Recv(&procVal, 1, MPI_INT, rank/2-1, TAG, MPI_COMM_WORLD, &stat); //right child waits for answer from parent
            }
            else {                  //if it's left child
                MPI_Send(&procVal, 1, MPI_INT, (rank-1)/2, TAG, MPI_COMM_WORLD); //left child send value to parent
                MPI_Recv(&procVal, 1, MPI_INT, (rank-1)/2, TAG, MPI_COMM_WORLD, &stat); //left child waits for answer from parent
            }
        } 

        if (rank < lproc-1) {                             //for all nonleafs
            if (rank != 0) {                              //all nonleaf processors send their values to parents (from level 1 to the rest higher levels including the root)
                if (rank%2 == 0) {
                    MPI_Send(&procVal, 1, MPI_INT, rank/2-1, TAG, MPI_COMM_WORLD); 
                    MPI_Recv(&procVal, 1, MPI_INT, rank/2-1, TAG, MPI_COMM_WORLD, &stat);
                } 
                else {
                    MPI_Send(&procVal, 1, MPI_INT, (rank-1)/2, TAG, MPI_COMM_WORLD);
                    MPI_Recv(&procVal, 1, MPI_INT, (rank-1)/2, TAG, MPI_COMM_WORLD, &stat);
                }
            }

            if ((rank == 0) && (procVal != EMPTY)) {      //checks if root has value, then puts it in output buffer
                MPI_Recv(&lchildValue, 1, MPI_INT, rank*2+1, TAG, MPI_COMM_WORLD, &stat); //needed for processes synchronization
                MPI_Recv(&rchildValue, 1, MPI_INT, rank*2+2, TAG, MPI_COMM_WORLD, &stat); //needed for processes synchronization

                MPI_Send(&lchildValue, 1, MPI_INT, rank*2+1, TAG, MPI_COMM_WORLD); //needed for processes synchronization
                MPI_Send(&rchildValue, 1, MPI_INT, rank*2+2, TAG, MPI_COMM_WORLD); //needed for processes synchronization

                outputNumbers.push_back(procVal);
                procVal = EMPTY;
            } else if (procVal == EMPTY) {                //if nonleaf processor is empty - accept values of it's children
                MPI_Recv(&lchildValue, 1, MPI_INT, rank*2+1, TAG, MPI_COMM_WORLD, &stat); 
                MPI_Recv(&rchildValue, 1, MPI_INT, rank*2+2, TAG, MPI_COMM_WORLD, &stat);
                   
                //choosing numbers of children
                if ((lchildValue != EMPTY) && (rchildValue == EMPTY)) {
                    procVal = lchildValue;
                    lchildValue = EMPTY;
                } else if ((lchildValue == EMPTY) && (rchildValue != EMPTY)) {
                    procVal = rchildValue;
                    rchildValue = EMPTY;
                } else if (lchildValue < rchildValue) {
                    procVal = lchildValue;
                    lchildValue = EMPTY;
                } else if (lchildValue > rchildValue){
                    procVal = rchildValue;
                    rchildValue = EMPTY;
                } else if (lchildValue == rchildValue) {
                    procVal = rchildValue;
                    rchildValue = EMPTY;
                }
                
                //cout<<"Cycle "<<cycles<<" Rank "<<rank<<" Proc value "<<procVal<<" Sent to Left c "<<lchildValue<<" Sent to Right c "<<rchildValue<<endl;

                //sending values back to children
                MPI_Send(&lchildValue, 1, MPI_INT, rank*2+1, TAG, MPI_COMM_WORLD);
                MPI_Send(&rchildValue, 1, MPI_INT, rank*2+2, TAG, MPI_COMM_WORLD);
            } else if (procVal != EMPTY) { //needed for processes synchronization
                MPI_Recv(&lchildValue, 1, MPI_INT, rank*2+1, TAG, MPI_COMM_WORLD, &stat); 
                MPI_Recv(&rchildValue, 1, MPI_INT, rank*2+2, TAG, MPI_COMM_WORLD, &stat);

                MPI_Send(&lchildValue, 1, MPI_INT, rank*2+1, TAG, MPI_COMM_WORLD);
                MPI_Send(&rchildValue, 1, MPI_INT, rank*2+2, TAG, MPI_COMM_WORLD); 
            }
        } 

    } 

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();                      //ending time of measurements

  // Output the result
    if(rank == 0) {
        for(int i = 0; i < outputNumbers.size(); i++) {
            cout<<outputNumbers[i]<<endl;
        }
        //cout<<"Runtime "<<end-start<<endl;
    }

    MPI_Finalize(); 
    return 0;
}