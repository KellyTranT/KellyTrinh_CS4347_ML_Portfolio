#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <numeric>


using namespace std;

/* Function to find the sum of the vector */
double sum_total(vector<double> data) {
    double sum_of_vector = 0.0;
    
    // For-loop that iterates through the vector
    for(int i = 0; i < data.size(); i++) {
        sum_of_vector = sum_of_vector + data[i];
    }

    return sum_of_vector;
}

/* Function that calculates the mean of the vector */
double mean(vector<double> data) {
    double sum_of_vector = sum_total(data); // Call sum function
    double mean_of_vector = sum_of_vector / data.size();

    return mean_of_vector;
}

/* Function that finds the median of the vector*/
double median(vector<double> data) {
    sort(data.begin(), data.end());

    
    if(data.size() % 2 == 0) { // If the vector has an even amount of values, calculate the mean between the two middle values
        return (data[data.size() / 2 - 1] + data[data.size() / 2]) / 2;
    } else { // Else, the median is the middle value
        return data[data.size() / 2];
    }

    return data[data.size() / 2];
}

/* Function that finds the range of the vector */
double range(vector<double> data) {
    sort(data.begin(), data.end()); // Sort the vector

    // Find the difference between the largest and smallest value
    double range_of_vector = data[data.size() - 1] - data[0];

    return range_of_vector;
}

/* Function that calculates the covariance of the two vectors */
double covariance(vector<double> rm, vector<double> medv) {
    double cov_sum = 0.0;
    double rm_mean = mean(rm);
    double medv_mean = mean(medv);

    // Calculate the total sum of the difference between each value and the mean for both vectors
    for(int i = 0; i < rm.size(); i++) {
        double cur = (rm[i] - rm_mean) * (medv[i] - medv_mean);
        cov_sum = cov_sum + cur;
    }

    double cov = cov_sum / (rm.size() - 1);

    return cov;
}

/* Function that calculates the variance of the vector */
double variance(vector<double> data) {
    double mean_of_data = mean(data);
    double var_sum = 0.0;
    
    // Find the sum of the square difference of the vector 
    for(int i = 0; i < data.size(); i++) {
        double var_diff = (data[i] - mean_of_data) * (data[i] - mean_of_data);
        var_sum = var_sum + var_diff;
    }

    double var = var_sum / (data.size() - 1);

    return var;
}

/* Function that calculates the Pearson correlation between the two vectors */
double correlation(vector<double> rm, vector<double> medv) {
    double cov = covariance(rm, medv);
    double var_rm = variance(rm);
    double var_medv = variance(medv);

    double correlation = cov / (sqrt(var_rm) * sqrt(var_medv)); // Divide by the square root of each variance (standard deviation)
    
    return correlation;
}

int main(int argc, char** argv) {
    ifstream inFS; // Input file stream
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);

    // Try to open file
    cout << "Opening file Boston.csv." << endl;

    inFS.open("Boston.csv");
    if(!inFS.is_open()) {
        cout << "Could not open file Boston.csv." << endl;
        return 1; // 1 indicates error
    }

    // Can now use inFS stream like cin stream
    // Boston.csv should contain two doubles

    cout << "Readling line 1" << endl;
    getline(inFS, line);

    // echo heading
    cout << "heading: " << line << endl;

    int numObservations = 0;
    while(inFS.good()) {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');

        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "new length " << rm.size() << endl;

    cout << "Closing Boston.csv file" << endl;
    inFS.close();
    cout << "Number of records: " << numObservations << endl;



    /* MY CODE STATS HERE */

    /* Output stats for rm */
    cout << "\nStats for rm:" << endl;
    cout << "Sum = " << sum_total(rm) << endl;
    cout << "Mean = " << mean(rm) << endl;
    cout << "Median = " << median(rm) << endl;
    cout << "Range = " << range(rm) << endl;
    // cout << "Variance: " << variance(rm) << endl; -- Check for variance

    /* Output stats for medv */
    cout << "\nStats for medv:" << endl;
    cout << "Sum = " << sum_total(medv) << endl;
    cout << "Mean = " << mean(medv) << endl;
    cout << "Median = " << median(medv) << endl;
    cout << "Range = " << range(medv) << endl;
    // cout << "Variance: " << variance(medv) << endl; -- Check for variance

    /* Output covariance and correlation */
    cout << "\nCovariance between rm and medv: " << covariance(rm, medv) << endl;
    cout << "Correlation between rm and medv: " << correlation(rm, medv) << endl;

    return 0;
}
