/**
 * @author: Runwei Qiang <qiangrw@gmail.com>
 * @create_time: 2012-7
 */

#include "lib.h"
#include "Index.hpp"
#include "IndexManager.hpp"
#include <exception>

using namespace std;
using namespace lemur::api;
using namespace lemur::parse;
#ifndef CORPUS_CPP
#define CORPUS_CPP

class Corpus
{
    private:
        Index* idx;
        map<string,double> tf;
        int doc_num;
        int term_num;

    public:
        Corpus(string path) {
            idx = IndexManager::openIndex(path);
            doc_num = idx->docCount();
            term_num = idx->termCount();

            cout << "Init MAP TOTAL " << idx->termCountUnique() << endl;
            cout << "Total Doc Count " << idx->docCount() << endl;
            cout << "Total Term Count " << idx->termCount() << endl;
            cout << "Unique Term Count " << idx->termCountUnique() << endl;
            for (int termID = 1; termID <= idx->termCountUnique(); termID++) {
                //cout << "term->document index entries for term : " << idx->term(termID) << " COUNT:" << idx->termCount(termID) << endl;
                tf.insert(make_pair(idx->term(termID), idx->termCount(termID)));
            }
            cout << "Init TF MAP DONE" << endl;
        }

        /**
         * when a new tweet come,change the corpus relatively
         * @return
         */ 
        void add_new_tweet(){
            doc_num ++;
        }

        /**
         * when a new tweet come, add the tweet word to tf map
         * @param string word
         * @return 
         */ 
        void add_tweet_word(string word) {
            if(tf.find(word) != tf.end()) {
                tf[word]++;
            } else {
                tf.insert(make_pair(word,1));
            }
            term_num ++;
        }


        /**
         * get the tf value from index
         * @param string word
         * @return double the tf value of word
         */ 
        double p(string word) 
        {
            if(tf.find(word) != tf.end()) {
                return (double)tf[word] / (double)term_num;
            } else {
                return 0.0;
            }
        }

};
#endif
