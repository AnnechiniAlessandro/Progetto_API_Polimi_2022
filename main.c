#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define ALPHL 64
#define FNUM '0'
#define LNUM '9'
#define FMAI 'A'
#define LMAI 'Z'
#define FMIN 'a'
#define LMIN 'z'
#define MINS '-'
#define UND '_'
#define NUMOFNUM 10
#define ALPHANUM 26
#define ERROR '?'
#define MAXLOAD 2.0
#define CMDLEN 22
#define NEWMATCH "+nuova_partita"
#define PRINTFIL "+stampa_filtrate"
#define INSIN "+inserisci_inizio"
#define INSFIN "+inserisci_fine"
#define NOTINDIC "not_exists"
#define RLRP '+'
#define RLWP '|'
#define WLWP '/'
#define AT_LEAST 1
#define AT_MOST 2
#define IN_POS 3
#define NOT_POS 4
#define CHASHDIM 1

//TYPEDEF

typedef struct list_s{
	char *word;
	struct list_s *next;
	struct list_s *prev;
	struct list_s *nextC;
}list_t;

typedef struct hash_s{
	int dim;
	long long int numOfWords;
	float loadFactor;
	float maxLoad;
	list_t **table;
}hash_t;

typedef struct cons_s{
	short int type;
	char arg;
	int value;
	struct cons_s *next;
}cons_t;

//FUNZIONI

int mapl(char);
int myStrcmp(char[],char[]);
list_t* wListInsert(list_t*,char[]);
long long int hashBucket(char[],int);
void hashInsert(hash_t*,char[]);
void extendTable(hash_t*);
void getLine(char[]);
hash_t* createHash(float);
void getDictionary();
void clearAll();
void match();
void startMatch();
int checkWord();
void appendCons(int,char,int);
long long int createFirstLink();
int checkCurrCons(list_t*);
long long int updateLink();
int addInGame();
void hashInsertIG(hash_t*,char[],int*,list_t**);
list_t* wListInsertIG(list_t *,char[],int*,list_t**);
void printFiltered();
void clearCons();
char unmapl(int);
int checkHisCons(list_t*);
void clearAllCons();
hash_t* createCandHash();
void chashIns(list_t *);
void chashInsIG(list_t *);
void chashDelete(list_t *);
void chashClear();

//VAR GLOBALI

int k;
char *sol;
int solcount[ALPHL];
char *res;
list_t* dict;
hash_t* ght;
hash_t* cht;
list_t* firstC;
list_t* last;
cons_t* constraints;
cons_t* history;
int attempts;
int atleast[ALPHL],atmost[ALPHL],*pos;

//MAIN

int main(){

	sol=NULL;
	res=NULL;
	dict=NULL;
	ght=NULL;
	firstC=NULL;
	last=NULL;
	constraints=NULL;
	history=NULL;
	pos=NULL;
	cht=NULL;
	
	if(fscanf(stdin,"%d",&k)!=1);
	ght=createHash(MAXLOAD);
	
	getDictionary();
	match();
	clearAll();
	return 0;
}

//CORPO FUNZIONI

hash_t* createHash(float max){
	hash_t* ht;
	int i;
	
	ht=NULL;
	while(ht==NULL) ht=malloc(sizeof(hash_t));
	ht->table=NULL;
	while(ht->table==NULL) ht->table=malloc(sizeof(list_t*)*ALPHL);
	for(i=0;i<ALPHL;i++) ht->table[i]=NULL;
	ht->dim=0;
	ht->numOfWords=0;
	ht->loadFactor=0.0;
	ht->maxLoad=max;
	return ht;
}

int mapl(char letter){
	if(letter==MINS) return 0;
	if(letter>=FNUM && letter<=LNUM) return letter-FNUM+1;
	if(letter>=FMAI && letter<=LMAI) return letter-FMAI+NUMOFNUM+1;
	if(letter==UND) return ALPHANUM+NUMOFNUM+1;
	if(letter>=FMIN && letter<=LMIN) return letter-FMIN+ALPHANUM+NUMOFNUM+2;
	return 0;
}

