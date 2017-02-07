/*
 * Algorithm: carry look ahead parallel binary adder
 * Author: Anna Ostroukh <xostro00>
 */
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include <sstream>

using namespace std;

 #define TAG 0
 #define X 1
 #define Y 2

/* Table of values for operation 'dot in circle' for computing D-scan
*/
char getPrefix(char row, char col) {
	if (row == 'S')
		return 'S';
	else if (row == 'G')
		return 'G';
	else
		return col;
}

int main(int argc, char *argv[]) {
    int numprocs, lproc, nlproc;				 	// total number of processors, number of leaf processor, non-leaf processor 
    int rank;                                		// rank of processor  
    char D, leftCh, rightCh, prefix, procVal, parentVal; // prefixes
    bool overflow = false;
    
    int valX, valY, C, Z;          			// bits in processor
    double start, end;                       // start time and end time for measurements
    
    MPI_Status stat;                
 	
    //MPI INIT
    MPI_Init(&argc,&argv);                              // MPI inicialization
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);           // total amount of processes running 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);               // id of a particular process

    lproc = (numprocs + 1) / 2;                         // count the number of leaf processors
    nlproc = numprocs - lproc;							// count the number of non-leaf processors

/* Processor with rank 0 reads values from file and send them to other processors;
*/
    if(rank == 0) {	                                    // processor 0 gets values from file and send it to other processors
        char filename[] = "numbers";                    // name of the file we should parse  
	    char value;
	    int n = 2;
	    int invar = 0;
	    string line; 
	    deque <int> x;
	    deque <int> y;
	    fstream fin;   

	    fin.open(filename, ios::in);

	   	// reading from file 'numbers'
	   	while (fin.good()) {                           
	        while(getline(fin, line)){
	        	istringstream ss(line);
	        	while(ss >> value) {
	        		if(!fin.eof()) 
	        			x.push_back(((int)value - 48));	 	//reading X numbers	
	        		else 
	        			y.push_back(((int)value - 48));		//reading Y numbers
	        	}	
	        }
	        if (!fin.good()) break; 
	    } 

	    fin.close();
    	
    	while (n < x.size() || n < y.size())    // checks a number of values (if it is =2^k)
        	n = n * 2;

        if (x.size() < n) {
            while (x.size() < n)
                x.push_front(0);          // add 0 values to X values to the left if needed
        }	

        if (y.size() < n) {
            while (y.size() < n)
                y.push_front(0);          // add 0 values to Y values to the left if needed
        }				
     
        if (x.size() < numprocs) {        //sends char value to non-leaf processors (actually just for sync)
            procVal = '*';
            while (invar < lproc-1) {
                MPI_Send(&procVal, 1, MPI_CHAR, invar, TAG, MPI_COMM_WORLD);
                invar++;
            }
        }
    
        if (invar < lproc) {                             // sends X, Y values to leaf processors
           for (int i = 0; i < lproc; i++) {
                MPI_Send(&x[i], 1, MPI_INT, invar, X, MPI_COMM_WORLD); 
                MPI_Send(&y[i], 1, MPI_INT, invar, Y, MPI_COMM_WORLD); 
               	invar++;
            }
        }
    } 

    if (rank >= nlproc) { //all leaf processors receive numbers
    	MPI_Recv(&valX, 1, MPI_INT, 0, X, MPI_COMM_WORLD, &stat);
    	MPI_Recv(&valY, 1, MPI_INT, 0, Y, MPI_COMM_WORLD, &stat);

    	// setting up initial D prefixes
    	if (valX == 1 && valY == 1)
    		D = 'G';
    	else if(valX == 0 && valY == 0)
    		D = 'S';
    	else 
    		D = 'P';  	
    } else if (rank < nlproc) {
    	MPI_Recv(&procVal, 1, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &stat);
    }

