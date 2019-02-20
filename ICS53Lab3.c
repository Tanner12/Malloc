#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

//GLOBAL VARIABLES
int blockNum = 0;

int  allocate(int bytes, unsigned char* myHeap) {
	int successful = 0;
	if(bytes >= 127) {
		printf("Invalid byte entered!\n");
		return successful;
	}
	else if (blockNum == 256) {
		printf("Heap is full!\n");
		return successful;
	}

	unsigned char index = 0;
	for (;index < 128; index = index + (((myHeap[index+1]) & -2) >> 1) + 1) {
		//"and" the size with 1 and shift right 1 to get the original size back
		// if the orignal size is greater than the needed byte than do splitting
		if (((((myHeap[index+1]) & -2) >> 1) - (((unsigned char)bytes) + 2)) == 1 && (myHeap[index+1] & 1) == 0) {
			++blockNum;
			successful = 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 3;
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			myHeap[index+1] = blockSize;

			break;
		}
	/*	if ((((((myHeap[index+1]) & -2) >> 1)+1) - (((unsigned char)bytes) + 2)) == 1 && (myHeap[index+1] & 1) == 0) {
			printf("HERE\n");
			++blockNum;
			successful = 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 3;
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			myHeap[index+1] = blockSize;

			break;
		}*/


		else if (((((myHeap[index+1]) & -2) >> 1) - (((unsigned char)bytes) + 2)) == 2 && (myHeap[index+1] & 1) == 0) {
			++blockNum;
			successful = 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 4; 		
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			myHeap[index+1] = blockSize;

			break;
		}

		/*else if ((((((myHeap[index+1]) & -2) >> 1)+1) - (((unsigned char)bytes) + 2)) == 2 && (myHeap[index+1] & 1) == 0) {
			++blockNum;
			successful = 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 4; 		
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			myHeap[index+1] = blockSize;

			break;
		}*/

		else if ((((myHeap[index+1]) & -2) >> 1) >( (unsigned char)bytes + 2) && (myHeap[index+1] & 1) == 0) {
			++blockNum;
			successful = 1;
			unsigned char oldBlockSize = ((myHeap[index+1]) & -2) >> 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 2; //check if have to minus 1
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			//stores the new size into index + 1 and mark the last bit as "allocated" = 1
			myHeap[index+1] = blockSize;
			//have to set the size of the "ending" byte (in the 2nd byte)
			unsigned char getSize = ((blockSize & -2) >> 1);
			unsigned char calcIndexNewSize = ((oldBlockSize - getSize) << 1);
			myHeap[index + ((myHeap[index + 1] & -2) >> 1) + 2] = calcIndexNewSize;
			break;
		}
/*		else if (((((myHeap[index+1]) & -2) >> 1)+1) >( (unsigned char)bytes + 2) && (myHeap[index+1] & 1) == 0) {
			++blockNum;
			successful = 1;
			unsigned char oldBlockSize = ((myHeap[index+1]) & -2) >> 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 2; //check if have to minus 1
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			//stores the new size into index + 1 and mark the last bit as "allocated" = 1
			myHeap[index+1] = blockSize;
			//have to set the size of the "ending" byte (in the 2nd byte)
			unsigned char getSize = ((blockSize & -2) >> 1);
			unsigned char calcIndexNewSize = ((oldBlockSize - getSize + 1) << 1);
			myHeap[index + ((myHeap[index + 1] & -2) >> 1) + 2] = calcIndexNewSize;
			break;
		}*/

		else if ((((myHeap[index+1]) & -2) >> 1) == ((unsigned char)bytes +2) && (myHeap[index+1] & 1) == 0) {
			++blockNum;
			successful = 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 2; //check if have to minus 1
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			//stores the new size into index + 1 and mark the last bit as "allocated" = 1
			myHeap[index+1] = blockSize;
			break;
		}
/*		else if (((((myHeap[index+1]) & -2) >> 1)+1) == ((unsigned char)bytes +2) && (myHeap[index+1] & 1) == 0) {

			++blockNum;
			successful = 1;

			//change the header (first byte) to the incremented block number
			myHeap[index] = blockNum;

			unsigned char blockSize = (unsigned char)bytes + 2; //check if have to minus 1
			blockSize = blockSize << 1;
			blockSize = blockSize | 1;

			//stores the new size into index + 1 and mark the last bit as "allocated" = 1
			myHeap[index+1] = blockSize;
			break;
		}*/

	}

	if (successful == 0) {
		printf("Heap cannot allocate that much space!\n");
		return;
	}
	printf("%d\n", blockNum);
}

void freeBlock(int prevBlockNum, unsigned char* myHeap) {
	if (prevBlockNum != blockNum) {
		printf("This is not the previous block!\n");
	}
	unsigned char index = 0;
	for (;index < 128; index = index + (((myHeap[index+1]) & -2) >> 1) + 1) {
		if(prevBlockNum == myHeap[index]) {
			myHeap[index + 1] = myHeap[index + 1] & -2;
			break;
		}
	}
}

