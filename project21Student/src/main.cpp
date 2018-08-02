#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "mnist_reader.hpp"
#include "mnist_utils.hpp"
#include "bitmap.hpp"
#include <sstream>
#include <fstream>
#define MNIST_DATA_DIR "../../mnist_data"


static void calculatePriorProbabilities( mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset);
static void calculateForEachPixelProbability(mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset);
static void calculateProbabilityBelongsToClass(mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset);

int main(int argc, char* argv[]) {
    //Read in the data set from the files
    mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset =
    mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(MNIST_DATA_DIR);
    //Binarize the data set (so that pixels have values of either 0 or 1)
    mnist::binarize_dataset(dataset);
    //There are ten possible digits 0-9 (classes)
    int numLabels = 10;
    //There are 784 features (one per pixel in a 28x28 image)
    int numFeatures = 784;
    //Each pixel value can take on the value 0 or 1
    int numFeatureValues = 2;
    //image width
    int width = 28;
    //image height
    int height = 28;
    //image to print (these two images were randomly selected by me with no particular preference)
    int trainImageToPrint = 50;
    int testImageToPrint = 5434;
    // get training images
    std::vector<std::vector<unsigned char>> trainImages = dataset.training_images;
    // get training labels
    std::vector<unsigned char> trainLabels = dataset.training_labels;
    // get test images
    std::vector<std::vector<unsigned char>> testImages = dataset.test_images;
    // get test labels
    std::vector<unsigned char> testLabels = dataset.test_labels;
    //print out one of the training images
    for (int f=0; f<numFeatures; f++) {
        // get value of pixel f (0 or 1) from training image trainImageToPrint
        int pixelIntValue = static_cast<int>(trainImages[trainImageToPrint][f]);
        if (f % width == 0) {
            std::cout<<std::endl;
        }
        std::cout<<pixelIntValue<<" ";
    }
    std::cout<<std::endl;
    // print the associated label (correct digit) for training image trainImageToPrint
    std::cout<<"Label: "<<static_cast<int>(trainLabels[trainImageToPrint])<<std::endl;
    //print out one of the test images
    for (int f=0; f<numFeatures; f++) {
        // get value of pixel f (0 or 1) from training image trainImageToPrint
        int pixelIntValue = static_cast<int>(testImages[testImageToPrint][f]);
        if (f % width == 0) {
            std::cout<<std::endl;
        }
        std::cout<<pixelIntValue<<" ";
    }
    std::cout<<std::endl;
    // print the associated label (correct digit) for test image testImageToPrint
    std::cout<<"Label: "<<static_cast<int>(testLabels[testImageToPrint])<<std::endl;
    std::vector<unsigned char> trainI(numFeatures);
    std::vector<unsigned char> testI(numFeatures);
    for (int f=0; f<numFeatures; f++) {
        int trainV = 255*(static_cast<int>(trainImages[trainImageToPrint][f]));
        int testV = 255*(static_cast<int>(testImages[testImageToPrint][f]));
        trainI[f] = static_cast<unsigned char>(trainV);
        testI[f] = static_cast<unsigned char>(testV);
    }
    std::stringstream ssTrain;
    std::stringstream ssTest;
    ssTrain << "../../output/train" <<trainImageToPrint<<"Label"<<static_cast<int>(trainLabels[trainImageToPrint])<<".bmp";
    ssTest << "../../output/test" <<testImageToPrint<<"Label"<<static_cast<int>(testLabels[testImageToPrint])<<".bmp";
    Bitmap::writeBitmap(trainI, 28, 28, ssTrain.str(), false);
    Bitmap::writeBitmap(testI, 28, 28, ssTest.str(), false);

    
    //calculatePriorProbabilities(dataset);
   // calculateForEachPixelProbability(dataset);
    calculateProbabilityBelongsToClass(dataset);
     

    //call network output
    //call classification output

    return 0;
}


// classification-summary.txt 
//This file should contain a 10 × 10 matrix of integers. 
//The integer in row r and column c should be the number of images in the test set of digit r 
//which our model predicted was an image of digit c. 
//The final line of this file should be the final accuracy of your Naive Bayes model 
//(number correctly classified/10,000) on the test set of 10,000 images. For comparison,
// the reference implementation achieves a test accuracy of 84.43%.

// For a test image Ti and pixel Fj, let ti,j denote the value of pixel j in test image Ti. 
// Note that ti,j will be either 0 or 1 because we have binarized the test set as well as the training set.
// For each test image Ti, compute the probability that it belongs to each class c 2 {0,1,2,3,4,5,6,7,8,9}:
// P(C=c|F0 =ti,0,...,F783 =ti,783)=P(F0 =ti,0,...,F783 =ti,783|C=c)·P(C=c) P(F0 =ti,0,...,F783 =ti,783)
// = (Qj2{0,...,783} PL(Fj = ti,j|C = c)) · P(C = c)

//output to classification for testing


//calculate testing called by classification

//matrix

//visualization from networking = print avergae representative image based on training set

static void calculateProbabilityBelongsToClass(mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset) {
    
    std::vector<std::vector<unsigned char>> testImages = dataset.test_images; // get test images
    std::vector<unsigned char> testLabels = dataset.test_labels; // get test labels

    std::ofstream classification;
    classification.open ("../classification-summary.txt");
    classification << "Testing writing to classy summary \n";
    classification.close();
}