char unmapl(int num){
	if(num==0) return MINS;
	if(num>=1 && num<NUMOFNUM+1) return num+FNUM-1;
	if(num>=NUMOFNUM+1 && num<NUMOFNUM+ALPHANUM+1) return num+FMAI-1-NUMOFNUM;
	if(num==NUMOFNUM+ALPHANUM+1) return UND;
	if(num>=0 && num<ALPHL) return num+FMIN-ALPHANUM-NUMOFNUM-2;
	return ERROR;
}

int myStrcmp(char wordnew[],char wordinlist[]){
	int i;
	for(i=0;i<k;i++){
		if(wordnew[i]>wordinlist[i]) return 1;
		if(wordnew[i]<wordinlist[i]) return 0;
	}
	return 2;
}

list_t* wListInsert(list_t *head,char word[]){
	list_t *tmp,*tmp2;
	
	tmp=NULL;
	while(tmp==NULL) tmp=malloc(sizeof(list_t));
	tmp->word=NULL;
	while(tmp->word==NULL) tmp->word=malloc(sizeof(char)*(k+1));
	strcpy(tmp->word,word);
	tmp->next=NULL;
	
	if(head==NULL){
		tmp->next=NULL;
		tmp->prev=NULL;
		if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
		if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
		return tmp;
	}
	
	tmp2=head;
	if(!myStrcmp(tmp->word,tmp2->word)){
		if(tmp2->prev!=NULL) tmp2->prev->next=tmp;
		tmp->next=tmp2;
		tmp->prev=tmp2->prev;
		tmp2->prev=tmp;
		if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
		if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
		return tmp;
	}
	
	while(tmp2->next!=NULL && myStrcmp(tmp->word,tmp2->word)){
		tmp2=tmp2->next;
	}
	if(myStrcmp(tmp->word,tmp2->word)){
		tmp2->next=tmp;
		tmp->prev=tmp2;
		tmp->next=NULL;
		if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
		if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
		return head;
	}
	tmp->next=tmp2;
	tmp->prev=tmp2->prev;
	if(tmp2->prev!=NULL) tmp2->prev->next=tmp;
	tmp2->prev=tmp;
	if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
	if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
	return head;
}

long long int hashBucket(char string[],int dim){
	int level;
	long long int ris,i;
	if(dim==0) return mapl(string[0]);
	level=(dim-1)/6+1;
	ris=0;
	for(i=0;i<=level;i++){
		if(dim>6*i) ris+=mapl(string[i])<<(dim-6*i);
		else ris+=mapl(string[i])>>(6*i-dim);
	}
	return ris;
}

void hashInsert(hash_t* ht,char word[]){
	list_t* ptr;
	long long int i,pos;
	
	ht->numOfWords++;
	ht->loadFactor=(float)ht->numOfWords/(ALPHL<<(ht->dim));
	pos=hashBucket(word,ht->dim);
	if(ht->table[pos]==NULL){
		if(last==NULL){
			ptr=wListInsert(dict,word);
			dict=ptr;
			for(i=pos;i>=0;i--) ht->table[i]=ptr;
			if(ht->loadFactor>ht->maxLoad) extendTable(ht); //REALLOC
			return;
		}
		ptr=wListInsert(last,word)->next;
		last=ptr;
		for(i=pos;i>=0 && ht->table[i]==NULL;i--) ht->table[i]=ptr;
		if(ht->loadFactor>ht->maxLoad) extendTable(ht); //REALLOC
		return;
	}
	ht->table[pos]=wListInsert(ht->table[pos],word);
	if(pos>0 && !myStrcmp(word,ht->table[pos-1]->word)) for(i=pos-1;i>=0 && !myStrcmp(word,ht->table[i]->word);i--) ht->table[i]=ht->table[pos];
	else for(i=pos+1;i<ALPHL<<(ht->dim) && ht->table[i]!=NULL && myStrcmp(word,ht->table[i]->word);i++) ht->table[i]=ht->table[pos];
	if(ht->loadFactor>ht->maxLoad) extendTable(ht); //REALLOC
	return;
}

