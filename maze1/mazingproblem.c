#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 5000
#define flag 1


int i, j;
int zero=0;
int height, width;
int exit_row, exit_col;

typedef struct s1
{
    int vert;
    int horiz;
}offsets;

typedef struct s2
{
    int row;
    int col;
    int dir;
}element;

int top=-1;
offsets move[8];
element *stack;
char **newmaze;

void count_maze_size(FILE *fp);
void make_maze();
void path();
element pop();
void push(element item);
void print_stack(int a, int b);

int main()
{
    FILE *fp = NULL;
    fp = fopen("maze1/maze.txt", "r");
    if(fp != NULL)
    {
       count_maze_size(fp); 
       make_maze(); 
    }
    fclose(fp);

    return 0;
}

void count_maze_size(FILE *fp)
{
    char buffer[MAX] = {0};
    if(fgets(buffer, 2000, fp))
    {
        width = strlen(buffer)-1;
        height = 1;
    }
    while(fgets(buffer, 2000, fp))
    {
        height++;
    }

    exit_row=height;
    exit_col=width;

    fclose(fp);
    fp = NULL;
} 

void make_maze()
{
    FILE *fp = NULL;   
    char buffer[MAX] = {0};
    char** array = (char**)malloc(sizeof(char*)*height);
    newmaze = (char**)malloc((height+2)*sizeof(char*));
    fp = fopen("maze1/maze.txt", "r");

    for(i = 0; i < height; i++)
    {
        array[i] = (char*)malloc(sizeof(char)*width);
    }

    for(i = 0; i < (height+2); i++)
    {
        newmaze[i] = (char*)malloc((width+2)*sizeof(char));
    }
    
    if(fp != NULL)
    {
        for(i = 0; i < height; i++)
        {
            
            fgets(buffer, 2000, fp);
            strcpy(array[i], buffer);
        }

        if(flag==1)
        {
            for(i=0;i<height;i++)
            {
                for(int j=0; j<width; j++)
                {
                    printf("%c", array[i][j]);
                }
                printf("\n");
            }   
        }
        

        for(int i=0;i<(height+2);i++)
        {
            for(int j=0; j<(width+2);j++)
            {
                newmaze[i][j]=1;
            }
        }
        for(i=0; i<height; i++)
        {
            for(j=0; j<width; j++)
            {
                newmaze[i+1][j+1]=array[i][j]%2;
                if(newmaze[i+1][j+1]==0)
                {
                    zero=zero+1;
                }
            }
        }

        

        if(flag==0)
        {
            printf("%d\n", zero);
            for(i=0; i<(height+2);i++)
            {
                for(j=0; j<(width+2); j++)
                {
                    printf("%d", newmaze[i][j]);
                }
                printf("\n");
            }
        }
    }
    
    fclose(fp);
    for(i = 0; i < height; i++)
    {
        free(array[i]);
    }
    free(array);

    path();

}

void path()
{
    char **mark=(char**)malloc(sizeof(char*)*(height+2));

    for(i=0; i<(height+2); i++)
    {
        mark[i]=(char*)malloc(sizeof(char)*(width));
    }

    for(i=0; i<(height+2); i++)
    {
        for(j=0;j<(width+2);j++)
        {
            mark[i][j]=0;
        }
    }

    if(flag==0)
    {
        for(i=0; i<(height+2); i++)
        {
            for(j=0;j<(width+2);j++)
            {
                printf("%d", newmaze[i][j]);
            }
            printf("\n");
        }
    }

    move[0].vert = -1;	move[0].horiz = 0;
	move[1].vert = -1;	move[1].horiz = 1;
	move[2].vert = 0;	move[2].horiz = 1;
	move[3].vert = 1;	move[3].horiz = 1;
	move[4].vert = 1;	move[4].horiz = 0;
	move[5].vert = 1;	move[5].horiz = -1;
	move[6].vert = 0;	move[6].horiz =-1;
	move[7].vert = -1;	move[7].horiz = -1;


/*---------------------------------------------------*/

    stack=(element*)malloc(sizeof(element)*zero);
    
    int found=0;
    int dir, row, col;
    int next_row, next_col;
    element position;
    mark[1][1]=1;
    
    top=0;
    stack[top].row=1;
    stack[top].col=1;
    stack[top].dir=1;
    
    while(top>-1 && !found)
    {
        position=pop();
        row=position.row;
        col=position.col;
        dir=position.dir;
        while(!found && dir<8)
        {
            next_row=row+move[dir].vert;
            next_col=col+move[dir].horiz;


            if(next_row==exit_row && next_col==exit_col)
            {
                found=1;

            }
            else if(!newmaze[next_row][next_col] && !mark[next_row][next_col])
            {
                mark[next_row][next_col]=1;
                position.row=row;
                position.col=col;
                position.dir=++dir;
                push(position);
                row=next_row;
                col=next_col;
                dir=0;
            }
            else
            {
                dir++;
            }
            
        }

    }

    print_stack(row, col);

    for(i=0; i<(height+2); i++)
    {
        freegit (mark[i]);
    }
    free(mark);

    for(i = 0; i < (height+2); i++)
    {
        free(newmaze[i]);
    }
    free(newmaze);
    
}

void push(element item)
{
        stack[++top]=item;
}

element pop()
{
    if(top<=-1)
        exit(1);
    return stack[top--];
}

void print_stack(int a, int b)
{
    
    char **answer=(char**)malloc(sizeof(char*)*(height+2));
    for(i=0;i<(height+2);i++)
    {
        answer[i]=(char*)malloc(sizeof(char)*(width+2));
        for(j=0;j<(width+2);j++)
        {
            answer[i][j]=1;
        }
    }
    printf("===============================\n");
    for(i=0;i<=top;i++)
    {
        printf("(%d %d)\n", stack[i].row, stack[i].col);
        answer[stack[i].row][stack[i].col]=0;
    }
    printf("(%d %d)\n", a, b);
    answer[a][b]=0;
    printf("(%d %d)\n", exit_row, exit_col);
    answer[exit_row][exit_col]=0;

    for(i=0;i<(height+2);i++)
    {
        for(j=0;j<(width+2);j++)
        {
            printf("%d", answer[i][j]);
        }
        printf("\n");
        
    }

    
    for(i=0;i<(height+2);i++)
    {
        free(answer[i]);
    }
    free(answer);
}

//성공한 파일