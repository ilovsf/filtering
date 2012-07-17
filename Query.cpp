#include "lib.h"
#include "Utility.cpp"
#include "Corpus.cpp"
using namespace std;

#ifndef QUERY_CPP
#define QUERY_CPP

class Query
{
    private:
        int word_num;
        string id;
        string topic;
        map<string, double> tf;

    public:
        Query(string _id, string _topic, Corpus &ci)
        {
            // Add MicroBlog Track Prefix
            if (_id.length() == 1) id = "MB00" + _id;
            else id = "MB0" + _id;
            topic = _topic;

            vector<string> topic_vector =  Utility::split(_topic,' ');
            word_num = topic_vector.size();
            for (int i = 0; i < topic_vector.size(); i++){
                add_to_dic(topic_vector[i]);
            }
        }

        string get_id() 
        {
            return id;
        }

        string get_topic() 
        {
            return topic;
        }

        map<string, double> get_tf() 
        {
            return tf;
        }

        double p(string word) 
        {
            if (tf.find(word) == tf.end()) {
                return 0.0;
            } else {
                return tf[word] / (double)word_num;
            }
        }

    private:
        void add_to_dic(string word) 
        {
            if (tf.find(word) == tf.end()) {
                tf.insert(make_pair(word, 1.0));
            } else {
                tf[word] += 1;
            }	
        }
};
#endif
