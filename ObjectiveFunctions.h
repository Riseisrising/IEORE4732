#include <vector>
#include <math.h>

using namespace std;

//class to store market data
class MarketData {
public:
    vector<double> L_mkt;
    vector<double> L_mat;
    vector<double> S_mkt;
    vector<double> S_mat;
};

//objective functions based on models
double SSRE_Vasicek(vector<double> x, MarketData md) {
    double sum1 = 0;
    for (int i = 0; i < md.L_mkt.size(); i++) {
        sum1 += pow((L_Vasicek(x, md.L_mat[i]) - md.L_mkt[i]) / md.L_mkt[i], 2.0);
        
    }
    
    double sum2 = 0;
    for (int j = 0; j < md.S_mkt.size(); j++) {
        sum2 += pow((S_Vasicek(x, md.S_mat[j]) - md.S_mkt[j]) / md.S_mkt[j], 2.0);
    }
    return sum1 + sum2;
}

double SSRE_CIR(vector<double> x, MarketData md) {
    double sum1 = 0;
    for (int i = 0; i < md.L_mkt.size(); i++) {
        sum1 += pow((L_CIR(x, md.L_mat[i]) - md.L_mkt[i]) / md.L_mkt[i], 2.0);
    }
    
    double sum2 = 0;
    for (int j = 0; j < md.S_mkt.size(); j++) {
        sum2 += pow((S_CIR(x, md.S_mat[j]) - md.S_mkt[j]) / md.S_mkt[j], 2.0);
    }
    return sum1 + sum2;
}

double SSRE_CIR_Two_Factor(vector<double> x, MarketData md) {
    double sum1 = 0;
    for (int i = 0; i < md.L_mkt.size(); i++) {
        sum1 += pow((L_CIR_Two_Factor(x, md.L_mat[i]) - md.L_mkt[i]) / md.L_mkt[i], 2.0);
    }
    
    double sum2 = 0;
    for (int j = 0; j < md.S_mkt.size(); j++) {
        sum2 += pow((S_CIR_Two_Factor(x, md.S_mat[j]) - md.S_mkt[j]) / md.S_mkt[j], 2.0);
    }
    return sum1 + sum2;
}
