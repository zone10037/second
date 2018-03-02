#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define  MAX 500 
#define  ACLR11 90
#define  ACS11 50 
#define  Np  10e-10
#define  Ps  43 
#define  IsameFreinterfere 15 

#define  file1  "effACIR_KM_Q.csv"
#define  file2  "SINRvim_KM_Q.csv"
#define  file3  "C_KM_Q.csv"
#define  file4  "C1_KM_Q.csv"
#define  file5  "C2_KM_Q.csv"
#define  file6  "C3_KM_Q.csv"

#define STATE_SIZE 12
#define ACTION_SIZE 2
#define K 10
#define TRY_NUM 100
#define MAX_COUNT 100


int selectAction(double);
double getMaxQValue(double);
double calcQValue(double, double);
double Reward(double);


typedef struct {
         double orign;
         double qlearn;
    }ACTION;

typedef struct {
        ACTION action[1]; 
    }STATE;
    
STATE current_state[MAX];  //宣告結構變數目前狀態    
    
/*原始*/ 
int main(int argc, char *argv[])
{
   //srand(time(NULL));
   double Bvictim[MAX]; 
   double ACIR[MAX]; 
   double ACS[MAX],ACS1=0;
   double ACLR[MAX];
   double Baggressor[MAX];
   double ACLReff[MAX],ACSeff[MAX],effACIR[MAX];  
      int  i,j;
   double s[MAX];
   double min,Max;
   double optACIR,toptACIR,optPower,toptPower,tmp=0;
   double avg,avgPower;
   double *mid,*top;
   double SINRvim[MAX];
   double ItoVimUE[MAX],ItoVimsys[MAX], NBIOTUE_BW[MAX];
   
   
   
   FILE *out1=fopen(file1,"w" );
   FILE *out2=fopen(file2,"w" ); 
   FILE *out3=fopen(file3,"w" ); 
   FILE *out4=fopen(file4,"w" ); 
   FILE *out5=fopen(file5,"w" );
   FILE *out6=fopen(file6,"w" ); 
   //FILE *out7=fopen(file7,"w" );
  
  
  
   for(i=0;i<MAX;i++)
   {
        Baggressor[i]=(rand()%2000000)+1;
        //printf("Baggressor[%d]=%lf\n",i,Baggressor[i]);	
    }
    //ACS1=ACS[0];
   for(i=0;i<MAX;i++)
   { 
      ACLR[i]=(double)ACLR11;//(rand()%100)+1;
     // printf("ACLR[%d]=%lf\n",i,ACLR[i]);
    } 
    printf("\n\n");
   for(i=0;i<MAX;i++)
   {  
        Bvictim[i]=(rand()%180000)+1;                      
    }        
   for(i=0;i<MAX;i++)
   {     
        s[i]=10*(log10(Bvictim[i]/Baggressor[i]));           
        //printf("%lf\n",s[i]);              
        ACLReff[i]=ACLR[i]-s[i];            
        //printf("ACLReff[%3d]=%lf dB\n",i,ACLReff[i]);    
    }    
 
   /* -------------------------算有效值ACIR ----------------------------------------*/ 
   printf("\n\n");
   double sum1=0;
   for(i=0;i<MAX;i++)
   {
	    sum1=sum1+ACLReff[i];
	    avg=sum1/MAX;
     } 
   printf("平均ACLReff=%lf\n",avg);  
   for(i=0;i<MAX;i++) 
   {
   	    //for(j=1;j<100;j++)
	    ACSeff[i]=ACS11;
		//fprintf(out2,"ACSeff[%2d]=%lf\n",i,ACSeff[i]);
	} 	        
	// printf("ACSeff[%2d]=%lf\n",i,ACSeff[i]);
        
   for(i=0;i<MAX;i++) 
   {  
         if(ACLReff[i]<=ACSeff[i]) {  
            effACIR[i]=1/((1/ACLReff[i])+(1/ACSeff[i]));
            fprintf(out1,"effACIR[%d]=%lf\n",i,effACIR[i]);
              //fprintf(out2,"%lf\n",ACIR[i]);
              //printf("ACIR[%d]=%lf\n",i,ACIR[i]);        
         }
         else {
            effACIR[i]=ACLReff[i]; 
            fprintf(out1,"effACIR[%d]=%lf\n",i,effACIR[i]);      
            //fprintf(out2,"%lf\n",ACIR[i]);
            //printf("ACIR[%d]=%lf\n",i,ACIR[i]);
          } 
	} 
	printf("\n\n");
   
   /* -------------------------算SINR ---------------------------------------------*/ 
    double sum=0;  
    for(i=0;i<MAX;i++)      
    { 
       sum=sum+effACIR[i];
       //printf("sum=%lf\n",sum);
       avg=sum/MAX;          
    } 
	printf("平均ACIR=%lf\n",avg);
	double Av_SINRvim=0,Tot_SINRvim[MAX];
    for(i=0;i<MAX;i++)
	{
		ItoVimsys[i]=20;
	    //printf("ItoVimsys[%2d]=%lf\n",i,ItoVimsys[i]);
     }
    printf("--------------------------------------------\n");
	for(i=0;i<MAX;i++)
	{
	   ItoVimUE[i]= (ItoVimsys[i]/effACIR[i])*(Bvictim[i]/Baggressor[i]);
	   //printf("ACLReff[%3d]=%lf dB\n",i,ACLReff[i]);
	   //printf("ItoVimUE[%2d]=%lf\n",i,ItoVimUE[i]);
    }
    printf("--------------------------------------------\n");
	int k;
	//double k0[MAX];
	for(i=0;i<MAX;i++)
	{ 
	  SINRvim[i]= (double)Ps/(ItoVimUE[i] + (double)IsameFreinterfere + (double)Np);
	  //printf("SINRvim[%2d]=%lf\n",i,SINRvim[i]);
	  //k0[i] = SINRvim[i]*10;    /*K-mean會用*/
	  
	  //printf("%d\n",k);
	  fprintf(out2,"SINRvim[%2d]=%lf\n",i,SINRvim[i]);
    }
     printf("--------------------------------------------\n");

/* -------------------------算通道容量 C--------------------------------------------*/ 
	double C[MAX];
	double Tot_C=0;
	double Average_C=0;
	for(i=0;i<MAX;i++)
	{
	   C[MAX]=0;
    }
	for(i=0;i<MAX;i++)
	{   
	   C[i]=(15)*(log2(1+SINRvim[i]));
	   //printf("C[%2d]=%lf\n",i,C[i]);
	   Tot_C=Tot_C+C[i];
	   //printf("Tot_C=%lfk \n",Tot_C);
	   //
	   fprintf(out3,"C[%2d]=%lfK\n",i,C[i]);
     }  
    printf("--------------------------------------------\n");
    
    Average_C=Tot_C/MAX;
	if(Average_C<=64)
	   printf("Average_C=%lfK (bps)\n",Average_C);
	else
	   printf("超過!");
	   //printf("Average_C=%lfk (bps)\n",Average_C);	
				
	printf("\n\n");		   
	

/* -------------------------K-mean演算法開始 ----------------------------------------*/ 
    int i1,i2,i3,i4,i5;
    int t4,t5,t6;
	float t1,t2,t3;
	double k0[MAX];
    double k1[MAX];
    double k2[MAX];
    double k3[MAX]; 
     
    for(i1=0;i1<MAX;i1++)
    {
       k0[i1]=SINRvim[i1];
	   //printf("k0[%d]=%lf\n",i1,k0[i1]);
    }

    //initial means
    float m1=2.86;
    float m2=2.71;
    float m3=2.58;
    
     /*
    i1=(rand()%MAX)+1;
	m1=k0[i1]; 
	printf("m1=%lf\n",m1);
   
	i1=(rand()%MAX)+1;
	m2=k0[i1];
	printf("m2=%lf\n",m2); 
    i1=(rand()%MAX)+1;
	m3=k0[i1]; 
    printf("m3=%lf\n",m3);
    */
    
   double om1,om2,om3;    //old means
   do{ 
    //saving old means
       om1=m1;
       om2=m2;
       om3=m3;
    //creating clusters
       i1=i2=i3=i4=0;
       for(i1=0;i1<MAX;i1++)
       {
          //calculating distance to means
          t1=k0[i1]-m1;
          if(t1<0){
		       t1=-t1;
	  	  }
          t2=k0[i1]-m2;
          if(t2<0){
		      t2=-t2;
		   }
		  t3=k0[i1]-m3;
          if(t3<0){
		      t3=-t3;
		   } 
		   
		   
          if(t1<t2 || t1<t3){  //最靠近m1 
             
             k1[i2]=k0[i1];
             i2++;
            
            }
          else{                //最靠近m2
                if(t2<t3){      
                  k2[i3]=k0[i1];
                  i3++;
                  
			   }
			   else {          //最靠近m3 
			   	  k3[i4]=k0[i1];
                  i4++;        
			   }
            }
        }
        
        t2=0;
        //calculating new mean
        for(t4=0;t4<i2;t4++)
        {
           t2=t2+k1[t4];
         }
        m1=(double)t2/(double)i2;
        t2=0;
        for(t5=0;t5<i3;t5++)
        {
          t2=t2+k2[t5];
        }
        m2=(double)t2/(double)i3; 
        t2=0;
        for(t6=0;t6<i4;t6++)
        {
          t2=t2+k3[t6];
        }
        m3=(double)t2/(double)i4;
        
 
        //printing clusters
        printf("\nCluster 1: ");
        printf("\nm1=%.4f \n",m1);
        for(t1=0;t1<i2;t1++)
        {
           //printf("%lf\n",k1[t1]);
        } 
        printf("\n -----------------------------\n"); 
        
        printf("\nCluster 2: ");
		printf("\nm2=%.4f \n",m2);
        for(t1=0;t1<i3;t1++)
        {
           //printf("%lf\n",k2[t1]);
        }
        printf("\n ------------------------------");
             
        printf("\nCluster 3: ");
		printf("\nm3=%.4f \n",m3);
        for(t1=0;t1<i4;t1++)
        {
           //printf("%lf\n",k3[t1]);
        }
        printf("\n ------------------------------");
   
   }while(m1!=om1 && m2!=om2 && m3!=om3);
 
       printf("\n Clusters created\n");
 	
	/* -------------------------分群後算第一群capacity -----------------------------------*/ 
	double C1[i2];
	double Tot_C1=0;
	double Average_C1=0;
	for(i=0;i<i2;i++)
	{
	   C1[i]=0;
    }
	for(i=0;i<i2;i++)
	{   
	   C1[i]=(15)*(log2(1+k1[i]));
	   //printf("C1[%2d]=%lf\n",i,C1[i]);
	   Tot_C1=Tot_C1+C1[i];
	   //printf("Tot_C1=%lfk \n",Tot_C1);
	   //
	   fprintf(out4,"C1[%2d]=%lfK\n",i,C1[i]);
     }  
    printf("--------------------------------------------\n");
	Average_C1=Tot_C1/(double)i2;
	if(Average_C1<=64)
	   printf("Average_C1=%lfK (bps)\n",Average_C1);
	else
	   printf("超過!");
	   //printf("Average_C1=%lfk (bps)\n",Average_C1);	
				
	printf("\n\n");	

	/* -------------------------分群後算第2群capacity -----------------------------------*/ 
	double C2[i3];
	double Tot_C2=0;
	double Average_C2=0;
	
	for(i=0;i<i3;i++)
	{
	   C2[i]=0;
    }
	for(i=0;i<i3;i++)
	{   
	   C2[i]=(15)*(log2(1+k2[i]));
	   printf("C2[%2d]=%lf\n",i,C2[i]);
	   Tot_C2=Tot_C2+C2[i];
	   //printf("Tot_C2=%lfk \n",Tot_C2);
	   //
	   fprintf(out5,"C2[%2d]=%lfK\n",i,C2[i]);
     }  
    printf("--------------------------------------------\n");
	Average_C2=Tot_C2/(double)i3;
	if(Average_C2<=64)
	   printf("Average_C2=%lfK (bps)\n",Average_C2);
	else
	   printf("超過!");
	   //printf("Average_C2=%lfk (bps)\n",Average_C2);	
				
	printf("\n\n");	

	/* -------------------------分群後算第3群capacity -----------------------------------*/ 
	double C3[i4];
	double Tot_C3=0;
	double Average_C3=0;
	for(i=0;i<i4;i++)
	{
	   C3[i]=0;
    }
	for(i=0;i<i4;i++)
	{   
	   C3[i]=(15)*(log2(1+k1[i]));
	   //printf("C3[%2d]=%lf\n",i,C3[i]);
	   Tot_C3=Tot_C3+C3[i];
	   //printf("Tot_C1=%lfk \n",Tot_C1);
	   //
	   fprintf(out6,"C3[%2d]=%lfK\n",i,C3[i]);
     }  
    printf("--------------------------------------------\n");
    //printf("Tot_C4=%lfk \n",Tot_C1);
	Average_C3=Tot_C3/(double)i4;	
	if(Average_C3<=64)
	   printf("Average_C3=%lfK (bps)\n",Average_C3);
	else
	   printf("超過!");
	  // printf("Average_C3=%lfk (bps)\n",Average_C3);	
				
	printf("\n\n");	
	/* -------------------------分群後算總capacity -----------------------------------*/ 
   
	double average_Capacity;
	average_Capacity = (Average_C1)*((double)i2/(double)MAX) 
	+ (Average_C2)*((double)i3/(double)MAX)
	+ (Average_C3)*((double)i4/(double)MAX);
	printf("KM演算法平均average_Capacity=%lf K(bps)\n",average_Capacity);
	
	
	
	
	
	
    /* -------------------------分群後利用Q-Learning去做功率調配 -----------------------------------*/
	
    double I;     /*The quantization precision and range of I*/  
	double SINRth=3; /*maximum transmit SINR of NBIOT*/
	double x=1;   /*微調常數*/
	FILE *datafile;
   //srand((unsigned) time(NULL));
    datafile = fopen("C2_KM_Q.csv", "w");
    if(datafile == NULL){
        printf("Cannot open file\n");
        return 1;
    }
    //fprintf(datafile, "%d,%d\n", 0, 0);
    /*---------------------------------------current_state-------------------------------*/
    printf("受干擾的device個數=%d\n\n",i3);    //降低capacity 的device個數 
    
    for(i=0;i<i3;i++)
	{
		printf("k2[%d]=%lf\n",i,k2[i]);
        current_state[i].action[0].orign = k2[i];   //K2陣列存放C2的SINR
        //printf("current_state[%d].action[0].orign=%lf\n",i,current_state[i].action[0].orign);     
	 }
    /*---------------------------------------New_state-------------------------------*/
   
    double current[MAX];
    double final_state[MAX];
   
    for(i=0; i<MAX; i++)
	{
	  current[i]=current_state[i].action[0].orign;
	  //printf("current[%d]=%lf\n",i,current[i]);
	  //printf("selectedAct=%lf\n",selectAction(current[i]));
	  final_state[i]= calcQValue(current[i],selectAction(current[i])); 
      printf("final_state[%d]=%lf\n",i,final_state[i]);
       if(current_state[i+1].action[0].orign == 0){
            break;
        }
    }
    
    
  /*----------------------------KMean + Q-learning演算法後通道容量-------------------------------*/
     
    double after_C2[MAX];
    double Tot_after_C2=0;
    double Average_after_C2=0;
     
    for(i=0; i<i3; i++)
	{
	   after_C2[i]=15*(log2(1+final_state[i]));
       printf("after_C2[%d]=%lf\n",i,after_C2[i]);   
    }
    for(i=0; i<i3; i++)
	{
      Tot_after_C2 += after_C2[i];  
    } 
    Average_after_C2 = Tot_after_C2/i3;
    printf("Average_after_C2=%lf\n",Average_after_C2);
    
    
    
    
     fclose(datafile); 
     fclose(out1);
     fclose(out2);
     fclose(out3);    
	 fclose(out4); 
	 fclose(out5);
	 fclose(out6);
	// fclose(out7);
	
	
    system("PAUSE");	
    return 0;
}