void extendTable(hash_t* ptr){
	list_t** newht;
	list_t *tmp;
	long long int i;
	newht=NULL;
	while(newht==NULL) newht=malloc((ALPHL<<((ptr->dim)+1))*sizeof(list_t*));
	
	for(i=0;i<(ALPHL<<((ptr->dim)+1));i++) newht[i]=NULL;
	
	for(i=0;i<ALPHL<<ptr->dim;i++){
		if(ptr->table[i]!=NULL){
			tmp=ptr->table[i];
			if(hashBucket(tmp->word,(ptr->dim)+1)>=2*i+1)
			{
				newht[2*i+1]=ptr->table[i];
				newht[2*i]=ptr->table[i];
			}
			else if(tmp->next==NULL && hashBucket(tmp->word,(ptr->dim)+1)==2*i){
				newht[2*i]=ptr->table[i];
				if(i==(ALPHL<<ptr->dim)-1) newht[2*i+1]=NULL;
				else newht[2*i+1]=ptr->table[i+1];
			}
			else{
				newht[2*i]=ptr->table[i];
				while(tmp!=NULL && hashBucket(tmp->word,(ptr->dim)+1)==2*i){
					tmp=tmp->next;
				}
				if(tmp!=NULL && hashBucket(tmp->word,(ptr->dim)+1)==2*i+1){
					newht[2*i+1]=tmp;
				}
				else{
					if(i==(ALPHL<<ptr->dim)-1) newht[2*i+1]=NULL;
					else newht[2*i+1]=ptr->table[i+1];
				}
			}
		}
		else{
			newht[2*i+1]=NULL;
			newht[2*i]=NULL;
		}
	}
	ptr->dim++;
	ptr->loadFactor=ptr->numOfWords/(float)(ALPHL<<ptr->dim);
	free(ptr->table);
	ptr->table=newht;
	
	return;
}

void getLine(char buff[]){
	char init;
	int i;
	init=getchar();
	while(!feof(stdin) && !isgraph(init)) init=getchar();
	buff[0]=init;
	i=1;
	init=getchar();
	while(!feof(stdin) && isgraph(init)){
		buff[i]=init;
		i++;
		init=getchar();
	}
	buff[i]='\0';
	while(!feof(stdin) && init!='\n') init=getchar();
}

void getDictionary(){
	int dim;
	char *string;
	
	dim=k+1;
	if(k<CMDLEN) dim=CMDLEN+1;
	string=NULL;
	while(string==NULL) string=malloc(sizeof(char)*dim);
	getLine(string);
	while(!feof(stdin) &&  strcmp(string,NEWMATCH) && strcmp(string,INSFIN)){
		if(string[0]!='+'){
			hashInsert(ght,string);
		}
		else if(!strcmp(string,PRINTFIL)){
			printFiltered();
		}
		getLine(string);
	}
	free(string);
	
	return;
}

void clearAll(){
	list_t *tmp2,*tmp;
	
	tmp=dict;
	
	while(tmp!=NULL){
		tmp2=tmp->next;
		free(tmp->word);
		free(tmp);
		tmp=tmp2;
	}
	free(sol);
	free(pos);
	free(res);
	free(ght->table);
	free(ght);
	return;
}

void match(){
	char *string=NULL;
	int dimtr;
	int ris;
	dimtr=k+1;
	if(k<CMDLEN) dimtr=CMDLEN+1;
	while(string==NULL) string=malloc(sizeof(char)*dimtr);
	while(!feof(stdin)){
		startMatch();
		clearCons();
		while(!feof(stdin) && attempts>0){
			getLine(string);
			if(!strcmp(string,PRINTFIL)){
				printFiltered();
			}
			else if(!strcmp(string,INSIN)){
				addInGame();
			}
			else{
				clearCons();
				ris=checkWord(string);
				
				
				printf("%s\n",res);
				if(ris==1){
					if(firstC==NULL) printf("%lld\n",createFirstLink());
					else printf("%lld\n",updateLink());
					attempts--;
				}
				else if(ris==2) attempts=-1;
			}
		}
		clearAllCons();
		chashClear();
		if(attempts==0) printf("ko\n");
		getLine(string);
		if(!feof(stdin) && (string[0]!='+' || string[1]!='n')){
			if(string[0]!='+') hashInsert(ght,string);
			while(!feof(stdin) && strcmp(string,NEWMATCH)){
				if(!strcmp(string,INSIN)) getDictionary();
				getLine(string);
			}
		}
	}
	
	
	free(string);
	return;
}

