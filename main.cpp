#include <iostream>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;

/**
 * This Perceptron implementation consists of a neuron which can be trained
 * to determine whether given point (x, y) is located above or below graph of f(x) = ax + b function.
 * Perceptron accuracy depends on number of learning iterations.
 * <p>
 * Input number of learning iterations to train the Perceptron and see how it performs -
 * after training it will be tested on 1000 random points.
 * <p>
 * This program uses f(x) = 2x + 50 function, point coordinates ranging from 0 to 100 and
 * random perceptron weights at the start.
 */

class Point{
    double x;
    double y;

public:
    Point(double inX, double inY) {
        x = inX;
        y = inY;
    }

    Point() {
    }

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }
};

class Perceptron {
    double weightX;
    double weightY;
    double bias;
    double learningRate;

public:
    Perceptron(){
        random_device randomDevice;
        mt19937 generator(randomDevice());
        uniform_real_distribution<> distr(0, 1);
        weightX = distr(generator);
        weightY = distr(generator);
        bias = 1;
        learningRate = 0.00001;
    }

    //activation function
    bool perceptronFunction(Point point) {
        return point.getX() * weightX + point.getY() * weightY - bias > 0;
    }

    void learningFunction(double xCorrection, double yCorrection, double biasCorrection) {
        weightX += xCorrection * learningRate;
        weightY += yCorrection * learningRate;
        bias += biasCorrection * learningRate;
    }

    double netOutput(Point point) {
        return point.getX() * weightX + point.getY() * weightY - bias;
    }
};

Point* generatePoints(int arrLength){
    if (arrLength < 0 || arrLength > 100000){
        throw std::invalid_argument("input must be > 0 and < 100 000");
    }

    static Point points[100000];
    random_device randomDevice;
    mt19937 generator(randomDevice());
    uniform_real_distribution<> distr(0, 100);

    for(int i = 0; i < arrLength; i++){
        Point point(distr(generator), distr(generator));
        points[i] = point;
    }
    return points;
};

//verification functions
bool verifierFunction (Point p) {
    return p.getX() * -2 + p.getY() - 50 > 0;
}

double expectedOutput (Point p) {
    return p.getX() * -2 + p.getY() - 50;
}

int main() {
    cout << "Input nr of learning iterations (0 to 100 000): " << endl;
    int iter;
    cin >> iter;

    //create points
    Point* pRandPoints = generatePoints(iter);
    Point randomPoints[iter];

    for (int i = 0; i < iter; i++) {
        randomPoints[i] = *(pRandPoints + i);
    }

    //create perceptron
    Perceptron perceptron;

    //train
    auto sTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    for (Point p : randomPoints){
        bool teacherScore = verifierFunction(p);
        bool perceptronScore = perceptron.perceptronFunction(p);
        if (teacherScore != perceptronScore) {
            double error = expectedOutput(p) - perceptron.netOutput(p);
            double xCorrection = p.getX() * error;
            double yCorrection = p.getY() * error;
            double biasCorrection = error;
            perceptron.learningFunction(xCorrection, yCorrection, biasCorrection);
        }
    }
    auto endTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    long totalTime = (long)endTime - (long)sTime;

    //test
    int nrOfTestPoints = 1000;
    Point* testArrPointer = generatePoints(nrOfTestPoints);
    Point testPoints[nrOfTestPoints];
    for (int i = 0; i < nrOfTestPoints; i++) {
        testPoints[i] = *(testArrPointer + i);
    }

    bool scores[nrOfTestPoints];
    for (int i = 0; i < nrOfTestPoints; i++){
        bool perceptronScore = perceptron.perceptronFunction(testPoints[i]);
        bool verifierScore = verifierFunction(testPoints[i]);
        if (perceptronScore == verifierScore){
            scores[i] = true;}
        else {
            scores[i] = false;}
    }
    int correctCount = 0;
    for (bool b : scores) {
        if (b)
            correctCount++;
    }

    //print result
    cout << "Nr of learning iterations: " << iter << endl;
    cout << "Learning time: " << totalTime << " milliseconds" << endl;
    cout << "Correct answers: " << correctCount << " out of " << nrOfTestPoints << endl;

    return 0;
}