/* -------------------------分群後利用Q-Learning去做功率調配 -----------------------------------*/ 

  
/*-----------------------------------------reward----------------------------------*/
   double Reward(double i) 
   { 
	   double SINRth=3; /*maximum transmit SINR of NBIOT*/
	   double reward=-1; 
	   int cnt1=0,cnt2=0;  

            if(i <= SINRth){
		         reward = exp(i/(double)K);
		        // printf("reward_table=%lf\n",reward);
		         cnt1++;  
            }
            else{
                //reward=-1;
                cnt2++;
            	//printf("reward_table=%lf\n",reward);
	 	    }	
       
      //printf("cnt1=%d\n",cnt1);
      // printf("cnt2=%d\n",cnt2);
      
      return reward;
     
   }


/*----------------------------------------selectedAct----------------------------------*/
    
int selectAction(double s)
{
	double SINRth=3; /*maximum transmit SINR of NBIOT*/
	double x=1;   /*微調常數*/
    /*		
        if(current_state[(int)s].action[0].qlearn < SINRth-x){
		    return I=0;      
        }
        else if((current_state[(int)s].action[0].qlearn>= SINRth-x)&&(current_state[(int)s].action[0].qlearn <= SINRth+x)){
            return I=1;	
		}
        else if(current_state[i].action[0].qlearn> SINRth+x){
        	return I=2;	
		}	
    */ 
        //printf("s=%lf\n",s);
        if(s < SINRth-x) {  return Reward(1); 
		}
        else if((s>= SINRth-x)&&(s<= SINRth+x)){  return Reward(2);	
		}
        else if(s > SINRth+x){	return Reward(3);	
		}

}
    
