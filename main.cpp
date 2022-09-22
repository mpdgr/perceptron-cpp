#include <iostream>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Point{
public:
    Point(double inX, double inY){
        x = inX;
        y = inY;
    }

    Point(){
    }

private:
    double x;
    double y;

public:
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
    double threshold;
public:
    Perceptron(){
        random_device randomDevice;
        mt19937 generator(randomDevice());
        uniform_real_distribution<> distr(0, 1);
        weightX = distr(generator);
        weightY = distr(generator);
        threshold = distr(generator);
    }

    bool perceptronFunction(Point point) {
        return point.getX() * weightX + point.getY() * weightY > threshold;
    }

    void learningFunction(int score) {
        switch (score) {
            case  1:
                weightX += 0.001;
            case -1:
                weightX -= 0.001;
        }
    }
};

Point* generatePoints(int arrLength){
    if (arrLength > 10000){
        throw std::invalid_argument("points array can't exceed 100 000 length");
    }

    static Point points[10000];
    random_device randomDevice;
    mt19937 generator(randomDevice());
    uniform_real_distribution<> distr(0, 1);

    for(int i = 0; i < arrLength; i++){
        Point point(distr(generator) * 100000, distr(generator) * 100000);
        points[i] = point;
    }
    return points;
};

//verification function
bool verifierFunction (Point p) {
    return p.getY() > p.getX() * 2;
}

int main() {

    //create points
    int nrOfFeedPoints = 10000;
    Point* arrPointer = generatePoints(nrOfFeedPoints);
    Point randomPoints[nrOfFeedPoints];

    for (int i = 0; i < nrOfFeedPoints; i++) {
        randomPoints[i] = *(arrPointer + i);
    }

    //create new perceptron
    Perceptron perceptron;

    //learn
    auto sTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    for (Point p : randomPoints){
        bool teacherScore = verifierFunction(p);
        bool perceptronScore = perceptron.perceptronFunction(p);
        if (teacherScore && !perceptronScore){perceptron.learningFunction(1);}
        if (!teacherScore && perceptronScore){perceptron.learningFunction(-1);}
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
    int trueCount = 0;
    for (bool b : scores) {
        if (b)
            trueCount++;
    }

    //print result
    cout << "Nr of learning iterations: " << nrOfFeedPoints << endl;
    cout << "Learning time: " << totalTime << " milliseconds" << endl;
    cout << "Correct answers: " << trueCount << " out of " << nrOfTestPoints << endl;

    return 0;
}
