#include "Corpus.cpp"
#include "Utility.cpp"
using namespace std;

#ifndef TWEET_CPP
#define TWEET_CPP

class Tweet
{
    private:
        int word_num;               //tweet term count
        string id;                  //tweet id
        string content;             //the tweet content
        map<string, double> tf;     //tweet tf

    public:
        Tweet(string _id, string tweet_text, Corpus &ci)            
        {
            id = _id;
            word_num = 0;
            content = tweet_text;

            ci.add_new_tweet();
            vector<string> word_vector = Utility::split(tweet_text,' ');
            for(int i=0; i<word_vector.size(); i++) {
                add_to_dic(word_vector[i],ci);
            }
        }

        int get_word_num() 
        {
            return word_num;
        }

        string get_id() 
        {
            return id;
        }

        string get_content() 
        {
            return content;
        }

        double p(string word) 
        {
            if(tf.find(word) != tf.end()) {
                return tf[word] / (double)word_num;                              
            } else {
                return 0.0;
            }
        }

    private:
        void add_to_dic(string word, Corpus &ci)
        {
            if(word.length() == 0) return;
            word_num ++;
            if (tf.find(word) != tf.end()) {
                tf[word] += 1;
            } else {
                tf.insert(make_pair(word, 1));
            }
            // add the word to the corpus
            ci.add_tweet_word(word);
        }
};
#endif