double getMaxQValue(double I) 
{
    double max = I;
    int i;
   // printf("I=%lf\n",I);
    for (i = 0; i < MAX; i++) {
        if (max < I) {
            max = I;
        }
    }
    //printf("max=%lf\n",max);
    return max;
}
 /*---------------------------------------optimal_state-------------------------------*/
double calcQValue(double i, double I) 
{

    static double alpha = 1.25;
    /*學習率*/
    static double gamma = 1.4;
    double r;
    double reward_q;
    int j=0;
  
    r = Reward(i);
    //printf("r=%lf\n",r);
    //printf("getMaxQValue=%lf\n",getMaxQValue(I));
    //printf("i=%lf\n",i);
    
    current_state[j].action[0].orign = i;
    
	reward_q = alpha * (r + gamma * getMaxQValue(I) - current_state[j].action[0].orign);
    current_state[j].action[0].orign +=reward_q;
    //printf("current_state[%d].action[0].orign=%lf\n",j,current_state[j].action[0].orign);
	//printf("reward_q=%lf\n",reward_q);
	//current_state[(int)i].action[(int)I].qlearn += reward_q;
    
    //printf("current_state[(int)i].action[(int)I].qlearn=%lf\n",current_state[(int)i].action[(int)I].qlearn);
    return current_state[j].action[0].orign;
}






