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

// calculate prior function
vector<double> apriori(vector<int> survived) {
    vector<double> priors(2);

    // calculate perished probability
    int perished = 0;
    for(int i = 0; i < survived.size(); i++) {
        if(survived[i] == 0) {
            perished++;
        }
    }

    // calculate survived probability
    int survivor = 0;
    for(int i = 0; i < survived.size(); i++) {
        if(survived[i] == 1) {
            survivor++;
        }
    }

    priors[0] = (double)perished / (double)survived.size();
    priors[1] = (double)survivor / (double)survived.size();

    return priors;
}

// calculate likelihoos of an attribute with two values
vector<vector<double>> likelihoodOfTwo(vector<int>& survived, vector<int>& predictor) {
    vector<double> size(2);
    vector<vector<double>> prob(2, size);

    // get survived counts for no and yes
    int survivor = 0;
    int notSurvived = 0;
    for(int i = 0; i < survived.size(); i++) {
        if(survived[i] == 1) {
            survivor++;
        } else {
            notSurvived++;
        }
    }
    
    // these values represent where they are in the matrix (i.e. 00, 01, etc.)
    // survived = no | sex = male
    // survived = no | sex = female
    // survived = yes | sex = male;
    // survived = yes | sex = female;
    int count00 = 0;
    int count01 = 0;
    int count10 = 0;
    int count11 = 0;  
    for(int i = 0; i < survived.size(); i++) { // for each passenger and each feature
        if(predictor[i] == 0 && survived[i] == 0) { // if factor = male and survived = no
            count00++;
            continue;
        }
        if(predictor[i] == 1 && survived[i] == 0) { // if factor = female and survived = no
            count01++;
            continue;
        }
        if(predictor[i] == 0 && survived[i] == 1) { // if factor = male and survived = yes
            count10++;
            continue;
        }
        if(predictor[i] == 1 && survived[i] == 1) { // if factor = female and survived = yes
            count11++;
            continue;
        }
    }

    prob[0][0] = (double)count00 / (double)notSurvived;
    prob[0][1] = (double)count01 / (double)notSurvived;
    prob[1][0] = (double)count10 / (double)survivor;
    prob[1][1] = (double)count11 / (double)survivor;

    return prob;
}

// calculate likelihood of an attribute that can have three values
vector<vector<double>> likelihoodOfThree(vector<int> survived, vector<int> predictor) {
    vector<double> size(3);
    vector<vector<double>> prob(2, size);

    // get survived counts for no and yes
    int survivor = 0;
    int notSurvived = 0;
    for(int i = 0; i < survived.size(); i++) {
        if(survived[i] == 1) {
            survivor++;
        } else {
            notSurvived++;
        }
    }

    // these values represent where they are in the matrix (i.e. 00, 01, etc.)
    int count00 = 0;
    int count01 = 0;
    int count02 = 0;
    int count10 = 0;
    int count11 = 0;
    int count12 = 0;
    for(int i = 0; i < survived.size(); i++) {
        if(predictor[i] == 1 && survived[i] == 0) { // if pclass = 1 and survived = no
            count00++;
            continue;
        }
        if(predictor[i] == 2 && survived[i] == 0) { // if pclass = 2 and survived = no
            count01++;
            continue;
        }
        if(predictor[i] == 3 && survived[i] == 0) { // if pclass = 3 and survived = no
            count02++;
            continue;
        }
        if(predictor[i] == 1 && survived[i] == 1) { // if pclass = 1 and survived = yes
            count10++;
            continue;
        }
        if(predictor[i] == 2 && survived[i] == 1) { // if pclass = 2 and survived = yes
            count11++;
            continue;
        }
        if(predictor[i] == 3 && survived[i] == 1) { // if pclass = 3 and survived = yes
            count12++;
            continue;
        }
    }

    prob[0][0] = (double)count00 / (double)notSurvived;
    prob[0][1] = (double)count01 / (double)notSurvived;
    prob[0][2] = (double)count02 / (double)notSurvived;
    prob[1][0] = (double)count10 / (double)survivor;
    prob[1][1] = (double)count11 / (double)survivor;
    prob[1][2] = (double)count12 / (double)survivor;

    return prob;
}

// calculate mean
vector<double> meanCalc (vector<int> survived, vector<int> age) {
    vector<double> mean(2);
    double sumSurvived = 0.0;
    int survivedCount = 0;
    double sumNotSurvived = 0.0;
    int notSurvivedCount = 0;
    for(int i = 0; i < age.size(); i++) {
        if(survived[i] == 0) {
            sumNotSurvived += (double)age[i];
            notSurvivedCount++;
        } else {
            sumSurvived += (double)age[i];
            survivedCount++;
        }
    }
    mean[0] = sumNotSurvived / (double)notSurvivedCount;
    mean[1] = sumSurvived / (double)survivedCount;

    return mean;
}

// calculate variance
vector<double> varianceCalc(vector<int> survived, vector<int> age, double deadMean, double aliveMean) {
    vector<double> variance(2);
    double varDead = 0.0;
    double varAlive = 0.0;
    int notSurvivedCount = 0;
    int survivedCount = 0;
    for(int i = 0; i < age.size(); i++) {
        if(survived[i] == 0) {
            varDead = varDead + pow((double)age[i] - deadMean, (double)2);
            notSurvivedCount++;
        } else {
            varAlive = varAlive + pow((double)age[i] - aliveMean, (double)2);
            survivedCount++;
        }
    }

    varDead = varDead / (double)(notSurvivedCount - 1);
    varAlive = varAlive / (double)(survivedCount - 1);

    variance[0] = varDead;
    variance[1] = varAlive;

    return variance;
}

