#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char mne[11][10] = {"STOP","ADD","SUB","MULT","MOVER","MOVEM","COMP","BC","DIV","READ","PRINT"};
char ad[5][10] = {"START","END","ORIGIN","EQU","LTORG"};
char conop[6][10] = {"LT","LE","EQ","GT","GE","ANY"};
char reg[4][10] = {"AREG","BREG","CREG","DREG"};
char DL[2][5] = {"DC","DS"};
int indtemp,litindx,location,lindxtmp,pooltab[10];
struct SYMTAB
{
	int index,addr;
	char sname[20];
}stable[30];

struct LITTAB
{
	int index,addr;
	char lname[20];
}ltable[30];


int searchIS(char *nmo)
{
	int i;
	for(i=0;i<11;i++)
	{
		if(strcmp(nmo,mne[i])==0)
		{
			return i;
		}
	}
	return -1;
}

int searchREG(char *Reg)
{
	int i;
	for(i=0;i<11;i++)
	{
		if(strcmp(Reg,reg[i])==0)
		{
			return i+1;
		}
	}
	return -1;
}

int searchAD(char *Adir)
{
	int i;
	for(i=0;i<5;i++)
	{
		if(strcmp(Adir,ad[i])==0)
		{
			return i+1;
		}
	}
	return -1;
}

int searchDL(char *Dl)
{
	int i=0;
	for(i=0;i<5;i++)
	{
		if(strcmp(Dl,DL[i])==0)
		{
			return i+1;
		}
	}
	return -1;
}

int searchOPE(char *ope)
{
	int i=0;
	for(i=0;i<5;i++)
	{
		if(strcmp(ope,conop[i])==0)
		{
			return i+1;
		}
	}
	return -1;
}

int searchSYM(char *sym)
{
	int i=0;
	for(i=0;i<30;i++)
	{
		if(strcmp(sym,stable[i].sname)==0)
		{
			return i;
		}
	}
	return -1;
}

int checkLit(char *lit)
{
	if(strstr(lit,"='"))
		return 1;
	return -1;
}