// ForeachpixelFj forj2{0,...,783}andforeachclassc2{0,1,2,3,4,5,6,7,8,9}, 
// determine P (Fj = 1|C = c), the probability that pixel Fj is white given 
// that it is an image of digit c:
// P (Fj = 1|C = c) = #{images of digit c where pixel Fj is white}
// Your program should output two additional files:
// • network.txt This file should list the values of all conditional probabilities parameterizing the network. The first 784 lines 
// should be P (Fj = 1|C = 0). The next 784 lines should be P(Fj = 1|C = 1). The final 10 lines should be the prior probabilities of 
// each class, in order from 0 to 9.
//output to network for training
static void calculateForEachPixelProbability(mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset) {
    // get training images
    std::vector<std::vector<unsigned char>> trainImages = dataset.training_images;
    // get training labels
    std::vector<unsigned char> trainLabels = dataset.training_labels;
    std::map<int, std::map<int, int> > classToFCount; //map to store each class to each f to count of whites
    std::map<int, int> tempFCount;   //temp map to store white counts for each f   key->f, value->count of 1s
    std::map<int, int>::iterator itf; //map iterator to find f
    std::map<int, std::map<int, double>> classToFProbabilities;

    //initialize my map <pixel fj, nubmer of 1s at that pixel for class c> 
    //all are 0
    for (int i = 0; i <= 783; i++) {
        std::pair<int, int>mypair(i, 0);
        tempFCount.insert(mypair);
    }

    //Loop through images/their labels
    for (int i = 0; i < trainLabels.size(); i++) {
         std::map<int, std::map<int, int>>::iterator itc = classToFCount.find(trainLabels.at(i));
        
        if (itc == classToFCount.end()) { //not found so initialize with empty map
            std::pair<int, std::map<int, int>> mypair(trainLabels.at(i), tempFCount);
            classToFCount.insert(mypair);
        }
        
        for (int c = 0; c <= 783; c++) {
            if (trainImages.at(i)[c] == 1) {
                classToFCount[trainLabels.at(i)][c] += 1;
            } 
        }
    }
    std::map<int,int> myCount;
    std::map<int, int>::iterator itf2;
    for (unsigned i=0; i<trainLabels.size(); i++) {
        itf2 = myCount.find(trainLabels.at(i));
        if (itf2 == myCount.end()) {
            std::pair<int, int> mypair (trainLabels.at(i), 1);
            myCount.insert(mypair);
        }
        else {
            myCount[trainLabels.at(i)] += 1;
        }
    }

    // std::map<int, double> classProb;
    // for (int i = 0; i <= 9; i++) {
    //     double p = (double)(myCount[i])/(trainImages.size());
    //     std::pair<int, double> myprobpair(i, p);
    //     classProb.insert(myprobpair);     
    //     // std::cout << "CLASS NUMBER: " << i;
    //     // printf("  PROBABILITY: %lf \n", classProb[i]);
    // }
    std::map <int , double> tempProb; //temp map of white pixels divided by count images for the class
     for (int i = 0; i <= 783; i++) {
        std::pair<int, double>mypair(i, 0.0);
        tempProb.insert(mypair);
    }
    std::ofstream network;
    network.open ("../network.txt");

   // std::map<int, std::map<int, double>>::iterator itp; 
    for (int i = 0; i < classToFCount.size(); i++) {
        std::map<int, std::map<int, double>>::iterator it3 = classToFProbabilities.find(i);
        if (it3 == classToFProbabilities.end()) { //not found so initialize with empty map
            std::pair<int, std::map<int, double>> mypair(i, tempProb);
            classToFProbabilities.insert(mypair);
        } 
        
        for (int j = 0; j <= 783; j++) {
            double probFinal = (double) (classToFCount[i][j]+1)/(myCount[i]+2);
            double p = (double)1-probFinal;
            classToFProbabilities[i][j] = probFinal;
            network << (double) classToFProbabilities[i][j] << "\n";
        }
    }       
    
    //TESTING
    // for (int i = 0; i < 10; i++) {
    //     printf("PROBS AT 42 PIXEL: %lf \n", (double)classToFProbabilities[i][42]);
    // }
  network.close();
}


//calculate prior called by network for training set only
// etermine the prior probabilities for each class c ∈ {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
// : P(C = c) = #{images of digit c}  9
// Makesurethat:  P(c)=1.

static void calculatePriorProbabilities( mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset) {
    // get training images
    std::vector<std::vector<unsigned char>> trainImages = dataset.training_images;
    // get training labels
    std::vector<unsigned char> trainLabels = dataset.training_labels;


    //Hashmap < int , int> count where key is class and value is count for specific class
std::map<int,int> myCount;
std::map<int, int>::iterator itf;
    for (unsigned i=0; i<trainLabels.size(); i++) {
        itf = myCount.find(trainLabels.at(i));
        if (itf == myCount.end()) {
             std::pair<int, int> mypair (trainLabels.at(i), 1);
             myCount.insert(mypair);
        }
        else {
            //itf->second+=1;
           // myCount.put(trainLabels.at(i), myCount.get(trainLabels.at(i)) + 1);
            myCount[trainLabels.at(i)] += 1;
        }
    }

    //PRINTING COUNT MAP
   // std::map<int, int>::iterator itp;
    // for (itp = myCount.begin(); itp!= myCount.end(); itp++) {
    //     std::cout << "FIRST: " << itp->first << "SECOND: " << itp->second << std::endl;
    // }

int totalImages = trainImages.size();
double totalProbVerification = 0.0; //should be 1.0

std::map<int, int>::iterator it;
std::map<int, double> myProbabilities;
    for (it = myCount.begin(); it != myCount.end(); it++) {
        std::pair<int, double> probpair (it->first, (double)(it->second)/(double)totalImages);
        myProbabilities.insert(probpair);
        totalProbVerification += (double)(it->second)/(double)totalImages;
    }

   // std::cout << "TOTAL VERIFIATION: " << totalProbVerification << std::endl;
}

