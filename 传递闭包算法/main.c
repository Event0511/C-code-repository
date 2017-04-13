#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int **NewMatrix(int num);
int **CreateMatrix(int num);
int **ReadMatrix(int *num);
void TransitiveClosure(int num, int **matrix);
void Warshall(int num, int **matrix);
void WarshallReformed(int num, int **matrix);
void PrintMatrix(int num, int **matrix);
void OutPutResult(int ver, int num, double span);

int main()
{
        int num, ver;
        clock_t tm_in, tm_out;
        double span;

        printf("请输入要测试方阵的行/列数：\n");
        scanf("%d", &num);
        printf("正在生成%d*%d的方阵，请稍等片刻...\n", num, num);
        int **matrix = NewMatrix(num);
        printf("方阵生成完毕，");
        do {
        printf("请选择测试算法版本（1/2/3）：\n");
        scanf("%d", &ver);
        printf("现在开始用%d*%d的矩阵测试第%d号算法，请稍等...\n", num, num, ver);
        //matrix = ReadMatrix(&num);
        //printf("邻接矩阵为：\n");
        //PrintMatrix(num, matrix);
        tm_in = clock();
        switch (ver)
        {
        case 1:
                TransitiveClosure(num, matrix);
        case 2:
                Warshall(num, matrix);
        case 3:
                WarshallReformed(num, matrix);
        }
        tm_out = clock();
        span = (double)(tm_out - tm_in)/1000;
        //printf("最终的传递闭包为：\n");
        //PrintMatrix(num, matrix);
        printf("生成完毕，总用时%f秒\n", span);
        OutPutResult(ver, num, span);
        printf("若想继续测试，");
        }while(1);
        return 0;
}
/**
*读取文件里的矩阵，并将顶点数写入num中，返回矩阵的指针
*/
int **ReadMatrix(int *num)
{
        int i, j;
        FILE *p;
        p = fopen("matrix.txt", "r");
        if (p == NULL)
        {
                printf("Open failed.Please check the file.\n");
                exit(-1);
        }
        fscanf(p, "%d", num);
        int **matrix = CreateMatrix(*num);
        for (i = 1; i < *num + 1; i++)
        {
                for (j = 1; j < *num + 1; j++)
                {
                        fscanf(p, "%d", &matrix[i][j]);
                }
        }
        return matrix;
}
/**
*创建新的随机矩阵，返回矩阵的指针
*/
int **NewMatrix(int num)
{
        int i, j;
        int **matrix = CreateMatrix(num);
        srand(time(NULL));
        for (i = 1; i < num + 1; i++)
        {
                for (j = 1; j < num + 1; j++)
                {
                        matrix[i][j] = rand()%2;
                }
        }
        return matrix;
}
/**
*创建一个num*num的矩阵（已忽略0行列）并将指针返回
*/
int **CreateMatrix(int num)
{
        int i;
        int **matrix = (int **)malloc(sizeof(int *) * (num + 1));
        for (i = 0; i < num + 1; i++)
        {
                matrix[i] = (int *)malloc(sizeof(int) * (num + 1));
        }
        return matrix;
}
/**
*将传入矩阵在屏幕上打印出来
*/
void PrintMatrix(int num, int **matrix)
{
        int i, j;
        for (i = 1; i < num + 1; i++)
        {
                for (j = 1; j < num + 1; j++)
                {
                        printf("%d ", matrix[i][j]);
                }
                printf("\n");
        }
}
void TransitiveClosure(int num, int **matrix)
{
        int i, j, k;
        int **temp = CreateMatrix(num);
        int **origin = CreateMatrix(num);
        for (i = 1; i < num + 1; i++)
        {
                for (j = 1; j < num + 1; j++)
                {
                        temp[i][j] = matrix[i][j];
                        origin[i][j] = matrix[i][j];
                }
        }
        for (i = 1; i < num; i++)
        {
                for (j = 1; j < num +1; j++)
                {
                        for (k = 1; k < num +1; k++)
                        {
                                temp[j][k] = origin[j][k] && temp[j][k];
                                matrix[j][k] = matrix[j][k] || temp[j][k];
                        }
                }
        }
}
/**
*用Warshall算法得出 num*num邻接矩阵处理后 最终的传递闭包
*/
void Warshall(int num, int **matrix)
{
        int i, j, k;
        for (k = 1; k < num + 1; k++)
        {
                for (i = 1; i < num + 1; i++)
                {
                        for (j = 1; j < num + 1; j++)
                        {
                                matrix[i][j] = (matrix[i][j]) || (matrix[i][k] && matrix[k][j]);
                        }
                }
        }

}
void WarshallReformed(int num, int **matrix)
{
        int i, j, k;
        for (k = 1; k < num + 1; k++)
        {
                for (i = 1; i < num + 1; i++)
                {
                        if (matrix[i][k] == 1)
                        {
                                for (j = 1; j < num + 1; j++)
                                {
                                        matrix[i][j] = (matrix[i][j]) || (matrix[k][j]);
                                }
                        }/*
                        else
                        {
                                for (j = 1; j < num + 1; j++)
                                {
                                        matrix[i][j] = (matrix[i][j]) || (matrix[i][k] && matrix[k][j]);
                                }
                        }*/
                }
        }
}
void OutPutResult(int ver, int num, double span)
{
        FILE *p = NULL;
        p = fopen("Log.txt", "a");
        while (p == NULL)
        {
                p = fopen("Log.txt", "w");
                printf("Open failed.A new file is created.\n");
                p = fopen("Log.txt", "a");
        }
        fprintf(p, "第%d号算法：\t%d*%d矩阵\t用时%f秒\n", ver, num, num, span);
        fclose(p);
}
