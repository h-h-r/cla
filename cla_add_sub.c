//CSCI 2500 Assignment 4
//Carry Lookahead Adder
// Buiding a 64bit carry lookahead  “hierarchy” adder (need to support both add and sub)
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void storearray10_2(long dec, int* array){//stores decimal number to 64 bit array
	int i;
	for (i = 0 ;i<64; i++){
		if (dec>0){
			array[i] = dec % 2;
			dec = dec/2;
		}else{
			array[i]=0;
		}
	}
}
void printarray(int* array, int asize){//print array in certain format 
	int i;
	for (i = asize-1 ;i>=0; i--){
		if (asize == 64){
			printf("%d",array[i] );
		}else if (asize ==16){
			printf("%d   ",array[i] );
		}else if (asize == 4){
			printf("%d               ",array[i] );
		}
	}
	printf("\n");
}
/////////////////////////  “hierarchy” FUNCTIONS /////////////////////////////
//========================================
void calculate_gi(int* a,int *b,int *g){
	int i;
	for (i = 63 ;i>=0; i--){
		g[i] = a[i] & b[i];
	}
}
void calculate_pi(int* a, int* b, int* p){
	int i;
	for (i = 63 ;i>=0; i--){
		p[i] = a[i] | b[i];
	}
}
//======================================
void calculate_ggj(int * g, int*p, int* gg){
	int j;
	int i;
	
	for (j = 0 ; j < 16 ; j++){
		i = j*4;
		gg[j] = g[i+3] | (p[i+3]&g[i+2]) | (p[i+3]&p[i+2]&g[i+1]) | (p[i+3]&p[i+2]&p[i+1]&g[i]);
	}
}
void calculate_gpj(int* p,int* gp){
	int j;
	int i;
	for (j = 0 ; j < 16 ; j++){
		i = j*4;
		gp[j] = p[i+3] & p[i+2] & p[i+1] & p[i];
	}
}
//==========================================
void calculate_sgk(int* gg, int* gp, int* sg){
	int k;
	int j;
	for (k = 0 ; k < 4 ; k++){
		j=4 * k;
		sg[k] = gg[j+3] | (gp[j+3]&gg[j+2]) | (gp[j+3]&gp[j+2]&gg[j+1]) | (gp[j+3]&gp[j+2]&gp[j+1]&gg[j]);
	}
}
void calculate_spk(int* gp,int* sp){
	int k;
	int j;
	for (k = 0 ; k < 4 ; k++){
		j=4*k;
		sp[k] = gp[j+3] & gp[j+2] & gp[j+1] & gp[j]; 
	}
}
//////////////////////////////////////////
void calculate_sck(int* sg,int* sp, int* sc,int mode){
	int k=0;
	sc[0] = sg[k] | (sp[k] & mode);;
	for (k=1 ; k < 4 ; k++){
		sc[k] = sg[k] | (sp[k] & sc[k-1]); 
	}
}
void calculate_gcj(int* gg, int* gp, int*gc, int* sc,int mode){
	int j = 0;
	gc[0] = gg[j] | (gp[j] & mode);

	for (j = 1 ; j < 16 ; j++){
		//gc[3,7,11,15]
		if ((j+1)%4==0){
			gc[j] = sc[(j-3)/4];
		}else{
			gc[j] = gg[j] | (gp[j] & gc[j-1]);
		}
	}
}
void calculate_ci(int* g, int* p, int* c, int* gc,int mode){
	int i = 0;
	c[0] = g[0] | (p[i] & mode);

	for (i = 1 ; i < 64; i++){
		//c[3,7,11,15,19,23......63]
		if ((i+1)%4 == 0){
			c[i] = gc[(i-3)/4];
		}else {
			c[i] = g[i] | (p[i] & c[i-1]);
		}
	}
}
void calculate_sum(int* a,int* b, int*sum, int*c,int mode){
	int i ;
	sum[0] = a[0] ^ b[0] ^ mode;
	for (i = 1; i < 64; i++){
		sum[i] = a[i] ^ b[i] ^ c[i-1];
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void reverse(int * b){//reverse the binary array
	int i ;
	for(i = 0; i < 64 ; i++){
		if (b[i] == 0){
			b[i]=1;
		}else{
			b[i]=0;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////   MAIN  //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
int main(){
	//=============================declare variablels===================
	long A;
	long B;
	long S;
	int mode; // mode0: add , mode1: sub
	//----------
	int a[64]; //binary A
	int b[64]; //binary B
	int c[64]; //carryout for each bit
	int sum[64]; // sum for each bit
	//-----------
	int g[64]; //generate
	int p[64]; //propagate
	//-----------
	int gg[16];//group generate      each group has 4 binary bit
	int gp[16];//group propogate
	int gc[16];//group carry out
	//-----------
	int sg[4];//section generate  	each section has 16 binary bit
	int sp[4];//section propogate
	int sc[4];//group carry out
	//==========================================================================
	//user intput A,B and mode, then store A,B in the binary array a,b
	printf("Enter A (hex):\n");
	scanf("%lx",&A);
	storearray10_2(A,a);
	
	printf("Enter B (hex):\n");
	scanf("%lx",&B);
	storearray10_2(B,b);

	printf("Add (0) or subtract (1):\n");
	scanf("%d",&mode);
	//print A,B in decimal and a,b in binary
	printf("\nA is %016lx or %ld\n",A,A );
	printf("B is %016lx or %ld\n",B,B );
	//inverting b if we are doing a subtraction
	if (mode == 1){
		reverse(b);
		printf("Inverting %ld\n",B);
		printf("B (bin) : ");
		printarray(b,64);
	}
	printf("\nCalculate sum, S:\n");
	if (mode == 0){
		S=A+B;
	}else{
		S= A-B;
	}
	//=========================binary  calculation  ================================
	//========= uncomment the print functions in this section for checking each step
	// printf("a,b: \n");
	// printarray(a,64);
	// printarray(b,64);

	// printf("g,p:\n");
	calculate_gi(a,b,g);
	calculate_pi(a,b,p);
	// printarray(g,64);
	// printarray(p,64);

	// printf("gg,gp: \n");
	calculate_ggj(g,p,gg);
	calculate_gpj(p,gp);
	// printarray(gg,16);
	// printarray(gp,16);

	// printf("sg,sp: \n");
	calculate_sgk(gg,gp,sg);
	calculate_spk(gp,sp);
	// printarray(sg,4);
	// printarray(sp,4);

	// printf("sc:\n");
	calculate_sck(sg,sp,sc,mode);
	// printarray(sc,4);

	// printf("gc: \n");
	calculate_gcj(gg,gp,gc,sc,mode);
	// printarray(gc,16);

	// printf("c:\n");
	calculate_ci(g,p,c,gc,mode);
	// printarray(c,64);

	// printf("sum\n");
	calculate_sum(a,b,sum,c,mode);
	// printarray(sum,64);

	//======================== end of calculation ==================================

	//===================   print result =================================
	printf("\nA (bin) : ");
	printarray(a,64);
	printf("B (bin) : ");
	printarray(b,64);
	printf("S (bin) : ");
	printarray(sum,64);
	printf("\nS is %016lx or %ld",S,S );
}