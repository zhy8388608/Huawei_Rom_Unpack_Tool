#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

unsigned long long MAX_BUFFER=2<<28;

char name[32];
char head[]={0x55,0xAA,0x5A,0xA5};
char back[]=".img";

int main(int argc,char * argv[])
{
	FILE* filein=fopen(argv[1],"rb");
	FILE* fileout;
	if(!filein)
	{
		printf("Can't open file: %s\n",argv[1]);
		system("pause");
		return 0;
	}
	void* buffer=malloc(MAX_BUFFER);
	int count=0,tmp=0;
	char ctmp;
	uint32_t headsize,romsize;
	int flag=0;
	int i,j;
	while(!feof(filein))
    {
    	fread(&ctmp,1,1,filein);
    	if(ctmp==head[count])
    	{
    		count++;
    		//Find head
    		if(count==3)
			{
				count=0;
				//Read head
				_fseeki64(filein,1,SEEK_CUR);
				fread(&headsize,4,1,filein);
				_fseeki64(filein,16,SEEK_CUR);
				fread(&romsize,4,1,filein);
				_fseeki64(filein,32,SEEK_CUR);
				fread(name,32,1,filein);
				_fseeki64(filein,headsize-92,SEEK_CUR);
				printf("%s %dKB\n",name,(romsize>>10)+1);
				//Generate name
				for(i=0;i<32;i++)
					if(name[i]=='\0')
					{
						for(j=0;j<strlen(back);j++) 
							name[i+j]=back[j];
						break;
					}else if(name[i]>='A'&&name[i]<='Z')
						name[i]=name[i]-'A'+'a';
				//Dump file
				fileout=fopen(name,"wb");
				tmp=romsize/MAX_BUFFER;
				for(i=0;i<tmp;i++)
				{
					fread(buffer,MAX_BUFFER,1,filein);
					fwrite(buffer,MAX_BUFFER,1,fileout);
				}
				tmp=romsize%MAX_BUFFER;
				fread(buffer,tmp,1,filein);
				fwrite(buffer,tmp,1,fileout);
				fclose(fileout);
			} 
		}else count=0;
	}
	fclose(filein);
	free(buffer);
	printf("\nDone.\n");
	system("pause");
	return 0;
}
