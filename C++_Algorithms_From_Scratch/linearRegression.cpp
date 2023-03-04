#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <numeric>
#include <chrono>

using namespace std;


// define the sigmoid function
double sigmoid(int z) {
    return (1.0 / (1 + exp(-z)));
}

// logistic regression function
void logisticRegression(double learning_rate, double& w0, double& w1, int iterations, vector<int>& predictor, vector<int>& labels) {

    // loop through all the iterations
    for(int it = 0; it < iterations; it++) {
        double sum0 = 0.0;
        double sum1 = 0.0;

        // gradient descent
        for(int i = 0; i < labels.size(); i++) {
            double h = sigmoid(w0 + w1 * predictor[i]);
            double error = labels[i] - h;
            sum0 += error;
            sum1 += error * predictor[i];
        }

        w0 += learning_rate * sum0;
        w1 += learning_rate * sum1;
    }

    cout << "intercept: " << w0 << endl;
    cout << "sex slope: " << w1 << endl;

    return;
}

// predict on test set
vector<double> test(vector<int>& sexTest, double w0, double w1) {
    vector<double> predictions(1046-800);

    for(int i = 0; i < sexTest.size(); i++) {
        double z = w0 + w1*sexTest[i];

        predictions[i] = sigmoid(z);
    }

    return predictions;
}

// accuracy function
double accuracy(vector<double>& predictions, vector<int>& labels) {
    int matched = 0;
    vector<double> temp = predictions;

    for(int i = 0; i < predictions.size(); i++) {
        if(predictions[i] >= 0.5) {
            temp[i] = 1;
        } else {
            temp[i] = 0;
        }
        if(temp[i] == (double)labels[i]) {
            matched++;
        }
    }

    return (double)matched / (double)predictions.size();
}

// sensitivity function
double sensitivity(vector<double>& predictions, vector<int>& labels) {
    int TP = 0;
    int FN = 0;

    for(int i = 0; i < predictions.size(); i++) {
        if(predictions[i] >= 0.5 && labels[i] == 1) {
            TP++;
        } else if(predictions[i] < 0.5 && labels[i] == 1.0) {
            FN++;
        }
    }

    return (double)TP / ((double)TP + (double)FN);
}

// specificity function
double specificity(vector<double>& predictions, vector<int>& labels) {
    int TN = 0;
    int FP = 0;

    for(int i = 0; i < predictions.size(); i++) {
        if(predictions[i] < 0.5 && labels[i] == 0.0) {
            TN++;
        } else if(predictions[i] >= 0.5 && labels[i] == 0.0) {
            FP++;
        }
    }

    return (double)TN / ((double)TN + (double)FP);
}


int main(int argc, char** argv) {
 
    ifstream inFS; // Input file stream
    string line;
    //string rm_in, medv_in;
    string sex_in, pclass_in, age_in, survived_in, passenger;
    const int MAX_LEN = 1050;
    //vector<double> rm(MAX_LEN);
    //vector<double> medv(MAX_LEN);
    vector<int> sex(MAX_LEN);
    vector<int> pclass(MAX_LEN);
    vector<int> age(MAX_LEN);
    vector<int> survived(MAX_LEN);
    
    
    // Try to open file
    cout << "Opening file titanic_project.csv." << endl;
    
    inFS.open("titanic_project.csv");
    
    if(!inFS.is_open()) {
        cout << "Could not open file titanic_project.csv." << endl;
        return 1; // 1 indicates error
    }
    
    // Can now use inFS stream like cin stream

    // titanic_project.csv should contain two doubles
    cout << "Readling line 1" << endl;
    getline(inFS, line);
    
    // echo heading
    cout << "heading: " << line << endl;
    
    int numObservations = 0;
    
    while(inFS.good()) {
        getline(inFS, passenger, ',');
        getline(inFS, pclass_in, ',');
        getline(inFS, survived_in, ',');
        getline(inFS, sex_in, ',');
        getline(inFS, age_in, '\n');

        sex.at(numObservations) = stoi(sex_in);
        pclass.at(numObservations) = stoi(pclass_in);
        age.at(numObservations) = stoi(age_in);
        survived.at(numObservations) = stoi(survived_in);

        numObservations++;
    }
    
    sex.resize(numObservations);
    pclass.resize(numObservations);
    age.resize(numObservations);
    survived.resize(numObservations);
    cout << "new length " << survived.size() << endl;
    cout << "Closing titanic_project.csv file" << endl;
    
    inFS.close();
    
    cout << "Number of records: " << numObservations << endl;

    auto start = chrono::steady_clock::now();
    

    // Logistic Regression
    double learning_rate = 0.001;
    int iterations = 500000;

    // create testing vectors
    vector<int> sexTrain(800);
    vector<int> survivedTrain(800); // this is the labels matrix
    for(int i = 0; i < 800; i++) {
        sexTrain[i] = sex[i];
        survivedTrain[i] = survived[i];
    }

    // create data matrix
    vector<double> size(2);
    vector<vector<double>> dataMatrix(800, size);
    for(int i = 0; i < 800; i++) {
        for(int j = 0; j < 2; j++) {
            if(j == 0) {
                dataMatrix[i][0] = 1;
            } else {
                dataMatrix[i][1] = sexTrain[i];
            }
        }
    }

    // weights start off at 1, 1. I did not use a matrix for the weights to simplify the program
    double w0 = 1.0;
    double w1 = 1.0;
    logisticRegression(learning_rate, w0, w1, iterations, sexTrain, survivedTrain); // call logistic regression

    // create test sets
    vector<int> sexTest(numObservations-800);
    vector<int> survivedTest(numObservations-800);
    int counter = 800;
    for(int i = 0; i < numObservations-800; i++) {
        sexTest[i] = sex[counter];
        survivedTest[i] = survived[counter];
        counter++;
    }

    // test call
    vector<double> testing = test(sexTest, w0, w1);

    // calculate metrics
    double acc = accuracy(testing, survivedTest);
    cout << "accuracy: " << acc << endl;

    double sens = sensitivity(testing, survivedTest);
    cout << "sensitivity: " << sens << endl;

    double spec = specificity(testing, survivedTest);
    cout << "specificity: " << spec << endl;
  
    auto end = chrono::steady_clock::now();

    cout << "elapsed time to run this program: " << chrono::duration_cast<chrono::seconds>(end - start).count() << " seconds" << endl;

    return 0;
}
