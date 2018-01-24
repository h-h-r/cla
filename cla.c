#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int add_two_bits(int d0, int d1, int carry_in, int *carry_out)
{
  // Use your implementation from problem 1 
  *carry_out = ((d0 ^ d1) & carry_in ) | (d0 & d1);
   return (d0 ^ d1) ^ carry_in;
}

void print10_2(int n){//print 
	if (n/2>0){
		print10_2(n/2);
		printf("%d",n%2 );
	}else{
		printf("%d",n );
	}
}
void storearray10_2(long dec, int* array){//64 bit array
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
void printarray(int* array, int asize){
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
//////////////////////////////////////////
void calculate_sck(int* sg,int* sp, int* sc){
	int k=0;
	sc[0] = sg[k] | (sp[k] & 0);;
	for (k=1 ; k < 4 ; k++){
		sc[k] = sg[k] | (sp[k] & sc[k-1]); 
	}
}
void calculate_gcj(int* gg, int* gp, int*gc, int* sc){
	int j = 0;
	gc[0] = gg[j] | (gp[j] & 0);

	for (j = 1 ; j < 16 ; j++){
		//gc[3,7,11,15]
		if ((j+1)%4==0){
			gc[j] = sc[(j-3)/4];
		}else{
			gc[j] = gg[j] | (gp[j] & gc[j-1]);
		}
	}
}
void calculate_ci(int* g, int* p, int* c, int* gc){
	int i = 0;
	c[0] = g[0] | (p[i] & 0);

	for (i = 1 ; i < 64; i++){
		//c[3,7,11,15,19,23......63]
		if ((i+1)%4 == 0){
			c[i] = gc[(i-3)/4];
		}else {
			c[i] = g[i] | (p[i] & c[i-1]);
		}
	}
}
void calculate_sum(int* a,int* b, int*sum, int*c){
	int i ;
	sum[0] = a[0] ^ b[0] ^ 0;
	for (i = 1; i < 64; i++){
		sum[i] = a[i] ^ b[i] ^ c[i-1];
	}
}
/////////////////////////////////////////////////////////////////////////////////////

int main(){
	long A;
	long B;
	int mode;
	//----------
	int a[64];
	int b[64];
	int c[64];
	int sum[64];
	//-----------
	int g[64];
	int p[64];
	//-----------
	int gg[16];
	int gp[16];
	int gc[16];
	//-----------
	int sg[4];
	int sp[4];
	int sc[4];

	//user intput A,B and mode
	printf("Enter A (hex):\n");
	scanf("%lx",&A);
	storearray10_2(A,a);
	
	printf("Enter B (hex):\n");
	scanf("%lx",&B);
	storearray10_2(B,b);

	printf("Add (0) or subtract (1):\n");
	scanf("%d",&mode);
	//print A,B a,b
	printf("A is %016lx or %ld\n",A,A );
	printf("B is %016lx or %ld\n",B,B );

	printf("a,b: \n");
	printarray(a,64);
	printarray(b,64);

	printf("g,p:\n");
	calculate_gi(a,b,g);
	calculate_pi(a,b,p);
	printarray(g,64);
	printarray(p,64);

	printf("gg,gp: \n");
	calculate_ggj(g,p,gg);
	calculate_gpj(p,gp);
	printarray(gg,16);
	printarray(gp,16);

	printf("sg,sp: \n");
	calculate_sgk(gg,gp,sg);
	calculate_spk(gp,sp);
	printarray(sg,4);
	printarray(sp,4);

	printf("sc:\n");
	calculate_sck(sg,sp,sc);
	printarray(sc,4);

	printf("gc: \n");
	calculate_gcj(gg,gp,gc,sc);
	printarray(gc,16);

	printf("c:\n");
	calculate_ci(g,p,c,gc);
	printarray(c,64);

	printf("sum\n");
	calculate_sum(a,b,sum,c);
	printarray(sum,64);
	//////////////////////////////////////////////////////




/////////////////////////////////////////////////////////

	//////////////////////////////////////////
	// if (mode == 1){
	// 	printf("Inverting %d\nB(bin) : ",B );

	// }	

	// printf("%d\n",sizeof(long long int) );
	// printf("%d\n",sizeof(int) );
	// printf("%d\n",sizeof(double) );
	// printf("%ld\n",sizeof(long) );
	// printf("%lld\n",sizeof(long long) );
	// printf("%lld\n",sizeof(unsigned long) );
	//printf("%d\n",sizeof(longs) );

}