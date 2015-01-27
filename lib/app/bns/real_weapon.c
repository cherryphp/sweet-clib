/**
  *
  * bns 获取真实武器数据
  * gcc -o real_weapon real_weapon.c bns_regex.c chtbl.c list.c bns_hash.c
  *
  **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bns_regex.h"
#include "chtbl.h"
#include "list.h"
#include "bns_hash.h"

#define LINE_BUF 8192*2
#define TITLE_BUF 8192

char *my_csv(char *s1, char *s2){
	char *p;
	p = s2;
	char *enter = "\n";
	char *space = " ";

	while (*s1 != *enter && (*s1 == *space ||  !isspace(*s1))  ){
		*p = *s1;
		p++;
		s1++;
		//break;
	}

	//printf("s1:%s\n", s1);
	if (*s1 != *enter && isspace(*s1)){
		s1++;
	}

	if (*s1 == *enter){
		//printf("123\n");
	}
	
	//printf("s2:%s\n", s2);
	//printf("p:%s\n", p);
	//printf("s1:%s\n", s1);
	return s1;
}


int main(int argc, char *argv[])
{
	FILE *fp;
	int line_num;
	char strline[LINE_BUF+1];
	char tmp_title[TITLE_BUF+1];
	char *title[300];
	char *ss1;
	char **tp;
	int n,i;
	char *enter = "\n";
	int block = 0;
	char *atk_min;

	char *regex_patten = "<[0-9a-zA-Z]+\\s[0-9a-zA-Z_=\"\\.]+>(.*)</font>";
	char *regex_icon = "([0-9]+),[0-9]+,[0-9]+";

	CHTbl 		htbl;
	SweetListKv *data;

	int tmp_id;

	if (argc < 2){
		fprintf(stderr,"error: no file!\n");
		exit(1);
	}

	if (argc < 3){
		fprintf(stderr, "error: no start_weapon_id\n");
		exit(1);
	}

	if (argc < 4){
		fprintf(stderr, "error: no end_weapon_id\n");
		exit(1);
	}

	if ((fp = fopen(argv[1],"r")) == NULL){
		printf("error!");
		return -1;
	}

	// init
	create_icon_hash(&htbl);

	//print_table(&htbl);

	/*if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL)
		return 1;
	data->key = "8583";
	data = chtbl_find_kv(&htbl, &data);
	printf("%s\n",data->value);

	free(data);*/
	// free hash table
	//remove_icon_hash(&htbl);

	line_num = 1;
	while (!feof(fp)){
		memset(strline, 0, sizeof(strline));
		fgets(strline, LINE_BUF,fp);
		//printf("%s\n\n",strline);
		
		ss1 = (char*)malloc(sizeof(char)*(LINE_BUF+1));
		sprintf(ss1, "%s", strline);
		
		n = 0;
		block = 0;

		if (line_num == 1){
			while(*ss1 != *enter){
				memset(tmp_title,0,sizeof(char)*(TITLE_BUF+1));
				sprintf(ss1, "%s", my_csv(ss1,tmp_title));
				
				title[n] = (char*)malloc(sizeof(char)*(strlen(tmp_title)+1)); // 动态分配数组指针
				//title[n] = tmp_title;	// 拷贝指针地址
				strcpy(title[n],tmp_title); // 拷贝指针中的具体内容 
				//printf("%s\n",title[n]);
				n++;
			}
		}else {
			atk_min = 0;
			tmp_id = 0;
			while (*ss1 != *enter){
				memset(tmp_title, 0 ,sizeof(char)*(TITLE_BUF+1));
				sprintf(ss1, "%s", my_csv(ss1,tmp_title));
				if (n == 0 && (atoi(tmp_title) >= atoi(argv[2]) && atoi(tmp_title) <= atoi(argv[3]) )){
				//if (n == 0 && (atoi(tmp_title) >= 623934 && atoi(tmp_title) <= 624058 )){
				//if (n == 0 && (atoi(tmp_title) >= 2607750 && atoi(tmp_title) <= 2607877 )){
					block = 1;
				}
				if (block){
					switch (n){
						case 0:
							tmp_id = atoi(tmp_title);
							//printf("{\"_id\":\"%s\",",tmp_title);
							//printf("\"weapons\":[\"%s\"],",tmp_title);
							break;
						case 1:
							if (atoi(tmp_title) > 1){
								printf("{\"_id\":\"%d_%s\",", tmp_id, tmp_title);
								printf("\"weapons\":[\"%d\"],",tmp_id);
								printf("\"collectweapon_id\":\"%d_%s\",", tmp_id, tmp_title);
							}else {
								printf("{\"_id\":\"%d\",",tmp_id);
								printf("\"weapons\":[\"%d\"],",tmp_id);
								printf("\"collectweapon_id\":\"%d\",",tmp_id);
							}
							printf("\"level\":%s,",tmp_title);
							break;
						case 229:
							printf("\"name\":\"%s\",",tmp_title);
							break;
						case 34:
							if (!strcmp(tmp_title,"7")){
								printf("\"item_grade\":\"6\",");
							}else{
								printf("\"item_grade\":\"%s\",",tmp_title);
							}
							break;
						case 8:
							if (!strcmp(tmp_title,"sword(1)"))
								printf("\"武器类型\":\"2\","); //剑
							else if (!strcmp(tmp_title,"gauntlet(2)"))
								printf("\"武器类型\":\"3\","); //拳套
							else if (!strcmp(tmp_title,"axe(3)"))
								printf("\"武器类型\":\"7\","); //斧头
							else if (!strcmp(tmp_title,"staff(4)"))
								printf("\"武器类型\":\"9\","); //法杖
							else if (!strcmp(tmp_title,"aura-bangle(5)"))
								printf("\"武器类型\":\"4\","); //彩绫
							else if (!strcmp(tmp_title,"dagger(6)"))
								printf("\"武器类型\":\"10\","); //短刀
							break;
						case 90:
							printf("\"命中\":%s,",tmp_title);
							break;
						case 94:
							printf("\"穿刺\":%s,",tmp_title);
							break;
						case 98:
							printf("\"暴击\":%s,",tmp_title);
							break;
						case 102:
							printf("\"暴击防御\":%s,",tmp_title);
							break;
						case 105:
							printf("\"闪避\":%s,",tmp_title);
							break;
						case 108:
							printf("\"格挡\":%s,",tmp_title);
							break;
						case 123:
							printf("\"最大生命\":%s,",tmp_title);
							break;
						case 267:
							if (filter_regex(regex_patten,tmp_title) != NULL){
								printf("\"攻击效果\":\"%s\"}\n",filter_regex(regex_patten,tmp_title));
							}else {
								printf("\"攻击效果\":\"\"}\n");
							}
							break;
						case 25:
							printf("\"需要等级\":%s,",tmp_title);
							break;
						case 28:
							if (!strcmp(tmp_title,"0,0"))
								printf("\"职业\":\"\",");
							else if (!strcmp(tmp_title,"1,0"))
								printf("\"职业\":\"剑师\",");
							else if (!strcmp(tmp_title,"1,8"))
								printf("\"职业\":\"剑师\",\"灵剑士\",");
							else if (!strcmp(tmp_title,"2,0"))
								printf("\"职业\":\"拳师\",");
							else if (!strcmp(tmp_title,"3,0"))
								printf("\"职业\":\"气功师\",");
							else if (!strcmp(tmp_title,"5,0"))
								printf("\"职业\":\"力士\",");
							else if (!strcmp(tmp_title,"6,0"))
								printf("\"职业\":\"召唤师\",");
							else if (!strcmp(tmp_title,"7,0"))
								printf("\"职业\":\"刺客\",");
							else if (!strcmp(tmp_title,"7,9"))
								printf("\"职业\":\"刺客\",\"咒术师\",");
							else if (!strcmp(tmp_title,"8,0"))
								printf("\"职业\":\"灵剑士\",");
							else if (!strcmp(tmp_title,"9,0"))
								printf("\"职业\":\"咒术师\",");
							break;
						case 14:
							printf("\"不可交易\":\"%s\",",tmp_title);
							if (!strcmp(tmp_title,"y")){
								printf("\"归属\":\"2\",");
							}
							break;
						case 4:
							printf("\"售价\":%s,",tmp_title);
							break;
						case 24:
							printf("\"gear_score\":%s,",tmp_title);
							break;
						case 232:
							if ((data = (SweetListKv *)malloc(sizeof(SweetListKv))) == NULL){
								return 1;
							}
							data->key = (char *)malloc(sizeof(char)*(strlen(filter_regex(regex_icon,tmp_title))+1));
							strcpy(data->key, filter_regex(regex_icon,tmp_title));
							
							//printf("icon:%s,%s,%s\n",filter_regex(regex_icon,tmp_title),data->key,chtbl_find_kv(&htbl, &data)->value);
							printf("\"icon\":\"%s\",",chtbl_find_kv(&htbl, &data)->value);
							free(data);

							break;
						case 35:
							printf("\"耐久度\":\"100/100\",");
							printf("\"disable\":0,");
							printf("\"growth_level_exp\":\"0\",");
							printf("\"growth_recipe_cost\":0,");
							printf("\"mesh_id\":0,");
							printf("\"hidden\":0,");
							printf("\"集中\":0,");
							printf("\"额外伤害\":0,");
							break;
						case 81:
							atk_min = (char *)malloc(sizeof(char)*(strlen(tmp_title)+1));
							strcpy(atk_min, tmp_title);
							//printf("攻击力最小:%s\n",atk_min);
							break;
						case 82:
							printf("\"攻击力\":%d,",(atoi(atk_min) + atoi(tmp_title))/2);
							free(atk_min);
							break;
						case 85:
							printf("\"防御\":%s,",tmp_title);
							break;
						case 260:
							if (filter_regex(regex_patten,tmp_title) != NULL){
								printf("\"描述\":\"%s\",",filter_regex(regex_patten,tmp_title));
							}else {
								printf("\"描述\":\"\",");
							}
							break;
						case 175:
							printf("\"恢复\":%s,",tmp_title);
							printf("\"战斗中恢复\":%s,",tmp_title);
							break;
						default:
							break;
					}
					//printf("#%d %s: %s\n",n,title[n],tmp_title);
				}
				n++;
			}
		}


		free(ss1);
		line_num++;
		/*if (line_num > 2){
			break;
		}*/
	}
	
	for (i = 0; i < 300; i++){
		free(title[i]);
	}

	// free hash table
	remove_icon_hash(&htbl);

	fclose(fp);
	return(0);
}