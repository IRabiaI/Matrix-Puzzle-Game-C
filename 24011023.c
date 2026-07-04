#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum{Manuel=0,Automatic=1}game_mode;
typedef enum{Random=0,File=1}game_create;
typedef struct{
    char name[50];
    int N;
    double score;
    game_mode mode;
    game_create create;
    int wrong_move;
    int count;
    int empty_block;
}User;

typedef struct{
    char name[50];
    double u_score;
}Score_list;

typedef struct{
    int n;
    int **matriks;
    int yedek_sayilar[10];
    int yedek_tamamlanan;
}Matrix;

typedef struct{
    int move_num;
    int cord[4];//sx,sy,dx,dy
    int sayilar[10];
    int tamamlanan_eslesme;
    Matrix *undo_board;
}Game;

typedef struct{
    int sayi;
    int s_cord[4];
}Auto_matrix;

void print_user_score();
void print_menu();
void print_game_opt();
void calculate_score(User *new_user);
void add_score(double score,char name[50]);
void create_user(User *new_user,int i);
void create_matrix(Matrix *game_board,int n);
void free_matrix(Matrix *game_board);
void create_random(Matrix *board);
void read_file(Matrix *board);
void print_matrix(Matrix board);
void play_manual(Matrix *board, User *user);
void enter_move(Matrix *board,Game *move);
void undo_move(Matrix *board,Game *move);
void create_undo_matrix(Game *move,int n);
int solve_matrix(Matrix *board,int current_num,int x,int y,Auto_matrix *kontrol,int oyun);
void play_auto(Matrix *board,User *user);
int main(){
    srand(time(NULL));
    int islem_main,islem_game;
    User new_user={0};
    Matrix board;
    do{
        print_menu();
        printf("Enter Option: ");scanf("%d",&islem_main);
        switch (islem_main){
        case 1:
        case 2:
            create_user(&new_user,(islem_main-1));
            create_matrix(&board,new_user.N);
            if(islem_main == 1)create_random(&board);
            else read_file(&board);
            do{
                //print_matrix(board);
                print_game_opt();
                printf("Enter Game Option: ");scanf("%d",&islem_game);
                switch(islem_game){
                    case 1:
                        new_user.mode = Manuel;
                        play_manual(&board,&new_user);
                        new_user.count++;
                        calculate_score(&new_user);
                        break;
                    case 2:
                        new_user.mode = Automatic;
                        play_auto(&board,&new_user);
                        new_user.count++;
                        calculate_score(&new_user);
                        break;
                    case 3:
                        printf("Returning To The Main Menu\n");   
                        break;
                    default:
                        printf("Wrong Option!");
                        break;
                }
            }while(islem_game!=3);
            if(board.matriks!=NULL){free_matrix(&board);board.matriks=NULL;}
            break;
        case 3:
            print_user_score();
            break;
        case 4:
            add_score(new_user.score,new_user.name);
            printf("Exiting The Game");
            break;
        default:
            printf("Wrong Option!");
            break;
        }
    }while(islem_main!=4);
    return 0;
}

void play_auto(Matrix *board,User *user){
    Auto_matrix *kontrol = (Auto_matrix*)malloc(board->n*sizeof(Auto_matrix));
    int sonuc;
    for(int i=0;i<board->n;i++){(kontrol+i)->s_cord[0]=-1;}
    for(int x=0;x<board->n;x++){
        for(int y=0;y<board->n;y++){
            if(board->matriks[x][y]!=0){
                (kontrol+board->matriks[x][y]-1)->sayi = board->matriks[x][y];
                if((kontrol+board->matriks[x][y]-1)->s_cord[0]==-1){
                    (kontrol+board->matriks[x][y]-1)->s_cord[0]=x;
                    (kontrol+board->matriks[x][y]-1)->s_cord[1]=y;
                }
                else{
                    (kontrol+board->matriks[x][y]-1)->s_cord[2]=x;
                    (kontrol+board->matriks[x][y]-1)->s_cord[3]=y;
                }
            }
        }
    }
    sonuc = solve_matrix(board,1,(kontrol+0)->s_cord[0],(kontrol+0)->s_cord[1],kontrol,1);
    if(sonuc == 1) {
        printf("Game Is Succesful\n");print_matrix(*board);
        user->empty_block = 0;
        for(int x=0;x<board->n;x++){
            for(int y=0;y<board->n;y++){
                if(board->matriks[x][y]==0)user->empty_block++;
            }
        }
    }
    else printf("Not Found Any Solution\n");
    free(kontrol);
}