// probability density function -- likelihood of a quantitative variable
double likelihoodQuant(int variable, double mean, double variance) {
    double probabilityDens = 0.0;

    return probabilityDens = (1 / sqrt(2 * M_PI * pow(variance, 2))) * exp(-1 * (pow(variable - mean, 2)) / (2 * pow(variance, 2)));
}

// calculate raw probility for passengers
vector<double> rawProb(vector<vector<double>> pclassLh, vector<vector<double>> sexLh, int age, int sex, int pclass, vector<double> apriori, vector<double> ageMean, vector<double> ageVar){
    double probSurvive = (double)pclassLh[1][pclass] * (double)sexLh[1][sex] * apriori[1] * likelihoodQuant(age, ageMean[1], ageVar[1]);
    double denominator = ((double)pclassLh[1][pclass] * (double)sexLh[1][sex] * apriori[1] * likelihoodQuant(age, ageMean[1], ageVar[1])) + ((double)pclassLh[0][pclass] * (double)sexLh[0][sex] * apriori[0] * likelihoodQuant(age, ageMean[0], ageVar[0]));
    double probDead = (double)pclassLh[0][pclass] * (double)sexLh[0][sex] * apriori[1] * likelihoodQuant(age, ageMean[0], ageVar[0]);
    
    probDead = probDead / denominator;
    probSurvive = probSurvive / denominator;
    vector<double> prob(2);
    prob[0] = probDead;
    prob[1] = probSurvive;

    return prob;
}

// accuracy function
double accuracy(vector<int>& predictions, vector<int>& labels) {
    int matched = 0;
    vector<int> temp = predictions;

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
double sensitivity(vector<int>& predictions, vector<int>& labels) {
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
double specificity(vector<int>& predictions, vector<int>& labels) {
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
    auto start = chrono::steady_clock::now();
    
    cout << "Number of records: " << numObservations << endl;

    // create training data and testing data
    vector<int> pclassTrain(800);
    vector<int> survivedTrain(800);
    vector<int> ageTrain(800);
    vector<int> sexTrain(800);
    
    for(int i = 0; i < 800; i++) {
        pclassTrain[i] = pclass[i];
        survivedTrain[i] = survived[i];
        ageTrain[i] = age[i];
        sexTrain[i] = sex[i];
    }

    int testSize = numObservations - 800;
    int count = 800;
    vector<int> pclassTest(testSize);
    vector<int> survivedTest(testSize);
    vector<int> ageTest(testSize);
    vector<int> sexTest(testSize);
    for(int i = 0; i < testSize; i++) {
        pclassTest[i] = pclass[count];
        survivedTest[i] = survived[count];
        ageTest[i] = age[count];
        sexTest[i] = sex[count];
        count++;
    }


    cout << endl;

    // calculate priors
    vector<double> priors = apriori(survivedTrain);
    cout << "prior probability, survived=no, survived=yes: " << priors[0] << " " << priors[1] << endl;

    cout << endl;

    // calculate likelihood for sex|survived
    vector<vector<double>> probSex = likelihoodOfTwo(survivedTrain, sexTrain);
    cout << "likelihood values for p(sex|survived): " << endl;
        cout << "    [1]    [2]     " << endl; 
    for(int i = 0; i < probSex.size(); i++) {
        cout << "[" << i + 1 << "] ";
        for(int j = 0; j < probSex[0].size(); j++) {
            cout << setprecision(6) << probSex[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    // calculate likelihood for pclass|survived
    vector<vector<double>> probPclass = likelihoodOfThree(survivedTrain, pclassTrain);
    cout << "likelihood values for p(pclass|survived): " << endl;
    cout << "    [1]    [2]    [3]     " << endl; 
    for(int i = 0; i < probPclass.size(); i++) {
        cout << "[" << i + 1 << "] ";
        for(int j = 0; j < probPclass[0].size(); j++) {
            cout << setprecision(6) << probPclass[i][j] << " ";
        }
        cout <<  endl;
    }
    cout << endl;

    // calculate mean of age
    vector<double> means = meanCalc(survivedTrain, ageTrain);
    cout << "means: " << endl;
    cout << "[1] " << means[0] << " " << means[1] << endl;
    cout << endl;

    // calculate variance of ages
    vector<double> variances = varianceCalc(survivedTrain, ageTrain, means[0], means[1]);
    cout << "variances: " << endl;
    cout << "[1] " << variances[0] << " " << variances[1] << endl;
    cout << endl;

    // Apply to first 5 test observations
    vector<double> probSize(2);
    vector<vector<double>> raw(testSize, probSize);
    vector<int> predictions(testSize);
    for(int i = 0; i < testSize; i++) {
        raw[i] = rawProb(probPclass, probSex, ageTest[i], sexTest[i], pclassTest[i], priors, means, variances);
        if(raw[i][0] >= 0.5) {
            predictions[i] = 0;
        } else {
            predictions[i] = 1;
        }
    }
    

    // display probabilities
    cout << "Predictions: " << endl;
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 2; j++) {
            cout << setprecision(5) << raw[i][j] << "             ";
        }
        cout << endl;
    }

    // test metrics
    cout << "accuracy: " << accuracy(predictions, survivedTest) << endl;
    cout << "sensitivity: " << sensitivity(predictions, survivedTest) << endl;
    cout << "specificity: " << specificity(predictions, survivedTest) << endl;
 


    auto end = chrono::steady_clock::now();

    cout << "elapsed time to run this program: " << chrono::duration_cast<chrono::seconds>(end - start).count() << " seconds" << endl;
    return 0;
}