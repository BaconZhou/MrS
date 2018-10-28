// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

// we only include RcppArmadillo.h which pulls Rcpp.h in for us
#include "RcppArmadillo.h"
#include "regression.hpp"
#include "split.hpp"
#include "gitree.hpp"
#include <vector>
#include <fstream>
#include <string>

#ifdef _OPENMP
    #include <omp.h>
#endif

// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::plugins(openmp)]]

// [[Rcpp::plugins(cpp11)]]

// [[Rcpp::export]]
Rcpp::IntegerVector characterToInterger(const Rcpp::CharacterVector &x, const Rcpp::CharacterVector &levels) {
    
    const auto &N = x.length();
    const auto &p = levels.length();

    Rcpp::IntegerVector result(N);
    
    for (int i = 0; i < p; i++) {
        for(int j = 0; j < N; j ++) {
            if (Rcpp::CharacterVector::is_na(x(j))) result(j) = p + 1;
            if (x(j) == levels(i)) result(j) = i + 1;
        }
    }
    return result;
}

// [[Rcpp::export]]
Rcpp::NumericMatrix dataFramToNumeric(const Rcpp::DataFrame &numX) {
    const auto &p = numX.ncol();
    const auto &N = numX.nrow();
    
    Rcpp::NumericMatrix result(N, p);
    
    for (int i = 0; i < p; i++) {
        const Rcpp::NumericVector &x = numX[i];
        for (int j = 0; j < N; j++) {
            result(j, i) = Rcpp::NumericVector::is_na(x(j)) ? R_PosInf : x(j);
        }
    }
    return result;
}

// [[Rcpp::export]]
Rcpp::List characterDict(const Rcpp::DataFrame &charX, const Rcpp::List &levels) {
    const auto &p = charX.ncol();
    const auto &N = charX.nrow();
    
    Rcpp::IntegerMatrix cX(N, p);
    
    for (int i = 0; i < p; i ++) {
        const Rcpp::CharacterVector &x = charX[i];
        cX.column(i) = characterToInterger(x, levels[i]);
    }
    
    return Rcpp::List::create(Rcpp::Named("intX") = cX,
                        Rcpp::Named("Levels") = levels);
}



/**
 * Export Variable selection unit
 */

/*
// [[Rcpp::export]]
Rcpp::List GiNumThresh(const arma::vec &x, const arma::mat &numX, const arma::mat &Y, const arma::ivec trt,
             const std::vector<arma::uvec> &bestInd, const int &batchNum, const int& minData, 
             const int &minTrt) {
    
    SubGuide::RegSol::RegFun *linearNode;
    linearNode = new SubGuide::RegSol::LinReg;
    
    SubGuide::SplitSol::GiSplit splitRes(linearNode, batchNum, minData, minTrt);
    splitRes.findNumThresh(x, numX, trt, Y, bestInd);
    
    Rcpp::List result = Rcpp::List::create(Rcpp::Named("Threshold") = splitRes.getThreshold(),
                               Rcpp::Named("MisDirection") = splitRes.getMissDir());
    return result;
}

// [[Rcpp::export]]
Rcpp::List GiCateThresh(const arma::ivec &x, const arma::mat &numX, const arma::mat &Y, const arma::ivec trt,
                const std::vector<arma::uvec> &bestInd, const int &batchNum, const int& minData, 
                const int &minTrt) {
    
    SubGuide::RegSol::RegFun *linearNode;
    linearNode = new SubGuide::RegSol::LinReg;
    
    SubGuide::SplitSol::GiSplit splitRes(linearNode, batchNum, minData, minTrt);
    splitRes.findCateThresh(x, numX, trt, Y, bestInd);
    
    Rcpp::List result = Rcpp::List::create(Rcpp::Named("ThreshSet") = splitRes.getThreshSet());
    return result;
}
*/


// [[Rcpp::export]]
void initLog(const int level) {
     auto logger = spdlog::stdout_logger_st("tree");
     auto loggerS = spdlog::stdout_logger_st("split");
     auto loggerD = spdlog::stdout_logger_st("debug");
     
     if (level == 1) {
         loggerS->set_level(spdlog::level::off);
         logger->set_level(spdlog::level::off);
         loggerD->set_level(spdlog::level::off);
     } else if (level == 2) {
         loggerS->set_level(spdlog::level::info);
         logger->set_level(spdlog::level::info);
         loggerD->set_level(spdlog::level::info);
     } else {
         loggerS->set_level(spdlog::level::debug);
         logger->set_level(spdlog::level::debug);
         loggerD->set_level(spdlog::level::debug);
     }
}


/**
 * Export methods
 */
// [[Rcpp::export]]
void GiStepWisePure(const arma::mat &numX, const arma::imat &catX, 
                const arma::mat &Y, const arma::ivec trt,
                const arma::uvec &splitIndex, const arma::uvec &fitIndex, 
                const arma::uvec &holdIndex, const int &bestK,
                const int &maxDepth, const int& minData, 
                const int &minTrt, const int &batchNum,
                const int &CVFold, const double &CVSE,
                const int &bootNum, const double &alpha,
                const bool &faster, const bool &display,
                const std::vector<std::string>&varName,
                const std::string &treeName, const std::string &nodeName,
                const std::string &bootName) {
    SubGuide::RegSol::RegFun *linearNode;
    linearNode = new SubGuide::RegSol::LinReg;
    
    SubGuide::Tree::GiTree resTree(linearNode, maxDepth, minData, minTrt, batchNum, CVFold, CVSE, bootNum, alpha);
    
    resTree.fit(numX, catX, Y, trt, splitIndex, fitIndex, holdIndex, bestK, faster);
    
    if (display) resTree.display();
    
    std::ofstream myfile;
    myfile.open(treeName);
    myfile << resTree.writeTree(varName);
    myfile.close();
    
    myfile.open(nodeName);
    myfile << "node\n";
    myfile << resTree.predictNode(numX, catX);
    myfile.close();
    
    if (bootNum > 50) {
        myfile.open(bootName);
        myfile << resTree.getBootAlpha();
        myfile.close();  
    } 
}
