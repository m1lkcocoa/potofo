#include <stdio.h>

int is_valid(int board[9][9], int r, int c, int val);
int is_solved(int board[9][9]);

int main(void) {
    /*盤面のデータ*/
    int board [9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };




 int row, col, num;

 /*入力のループ*/
 while (1) {
    /*ここから、盤面の表示を繰り返す処理*/
    /*行を０～８までを繰り返す*/
    for (int i = 0; i < 9; i++) {

        /*列を０～８までを繰り返す*/
        for (int j = 0; j < 9; j++){

            /*盤面の数字が０＝空のマスにゎ -　を表示*/
            if(board[i][j] == 0){
                printf(" - ");
            } else {
                /*盤面のマスに数値を表示する*/
                printf("%2d ", board[i][j]);
            }

            /*列の区切り線を入れる*/
            if (j == 2 || j == 5){
                printf(" | ");
            }
        }

        /*１行終わったら改行を入れる*/
        printf("\n");

        /*行の区切り線を入れる*/
        if (i == 2 || i == 5){
            printf("--------------------------------\n");
        }
    }
    /*ここまでが盤面の表示を繰り返す処理*/

    /*入力欄の表示*/
    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("※　数字の間にゎ、空白を入れて、半角の数字のみで入力してください。　※\n");
    printf("上から何番目か（１～９）、左から何番目か（１－９）、入れたい数字（１－９）の順で入力してください。（例：1 2 3）\n");
    printf("-1 と入力すると、強制終了ができます。\n");
    printf("入力欄：");


    int scan_result = scanf("%d", &row);

    if (scan_result == 1 && row == -1){
        printf("\n強制終了します。\n");
        while (getchar() != '\n' && getchar() != EOF);
        return 0;
    }

    if (scan_result != 1){
        printf("\n入力が正しくありません。※の条件に従って入力してください。\n");
        while (getchar() != '\n' && getchar() != EOF);
        continue;
    }

    if (scanf("%d %d", &col, &num) != 2){
        printf("\n入力が正しくありません。例の通りに入力してください。");
        while (getchar() != '\n' && getchar() != EOF);
        continue;
    }

    /*入力した値を配列番号と合わせる*/
    row = row - 1;
    col = col - 1;

    /*★ここから入力した値のチェック処理*/
    if (row < 0 || row > 8 || col < 0 || col > 8){
        printf("行と列は１～９の範囲で入力してください。\n");
        continue;

    }

    if (num < 1 || num > 9){
        printf("数字は１～９の範囲で入力してください。\n");
        continue;
    }

    if (board[row][col] != 0){
        printf("そのマスにゎ、問題として数字が埋まっているため、変更ゎできません。\n");
        continue;
    }

    if (!is_valid(board, row, col, num)){
        printf("その数字ゎ、縦・横またゎ、３×３のボックス内で重複しています。\n");
    }

    board[row][col] = num;
    printf("\nマス（%d行、%d列）に %d を入力しました。\n",row + 1, col + 1, num);


    if (is_solved(board)){

        printf("\n==================================\n");
        printf("★ ★ ★ ★ ★ ゲームクリア ★ ★ ★ ★ ★\n");
        printf("==================================\n");
        break;
    }

    
 }



    return 0;
}













/*入力された値が正解かのチェック処理の関数*/
int is_valid (int board[9][9], int r, int c, int val){
    
    /*横と縦に入力しようとしている数字があるかの確認*/
    for (int i = 0; i < 9; i++){
        /*横の確認。 i != rゎ、入力マスを省くため。*/
        if (i != r && board[i][c] == val){
            return 0;
        }

        /*縦の確認。 i != c　も入力マスを省くため。*/
        if (i != c && board[r][i] == val){
            return 0;
        }
    }

    /*確認の開始位置を３×３ブロックの左上から開始させるための計算*/
    int start_row = r - r % 3;
    int start_col = c - c % 3;

    /*左上から、左真ん中、左下、中上、真ん中、中下、右上、右中、右下の順で確認していくループ処理*/
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int current_r = start_row + i;
            int current_c = start_col + j;

            /*入力マスを省くための処理*/
            if (current_r == r && current_c == c){
                continue;
            }

            if (board[current_r][current_c] == val){
                return 0;
            }
        }
    }

    return 1;
}




/*ゲームクリアの判定をする関数*/
int is_solved(int board[9][9]) {

    /*０＝空いてるマス。これがどこにもなく、全部入力されているかの確認。*/
    for (int r = 0; r < 9; r++){
        for (int c = 0; c < 9; c++){
            if (board[r][c] == 0){
                return 0;
            }
        }
    }

    for (int r = 0; r < 9; r++){
        for (int c = 0; c < 9; c++){
            int val = board[r][c];


            board[r][c] = 0;

            if (!is_valid(board, r, c, val)){
                board[r][c] = val;
                return 0;
            }
            board[r][c] = val;
        }
    }

    return 1;
}