void startMatch(){
	int i;
	while(sol==NULL){
		if(k<CMDLEN) sol=malloc(sizeof(char)*(CMDLEN+1));
		else sol=malloc(sizeof(char)*(k+1));
	}
	while(res==NULL){
		if(k<CMDLEN) res=malloc(sizeof(char)*(CMDLEN+1));
		else res=malloc(sizeof(char)*(k+1));
	}
	while(pos==NULL) pos=malloc(sizeof(int)*k*ALPHL);
	for(i=0;i<ALPHL;i++) solcount[i]=0;
	for(i=0;i<ALPHL;i++) atleast[i]=0;
	for(i=0;i<ALPHL;i++) atmost[i]=k+1;
	for(i=0;i<ALPHL*k;i++) pos[i]=-1;
	firstC=NULL;
	getLine(sol);
	while(sol[0]=='+'){
		if(!strcmp(sol,PRINTFIL)) printFiltered(); 
		else if(!strcmp(sol,INSIN)) getDictionary();
		getLine(sol); 
	}
	
	for(i=0;i<k;i++) solcount[mapl(sol[i])]++;
	cht=createCandHash();
	
	getLine(res);
	while(sol[0]=='+'){
		if(!strcmp(res,PRINTFIL)) printFiltered(); 
		else if(!strcmp(res,INSIN)) getDictionary();
		getLine(res); 
	}
	attempts=atoi(res);
	return;
}

int hashSearch(char word[]){
	long long int pos;
	list_t *tmp;
	pos=hashBucket(word,ght->dim);
	if(ght->table[pos]==NULL){
		return 0;
	}
	tmp=ght->table[pos];
	while(tmp!=NULL && myStrcmp(word,tmp->word)==1){
		tmp=tmp->next;
	}
	if(tmp==NULL || myStrcmp(word,tmp->word)==0) return 0;
	return 1;
}

int checkWord(char word[]){
	int i,j;
	int count[ALPHL];
	
	
	if(hashSearch(word)==0){
		strcpy(res,NOTINDIC);
		return 0;
	}
	if(!strcmp(word,sol)){
		strcpy(res,"ok");
		return 2;
	}
	res[k]='\0';
	
	for(i=0;i<ALPHL;i++){
		count[i]=0;
	}
	for(i=0;i<k;i++){
		if(word[i]==sol[i]){
			res[i]=RLRP;
			count[mapl(word[i])]++;
			if(pos[i*ALPHL+mapl(word[i])]!=1){
				pos[i*ALPHL+mapl(word[i])]=1;
				for(j=0;j<ALPHL;j++){
					if(j!=mapl(word[i])) pos[i*ALPHL+j]=0;
				}
				appendCons(IN_POS,word[i],i);
			}
		}
	}
	for(i=0;i<k;i++){
		if(word[i]!=sol[i]){
			if(count[mapl(word[i])]<solcount[mapl(word[i])]){
				res[i]=RLWP;
				count[mapl(word[i])]++;
				if(pos[i*ALPHL+mapl(word[i])]!=0){
					pos[i*ALPHL+mapl(word[i])]=0;
					appendCons(NOT_POS,word[i],i);
				}
			}
			else{
				res[i]=WLWP;
				if(pos[i*ALPHL+mapl(word[i])]!=0){
					pos[i*ALPHL+mapl(word[i])]=0;
					if(solcount[mapl(word[i])]>0) appendCons(NOT_POS,word[i],i);
					if(count[mapl(word[i])]==solcount[mapl(word[i])] && count[mapl(word[i])]<atmost[mapl(word[i])]){
						appendCons(AT_MOST,word[i],count[mapl(word[i])]);
						atmost[mapl(word[i])]=count[mapl(word[i])];
					}
				}
				else{
					if(count[mapl(word[i])]==solcount[mapl(word[i])] && count[mapl(word[i])]<atmost[mapl(word[i])]){
						appendCons(AT_MOST,word[i],count[mapl(word[i])]);
						atmost[mapl(word[i])]=count[mapl(word[i])];
					}
					appendCons(NOT_POS,word[i],i);
				}
				count[mapl(word[i])]++;
			}
		}
	}
	
	for(i=0;i<ALPHL;i++){
		if(count[i]>atleast[i] && count[i]<=solcount[i]){
			atleast[i]=count[i];
			appendCons(AT_LEAST,unmapl(i),atleast[i]);
		}
	}
	
	cons_t *tmp;
	tmp=constraints;
	while(tmp!=NULL){
		tmp=tmp->next;
	}
	
	return 1;
}