/* UPSWEEP
*/
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();                                //starting time of measurements

    if (rank >= nlproc ) {
	    if (rank%2 == 0) {								
		    procVal = D;
		    MPI_Send(&procVal, 1, MPI_CHAR, rank/2-1, TAG, MPI_COMM_WORLD); //if it's right child
	    } else {					
		    procVal = D;
		    MPI_Send(&procVal, 1, MPI_CHAR, (rank-1)/2, TAG, MPI_COMM_WORLD); //if it's left child
    	}
    }
    if ((rank < nlproc) && (rank != 0)) { 	
    	MPI_Recv(&rightCh, 1, MPI_CHAR, rank*2+2, TAG, MPI_COMM_WORLD, &stat);
    	MPI_Recv(&leftCh, 1, MPI_CHAR, rank*2+1, TAG, MPI_COMM_WORLD, &stat);

    	prefix = getPrefix(leftCh, rightCh);				// computing new prefix according to table of operation 'dot in circle'

    	if (rank%2 == 0) {       
            procVal = prefix;
            MPI_Send(&procVal, 1, MPI_CHAR, rank/2-1, TAG, MPI_COMM_WORLD);   //right child send prefix to parent
        }
        else {                  
          	procVal = prefix;
            MPI_Send(&procVal, 1, MPI_CHAR, (rank-1)/2, TAG, MPI_COMM_WORLD); //left child send prefix to parent
        }     

    } else if(rank == 0) {
    	MPI_Recv(&rightCh, 1, MPI_CHAR, rank*2+2, TAG, MPI_COMM_WORLD, &stat);
    	MPI_Recv(&leftCh, 1, MPI_CHAR, rank*2+1, TAG, MPI_COMM_WORLD, &stat);

        procVal = 'P'; // neutral element in root for DOWNSWEEP
    }		

/* DOWNSWEEP
*/
    if (rank < nlproc) {
     	if (rank == 0) {
     		MPI_Send(&procVal, 1, MPI_CHAR, rank*2+2, TAG, MPI_COMM_WORLD);
     	} else {
     		if (rank%2 == 0) {			// right child receives prefix from it's parent
     			MPI_Recv(&parentVal, 1, MPI_CHAR, rank/2-1, TAG, MPI_COMM_WORLD, &stat);

     			prefix = getPrefix(procVal, parentVal);
	     		procVal = parentVal;

	     		MPI_Send(&prefix, 1, MPI_CHAR, rank-1, TAG, MPI_COMM_WORLD); // right child sends it's prefix to neighbour

	     		MPI_Send(&procVal, 1, MPI_CHAR, rank*2+2, TAG, MPI_COMM_WORLD);
     		} else {
     			MPI_Recv(&prefix, 1, MPI_CHAR, rank+1, TAG, MPI_COMM_WORLD, &stat);
				procVal = prefix;

     			MPI_Send(&procVal, 1, MPI_CHAR, rank*2+2, TAG, MPI_COMM_WORLD);
     		}
     	}
    } 
    if (rank >= nlproc) {
     	if (rank%2 == 0){
     		MPI_Recv(&parentVal, 1, MPI_CHAR, rank/2-1, TAG, MPI_COMM_WORLD, &stat);

     		prefix = getPrefix(procVal, parentVal);
     		procVal = parentVal;

     		MPI_Send(&prefix, 1, MPI_CHAR, rank-1, TAG, MPI_COMM_WORLD);
     	} else {
     		MPI_Recv(&prefix, 1, MPI_CHAR, rank+1, TAG, MPI_COMM_WORLD, &stat);
     		procVal = prefix;
     	}

     	if(procVal == 'G')
    		C = 1;
    	else
    		C = 0;
    	
    	Z = valX + valY + C;		// computing Z

    	if (Z > 1)
    		overflow = true;		// identifying an MSB overflow

    	if (Z == 2)
    		Z = 0;
    	else 
    		Z = 1;

    	cout<<rank<<":"<<Z<<endl;

    	if (rank == nlproc && overflow){
    		cout<<"overflow"<<endl;
    	}
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();                      //ending time of measurements
    
    MPI_Finalize(); 
    return 0;
}
