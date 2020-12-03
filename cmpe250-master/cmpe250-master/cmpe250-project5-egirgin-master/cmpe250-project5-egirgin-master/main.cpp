//RABINKARP ALGORITHM FROM : https://github.com/maheshwari-ashish/algorithms/blob/master/Rabin-karp.cpp
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

vector<int> rabinKarp(string str, string pat) {
    vector<int> result;
    if (str.length() && pat.length()) {          // check for empty strings
        int n = str.length();
        int m = pat.length();
        int i, j;                                // iterators for loops
        int s = 0, p = 0;                        // s = hash of string, p = hash of pattern
        const int pm = 256;                      // no of characters in alphabet
        const int q = 101;                       // large prime number
        int h = 1;                               // h = multiplier for MSB
        bool flag = false;

        // the value of h would be eqv to pow(pm, m-1)
        for (i = 0; i < m-1; i++)
            h = (h * pm) % q;

        // calculating initial hash for string and pattern
        for (i = 0; i < m; i++) {
            s = (pm * s + str[i]) % q;
            p = (pm * p + pat[i]) % q;
        }

        for (i = 0; i <= n-m; i++) {
            if (s == p) {                       // comparing hash of string and pattern
                for (j = 0; j < m; j++)
                    if (str[i+j] != pat[j])
                        break;
                if (j == m) {
                    //cout<<"Pattern found at pos: "<<i+1<<endl;
                    flag = true;
                    result.push_back(i+1);
                }
            }
            // calculating hash of next substring
            s = (pm * (s - h * str[i]) + str[i+m]) % q;
            if (s < 0)                          // still dont know why we do this :|
                s = s + q;
        }
        if (!flag) {
            //cout << "Pattern not found.." << endl;
        }
        else
            return result;
    }
    //cout<<"Text or pattern cannot be empty.."<<endl;
}

int main(int argc, char* argv[]) {

    clock_t start = clock();

    ifstream scanner(argv[1]);

    string message;
    scanner >> message;

    int message_size = message.size();

    int dic_size;

    scanner >> dic_size;

    priority_queue< pair<int,string> , vector<pair<int,string>>, greater<pair<int,string>> > indices;

    vector<vector<int>> indexler (message_size+1, vector<int>());

    for(int i = 0; i<dic_size;i++){
        string word;
        scanner >> word;
        vector<int> index = move(rabinKarp(message,word));

        for(int j=0;j<index.size();j++){
            indexler[index[j]].push_back(word.size());
        }

        /*
        for(int j=0;j<index.size();j++){
            indices.push(make_pair(index[j], word));
        }
         */
    }

    scanner.close();

    vector<long long int> dp(message.size()+1,0);
    //int dp[1001] = {0};
    dp[0] = 1;

    for(int i=0;i<indexler.size();i++){
        for(int j=0;j<indexler[i].size();j++){
            dp[i+indexler[i][j]-1] = (dp[i+indexler[i][j]-1]+dp[i-1])%(1000000007);
        }
    }

    /*
    while(!indices.empty()){
        pair<long long int, string> words = indices.top();
        dp[words.first+words.second.length()-1] = (dp[words.first+words.second.length()-1]+dp[words.first-1])%(1000000007);
        indices.pop();
    }
     */

    ofstream writer(argv[2]);

    writer<<dp[message.size()];

    writer.close();

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    return 0;
}