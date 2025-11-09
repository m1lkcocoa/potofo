#include <stdio.h>

int is_valid(int board[9][9], int r, int c, int val);
int is_solved(int board[9][9]);
int solve(int board[9][9]);
void display_board(int board[9][9]);


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
                printf("\x1b[36m");
                printf(" | ");
                printf("\x1b[0m");
            }
        }

        /*１行終わったら改行を入れる*/
        printf("\n");

        /*行の区切り線を入れる*/
        if (i == 2 || i == 5){
            printf("\x1b[36m");
            printf("--------------------------------\n");
            printf("\x1b[0m");
        }
    }
    /*ここまでが盤面の表示を繰り返す処理*/

    /*入力欄の表示*/
    printf("\x1b[36m");
    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("※　数字の間にゎ、空白を入れて、半角の数字のみで入力してください。　※\n");
    printf("上から何番目か（１～９）、左から何番目か（１－９）、入れたい数字（１－９）の順で入力してください。（例：1 2 3）\n");
    printf("-1 と入力すると、強制終了ができます。\n");
    printf("\x1b[34m");
    printf("入力欄：");
    printf("\x1b[0m");


    int scan_result = scanf("%d", &row);

    if (scan_result == 1 && row == -1){
        printf("\n強制終了します。\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return 0;
    }


// ★★★ 2. ソルブ＆答え表示コマンド (-2) の修正 ★★★
if (scan_result == 1 && row == -2){
    
    // 答えを出す前にバッファクリア（もしユーザーが "-2 a b" のように入力した場合のため）
    int ch; while ((ch = getchar()) != '\n' && ch != EOF); 

    // solve関数が成功したかどうかを判定
    if (solve(board)) {
        printf("\n==================================\n");
        printf(" ★ ★ 盤面をすべて、埋めました。 ★ ★\n");
        printf("==================================\n");
        
        // 答えが埋まった盤面を表示
        display_board(board);
    } else {
        printf("\n==================================\n");
        printf("\x1b[31m"); // 赤色開始
        printf(" ⚠️ 問題が成立していません (解けません)。 ⚠️\n");
        printf("\x1b[0m");  // 色リセット
        printf("==================================\n");
    }
    
    // 処理が完了したので、プログラムを終了
    return 0; 
}


    if (scan_result != 1){
        printf("\x1b[31m");
        printf("\n入力が正しくありません。※ の条件に従って入力してください。\n");
        printf("\x1b[0m");
        int ch; 
        while ((ch = getchar()) != '\n' && ch != EOF);
        continue;
    }

    if (scanf("%d %d", &col, &num) != 2){
        printf("\x1b[31m");
        printf("\n入力が正しくありません。例の通りに入力してください。");
        printf("\x1b[0m");
        int ch; 
        while ((ch = getchar()) != '\n' && ch != EOF);
        continue;
    }

    /*入力した値を配列番号と合わせる*/
    row = row - 1;
    col = col - 1;

    /*★ここから入力した値のチェック処理*/
    if (row < 0 || row > 8 || col < 0 || col > 8){
        printf("\x1b[31m");
        printf("行と列は１～９の範囲で入力してください。\n");
        printf("\x1b[0m");
        continue;

    }

    if (num < 1 || num > 9){
        printf("\x1b[31m");
        printf("数字は１～９の範囲で入力してください。\n");
        printf("\x1b[0m");
        continue;
    }

    if (board[row][col] != 0){
        printf("\x1b[31m");
        printf("そのマスにゎ、問題として数字が埋まっているため、変更ゎできません。\n");
        printf("\x1b[0m");
        continue;
    }

    if (!is_valid(board, row, col, num)){
        printf("\x1b[31m");
        printf("その数字ゎ、縦・横またゎ、３×３のボックス内で重複しています。\n");
        printf("\x1b[0m");
    }

    board[row][col] = num;
    printf("\nマス（%d行、%d列）に %d を入力しました。\n",row + 1, col + 1, num);


    if (is_solved(board)){

        printf("\x1b[33m");
        printf("\n==================================\n");
        printf("★ ★ ★ ★ ★ ゲームクリア ★ ★ ★ ★ ★\n");
        printf("==================================\n");
        printf("\x1b[0m");
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





/*盤面の自動完成*/
// ★★★ solve 関数（ソルバーの基礎） ★★★
int solve(int board[9][9]) {
    
    // 盤面を左上から順に走査し、空きマス（0）を探す
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            
            if (board[r][c] == 0) { // ★ 空きマスを見つけたら

                // 1から9までの数字を順番に試す
                for (int num = 1; num <= 9; num++) {
                    
                    // 1. その数字を置いてもルール違反がないか、is_validで確認
                    if (is_valid(board, r, c, num)) { 
                        
                        // 2. ルール違反がなければ、とりあえずその数字を置いてみる
                        board[r][c] = num;
                        
                        // 3. ★再帰処理：次の空きマスも解けるか試す（最重要）
                        if (solve(board)) {
                            return 1; // 次のマス以降も全て解けたら、成功を返す
                        }
                        
                        // 4. 次のマス以降が解けなかった場合（後戻り／バックトラック）
                        //    → 置いた数字を 0 に戻し、次の num を試す
                        board[r][c] = 0; 
                    }
                }
                
                // 1～9のどの数字を試しても解けなかった場合
                return 0; // 失敗を返す（呼び出し元で後戻り処理が行われる）
            }
        }
    }
    
    // 全てのマスに数字が埋まり、空きマスが見つからなかった場合
    return 1; // 完成！成功を返す
}





/*盤面表示　関数*/
void display_board(int board[9][9]) {
    printf("\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                printf(" . ");
            } else {
                printf("%2d ", board[i][j]);
            }
            if ((j + 1) % 3 == 0 && j != 8) {
                printf("| ");
            }
        }
        printf("\n");
        if ((i + 1) % 3 == 0 && i != 8) {
            printf("---------|----------|---------\n");
        }
    }
    printf("\n");
}