void blocklist(unsigned char* myHeap) {
	printf("Size Allocated Start          End\n");
	unsigned char index = 0;
	int check = 0;
	int count = 0;
	for (;index < 128; index = index + (((myHeap[index+1]) & -2) >> 1) + 1) {
		count += myHeap[index+1] >> 1;
		if ( count == 127 || (myHeap[index+1] >> 1 ) == 127 ){
			check = 1;
		}
		printf("%-5u", (myHeap[index+1] >> 1) + check );
		check = 0;
		if ( (myHeap[index+1] & 1) == 0 ) {
			printf("no        ");
		}
		else {
			printf("yes       ");
		}
		printf("%-15p", (void* )&myHeap[index]);
		printf("%p\n", (void* )&myHeap[(index + (((myHeap[index+1]) & -2) >> 1) + 1)-1]);
	}
}

void writeHeap(int bnum, char* copyChar, int numCopies, unsigned char* myHeap) {
	if (bnum > blockNum || bnum <= 0) {
		printf("Block number does not exist!\n");
		return;	
	}
	unsigned char index = 0;
	for (;index < 128; index = index + (((myHeap[index+1]) & -2) >> 1) + 1) {
		if (myHeap[index] == bnum && ((myHeap[index + 1] >> 1) >= (numCopies + 2)) && (myHeap[index+1] & 1) == 1) {//delete the minus 1 
			unsigned char i = 0;
			for (; i < numCopies; ++i) {
				myHeap[index + i + 2] = *copyChar;
			}
			return;
		}
	}	
	printf("Too many copies or that block is free!\n");
}

void printHeap(int bnum, int numBytes, unsigned char* myHeap) {
	if (bnum > blockNum || bnum <= 0) {
		printf("Block number does not exist!\n");
		return;	
	}
	if (numBytes > 126) {
 		printf("Too many bytes!\n");
		return;
	}
	unsigned char index = 0;
	for (;index < 128; index = index + ((((myHeap[index+1]) & -2) >> 1) + 1)) {
		if (myHeap[index] == bnum && (myHeap[index + 1] & 1) == 1) {
			unsigned char i = 0;
			for (; i <numBytes; ++i) {
				printf("%c", myHeap[index + i + 2]);
			}
			printf("\n");
			return;
		}
	}	
	
	printf("That block is free!\n");
}

void printHeader(int bnum, unsigned char* myHeap) {
	if (bnum > blockNum || bnum <= 0) {
		printf("Block number does not exist!\n");
		return;	
	}
	unsigned char index = 0;
	for (;index < 128; index = index + (((myHeap[index+1]) & -2) >> 1) + 1) {
		if (myHeap[index] == bnum && (myHeap[index+1] & 1) == 1) {
			printf("%02x%02x\n",myHeap[index], ((((myHeap[index + 1] >> 1) - 1) << 1) + 1));
			return;
		}
	}
	printf("That block is free!\n");
}

int main() {
	char user_command[BUFSIZ];
	char *command, *command1, *command2, *command3, *command4;
	unsigned char *myHeap;
	
	myHeap = (unsigned char*) malloc (128 * sizeof(unsigned char*));
	unsigned char tempBlockNum = 0;
	unsigned char tempBlockSize = 127;
	tempBlockSize = tempBlockSize << 1;
	myHeap[0] = tempBlockNum;
	myHeap[1] = tempBlockSize;

	while (1) {	
		printf("> ");

		fgets(user_command, 100, stdin);
		command = strtok(strtok(user_command, "\n"), " ");
		command1 = strtok(NULL, " ");
		command2 = strtok(NULL, " ");
		command3 = strtok(NULL, " ");
		command4 = strtok(NULL, " ");
		int temp;
	
		if (strlen(user_command) <= 1) { //user just press enter
			printf("Invalid command, please try again!\n");
		}
		else if (strcmp(command, "allocate") == 0 && command1 != NULL && sscanf(command1, "%d", &temp) == 1 && command2 == NULL) {	
			allocate(atoi(command1), myHeap);
		}
		
		else if (strcmp(command, "free") == 0 && command1 != NULL && sscanf(command1, "%d", &temp) == 1 && command2 == NULL) {
			freeBlock(atoi(command1), myHeap);
		}

		else if (strcmp(command, "blocklist") == 0 && command1 == NULL) {
			blocklist(myHeap);
		}
		
		else if (strcmp(command, "writeheap") == 0 && command1 != NULL && sscanf(command1, "%d", &temp) == 1 
			&& command2 != NULL && sscanf(command2, "%c ", &temp) == 1 &&
			 command3 != NULL && sscanf(command3, "%d", &temp) == 1 && command4 == NULL) { //fix the character reading problem
			writeHeap(atoi(command1), command2, atoi(command3), myHeap);
		}
		
		else if (strcmp(command, "printheap") == 0 && command1 != NULL && sscanf(command1, "%d", &temp) == 1 && command2 != NULL && sscanf(command2, "%d", &temp) == 1 && command3 == NULL ) {
			printHeap(atoi(command1), atoi(command2), myHeap);
		}

		else if (strcmp(command, "printheader") == 0 && command1 != NULL && sscanf(command1, "%d", &temp) == 1 && command2 == NULL) {
			printHeader(atoi(command1), myHeap);
		
		}
		
		else if (strcmp(command, "quit") == 0) {
			break;
		}
		else {
			printf("Invalid command, please try again!\n");
		}
	}
	return 0;
}
