#include <stdio.h>

/* --- プロトタイプ宣言 --- */
int is_valid(int board[9][9], int r, int c, int val); // 数字がルールに違反していないかチェック
int is_solved(int board[9][9]);                       // ゲームクリア判定
int solve(int board[9][9]);                           // 空きマスを埋めて答えを導く（ソルバーの基礎）
void display_board(int board[9][9]);                  // 盤面表示（装飾、色付き）


int main(void) {
    /* --- 盤面の初期データ（問題） --- */
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

    /* --- メインの入力ループ --- */
    while (1) {
        // 盤面を表示
        display_board(board);

        /* --- 入力待ちメッセージと色の設定 --- */
        printf("\x1b[36m"); // メッセージの色をシアンに設定
        printf("\n---------------------------------------------------------------------------------------------\n");
        printf("※　数字の間にゎ、空白を入れて、半角の数字のみで入力してください。　※\n");
        printf("上から何番目か（１～９）、左から何番目か（１－９）、入れたい数字（１－９）の順で入力してください。（例：1 2 3）\n");
        printf("-1 と入力すると、強制終了ができます。 / -2 と入力すると、答えを表示します。\n");
        printf("\x1b[34m"); // 入力欄の色を青に設定
        printf("入力欄：");
        printf("\x1b[0m"); // 色をリセット


        /* --- 1回目の入力チェック（コマンド判定のため row のみ読み込む） --- */
        int scan_result = scanf("%d", &row);

        /* --- コマンド判定 1: 強制終了 (-1) --- */
        if (scan_result == 1 && row == -1){
            printf("\n強制終了します。\n");
            // バッファクリア処理（残っている入力を捨てる）
            int ch; while ((ch = getchar()) != '\n' && ch != EOF);
            return 0; // プログラム終了
        }


        /* --- コマンド判定 2: 答え表示 (-2) --- */
        if (scan_result == 1 && row == -2){
            
            // 答えを出す前にバッファクリア（もしユーザーが "-2 a b" のように入力した場合のため）
            int ch; while ((ch = getchar()) != '\n' && ch != EOF); 

            // solve関数で現在の盤面が解けるか（答えがあるか）を判定
            if (solve(board)) {
                printf("\n==================================\n");
                printf(" ★ ★ 盤面をすべて、埋めました。 ★ ★\n");
                printf("==================================\n");
                
                // 答えが埋まった盤面を表示
                display_board(board);
            } else {
                // solveが失敗（0を返す）した場合は解けない
                printf("\n==================================\n");
                printf("\x1b[31m"); // 赤色開始
                printf(" ⚠️ 問題が成立していません (解けません)。 ⚠️\n");
                printf("\x1b[0m");  // 色リセット
                printf("==================================\n");
            }
            
            // 答え表示（またはエラー表示）後、プログラムを終了
            return 0; 
        }


        /* --- 入力チェック 1: 1つ目の値が数字ではなかった場合 --- */
        if (scan_result != 1){
            printf("\x1b[31m"); // 赤色開始
            printf("\n入力が正しくありません。※ の条件に従って入力してください。\n");
            printf("\x1b[0m"); // 色リセット
            // バッファクリア
            int ch; 
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue; // ループの最初に戻る
        }

        /* --- 入力チェック 2: 残りの2つの値 (col, num) が揃っているか --- */
        if (scanf("%d %d", &col, &num) != 2){
            printf("\x1b[31m");
            printf("\n入力が正しくありません。例の通りに入力してください。");
            printf("\x1b[0m");
            // バッファクリア
            int ch; 
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        /* --- 配列インデックスへの変換 (1-9 から 0-8 へ) --- */
        row = row - 1;
        col = col - 1;

        /* --- 値の範囲チェック --- */
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

        /* --- 初期盤面の数字保護チェック --- */
        // ※ 0でないマスは問題として埋まっているものとみなし、変更を許可しない
        if (board[row][col] != 0){
            printf("\x1b[31m");
            printf("そのマスにゎ、問題として数字が埋まっているため、変更ゎできません。\n");
            printf("\x1b[0m");
            continue;
        }

        /* --- ルール違反チェック --- */
        if (!is_valid(board, row, col, num)){
            printf("\x1b[31m");
            printf("その数字ゎ、縦・横またゎ、３×３のボックス内で重複しています。\n");
            printf("\x1b[0m");
            continue; // ループの最初に戻る
        }

        /* --- 数字の代入とフィードバック --- */
        board[row][col] = num;
        printf("\nマス（%d行、%d列）に %d を入力しました。\n",row + 1, col + 1, num);


        /* --- ゲームクリア判定 --- */
        if (is_solved(board)){
            display_board(board); // クリア後の最終盤面表示
            printf("\x1b[33m"); // 黄色開始
            printf("\n==================================\n");
            printf("★ ★ ★ ★ ★ ゲームクリア ★ ★ ★ ★ ★\n");
            printf("==================================\n");
            printf("\x1b[0m"); // 色リセット
            break; // ループを抜けてプログラム終了
        }

    } // while(1) 終了

    return 0;
}













/* --- is_valid 関数: ルール違反チェック --- */
int is_valid (int board[9][9], int r, int c, int val){
    
    /* --- 1. 横と縦の確認 --- */
    for (int i = 0; i < 9; i++){
        
        // 横（同じ行 r の i 列目）の確認。 i != c は入力マスを省くため。
        if (i != c && board[r][i] == val){
            return 0; // ルール違反 (同じ行に val がある)
        }

        // 縦（同じ列 c の i 行目）の確認。 i != r は入力マスを省くため。
        if (i != r && board[i][c] == val){
            return 0; // ルール違反 (同じ列に val がある)
        }
    }

    /* --- 2. 3x3 ブロックの確認 --- */
    // 確認の開始位置（3x3ブロックの左上のインデックス）を計算
    int start_row = r - r % 3;
    int start_col = c - c % 3;

    // ブロック内を走査するループ処理
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int current_r = start_row + i;
            int current_c = start_col + j;

            // 入力マス（r, c）を省くための処理
            if (current_r == r && current_c == c){
                continue;
            }

            // 3x3 ブロック内に val と同じ数字があるかチェック
            if (board[current_r][current_c] == val){
                return 0; // ルール違反 (同じブロックに val がある)
            }
        }
    }

    return 1; // ルール違反なし
}





