#include "PricingEngines.h"
#include "ObjectiveFunctions.h"
#include "simplex.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//function to print iteration steps in optimization
template<class Con>
void printcon(const Con& c){
    std::cout.precision(8);
    cout << "results are:";
    copy(c.begin(), c.end(), ostream_iterator<typename Con::value_type>(cout, "  "));
    cout << endl;
}

int main(int argc, const char * argv[]) {
    
    //test if there are 5 arguments in the terminal
    if (argc != 5) {
        cout << "Usage: calibration LIBOR_yyyymmdd.txt Swap_yyyymmdd LIBOR_calibrated_yyyymmdd Swap_calibrated_yyyymmdd.txt" << endl;
        return -1;
    }
    
    //open the txt files
    ifstream infile1(argv[1]);
    ifstream infile2(argv[2]);
    if (!infile1 || !infile2) {
        cout << "Input files could't be opened" << endl;
        return -1;
    }
    
    /*
    ifstream infile1("/Users/Sheng/Desktop/Sheng's/Courses/2016Fall/IEORE4732/Project/calibration/calibration/LIBOR_20110214.txt");
    ifstream infile2("/Users/Sheng/Desktop/Sheng's/Courses/2016Fall/IEORE4732/Project/calibration/calibration/Swap_20110214.txt");
    */
    
    //read in the txt files
    MarketData md;
    
    cout << "LIBOR rates market data:" << endl;
    string line;
    getline(infile1, line);
    cout << line << endl;
    double a, b;
    while (infile1 >> a >> b) {
        cout << a << "    " << b << endl;
        md.L_mat.push_back(a/12);
        md.L_mkt.push_back(b);
    }
    infile1.close();
    
    cout << "Swap rates market data:" << endl;
    getline(infile2, line);
    cout << line << endl;
    while (infile2 >> a >> b) {
        cout << a << "    " << b << endl;
        md.S_mat.push_back(a);
        md.S_mkt.push_back(b);
    }
    infile2.close();
    
    /*
    md.L_mkt = {0.2647, 0.2890, 0.3140, 0.4657, 0.7975};
    md.L_mat = {1.0/12, 2.0/12, 3.0/12, 6.0/12, 12.0/12};
    md.S_mkt = {1.0481, 1.5577, 2.5569, 3.1850, 3.7225, 4.1683, 4.4407};
    md.S_mat = {2, 3, 5, 7, 10, 15, 30};
    */
    
    //initialize parameters for optimization
    using BT::Simplex;
    
    vector<double> init1 = {0.1, 0.1, 0.001, 0.001};
    vector<double> par1 = Simplex(SSRE_Vasicek, md, init1, 1e-4);
    
    vector<double> init2 = {0.01, 1, 0.1, 0.001};
    vector<double> par2 = Simplex(SSRE_CIR, md, init2, 1e-4);
    
    vector<double> init3 = {0.01, 0.01, 1, 1, 0.1, 0.1, 0.001, 0.001};
    vector<double> par3 = Simplex(SSRE_CIR_Two_Factor, md, init3, 1e-4);
    
    //calculate the calibrated model price
    vector<double> L_mdl_Vasicek;
    vector<double> L_mdl_CIR;
    vector<double> L_mdl_CIR_Two_Factor;
    vector<double> S_mdl_Vasicek;
    vector<double> S_mdl_CIR;
    vector<double> S_mdl_CIR_Two_Factor;
    
    for (int i = 0; i < md.L_mkt.size(); i++) {
        L_mdl_Vasicek.push_back(L_Vasicek(par1, md.L_mat[i]));
        L_mdl_CIR.push_back(L_CIR(par2, md.L_mat[i]));
        L_mdl_CIR_Two_Factor.push_back(L_CIR_Two_Factor(par3, md.L_mat[i]));
    }
    
    for (int i = 0; i < md.S_mkt.size(); i++) {
        S_mdl_Vasicek.push_back(S_Vasicek(par1, md.S_mat[i]));
        S_mdl_CIR.push_back(S_CIR(par2, md.S_mat[i]));
        S_mdl_CIR_Two_Factor.push_back(S_CIR_Two_Factor(par3, md.S_mat[i]));
    }
    
    //write calibrated model price in the txt file
    ofstream outfile1(argv[3]);
    ofstream outfile2(argv[4]);
    
    if (!outfile1 || !outfile2) {
        cout << "Output files Couldn't be created" << endl;
        return -1;
    }
    
    outfile1 << "maturity(months) LIBOR_rates_Vasicek(%) LIBOR_rates_CIR(%) LIBOR_rates_CIR_Two_Factor(%)" << endl;
    for (int i = 0; i < md.L_mkt.size(); i++) {
        outfile1 << md.L_mat[i] * 12 << " " << L_mdl_Vasicek[i] << " " << L_mdl_CIR[i] << " " << L_mdl_CIR_Two_Factor[i] << endl;
    }
    
    outfile2 << "term(year) Swap_rates_Vasicek(%) Swap_rates_CIR(%) Swap_rates_CIR_Two_Factor(%)" << endl;
    for (int i = 0; i < md.S_mkt.size(); i++) {
        outfile2 << md.S_mat[i] << " " << S_mdl_Vasicek[i] << " " << S_mdl_CIR[i] << " " << S_mdl_CIR_Two_Factor[i] << endl;
    }
    
    /*
    cout <<"LIBOR Vasicek model price:"<<endl;
    for (int i = 0; i < L_mdl_Vasicek.size(); i++) {
        cout << L_mdl_Vasicek[i] << endl;
    }
    
    cout <<"LIBOR CIR model price:"<<endl;
    for (int i = 0; i < L_mdl_CIR.size(); i++) {
        cout << L_mdl_CIR[i] << endl;
    }
    
    cout <<"LIBOR CIR Two Factor model price:"<<endl;
    for (int i = 0; i < L_mdl_CIR_Two_Factor.size(); i++) {
        cout << L_mdl_CIR_Two_Factor[i] << endl;
    }
    
    cout <<"Swap Vasicek model price:"<<endl;
    for (int i = 0; i < S_mdl_Vasicek.size(); i++) {
        cout << S_mdl_Vasicek[i] << endl;
    }
    
    cout <<"Swap CIR model price:"<<endl;
    for (int i = 0; i < S_mdl_CIR.size(); i++) {
        cout << S_mdl_CIR[i] << endl;
    }
    
    cout <<"Swap CIR Two Factor model price:"<<endl;
    for (int i = 0; i < S_mdl_CIR_Two_Factor.size(); i++) {
        cout << S_mdl_CIR_Two_Factor[i] << endl;
    }
    */
    return 0;
}
