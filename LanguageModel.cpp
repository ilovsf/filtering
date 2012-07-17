/*
 * The LanguageModel Class
 * @author: Runwei Qiang <qiangrw@gmail.com>
 * @created Time: 2012/7/13
 * @file: LanguageModel.cpp
 * @description: Using language model to complete the filtering task 
 ************************************************************************
 */

#include "lib.h" 
#include "Tweet.cpp"
#include "Corpus.cpp"
#include "Query.cpp"

using namespace std;

#ifndef LANGUAGEMODEL_CPP
#define LANGUAGEMODEL_CPP

class LanguageModel
{
    private:
        double thresold;        // score thresold
        double mu;              // Diri parameter mu
        string tweet_path;      // tweet path to be processed

    public:
        LanguageModel()
        {
            tweet_path = "";
            thresold = 0;
            mu = 1000;
        }

        LanguageModel(string _tweet_path,  double _mu, double _thresold)
        {
            tweet_path = _tweet_path;
            thresold = _thresold;
            mu = _mu;
        }


        /**
         * LanguageModel::processTweets
         *
         * @param Corpus &ci
         * @param query &q
         * @return void
         */ 
        void processTweets(Corpus &ci, Query &q, string run = "PKUICST",  bool debug = false)
        {
            ifstream file(tweet_path.c_str());
            string line;
            while(getline(file, line)){
                int pos = line.find("\t");
                string id = line.substr(0,pos);
                Tweet t(id,line.erase(0,pos+1),ci);

                double score = computeKLDivergence(q,t,ci); 
                if(score >= thresold) {
                    printResult(q,t,score,"yes",run,debug);
                }

            }
            file.close();
        }


        /**
         * LanguageModel::processTweetsBash
         *
         * @param Corpus &ci
         * @param Query &q
         * @param double inc
         * @param int number
         * @param string result_path
         * @return void
         */ 
        void processTweetsBash(ofstream output[],Corpus &ci, Query &q, double inc, int number=0, string result_path="", string run="PKUICST") {
            string line;
            ifstream file(tweet_path.c_str());

            while(getline(file, line)){
                ci.add_new_tweet();
                int pos = line.find("\t");
                string id = line.substr(0,pos);
                Tweet t(id,line.erase(0,pos+1),ci);

                double score = computeKLDivergence(q,t,ci); 
                for(int i=0; i<number; i++) {
                    double cur_thresold = thresold + inc * double(i);
                    if(score >= cur_thresold) {
                        printResultToFile(q,t,output[i],score,"yes",run);
                    }
                }
            }
            file.close();
        }


        /**
         * LanguageModel::printResult() print the tweet to the terminal
         *
         * @param Query q
         * @param Tweet t
         * @param string flag yes/no
         * @param string run  run name
         * @return void
         */ 
        void printResult(Query &q, Tweet &t, double score, string flag = "yes", string run = "PKUICST", bool debug = false ) {
            if(debug) cout <<  q.get_id() << " " << t.get_id() << " " << score << " " << t.get_content() << endl;
            else cout <<  q.get_id() << " " << t.get_id() << " " << score << " " << flag << " " << run << endl;
            
        }

        /**
         * LanguageModel::printResultToFile() print the tweet to the terminal
         *
         * @param Query q
         * @param Tweet t
         * @param ofstream output
         * @param string flag yes/no
         * @param string run  run name
         * @return void
         */ 
        void printResultToFile(Query &q, Tweet &t, ofstream &OUTPUT, double score, string flag = "yes", string run = "PKUICST" ) {
            if(!OUTPUT) {
                perror("WRITE ON CLOSED HANDLE");
                return;
            }
            OUTPUT <<  q.get_id() << " " << t.get_id() << " " << score << " " << flag << " " << run << endl;
        }

        /**
         * LanguageModel::computeKLDivergence compute the KLDivergence of query q and tweet t
         *
         * @param Query q
         * @param Tweet t
         * @param Corpus ci
         */ 
        double computeKLDivergence(Query& q,  Tweet &t, Corpus &collection)
        {
            double score = 0.0;
            map <string, double> tf = q.get_tf();
            map <string, double>::iterator it = tf.begin();
            while(it != tf.end()) {
                string word = it->first;
                double p_w_in_q = q.p(word);
                double p_w_in_t = t.p(word);
                double p_w_in_c = collection.p(word);

                double alpha = mu / (t.get_word_num() + mu);
                double p_w_in_d = (1-alpha) * p_w_in_t + alpha * p_w_in_c;

                score += (p_w_in_q * log(p_w_in_d));
                ++it;
            }
            return score;
        }


};

#endif