void passone()
{
	int count,indx,sindx,i,poolindx=1,temp=0;
	char c0[20],c1[20],c2[20],c3[20],c4[20],lnm[10];
	indtemp = 0;
	litindx = 0;
	lindxtmp = 1;
	FILE *fp,*out;
	fp = fopen("ABC.asm","r");
	out = fopen("IC.txt","w");
	//stable = fopen("SYM.txt","w");
	if(fp == NULL)
	{
		printf("\nFile Is Not Created\n");
	}
	else if(out == NULL)
	{
		printf("\nOUTPUT File Is Not Created\n");
	}
	else
	{
		while(fgets(c0,100,fp))
		{
			count = sscanf(c0,"%s %s %s %s",c1,c2,c3,c4);
			switch(count)
			{
				case 1:
					indx = searchIS(c1);
					if(indx==0)
					{
						fprintf(out,"(IS, %02d)\n",indx);
						location++;
					}
					else
					{
						indx = searchAD(c1);
						if(indx == 2 || indx == 5)
							fprintf(out,"(AD, %02d)\n",indx);
								pooltab[poolindx++] = lindxtmp;
								for(i=lindxtmp;i<=litindx;i++)
									{
										ltable[i].addr = location;
										fprintf(out,"(DL, 01) (C, %s)\n",ltable[i].lname);
										location++;
									}
								lindxtmp = litindx+1;
					}
				break;
				case 2:
					indx = searchIS(c1);
					if(indx==9 || indx==10)
					{
						fprintf(out,"(IS, %02d)",indx);
						location++;
						sindx = searchSYM(c2);
						if(sindx>0)
						{
							fprintf(out," (S, %d)\n",sindx);
						}
						else if(sindx==-1)
						{
							indtemp++;
							stable[indtemp].index = indtemp;
							strcpy(stable[indtemp].sname,c2);
							sindx = searchSYM(c2);
							fprintf(out," (S, %d)\n",sindx);
						}
					}
					else
					{
						indx = searchAD(c1);
						if(indx == 1 || indx == 3)
						{
							if(indx == 1)
								location = atoi(c2);
							if(indx == 3)
								location = atoi(c2);
							fprintf(out,"(AD, %02d) (C, %d)\n",indx,location);
						}
					}
				break;
				case 3:
					indx = searchIS(c1);
					if(indx > 0 && indx < 9)
					{
						fprintf(out,"(IS, %02d)",indx);
						location++;
						indx = searchOPE(c2);
								if(indx>0 && indx<6)
								{
									fprintf(out," %d",indx);
									sindx = checkLit(c3);
									if(sindx == -1)
									{
										sindx = searchSYM(c3);
										if(sindx>0)
										{
											fprintf(out," (S, %d)\n",sindx);
											//if(stable[sindx].addr==0)
											//stable[sindx].addr = location;
										}
										else if(sindx==-1)
										{
											indtemp++;
											stable[indtemp].index = indtemp;
											strcpy(stable[indtemp].sname,c3);
											sindx = searchSYM(c3);
											fprintf(out," (S, %d)\n",sindx);
										}
									}
									else
									{
										sprintf(lnm,"%c%c",c3[2],c3[3]);
										litindx++;
										ltable[litindx].index = litindx;
										strcpy(ltable[litindx].lname,lnm);
										fprintf(out," (L, %d)\n",litindx);
									}
								}
								else{
									indx = searchREG(c2);
									if(indx>0 && indx<5)
									{
										fprintf(out," %d",indx);
										sindx = checkLit(c3);
										if(sindx == -1)
										{
											sindx = searchSYM(c3);
											if(sindx>0)
											{
												fprintf(out," (S, %d)\n",sindx);
												if(stable[sindx].addr == 0)
												stable[sindx].addr = location;
											}
											else if(sindx==-1)
											{
												indtemp++;
												stable[indtemp].index = indtemp;
												strcpy(stable[indtemp].sname,c3);
												sindx = searchSYM(c3);
												fprintf(out," (S, %d)\n",sindx);
											}
										}
										else
										{
										    sprintf(lnm,"%c%c",c3[2],c3[3]);
											litindx++;
											ltable[litindx].index = litindx;
											strcpy(ltable[litindx].lname,lnm);
											fprintf(out," (L, %d)\n",litindx);
										}
									}
								}
					}
					else
					{
						indx = searchAD(c2);
						if(indx == 4)
						{
							fprintf(out,"(AD, %02d)\n",indx);
							sindx = searchSYM(c3);
							if(sindx > 0)
							{
								temp = stable[sindx].addr;
								sindx = searchSYM(c1);
								if(sindx > 0)
								{
									stable[sindx].addr = temp;
								}
								else
								{
									indtemp++;
									stable[indtemp].index = indtemp;
									strcpy(stable[indtemp].sname,c1);
									stable[indtemp].addr = temp;
									
								}
							}
							else
							{
								printf("\nError\n");
							}
						}
						else{
							indx = searchDL(c2);
							if(indx == 1 || indx ==2)
							{
								fprintf(out,"(DL, %02d)",indx);
								if(indx==1)
								{
									
									fprintf(out," (C, %d)\n",atoi(c3));
									sindx = searchSYM(c1);
									if(sindx>0)
									{
										if(stable[sindx].addr == 0)
										stable[sindx].addr = location;
									}
									else if(sindx==-1)
									{
										indtemp++;
										stable[indtemp].index = indtemp;
										strcpy(stable[indtemp].sname,c1);
										sindx = searchSYM(c1);
										stable[sindx].addr = location;
									}
									location++;
								}
								else if(indx==2)
								{
									fprintf(out," (C, %03d)\n",atoi(c3));
									sindx = searchSYM(c1);
									if(sindx>0)
									{
										if(stable[sindx].addr == 0)
										stable[sindx].addr = location;
									}
									else if(sindx==-1)
									{
										stable[indtemp].index = indtemp;
										strcpy(stable[indtemp].sname,c1);
										sindx = searchSYM(c1);
									}
									location = location + atoi(c3);
								}
							}
						}
					}
				break;
				case 4:
					sindx = searchSYM(c1);
					if(sindx>0)
					{
						//if(stable[sindx].addr==0)
						stable[sindx].addr = location;
					}
					else if(sindx==-1)
					{
						indtemp++;
						stable[indtemp].index = indtemp;
						strcpy(stable[indtemp].sname,c1);
						sindx = searchSYM(c1);
						if(stable[sindx].addr == 0);
							stable[sindx].addr = location;
					}
					indx = searchIS(c2);
					if(indx>0 && indx<9)
					{
						fprintf(out,"(IS, %02d)",indx);
						location++;
						indx = searchOPE(c3);
								if(indx>0 && indx<6)
								{
									fprintf(out," %d",indx);
								}
								else{
									indx = searchREG(c3);
									if(indx>0 && indx<5)
									{
										fprintf(out," %d",indx);
										sindx = checkLit(c4);
										if(sindx == -1)
										{
											sindx = searchSYM(c4);
											if(sindx>0)
											{
												fprintf(out," (S, %d)\n",sindx);
											}
											else if(sindx==-1)
											{
												indtemp++;
												stable[indtemp].index = indtemp;
												strcpy(stable[indtemp].sname,c4);
												sindx = searchSYM(c4);
												fprintf(out," (S, %d)\n",sindx);
											}
										}
										else
										{
											sprintf(lnm,"%c%c",c4[2],c4[3]);
											litindx++;
											ltable[litindx].index = litindx;
											strcpy(ltable[litindx].lname,lnm);
											fprintf(out," (L, %d)\n",litindx);
										}
									}
								}
					}
				break;
			}
		}
	}
	printf("\n**********SYMBOL TABLE**********\n");
	printf("\nINDEX\tSYMBOL\tADDRESS\n");
	for(i=1;i<=indtemp;i++)
		printf("%d\t%s\t%d\n",stable[i].index,stable[i].sname,stable[i].addr);
	printf("\n**********LITERAL TABLE**********\n");
	printf("\nINDEX\tLITERAL\tADDRESS\n");
	for(i=1;i<=litindx;i++)
		printf("%d\t%s\t%d\n",ltable[i].index,ltable[i].lname,ltable[i].addr);
	printf("\n**********POOL TABLE**********\n");
	for(i=1;i<poolindx;i++)
		printf("|#%d|\n",pooltab[i]);
	fclose(fp);
	fclose(out);
}