/* --- is_solved 関数: ゲームクリア判定 --- */
int is_solved(int board[9][9]) {

    /* --- 1. 盤面が全て埋まっているか（空きマス 0 がないか）の確認 --- */
    for (int r = 0; r < 9; r++){
        for (int c = 0; c < 9; c++){
            if (board[r][c] == 0){
                return 0; // 0 があれば未クリア
            }
        }
    }

    /* --- 2. 全マスがルールを満たしているか（重複がないか）の最終確認 --- */
    for (int r = 0; r < 9; r++){
        for (int c = 0; c < 9; c++){
            int val = board[r][c];

            // is_valid に正確にチェックさせるため、一時的にそのマスを 0 にする
            board[r][c] = 0; 

            // is_valid を呼び出し、現在の値が正しいかチェック
            if (!is_valid(board, r, c, val)){
                board[r][c] = val; // 値を元に戻す
                return 0; // ルール違反が見つかったので未クリア
            }
            board[r][c] = val; // 値を元に戻す
        }
    }

    return 1; // 全てOK、クリア！
}





/* --- solve 関数: 盤面の自動完成（バックトラッキング） --- */
int solve(int board[9][9]) {
    
    // 盤面を左上から順に走査し、空きマス（0）を探す
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            
            if (board[r][c] == 0) { // ★ 空きマスを見つけたら

                // 1から9までの数字を順番に試す
                for (int num = 1; num <= 9; num++) {
                    
                    // 1. その数字を置いてもルール違反がないか確認
                    if (is_valid(board, r, c, num)) { 
                        
                        // 2. ルール違反がなければ、とりあえずその数字を置いてみる
                        board[r][c] = num;
                        
                        // 3. ★再帰処理：次の空きマスも解けるか、solve自身に試させる（試行錯誤）
                        if (solve(board)) {
                            return 1; // 次のマス以降も全て解けたら、成功を返す
                        }
                        
                        // 4. 次のマス以降が解けなかった場合（後戻り／バックトラック）
                        //    → 置いた数字を 0 に戻し、次の num を試す
                        board[r][c] = 0; 
                    }
                }
                
                // 1～9のどの数字を試しても解けなかった場合
                return 0; // 失敗を返す（呼び出し元が後戻りして前のマスからやり直す）
            }
        }
    }
    
    // 全てのマスに数字が埋まり、空きマスが見つからなかった場合
    return 1; // 成功！盤面完成
}





/* --- display_board 関数: 盤面表示（色付き） --- */
void display_board(int board[9][9]) {
    printf("\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            
            // 盤面の数字が 0 なら '-' を表示（空きマス）
            if (board[i][j] == 0) {
                printf(" - ");
            } else {
                // 0 以外（1-9）なら、数字を2桁分のスペースで表示（整列のため）
                printf("%2d ", board[i][j]);
            }
            
            // 列の区切り線（3列目と6列目の後）
            if ((j + 1) % 3 == 0 && j != 8) {
                printf("\x1b[36m"); // シアン（水色）開始
                printf("| ");
                printf("\x1b[0m"); // 色リセット
            }
        }
        
        // 1行表示終了後の改行
        printf("\n");
        
        // 行の区切り線（3行目と6行目の後）
        if ((i + 1) % 3 == 0 && i != 8) {
            printf("\x1b[36m"); // シアン（水色）開始
            // ナンプレの区切り線に合わせて調整
            printf("---------|----------|---------\n");
            printf("\x1b[0m"); // 色リセット
        }
    }
    printf("\n");
}