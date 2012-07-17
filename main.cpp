/*
 *************************************************************************
 * @author: Runwei Qiang <qiangrw@gmail.com>
 * @create_time: 2012-7-13
 * @description: the main file
 *
 ************************************************************************
 */

#include "lib.h"
#include "Query.cpp"
#include "LanguageModel.cpp"
#include "Utility.cpp"
#include "Corpus.cpp"

/**
 * The Main Function
 * argv[0]: the config file
 * argv[1]: query id model,only the query which meets NO % 10 == argv[1] will be processed 
 */
int main(int argc, char **argv) 
{
    if(argc <= 1) {
        perror("USUAGE:Parameter File Need For Filtering\n");
        return 0;
    }

    string config_file = argv[1];
    map<string, string> config = Utility::load_config_file(config_file);

    // Open the Query Dir And Read All Files in the dir
    string query_dir =  config["queryPath"];
    vector<string> files = Utility::get_dir_files(query_dir);
    if(files.size() != 50) {
        perror("[ERROR] query dir should have 50 files\n");
        exit(-1);
    }


    string result_path = config["resultPath"];
    double thresold = atof(config["thresold"].c_str());
    double inc = atof(config["inc"].c_str());
    int number = atoi(config["number"].c_str());
    double mu = atof(config["mu"].c_str());
    const int MAX_BASH_NUM = 500;
    if(number > MAX_BASH_NUM) {
        cout << "[ERROR]number should not bigger than " << MAX_BASH_NUM << endl;
        exit(-1);
    }

    // open the output files
    ofstream output[MAX_BASH_NUM];

    for(int i=0; i<number; i++) {
        double cur_thresold = thresold + inc * double(i);
        char result_file[255];
        sprintf(result_file,"%slm_%f.res",result_path.c_str(), cur_thresold);
        output[i].open(result_file);
        if(!output[i]) {
            perror("ERROR IN OPEN RES FILE FOR WRITE\n");
            exit(-1);
        }
    }
    string run = config["run"];

    for (int i = 0; i < files.size(); i++)
    {
        string id = files[i];

        int v = atoi(config["v"].c_str());
        // Skip the querys that we don't care
        if (v != -1 && atoi(id.c_str()) % 10 != v) continue;

        cout << "process " << id << endl; 
        string topic = Utility::get_topic_from_file(query_dir + files[i]);
        cout << "topic:" << topic << endl;

        Corpus ci(config["indexPath"] + config["indexEx"] + id + "/basic.key"); 
        Query q(id, topic, ci);

        if(config["algorithm"].compare("lm") == 0) {
            string corpus_path = config["corpusPath"] + id + ".corpus";

            LanguageModel lm(corpus_path, mu, thresold);
            lm.processTweets(ci,q,run,true);
            //lm.processTweetsBash(output,ci,q,inc,number,result_path,run);

        }
    }

    // close the output files
    for(int i=0; i<number; i++) {
        output[i].close();
    }
    return 0;
}

