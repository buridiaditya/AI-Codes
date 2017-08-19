#include <iostream>
#include <fstream>
#include <vector>
#include <string.h> 
#include <time.h>
using namespace std;

int main(int argc,char** argv){

	fstream F("input.txt",ios::in | ios::out);

	int count = 0;
	vector<bool> hash(9,false);
	int value,NO = 0;
	
	for(int i = 0; i < strlen(argv[1]); i++){
		NO += argv[1][i]-'0';
		NO *= 10;
	}
	NO /= 10;
	srand(time(NULL));

	while(NO){
		value = rand()%9;

			if(count == 9){
				for(int i = 0; i < 9;i++)
					hash[i] = false;
				count = 0;
				NO--;
				F << "\n";
			}
		if(hash[value] == false){
			
			if(count%3 == 0)
				F << "\n";
			hash[value] = true;
			if(value == 0)
				F << -1 << " ";
			else
				F << value << " ";
			count++;
		}		
	}
    F.close();
	return 0;
}