void appendCons(int flag,char arg,int value){
	cons_t *newcs=NULL;
	while(newcs==NULL) newcs=malloc(sizeof(cons_t));
	newcs->type=flag;
	newcs->arg=arg;
	newcs->value=value;
	newcs->next=constraints;
	constraints=newcs;
	
	newcs=NULL;
	while(newcs==NULL) newcs=malloc(sizeof(cons_t));
	newcs->type=flag;
	newcs->arg=arg;
	newcs->value=value;
	newcs->next=history;
	history=newcs;
	return;
}

long long int createFirstLink(){
	list_t *wd,*tmp;
	long long int ris;
	
	ris=0;
	wd=dict;
	while(!checkCurrCons(wd)) wd=wd->next;
	firstC=wd;
	chashIns(wd);
	tmp=wd->next;
	ris++;
	while(tmp!=NULL){
		while(tmp!=NULL && !checkCurrCons(tmp)) tmp=tmp->next;
		wd->nextC=tmp;
		if(tmp!=NULL){
			ris++;
			wd=tmp;
			chashIns(tmp);
			tmp=tmp->next;	
		}
	}
	wd->nextC=NULL;

	
	return ris;
}

int checkCurrCons(list_t *word){
	cons_t *con;
	con=constraints;
	int count;
	int i;
	
	
	while(con!=NULL){
		if(con->type==AT_LEAST){
			count=0;
			for(i=0;i<k;i++) if(word->word[i]==con->arg) count++;
			if(count<con->value) return 0;
		}
		else if(con->type==AT_MOST){
			count=0;
			for(i=0;i<k;i++) if(word->word[i]==con->arg) count++;
			if(count!=con->value) return 0;
		}
		else if(con->type==IN_POS  && word->word[con->value]!=con->arg) return 0;
		else if(con->type==NOT_POS  && word->word[con->value]==con->arg) return 0;
		con=con->next;
	}
	return 1;
}

int checkHisCons(list_t *word){
	cons_t *con;
	con=history;
	int count;
	int i;
	
	
	while(con!=NULL){
		if(con->type==AT_LEAST){
			count=0;
			for(i=0;i<k;i++) if(word->word[i]==con->arg) count++;
			if(count<con->value) return 0;
		}
		else if(con->type==AT_MOST){
			count=0;
			for(i=0;i<k;i++) if(word->word[i]==con->arg) count++;
			if(count!=con->value) return 0;
		}
		else if(con->type==IN_POS  && word->word[con->value]!=con->arg) return 0;
		else if(con->type==NOT_POS  && word->word[con->value]==con->arg) return 0;
		con=con->next;
	}
	return 1;
}

long long int updateLink(){
	list_t *wd,*tmp;
	long long int ris;
	
	ris=0;
	wd=firstC;
	while(!checkCurrCons(wd)){
		chashDelete(wd);
		wd=wd->nextC;
	}
	firstC=wd;
	tmp=wd->nextC;
	ris++;
	while(tmp!=NULL){
		while(tmp!=NULL && !checkCurrCons(tmp)){
			chashDelete(tmp);
			tmp=tmp->nextC;
		}
		wd->nextC=tmp;
		if(tmp!=NULL){
			ris++;
			wd=tmp;
			tmp=tmp->nextC;	
		}
	}
	wd->nextC=NULL;
	
	return ris;
}

