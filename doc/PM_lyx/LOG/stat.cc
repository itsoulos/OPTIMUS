# include <stdio.h>
# include <stdlib.h>

int main(int argc,char **argv)
{
	FILE *fp=fopen(argv[1],"r");
	if(!fp) return 1;
	int count1=0,count2=0,count3=0;
	int x;
	while(fscanf(fp,"%d",&x)>0)
	{
		count1+=(x==1);
		count2+=(x==2);
		count3+=(x==3);
	}
	int total = count1+count2+count3;
	printf("Percent[mean]=%.2lf%% Percent[best]=%.2lf%% Percent[doubleBox]=%.2lf%%\n",
			count1*100.0/total,count2*100.0/total,count3*100.0/total);
	fclose(fp);
	return 0;
}
