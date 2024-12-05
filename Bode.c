#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>

#define double long double
#define MAX_N 10  // Assume that the maximum size of the matrix is 10x10 for easy debugging
#define EPSILON 1e-15

// Gauss-Jordan method solves the matrix
void gauss_jordan(double complex matrix[MAX_N][MAX_N], double complex *b, int n) 
{
    printf("\nStart Gauss-Jordan elimination...\n");
    for (int i = 0; i < n; i++) {

        // Find the element with the largest absolute value in the current column as the pivot
        int max_row = i;
        double max_val = cabs(matrix[i][i]);
        
        for (int k = i + 1; k < n; k++) {
            if (cabs(matrix[k][i]) > max_val) {
                max_val = cabs(matrix[k][i]);
                max_row = k;
            }
        }
        
        // Swap rows if needed
        if (max_row != i) {
            for (int j = 0; j < n; j++) {
                double complex temp = matrix[i][j];
                matrix[i][j] = matrix[max_row][j];
                matrix[max_row][j] = temp;
            }
            double complex temp = b[i];
            b[i] = b[max_row];
            b[max_row] = temp;
        }

        // Principal element normalization
        double complex pivot = matrix[i][i];
        if (cabs(pivot) < EPSILON) {
            continue; // If the coefficient is close to zero, skip the elimination of this line
        }

        for (int j = 0; j < n; j++) {
            matrix[i][j] /= pivot;
            if (cabs(matrix[i][j]) < EPSILON) {
                matrix[i][j] = 0;
            }
        }
        b[i] /= pivot;

        // Elimination of other rows
        for (int k = 0; k < n; k++) {
            if (k == i) continue;
            double complex factor = matrix[k][i];
            if (cabs(factor) < EPSILON) continue;  // If the coefficient is close to zero, skip the elimination of this line
            
            for (int j = 0; j < n; j++) {
                matrix[k][j] -= factor * matrix[i][j];
                if (cabs(matrix[k][j]) < EPSILON) {
                    matrix[k][j] = 0;
                }
            }
            b[k] -= factor * b[i];
        }
    }
    
}

// Calculate the gain and return the dB value of the gain
double calculate_gain(double complex Ve, double complex Vs) {

    double ve_mag = cabs(Ve);
    double vs_mag = cabs(Vs);

    double gain = vs_mag / 1;
    
    // 检查增益是否在合理范围内
    if (gain < -100.0) {
        return -100.0;  // 设置一个最小增益值（-100dB）
    }
    
    return 20 * log10(gain);
}

// Dynamically update the impedance value
void updateImpedance(double complex matrix[MAX_N][MAX_N], char types[MAX_N][MAX_N], double f, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (types[i][j] == 'L') {
                double complex Z_L = I * 2 * M_PI * f * cimag(matrix[i][j]);  // 更新电感阻抗
                matrix[i][j] = creal(matrix[i][j]) + Z_L;
            } else if (types[i][j] == 'C') {
                //double complex Z_C = I * 1e3/ (2 * M_PI * f * cimag(matrix[i][j]));
                double complex Z_C = I / (2 * M_PI * f * cimag(matrix[i][j]));  // 更新电容阻抗 电容单位为mF
                matrix[i][j] = creal(matrix[i][j]) + Z_C;
            }
        }
    }
}


int main() {
    int n;
    FILE *input = fopen("in.txt", "r");
    if (input == NULL) {
        perror("Unable to open file in.txt");
        return 1;
    }

    fscanf(input, "%d", &n);  // Read matrix size

    double complex Z[MAX_N][MAX_N];
    double complex voltages[MAX_N];
    double complex voltages_copy[MAX_N];
    double complex temp[MAX_N][MAX_N];
    char types[MAX_N][MAX_N];  // Used to store component types（R, L, C）
    
    // Read impedance matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double re, im;
            char type;
            fscanf(input, "%Lf %c %Le", &re, &type, &im);
            Z[i][j] = re + I * im;
            types[i][j] = type;    // Save element type（R, L, C）
        }
    }

    // Read the voltage source for each mailles
    for (int i = 0; i < n; i++) {
        double voltage;
        fscanf(input, "%Lf", &voltage);
        voltages[i] = voltage + 0.0 * I; // Define as complex
    }

    fclose(input);

    // Output the real part of the impedance of the last loop and obtain the resistance X value
    printf("The real part of the last loop impedance is: %.2f\n", creal(Z[n-1][n-1]));
    double X;
    do {
        printf("Please enter the last resistance (no more than the last real part): ");
        scanf("%Lf", &X);
    } while (X> creal(Z[n-1][n-1]));

    // Set the initial input voltage to 1V
    double complex Ve = 1.0 + 0.0 * I;

    // Calculate the gain and output the result
    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        perror("Unable to create file output.txt");
        return 1;
    }

    // Gain is calculated step by step on a logarithmic scale over the frequency range
    for (double freq = 1; freq <= 1e6; freq *= 1.25) {
        memcpy(temp, Z, sizeof(Z));
        updateImpedance(temp, types, freq, n);
        memcpy(voltages_copy, voltages, sizeof(voltages));
        gauss_jordan(temp, voltages_copy, n);
        double complex Vs = cabs(voltages_copy[n-1] * X);  // Take the voltage at both ends of X as Vs
        double gain_dB = calculate_gain(Ve, Vs);  // Calculated gain
        fprintf(output, "%.2Lf %.2Lf\n", freq, gain_dB);
    }

    fclose(output);
    return 0;
}
