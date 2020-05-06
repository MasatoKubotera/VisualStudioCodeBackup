//----------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "calculate.h"
//----------------------------------------------------------------------------------------------------
#define NumberFormulas 1000 //数式の数
#define NumberHistories 100  //履歴の数
//----------------------------------------------------------------------------------------------------
void NumSetup(int x,double *Num,int Time,int Flag,int DigitNum); //桁管理
void CautionStatement(); //ルール説明文
void FormulaInput(char *Str); //数式入力
void Shorten(int *NumFlag, char *Sign, double *Num); //詰め
void FuncCalcate(int *NumFlag, char *Sign, double *Num , int *cannot); //四則演算外演算
void AnsUseCheck(int NumFlag, int *AnsFlag, double *Result, double Ans, double Num); //Ans仕様確認
//----------------------------------------------------------------------------------------------------
int main(void){
//----------------------------------------------------------------------------------------------------
    /*初期設定*/
    CautionStatement(); //ルール説明文 表示

    double Ans = 0; //前回の計算結果

    double ResultHistory[ NumberHistories ] = {}; //計算結果履歴
    char FormulasHistory[NumberHistories][NumberFormulas] = {}; //計算式履歴
    int HistoryCount = 0; //履歴 添え字 カウント
//----------------------------------------------------------------------------------------------------  
    /*電卓*/
    while(1){
//----------------------------------------------------------------------------------------------------
        /*入力*/
		char Str [ NumberFormulas ] = {}; //数式(1文字ずつ)
        char *_Str = Str;
        FormulaInput(Str); //数式入力
//----------------------------------------------------------------------------------------------------
        /*電卓-計算式ごとにリセットする変数*/
		double Num [ NumberFormulas ] = {}; //数(1項)
		char Sign [ NumberFormulas ] = {}; //式(1項)
        int NumFlag [ NumberFormulas ] = {}; //数字が入力されているか

        int CalculateError = 0; //計算不可

		int Time = 0; //数・式配列 添え字
        double Result = 0; //計算結果

        int AnsFlag = 0; //Ansフラグ 0:通常 1:Ans
//----------------------------------------------------------------------------------------------------
		/*数式検出*/
        while(1) {
//----------------------------------------------------------------------------------------------------
            /*1項ごとにリセットする変数*/
        	int DecimalPointFlag = 0; //小数点検出 フラグ
			int DigitNum = 0; //桁数カウント(整数桁or少数桁)
//----------------------------------------------------------------------------------------------------
			/*数字*/
            while(1){
                if(*_Str >= '0' && *_Str <= '9'){
                    NumSetup(*_Str - 48, Num, Time, DecimalPointFlag, DigitNum);
                    NumFlag[Time] = 1; //数字入力フラグ
			        DigitNum++; //桁数カウント
                }
                else
                if(*_Str == '.'){
                    DecimalPointFlag = 1; //小数点検出 フラグ
                    DigitNum = 0; //桁数カウント リセット
                }
                else 
                    break;
		
                _Str ++; //次の文字へ
            }
//----------------------------------------------------------------------------------------------------
            /*記号*/
			switch(*_Str){
                case '+':
                case '-':
                case '*':
                case '/':
                case '!':
                case '^':
                    Sign[Time] = *_Str;
                    break;

                /*
				case '(': SignSetup('(', Sign, Time); break;
				case ')': SignSetup(')', Sign, Time); break;
                */
                case 'e':
                case 'E':
                    goto owari;

                case 'S':
                case 's':
                    if(*(_Str+1) == 'i' && *(_Str+2) == 'n'){
                        Sign[Time] = 's';
                        _Str+=2;
                    }
                    else
                    if(*(_Str+1) >= '0' && *(_Str+1) <= '9')
                        Sign[Time] = 's';
                    else
                        goto error;
                    break;

                case 'C':
                case 'c':
                    if(*(_Str+1) == 'o' && *(_Str+2) == 's'){
                        Sign[Time] = 'c';
                        _Str+=2;
                    }
                    else
                    if(*(_Str+1) >= '0' && *(_Str+1) <= '9')
                        Sign[Time] = 'c';
                    else
                        goto error;
                    break;

                case 'T':
                case 't':
                    if(*(_Str+1) == 'a' && *(_Str+2) == 'n'){
                        Sign[Time] = 't';
                        _Str+=2;
                    }
                    else
                    if(*(_Str+1) >= '0' && *(_Str+1) <= '9')
                        Sign[Time] = 't';
                    else
                        goto error;
                    break;

				case '=':
                    Sign[Time] = '=';
                    goto calc;

                case 'H':
                case 'h': 
                    goto history;

                default: goto error;
            }
			
			Time++; //数式 収納変数 添え字
            _Str ++;
        }
//----------------------------------------------------------------------------------------------------
    	calc:


            //関数計算
            for(int i = 0; Sign[i] != '='; i++){    
                switch(Sign[i]){
                    case 's':
                        Sign[i] = 0;
                        Num[i+1] = sin(Num[i + 1] * ( M_PI / 180 ));
                        break;

                    case 'c':
                        Sign[i] = 0;
                        Num[i+1] = cos(Num[i + 1] * ( M_PI / 180 ));
                        break;

                    case 't':
                        Sign[i] = 0;
                        if(Num[i+1] == 90 || Num[i+1] == 270)
                            goto error3;
                        else
                            Num[i+1] = tan(Num[i + 1] * ( M_PI / 180 ));
                        break;

                    case '^':
                        Num[i+1] = pow(Num[i],Num[i+1]);
                        Num[i] = 0;

                        NumFlag[i] = 0;
                        Sign[i] = 0;
                        break;

                    case '!':
                        Num[i+1] = 1;
                        NumFlag[i+1] = 1;

                        for(int j = 1; j <= Num[i]; ++j)
                            Num[i+1] *= j;
                        
                        Num[i] = 0;
                            
                        NumFlag[i] = 0;
                        Sign[i] = 0;
                        break;
                    default:
                        i++;
                } 

                for(int j = i; Sign[j] != '='; j++){
                    if(NumFlag[j] == 0 && Sign[j] == 0){
                        Num[j] = Num[j+1];
                        NumFlag[j] = NumFlag[j+1];
                        Sign[j] = Sign[j+1];

                        Sign[j+1] = 0;
                        Num[j+1] = 0;
                        NumFlag[j+1] = 0;

                        j--;
                    }
                }
                i--;
            }

            /*Ans使用判断*/
            if(NumFlag[0] == 0){
                AnsFlag = 1;
                Num[0] = Ans; //前回の計算結果
                NumFlag[0] = 1;
            }
            else{
                AnsFlag = 0;
                Result = Num[0]; //計算結果
            }            

            //'*'と'/'を先に計算する
            for(int i = 0; Sign[i] != '='; i++){
                if(Sign[i] == '*' || Sign[i] == '/'){

                    if(Sign[i] == '*')
                        Num[i+1] = Num[i] * Num[i+1];
                    
                    if(Sign[i] == '/'){
                        if(Num[i+1] != 0)
                            Num[i+1] = Num[i] / Num[i+1];
                        else 
                            goto error2;
                    }

                    Sign[i] = 0;
                    Num[i] = 0;
                    NumFlag[i] = 0;

                    for(int j = i; Sign[j] != '='; j++){
                        if(NumFlag[j] == 0 && Sign[j] == 0){
                            Num[j] = Num[j+1];
                            NumFlag[j] = NumFlag[j+1];
                            Sign[j] = Sign[j+1];

                            Sign[j+1] = 0;
                            Num[j+1] = 0;
                            NumFlag[j+1] = 0;

                            j--;
                        }
                    }

                    i--;
                }
            }
            
            //'+'・'-'計算
            for(int i = 0; Sign[i] != '='; i++){
                if(Sign[i] == '+' || Sign[i] == '-'){

                    if(Sign[i] == '+')
                        Num[i+1] = Num[i] + Num[i+1];
                    
                    if(Sign[i] == '-')
                        Num[i+1] = Num[i] - Num[i+1];

                    Sign[i] = 0;
                    Num[i] = 0;
                    NumFlag[i] = 0;

                    for(int j = i; Sign[j] != '='; j++){
                        if(NumFlag[j] == 0 && Sign[j] == 0){
                            Num[j] = Num[j+1];
                            NumFlag[j] = NumFlag[j+1];
                            Sign[j] = Sign[j+1];

                            Sign[j+1] = 0;
                            Num[j+1] = 0;
                            NumFlag[j+1] = 0;

                            j--;
                        }
                    }
                    i--;
                }
            }

            Result = Num[0];
//----------------------------------------------------------------------------------------------------
        finish:
            printf("解  : ");
            
            /*式表示*/
            if(AnsFlag != 0)                                                            
                printf("Ans");
            
            int StrCount = 0;
            for(; Str[StrCount]!=0; StrCount++)
                printf("%c",Str[StrCount]);
            
        	/*計算結果表示*/
        	if((int)Result == Result)
                printf("%d\n\n", (int)Result); //整数
            else
                printf("%lf\n\n", Result); //少数

            /*Ans*/
            Ans = Result;

            /*履歴管理*/
            ResultHistory[HistoryCount] = Result; //保存
            for(int i = 0; i < StrCount; i++){
                FormulasHistory[HistoryCount][i] = Str[i];
            }
            HistoryCount ++; //カウント

            continue;
//----------------------------------------------------------------------------------------------------
        error:
            /*指定文字以外の場合など*/
            printf("\n計算できません!\n\n");
            continue;
        error2:
            /*割れない場合*/
            printf("\n0で除算できません!\n\n");
            continue;
        error3:
            /*存在しない*/
            printf("\n値が存在しません!\n\n");
            continue;
//----------------------------------------------------------------------------------------------------
        history:
            /*履歴表示*/
            if(HistoryCount == 0){
                printf("表示できる履歴がありません。\n\n");
                continue;
            }

            for(int i = 0; i < HistoryCount; i++){
                printf("No.%d : ", i+1);
                for(int j = 0; FormulasHistory[i][j]!=0; j++){
                    printf("%c",FormulasHistory[i][j]);
                }
                
                printf("%lf\n",ResultHistory[i]); //少数
            }
            printf("\n\n");
            continue;
        owari:
            break;
//----------------------------------------------------------------------------------------------------
    } //電卓 while(1)
    
}
//----------------------------------------------------------------------------------------------------