int addInGame(){
	int dim;
	long long int ris;
	int ans;
	char *string;
	list_t *ind;
	
	ris=0;
	dim=k+1;
	if(k<CMDLEN) dim=CMDLEN+1;
	string=NULL;
	while(string==NULL) string=malloc(sizeof(char)*dim);
	getLine(string);
	while(strcmp(string,INSFIN)){
		if(!strcmp(string,PRINTFIL)){
			printFiltered();
		}
		else if(string[0]!='+'){
			ans=0;
			ind=NULL;
			hashInsertIG(ght,string,&ans,&ind);
			if(ans && firstC!=NULL){
				ris++;
				chashInsIG(ind);
			}
		}
		getLine(string);
	}
	free(string);
	return ris;
}

void hashInsertIG(hash_t* ht,char word[],int *ans,list_t **ind){
	list_t* ptr;
	long long int i,pos;
	
	ht->numOfWords++;
	ht->loadFactor=(float)ht->numOfWords/(ALPHL<<(ht->dim));
	pos=hashBucket(word,ht->dim);
	if(ht->table[pos]==NULL){
		if(last==NULL){
			ptr=wListInsertIG(dict,word,ans,ind);
			dict=ptr;
			for(i=pos;i>=0;i--) ht->table[i]=ptr;
			if(ht->loadFactor>ht->maxLoad) extendTable(ht); //REALLOC
			return;
		}
		ptr=wListInsertIG(last,word,ans,ind)->next;
		last=ptr;
		for(i=pos;i>=0 && ht->table[i]==NULL;i--) ht->table[i]=ptr;
		if(ht->loadFactor>ht->maxLoad) extendTable(ht); //REALLOC
		return;
	}
	ht->table[pos]=wListInsertIG(ht->table[pos],word,ans,ind);
	if(pos>0 && !myStrcmp(word,ht->table[pos-1]->word)) for(i=pos-1;i>=0 && !myStrcmp(word,ht->table[i]->word);i--) ht->table[i]=ht->table[pos];
	else for(i=pos+1;i<ALPHL<<(ht->dim) && ht->table[i]!=NULL && myStrcmp(word,ht->table[i]->word);i++) ht->table[i]=ht->table[pos];
	if(ht->loadFactor>ht->maxLoad) extendTable(ht); //REALLOC
	return;
}

list_t* wListInsertIG(list_t *head,char word[],int *ans,list_t **ind){
	list_t *tmp,*tmp2;
	
	tmp=NULL;
	while(tmp==NULL) tmp=malloc(sizeof(list_t));
	tmp->word=NULL;
	while(tmp->word==NULL) tmp->word=malloc(sizeof(char)*(k+1));
	strcpy(tmp->word,word);
	tmp->nextC=NULL;
	tmp->next=NULL;
	
	*ans=checkHisCons(tmp);

	*ind=tmp;
	
	if(head==NULL){
		tmp->next=NULL;
		tmp->prev=NULL;
		if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
		if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
		return tmp;
	}
	
	tmp2=head;
	if(!myStrcmp(tmp->word,tmp2->word)){
		if(tmp2->prev!=NULL) tmp2->prev->next=tmp;
		tmp->next=tmp2;
		tmp->prev=tmp2->prev;
		tmp2->prev=tmp;
		if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
		if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
		return tmp;
	}
	
	while(tmp2->next!=NULL && myStrcmp(tmp->word,tmp2->word)){
		tmp2=tmp2->next;
	}
	if(myStrcmp(tmp->word,tmp2->word)){
		tmp2->next=tmp;
		tmp->prev=tmp2;
		tmp->next=NULL;
		if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
		if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
		return head;
	}
	tmp->next=tmp2;
	tmp->prev=tmp2->prev;
	if(tmp2->prev!=NULL) tmp2->prev->next=tmp;
	tmp2->prev=tmp;
	if(dict==NULL || !myStrcmp(tmp->word,dict->word)) dict=tmp;
	if(last==NULL || myStrcmp(tmp->word,last->word)) last=tmp;
	return head;
}