int solve_matrix(Matrix *board,int current_num,int x,int y,Auto_matrix *kontrol,int oyun){
    int now_x,now_y;
    if(current_num>board->n){
        int var=0;
        for(int x=0;x<board->n;x++){
            for(int y=0;y<board->n;y++){
                if(board->matriks[x][y]==0) var=1;
            }
        }
        if(var==0)return 1;
        else return 0;
    }
    int hx=(kontrol+current_num-1)->s_cord[2];
    int hy=(kontrol+current_num-1)->s_cord[3];
    if((abs(x-hx)+abs(y-hy))==1){
        if(current_num==board->n) return solve_matrix(board, current_num+1, 0, 0, kontrol, oyun);
        else return solve_matrix(board,current_num+1,(kontrol+current_num)->s_cord[0],(kontrol+current_num)->s_cord[1],kontrol,oyun);
    }
    //right 0,1/left 0,-1/down 1,0/up -1,0 artış
    int yx[4] = {0,0,1,-1};
    int yy[4] = {1,-1,0,0};
    for(int i=0;i<4;i++){
        now_x = x + yx[i];
        now_y = y + yy[i];
        if((now_x>-1)&&(now_x<board->n)&&(now_y>-1)&&(now_y<board->n)){
            if(board->matriks[now_x][now_y] == 0){
                board->matriks[now_x][now_y] = current_num;
                if(oyun==1) printf("Number:%d Cordinat:%d,%d\n",current_num,now_x+1,now_y+1);
                if(oyun==1) print_matrix(*board);
                if(solve_matrix(board,current_num,now_x,now_y,kontrol,oyun)==1) return 1;
                board->matriks[now_x][now_y] = 0;
                if(oyun==1) printf("Wrong Direction\n");
                if(oyun==1) print_matrix(*board);
            }
        }
    }
    return 0;
}

void create_undo_matrix(Game *move,int n){
    move->undo_board = (Matrix*)realloc(move->undo_board,(move->move_num+1)*sizeof(Matrix));
    create_matrix(&(move->undo_board[move->move_num]),n);
}

void play_manual(Matrix *board, User *user){
    user->wrong_move = 0;
    user->empty_block = 0;
    Game move;int b,k,sayi;int islem=1;
    move.move_num = 0;
    move.undo_board = NULL;
    for(int i=0;i<10;i++){
        if(i<board->n) move.sayilar[i] = i+1;
        else move.sayilar[i] = 0;
    }
    move.tamamlanan_eslesme = 0;
    while(move.tamamlanan_eslesme<board->n){
        print_matrix(*board);
        printf("1-Enter Move\n2-Undo Move\nEnter Option: ");scanf("%d",&islem);
        if(islem==1){
            create_undo_matrix(&move,board->n);
            for(int x=0;x<board->n;x++){for(int y=0;y<board->n;y++){move.undo_board[move.move_num].matriks[x][y]=board->matriks[x][y];}}
            for(int i=0;i<10;i++){move.undo_board[move.move_num].yedek_sayilar[i]=move.sayilar[i];}
            move.undo_board[move.move_num].yedek_tamamlanan = move.tamamlanan_eslesme;
            move.move_num++;
            enter_move(board,&move);
            sayi = board->matriks[move.cord[0]][move.cord[1]];
            if(board->matriks[move.cord[2]][move.cord[3]] == board->matriks[move.cord[0]][move.cord[1]]){
                move.tamamlanan_eslesme++;
                move.sayilar[sayi-1] = 0;
            }
            if(move.cord[0]==move.cord[2]){
                if(move.cord[1]>move.cord[3]){b=move.cord[1]-1;k=move.cord[3];}
                else{b=move.cord[3];k=move.cord[1]+1;}
                for(int y=k;y<=b;y++){board->matriks[move.cord[0]][y] = sayi;}
            }
            else{
                if(move.cord[0]>move.cord[2]){b=move.cord[0]-1;k=move.cord[2];}
                else{b=move.cord[2];k=move.cord[0]+1;}
                for(int x=k;x<=b;x++){board->matriks[x][move.cord[1]] = sayi;}
            }
        }
        else if((islem==2)&&(move.move_num>0)){
            user->wrong_move++;
            undo_move(board,&move);
        }
        else if(move.move_num<=0) printf("No Move Back");
        else printf("Wrong Option");
    }
    print_matrix(*board);
    for(int x=0;x<board->n;x++){for(int y=0;y<board->n;y++){if(board->matriks[x][y]==0){user->empty_block++;}}}
    for(int i=0;i<move.move_num;i++){free_matrix(&(move.undo_board[i]));}
    free(move.undo_board);
}

