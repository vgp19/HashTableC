#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Node{
	char* data;
	struct Node* next;
};
int getKey(char* data);
void add(int key, char* data);
int search(int key,char* data);
void hash(char* data);
int hexaOrNot(char* data, int len);
struct Node** hashTable;
int counter=0;
int size=1000;

int getKey(char* data){
	int key=0;
	int number=0;
	number = (int)strtol(data, NULL, 16);	
	key=number%size;
	key=abs(key);
	return key;
}

int search(int key,char* data){
	if(hashTable[key]==NULL){
		return 0;
	}else{
		struct Node* tmp;
		tmp=hashTable[key];
		while(tmp!=NULL){
			if(strcmp(tmp->data,data)==0){
				return 1;
			}
			tmp=tmp->next;
		}
	}
	return 0;
}

void add(int key, char* data){
	if(hashTable[key]==NULL){
		struct Node* tmp;
		tmp=(struct Node*)malloc(sizeof(struct Node));
		tmp->data=data;
		tmp->next=NULL;
		hashTable[key]=tmp;
		counter++;
	}else{
		if(search(key,data)==0){
			struct Node* tmp;
			tmp=hashTable[key];
			while(tmp->next!=NULL){
				tmp=tmp->next;	
			}
			struct Node* tmpNode;
			tmpNode=(struct Node*)malloc(sizeof(struct Node));
			tmpNode->data=data;
			tmpNode->next=NULL;
			tmp->next=tmpNode;
			counter++;
		}
	}	
}

void hash(char* data){
	int loadFactor=counter/size;
	int i=0;
	int j=0;
	if(counter==0){
		hashTable=(struct Node**)malloc(size*sizeof(struct Node*));
		for(i=0;i<size;i++){
			hashTable[i]=NULL;
		}
	}
	if(loadFactor>=0.5){
		size=2*size;
		struct Node** tmpTable=(struct Node**)malloc(counter*sizeof(struct Node*));
		for(i=0;i<size/2;i++){
			if(hashTable[i]!=NULL){
				tmpTable[j]=hashTable[i];
				hashTable[i]=NULL;
				j++;
			}
		}
		free(hashTable);
		counter=0;
		hashTable=(struct Node**)malloc(size*sizeof(struct Node*));
		for(i=0;i<size;i++){
			hashTable[i]=NULL;
		}
		for(i=0;i<j;i++){
			struct Node* tmp=tmpTable[i];
			while(tmp!=NULL){
				add(getKey(tmp->data),tmp->data);
				tmp=tmp->next;
			}	
		}
		free(tmpTable);	
	}
	add(getKey(data),data);
	return;
}

int hexaOrNot(char* data, int len){
	int i=0;
	if(len<2)
		return 0;
	if(data[0]!='0'&&data[1]!='x'&&data[1]!='X')
		return 0;
	for(i=2;i<len;i++){
		if(!(data[i]=='a'||data[i]=='b'||data[i]=='c'||data[i]=='d'||data[i]=='e'||data[i]=='f'||
		   data[i]=='A'||data[i]=='B'||data[i]=='C'||data[i]=='D'||data[i]=='E'||data[i]=='F'||
		   data[i]=='0'||data[i]=='1'||data[i]=='2'||data[i]=='3'||data[i]=='4'||data[i]=='5'||
		   data[i]=='6'||data[i]=='7'||data[i]=='8'||data[i]=='9'))
			return 0;
	}
	return 1;
}

int main(int argc, char* argv[]){
	int number=0;
	FILE* f;
	char str[255];
	int len=0;
	char substr[255];
	char* data;
	int i=0;
	if(argc<2){
		return 0;
	}
	f=fopen(argv[1],"r");
	if(f==NULL){
		printf("error");
		return 0;
	}	
	if(fgets(str,255,f)==NULL){
		printf("0");
		fclose(f);
		return 0;
	}	
	do{
		len=strlen(str);
		len=str[len-1]=='\n'?len-1:strlen(str);
		data=(char*)malloc(sizeof(str));
		strcpy(data,str);
		if(hexaOrNot(data,len)==1)
			hash(data);
	}while(fgets(str,255,f)!=NULL);	
	printf("%d",counter);
	fclose(f);
	free(data);
	return 0;
}
