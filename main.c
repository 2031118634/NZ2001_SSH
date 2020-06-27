#include<stdio.h>
#include<stdlib.h>
#include"song.h"
#include"link.h"
#include"start_mplayer.h"
#include"console.h"
int main(int argc,char *argv[])
{
	char *file_data = NULL;
	int file_row=0;	
	
	//都取歌词文件
	file_data = read_song_file();
	
	char *buf[128] ={file_data,NULL};
	//按行切割
	strtok_data(buf,&file_row);
	//printf("%d\n",row);
	
	//逐行分析
	
	LRC *head=NULL;
	head=analysis(buf,head,file_row);
	//print_link(head);
	
	//启动maplayer
	char *path="./song/jiandanai.mp3";
	mplayer_play(path);
	//模拟时钟播放
	clock_show(buf,head);
	
	free_link(head);
	//printf("链表已释放！\n");
	cusor_show();
	if(file_data != NULL)
	{
		free(file_data);

		file_data = NULL;
	}
	return 0;
}
hello world
hello tangxinling
