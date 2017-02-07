BMS (Wireless and Mobile Networks)
-------------
**Task - Error Correction Code**

 Using C/C++ write two console programs (bms1A - coder, bms1B - decoder) which will implement appropriate Error Correction Code. Correction parameters has to be the following:
1. Maximum size of protected file: 175% of the original file
2. Corrupted data can be randomly located in all file - decoder has to be able to correct this file even when randomly corrupted X% symbols of the protected file.
3. Corruption can be burst - decoder has to correct the original file even when the corruption is X% of burst bits of the protected file 
    X is defined the following:
        17 ≤ X ≤ 21 => full amount of points for the project
        21 < X < 25 => full amount of points for the project + 1 additional point for the exam
               X ≥ 25 => full amount of points for the project + 3 additional points for the exam

Other notes:
- Consider that input file will be 1 MB minimum
- It's prohibited to use Shifra library
- Implementation algorithm - any
- Maximum running time during coding/decoding 1MB file is 10 seconds

**Solution:** 
For this project is used Reed-Solomon correction code algorithm and rscode library 