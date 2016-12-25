#include <vector>
#include <math.h>

using namespace std;

//bond price based on diffrent models
double P_Vasicek(vector<double> x, double T) {
    double k = x[0];
    double theta = x[1];
    double sigma = x[2];
    double r_0 = x[3];
    double B = (1 - exp(-k * T)) / k;
    double A = (theta - sigma * sigma / (2 * k * k)) * (B - T) - sigma * sigma / (4 * k) * B * B;
    return exp(A - B * r_0);
}

double P_CIR(vector<double> x, double T) {
    double k = x[0];
    double theta = x[1];
    double sigma = x[2];
    double r_0 = x[3];
    double gamma = sqrt(k * k + 2 * sigma * sigma);
    double A = 2 * k * theta / (sigma * sigma) * log(exp(k * T / 2) / (cosh(gamma * T / 2) + k / gamma * sinh(gamma * T / 2)));
    double B = 2.0 / (k + gamma * 1.0 / tanh(gamma * T / 2));
    return exp(A - B * r_0);
}

double P_CIR_Two_Factor(vector<double> x, double T) {
    double k1 = x[0];
    double k2 = x[1];
    double theta1 = x[2];
    double theta2 = x[3];
    double sigma1 = x[4];
    double sigma2 = x[5];
    double r_01 = x[6];
    double r_02 = x[7];
    double gamma1 = sqrt(k1 * k1 + 2 * sigma1 * sigma1);
    double A1 = 2 * k1 * theta1 / (sigma1 * sigma1) * log(exp(k1 * T / 2) / (cosh(gamma1 * T / 2) + k1 / gamma1 * sinh(gamma1 * T / 2)));
    double B1 = 2.0 / (k1 + gamma1 * 1.0 / tanh(gamma1 * T / 2));
    double gamma2 = sqrt(k2 * k2 + 2 * sigma2 * sigma2);
    double A2 = 2 * k2 * theta2 / (sigma2 * sigma2) * log(exp(k2 * T / 2) / (cosh(gamma2 * T / 2) + k2 / gamma2 * sinh(gamma2 * T / 2)));
    double B2 = 2.0 / (k2 + gamma2 * 1.0 / tanh(gamma2 * T / 2));
    return exp(A1 + A2 - B1 * r_01 - B2 * r_02);
}

//LIBOR based on different models
double L_Vasicek(vector<double> x, double T) {
    return 100.0 / T * (1.0 / P_Vasicek(x, T) - 1);
}

double L_CIR(vector<double> x, double T) {
    return 100.0 / T * (1.0 / P_CIR(x, T) - 1);
}

double L_CIR_Two_Factor(vector<double> x, double T) {
    return 100.0 / T * (1.0 / P_CIR_Two_Factor(x, T) - 1);
}

double S_Vasicek(vector<double> x, double T) {
    double sum = 0;
    for (int i = 0; i < 2 * T; i++) {
        sum += 1.0/2 * P_Vasicek(x, 1.0/2 * (i + 1));
    }
    return 100 * (1 - P_Vasicek(x, T)) / sum;
}

//swap based on different models
double S_CIR(vector<double> x, double T) {
    double sum = 0;
    for (int i = 0; i < 2 * T; i++) {
        sum += 1.0/2 * P_CIR(x, 1.0/2 * (i + 1));
    }
    return 100 * (1 - P_CIR(x, T)) / sum;
}

double S_CIR_Two_Factor(vector<double> x, double T) {
    double sum = 0;
    for (int i = 0; i < 2 * T; i++) {
        sum += 1.0/2 * P_CIR_Two_Factor(x, 1.0/2 * (i + 1));
    }
    return 100 * (1 - P_CIR_Two_Factor(x, T)) / sum;
}
