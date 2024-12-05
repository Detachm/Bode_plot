#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // 设置随机数种子
    srand(time(NULL));
    FILE *file = fopen("in.txt", "w");

    if (!file) {
        printf("无法创建文件！\n");
        return 1;
    }

    int matrix_size = rand() % 8 + 2;  // 10x10矩阵
    fprintf(file, "%d\n", matrix_size);

    // 随机生成矩阵内容
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            
            // 随机选择元件类型：R, L, C
            char types[] = {'R', 'L', 'C'};
            char type = types[rand() % 3];
            if (i == j && i == matrix_size - 1)
            {
                type = 'R' ;
            }
            
            // 根据元件类型生成不同的实部和虚部
            double real_part = 0.0, imag_part = 0.0;
            switch (type) {
                case 'R':  // 电阻
                    real_part = 100 + rand() % 900;  // 100到1000之间的电阻值
                    imag_part = 0.0;
                    break;
                case 'L':  // 电感
                    real_part = 0.0;
                    imag_part = 0.001 + ((double)rand() / RAND_MAX) * 0.01;  // 0.001H到0.01H
                    break;
                case 'C':  // 电容
                    real_part = 0.0;
                    imag_part = -(1 + ((double)rand() / RAND_MAX) * 10);  // 1nF到10nF
                    break;
            }
            // 写入文件
            fprintf(file, "%lf %c %lf\n", real_part, type, imag_part);
        }
    }

    // 增加源数据
    fprintf(file, "1\n");  // 1个源
    for (int i = 0; i < matrix_size-1; i++)
    {
            fprintf(file, "0\n");  // 电源电压源值（可以修改成实际值）
    }
    

    fclose(file);
    printf("文件已生成: in.txt\n");

    return 0;
}