void undo_move(Matrix *board,Game *move){
    move->move_num--;
    for(int i=0;i<board->n;i++){
        for(int j=0;j<board->n;j++){
            board->matriks[i][j] = move->undo_board[move->move_num].matriks[i][j];
        }
    }
    for(int i=0; i<10; i++) {move->sayilar[i] = move->undo_board[move->move_num].yedek_sayilar[i];}
    move->tamamlanan_eslesme = move->undo_board[move->move_num].yedek_tamamlanan;
    free_matrix(&(move->undo_board[move->move_num]));
    if(move->move_num>0)move->undo_board = (Matrix*)realloc(move->undo_board,(move->move_num)*sizeof(Matrix));
    else{free(move->undo_board);move->undo_board=NULL;}
}

void enter_move(Matrix *board,Game *move){
    int yes,b,k;
    do{
        printf("Enter Source X: ");scanf("%d",&move->cord[0]);move->cord[0]--;
        printf("Enter Source Y: ");scanf("%d",&move->cord[1]);move->cord[1]--;
    }while(!((move->cord[0]>-1)&&(move->cord[0]<board->n)&&(move->cord[1]>-1)&&(move->cord[1]<board->n)&&(board->matriks[move->cord[0]][move->cord[1]] != 0)&&(move->sayilar[board->matriks[move->cord[0]][move->cord[1]]-1] != 0)));
    do{
        yes = 0;
        printf("Enter Desination X: ");scanf("%d",&move->cord[2]);move->cord[2]--;
        printf("Enter Desination Y: ");scanf("%d",&move->cord[3]);move->cord[3]--;
        if((move->cord[0]==move->cord[2])^(move->cord[1]==move->cord[3])){
            yes = 1;
            if(move->cord[0]==move->cord[2]){
                if(move->cord[1]>move->cord[3]){b=move->cord[1]-1;k=move->cord[3];}
                else{b=move->cord[3];k=move->cord[1]+1;}
                for(int y=k;y<=b;y++){
                    if((board->matriks[move->cord[0]][y]!=0)&&(board->matriks[move->cord[0]][y]!=board->matriks[move->cord[0]][move->cord[1]]))yes = 0;
                }
            }
            else{
                if(move->cord[0]>move->cord[2]){b=move->cord[0]-1;k=move->cord[2];}
                else{b=move->cord[2];k=move->cord[0]+1;}
                for(int x=k;x<=b;x++){
                    if((board->matriks[x][move->cord[1]]!=0)&&(board->matriks[x][move->cord[1]]!=board->matriks[move->cord[0]][move->cord[1]])) yes = 0;
                }
            }
        }
    }while(yes==0);
}

void print_matrix(Matrix board){
    printf("x  ");
    for(int i=0;i<board.n;i++){
        printf("%d ",i+1);
    }
    printf("\n-----------------------\n");
    for(int i=0;i<board.n;i++){
        printf("%d| ",i+1);
        for(int j=0;j<board.n;j++){
            if(board.matriks[i][j]==0) printf("  ");
            else printf("%d ",board.matriks[i][j]);
        }
        printf("\n");
    }
}

void create_random(Matrix *board){
    int x,y;
    Auto_matrix kontrol[10];
    int sonuc=0;
    Matrix test;
    create_matrix(&test,board->n);
    do{
        for(int i=0;i<board->n;i++){
            for(int j=0;j<board->n;j++){test.matriks[i][j] = 0;}
            kontrol[i].s_cord[0] = -1;
        }
        for(int i=0; i<board->n; i++){
            for(int j=0; j<board->n; j++){board->matriks[i][j] = 0;}}
        for(int i=0;i<board->n;i++){
            for(int j=0;j<2;j++){
                do{
                    x = rand()%board->n;
                    y = rand()%board->n;
                }while(board->matriks[x][y]!=0);
                board->matriks[x][y] = i+1;
                if((kontrol+i)->s_cord[0]==-1){
                    (kontrol+i)->s_cord[0]=x;
                    (kontrol+i)->s_cord[1]=y;
                }
                else{
                    (kontrol+i)->s_cord[2]=x;
                    (kontrol+i)->s_cord[3]=y;
                }
            }
        }
        for(int i=0; i<board->n; i++){for(int j=0; j<board->n; j++){test.matriks[i][j] = board->matriks[i][j];}}
        if(solve_matrix(&test,1,(kontrol+0)->s_cord[0],(kontrol+0)->s_cord[1],kontrol,0)==1) sonuc = 1;
    }while(sonuc==0);
    free_matrix(&test);
    print_matrix(*board);
}

void read_file(Matrix *board){
    FILE *matriks_file;
    char dosya_adi[50];
    do{
        printf("Enter Game Board File Name: ");scanf(" %[^\n]",dosya_adi);
    }while((matriks_file=fopen(dosya_adi,"r"))==NULL);
    for(int i=0;i<board->n;i++){
        for(int j=0;j<board->n;j++){
            if(fscanf(matriks_file,"%d",&board->matriks[i][j]) != 1){
                printf("File format is incorret!\n");
                fclose(matriks_file);
                return;
            }
        }
    }
    fclose(matriks_file);
}

