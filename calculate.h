//----------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
//---------------------------------------------------------------------------------------------------
/*数式入力*/
void FormulaInput(char *Str){
    /*数式を入力してください*/
    printf("Please enter a formula or command : ");

    /*英数字・記号 読み込み*/
    unsigned int i = 0;
    do{
    	Str[i] = getchar();
    } while(Str[i++] != 0x0A); //0x0A:Ent

    /*文字列の最後を必ず'='にする*/
    Str[i-1] = (Str[i-2] == '=' ? 0 :'=');
}
//----------------------------------------------------------------------------------------------------
/*桁管理*/
void NumSetup(int x,double *Num,int Time,int Flag,int DigitNum){
	if(Flag == 0)
		Num[Time] = Num[Time] * 10 + x;
	else
        Num[Time] += x * pow(10,-(DigitNum+1));
    return;
}
//----------------------------------------------------------------------------------------------------
/*注意書き*/
void CautionStatement(){
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Rules for this calculator!\n\n");
    printf("After entering the formula, enter '=' or push 'Ent'.\n\n");
    printf("Available mathematical symbols : '+' , '-' , '*' , '/' , '!' , '^' \n");
    printf("Examples : 1+1 , 1-1 , 1*1 , 1/1 , 1! , 1^1 \n\n");

    printf("If you use a function, enter sin'deg' , s'deg' , cos'deg' , c'deg' , tan'deg' , t'deg'\n");
    printf("However, the angle is limited to 0-360\n");
    printf("Examples : sin30 , cos60 , tan45 , s30 , c60 , t45\n\n");

    printf("If you want to continue using the previous formula, enter '+', '-' , '*' and '/' instead of entering a number first.\n");
    printf("Examples : +1 , -1 , *1 , /1\n\n");

    printf("Other functions.\n");
    printf("Examples1 : sin45+cos45-tan45 *Four arithmetic operations of trigonometric functions.\n");
    printf("Examples2 : 'H' or 'h' , 'history' , 'History' *View formulas and results history.\n");
    printf("If you want to quit, enter 'end'\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    return;
}
//----------------------------------------------------------------------------------------------------
/*空白をなくし式を短くする*/
void Shorten(int *NumFlag, char *Sign, double *Num){
    for(int i = 0; ; i++){
        if(Sign[i] == '=')
            break;
        else
        if(NumFlag[i] == 0 && Sign[i] == 0){
            for(int j = i; ; j++){
                if(NumFlag[j] == 0 && Sign[j] == 0){
                    Num[j] = Num[j+1];
                    Num[j+1] = 0; 

                    NumFlag[j] = (NumFlag[j+1] == 1 ? 1 :0);
                    NumFlag[j+1] = 0;

                    Sign[j] = Sign[j+1];
                    Sign[j+1] = 0;
                    
                    if(Sign[j] == '=')
                        break;
                }
            }
        }
        if(Sign[i] == '=')
            break;
    }
    return;
}
//----------------------------------------------------------------------------------------------------