int passtwo()
{
	char t0[20],t1[20],t2[20],t3[20],t4[20],t5[20],tmp[20],tmp2[20];
	int count,i;
	location=0;
	FILE *fp1,*out1;
	fp1 = fopen("IC.txt","r");
	out1 = fopen("machine.txt","w");
	if(fp1 == NULL)
	{
		printf("\nFile Is Not Created\n");
	}
	else if(out1 == NULL)
	{
		printf("\nOUTPUT File Is Not Created\n");
	}
	else
	{
		while(fgets(t0,100,fp1))
		{
			count = sscanf(t0,"%s %s %s %s %s",t1,t2,t3,t4,t5);
			switch(count)
			{
				case 2:
					if(strstr(t1,"AD"))
					{
						location++;
					}
					else if(strstr(t1,"IS"))
					{
						fprintf(out1,"00 0 000\n");
					}
				break;
				case 4:
					t2[strlen(t2)-1] = '\0';
					t3[strlen(t3)-1] = '\0';
					t4[strlen(t4)-1] = '\0';
					if(strstr(t1,"AD"))
					{
						location = atoi(t4);
					}
					else if(strstr(t1,"IS"))
					{
						fprintf(out1,"%d) %02d 0 %03d\n",location++,atoi(t2),stable[atoi(t4)].addr);
					}
					else if(strstr(t1,"DL"))
					{
						if(atoi(t2) == 02)
						{
							for(i = 0; i < atoi(t4); i++)
							 fprintf(out1, "%d)\n", location++);
							location--;
						}
						else if(atoi(t2) == 01)
						{
							fprintf(out1,"%d) + 00 0 %03d\n",location++,atoi(t4));
						}
					}
				break;
				case 5:
					t2[strlen(t2)-1] = '\0';
					t5[strlen(t5)-1] = '\0';
					if(strstr(t4,"S"))
					{
						fprintf(out1,"%d) %02d %d %03d\n",location++,atoi(t2),atoi(t3),stable[atoi(t5)].addr);
					}
					else if(strstr(t4,"L"))
					{
						fprintf(out1,"%d) %02d %d %03d\n",location++,atoi(t2),atoi(t3),ltable[atoi(t5)].addr);
					}
				break;
			}
		}
	}
	fclose(fp1);
	fclose(out1);
	return 0;
}
int main()
{
	passone();
	passtwo();
	return 0;
}