void printFiltered(){
	list_t *tmp;
	if(firstC==NULL){
		tmp=dict;
		while(tmp!=NULL){
			printf("%s\n",tmp->word);
			tmp=tmp->next;
		}
		return;
	}
	tmp=firstC;
	while(tmp!=NULL){
		printf("%s\n",tmp->word);
		tmp=tmp->nextC;
	}
	return;
}

void clearCons(){
	cons_t *tmp,*tmp2;
	tmp=constraints;
	while(tmp!=NULL){
		tmp2=tmp->next;
		free(tmp);
		tmp=tmp2;
	}
	constraints=NULL;
	return;
}

void clearAllCons(){
	cons_t *tmp,*tmp2;
	tmp=constraints;
	while(tmp!=NULL){
		tmp2=tmp->next;
		free(tmp);
		tmp=tmp2;
	}
	constraints=NULL;
	
	tmp=history;
	while(tmp!=NULL){
		tmp2=tmp->next;
		free(tmp);
		tmp=tmp2;
	}
	history=NULL;
	
	return;
}

hash_t* createCandHash(){
	hash_t *chash;
	int i;
	
	chash=NULL;
	while(chash==NULL) chash=malloc(sizeof(hash_t));
	chash->table=NULL;
	while(chash->table==NULL) chash->table=malloc(sizeof(list_t*)*(ALPHL<<CHASHDIM));
	for(i=0;i<(ALPHL<<CHASHDIM);i++) chash->table[i]=NULL;
	chash->dim=CHASHDIM;
	chash->numOfWords=0;
	chash->loadFactor=0;
	chash->maxLoad=1e100;
	return chash;
}

void chashInsIG(list_t *ptr){
	long long int i,pos;
	list_t *tmp,*tmp2;
	
	pos=hashBucket(ptr->word,cht->dim);
	if(cht->table[pos]==NULL){
		cht->table[pos]=ptr;
		for(i=pos-1;i>=0 && cht->table[i]==NULL;i--);
		if(i<0){
			ptr->nextC=firstC;
			firstC=ptr;
		}
		else{
			tmp=cht->table[i];
			tmp2=NULL;
			while(tmp!=NULL && myStrcmp(ptr->word,tmp->word)){
				tmp2=tmp;
				tmp=tmp->nextC;
			}
			ptr->nextC=tmp;
			tmp2->nextC=ptr;
		}
	}
	else if(!myStrcmp(ptr->word,cht->table[pos]->word)){
		for(i=pos-1;i>=0 && cht->table[i]==NULL;i--);
		if(i<0){
			ptr->nextC=firstC;
			firstC=ptr;
		}
		else{
			tmp=cht->table[i];
			tmp2=NULL;
			while(tmp!=cht->table[pos]){
				tmp2=tmp;
				tmp=tmp->nextC;
			}
			cht->table[pos]=ptr;
			ptr->nextC=tmp;
			tmp2->nextC=ptr;
		}
	}
	else{
		tmp=cht->table[pos];
		tmp2=NULL;
		while(tmp!=NULL && myStrcmp(ptr->word,tmp->word)){
			tmp2=tmp;
			tmp=tmp->nextC;
		}
		ptr->nextC=tmp;
		tmp2->nextC=ptr;
	}
	return;
}

void chashDelete(list_t *ptr){
	long long int pos;
	pos=hashBucket(ptr->word,cht->dim);
	if(cht->table[pos]==ptr){
		if(ptr->nextC==NULL) cht->table[pos]=NULL;
		else if(hashBucket(ptr->nextC->word,cht->dim)!=pos) cht->table[pos]=NULL;
		else cht->table[pos]=ptr->nextC;
	}
	return;
}

void chashClear(){
	free(cht->table);
	free(cht);
}

void chashIns(list_t *ptr){
	long long int pos;
	
	cht->numOfWords++;
	cht->loadFactor=(float)cht->numOfWords/(ALPHL<<(cht->dim));
	
	pos=hashBucket(ptr->word,cht->dim);
	if(cht->table[pos]==NULL){
		cht->table[pos]=ptr;
		return;
	}
	if(!myStrcmp(ptr->word,cht->table[pos]->word)) cht->table[pos]=ptr;
	return;
	
	
	return;
}