void create_matrix(Matrix *game_board,int n){
    game_board->n = n;
    game_board->matriks = (int**)calloc(n,sizeof(int*));
    if(game_board->matriks==NULL){
        printf("Game Board Not Be Created!\n");
        return;
    }
    for(int i=0;i<n;i++){
        game_board->matriks[i] = (int*)calloc(n,sizeof(int));
        if(game_board->matriks[i]==NULL){
            printf("Game Board Not Be Created!");
            return;
        }
    }
}

void free_matrix(Matrix *game_board){
    for(int i=0;i<game_board->n;i++){
        free(game_board->matriks[i]);
    }
    free(game_board->matriks);
}

void create_user(User *new_user,int i){
    if(new_user->name[0]=='\0'){printf("Enter User Name: ");scanf(" %[^\n]",new_user->name);}
    printf("Enter Matrix Size: "); scanf("%d",&new_user->N);
    new_user->create = i;
    new_user->wrong_move = 0;
    new_user->empty_block = 0;
    new_user->count = 0;

}

void print_menu(){
    printf("--------MAIN MENU--------\n");
    printf("1. create Random Matrix\n");
    printf("2. Create Matrix From File\n");
    printf("3. Show User Scores\n");
    printf("4. Exit\n");
}

void print_game_opt(){
    printf("--------GAME MENU--------\n");
    printf("1. Play in Manual Mode\n");
    printf("2. Play in Automatic Mode\n");
    printf("3.Return To Main Menu\n");
}

void print_user_score(){
    FILE *score;
    if((score=fopen("user_score.csv","r"))==NULL){
        printf("user_score.csv could not be open!");
        return;
    }
    char satir[100];
    char *parca;
    int i=0;
    printf("--------User Scores\n");
    while(fgets(satir,sizeof(satir),score)!=NULL){
        if(satir[0]!='\n'){
            i++;
            parca=strtok(satir,",");
            printf("%d. User Name: %s ",i,parca);
            parca=strtok(NULL,"\n");
            printf("Score: %s\n",parca);
        }
    }
    fclose(score);
}

void calculate_score(User *new_user){
    double puan=new_user->score;
    //manuel mod 10 puan, otomatik mod 5 puan
    //matris boyutu * 5 puan
    //rastgele matris 5 puan, dosyadan matris 10 puan
    //her bir yanlış hamle sayısı için -0.1 puan
    //oynan her bir oyun sayısı * 5 puan
    //oyun bittikten sonra boş kalan her bir hücre için -0.5 puan
    puan += (5 * new_user->N);
    puan += (-0.1 * new_user->wrong_move);
    puan += (5);//her oyun sonrası çağrıldığı için oyun sayısı ile çarpılmıyor
    puan += (-0.5 * new_user->empty_block);
    if(new_user->mode == 0) puan += 10;
    else puan += 5;
    if(new_user->create == 0) puan += 5;
    else puan += 10;
    printf("Actual User Score: %.2lf\n",puan);
    new_user->score = puan;
}

void add_score(double score,char name[50]){
    FILE *score_file;
    if((score_file=fopen("user_score.csv","r"))==NULL){
        score_file = fopen("user_score.csv","w");
        if(score_file != NULL) fclose(score_file);
        score_file = fopen("user_score.csv","r");
    }
    Score_list *list=NULL;
    double now_score;
    int yer=0;
    int sayi=0;
    char satir[100];
    char *parca;
    while(fgets(satir,sizeof(satir),score_file)!=NULL){
        if(satir[0]!='\0'){
            list = (Score_list*)realloc(list,(sayi+1)*sizeof(Score_list));
            if(list==NULL){
                printf("Memory allocation for score failed!");
                fclose(score_file);
                return;
            }
            parca = strtok(satir,",");
            strcpy((list+sayi)->name,parca);
            parca = strtok(NULL,"\n");
            now_score = atof(parca);
            (list+sayi)->u_score = now_score;
            if(now_score>score) yer++;
            sayi++;
        }
    }
    fclose(score_file);
    FILE *score_file_1;
    if((score_file_1=fopen("user_score.csv","w"))==NULL){
        printf("user_score.csv could not be open!");
        return;
    }
    for(int i=0;i<yer;i++){
        fprintf(score_file_1,"%s,%lf\n",(list+i)->name,(list+i)->u_score);
    }
    fprintf(score_file_1,"%s,%lf\n",name,score);
    for(int i=yer;i<sayi;i++){
        fprintf(score_file_1,"%s,%lf\n",(list+i)->name,(list+i)->u_score);
    }
    fclose(score_file_1);
    free(list);
    printf("Your Score: %lf; Ranking: %d\n",score,yer+1);
}