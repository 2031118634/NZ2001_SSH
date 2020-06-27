#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include"song.h"
#include"link.h"
#include"console.h"

char *read_song_file()
{
	char *data=NULL;
	long file_length=0;
	FILE *fp=NULL;
	
	fp=fopen("./lrc/简单爱.lrc","r");
	if(fp == NULL)
	{
		perror("fopen");
		return NULL;
	}
	
	fseek(fp,0,2);
	file_length=ftell(fp);
	rewind(fp);
	
	data=(char *)calloc(1,file_length);
	if(data == NULL)
	{
		perror("calloc");
		return NULL;
	}
	
	fread(data,file_length,1,fp);
		
	fclose(fp);
	return data;
	
}

void strtok_data(char **arr,int *num)
{
	int i=0;
	
	while(1)
	{
		arr[i] = strtok(arr[i],"\r\n");
		if(arr[i] == NULL)
			break;
		i++;
	}
	*num=i;
}

LRC *analysis(char **buf,LRC *head,int num)
{	
	int i;
	for(i=4;i<num;i++)
	{
		char ch;		
		
		sscanf(buf[i],"[%c", &ch);//[]中的第一个字符	
		if((ch>='0') && (ch<='9') )
		{
			char *str_lrc = buf[i];
			
			//1、寻找歌词的位置
			while(*str_lrc == '[')
				str_lrc +=10;//str_lrc指向了歌词的位置
			
			char *str_time=buf[i];//逐个时间分析
			while(*str_time=='[')
			{
				int m=0,s=0,ms=0;
				int time=0;

				sscanf(str_time,"[%d:%d.%d]",&m,&s,&ms);		
				time=m*60+s;
				
				//将时间 和 歌词  一一对应 放入 结构体
				LRC tmp;
				tmp.time=time;
				strcpy(tmp.lrc,str_lrc);
				
				//调用链表的有序插入函数
				head =insert_link(head,tmp);
				str_time  += 10;//分析下一个时间
			}
			
		}
	}		
	return head;
}

int total_time(LRC *head)
{
	int time=0;
	LRC *pi=head;
	while(pi->next!=NULL)
	{
		pi=pi->next;
	}
	time=pi->time;
	
	return time;		
}
void show_four_lines(char **buf)
{
	int i=0;
	set_fg_color(COLOR_BLUE);
	while(1)
	{
		cusor_moveto(30, 2+i);  
		//方法1
		char tmp[128]="";//用来保存前4行：前面的字符串
		char tmp1[128]="";//用来保存前4行：后面的字符串
		char ch;		
		
		//[ti:简单爱]
		sscanf(buf[i],"[%c", &ch);//[]中的第一个字符
		
		if((ch<='z' && ch>='a')|| (ch <='Z' && ch >='A'))
		{
			sscanf(buf[i],"%*c%2s:%[^]]", tmp,tmp1);
			
			if(strcmp(tmp,"ti")==0)
			{
				       
				printf("歌名:%s\n",tmp1);
			}
			else if(strcmp(tmp,"ar")==0)
			{
				printf("歌手:%s\n",tmp1);
			}
			else if(strcmp(tmp,"al")==0)
			{
				printf("专辑:%s\n",tmp1);
			}
			else
			{
				printf("制作:%s\n",tmp1);
			}
			i++;
		}	
		else
		{
			break;
		}
	}
}
void clock_show(char **buf,LRC *head)
{
	int count=0,i=0;
	int all_time=total_time(head)+20;
    char str[128]="";
	int tmp_time=all_time/15;
	
	char buf1[128]="";
	char buf2[128]="";
	char buf3[128]="";
	char buf4[128]="";
	
	clear_screen();//清屏幕内容
	cusor_hide();
	show_four_lines(buf);
	
	MYTIME t;
	t.minute =0;
	t.second =0;
	
	while(1)
	{
		sleep(1);
		t.second++;
		if(t.second >= 60)
		{
			t.second=0;
			t.minute++;
			if(t.minute>= 60)
				t.minute=0;
		}
		if((t.minute*60+t.second)==tmp_time)
		{
			cusor_moveto(30, 12);
			strcat(str,"-");
			printf("%s\n",str);
			tmp_time=tmp_time+(all_time/15);
			i++;
		}	
			
		LRC *ret=search_link(head,t.minute*60+t.second);
		
		count=t.minute*60+t.second;
		if(ret!=NULL)
		{
			stpcpy(buf1,buf2);
			stpcpy(buf2,buf3);
			stpcpy(buf3,buf4);
			stpcpy(buf4,ret->lrc);
			
		    set_fg_color(COLOR_GREEN);
			cusor_moveto(26, 7);
			printf("%s                   \n",buf1);
			
			cusor_moveto(26, 8);
			printf("%s                   \n",buf2);
			
			cusor_moveto(26, 9);
			printf("%s                   \n",buf3);
			
			cusor_moveto(26, 10);
			set_fg_color(COLOR_RED);
			printf("%s                   \n",buf4);
			set_fg_color(COLOR_GREEN);
			fflush(stdout);		
		}
		cusor_moveto(24, 12);
        printf("%02d:%02d<",t.minute,t.second);	
		cusor_moveto(41, 12);
		printf(">%02d:%02d\n",all_time/60,all_time%60);
		
		fflush(stdout);
		
		if(count==(all_time))
			break;
	} 
}
//void cartoon()//动画











