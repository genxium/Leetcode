#define MAX_RECENT_TWEET_COUNT 10
#define PI pair<int, int>
#define VI vector<int>

/*
test case #1
["Twitter","postTweet","getNewsFeed","follow","getNewsFeed","unfollow","getNewsFeed"]
[[],[1,1],[1],[2,1],[2],[2,1],[2]]

*/
class Twitter {
private:
    int t = 0; // "t" is short for "time"
    unordered_map<int, unordered_set<int>> followees; // "followees[x]" is the set of "userId"s followed by userId "x" 
    unordered_map<int, list<PI>> tweets;
    
public:
    /** Initialize your data structure here. */
    Twitter() {
        
    }
    
    /** Compose a new tweet. */
    void postTweet(int userId, int tweetId) {
        //printf("postTweet, userId:%d, tweetId:%d\n", userId, tweetId);
        followees[userId].insert(userId);
        if (tweets.find(userId) == tweets.end()) {
            tweets[userId] = {};
        }
        tweets[userId].push_back({t, tweetId});
        if (MAX_RECENT_TWEET_COUNT < tweets[userId].size()) {
            tweets[userId].pop_front();
        }
        ++t;
        //printf("\tpostTweet, returning\n");
    }
    
    /** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
    vector<int> getNewsFeed(int userId) {
        //printf("getNewsFeed, userId:%d\n", userId);

        vector<int> ans;
        
        priority_queue<PI, vector<PI>, less<PI>> pq;
        
        // A better handling should be like https://github.com/genxium/Leetcode/tree/master/p23_Merge_K_Sorted_Lists.
        auto &thatFollowees = followees[userId];
        for (auto followee : thatFollowees) {
            for (auto ele : tweets[followee]) {
                pq.push(ele);
            }
        }
        
        for (int i = 0; i < MAX_RECENT_TWEET_COUNT && !pq.empty(); ++i) {
            ans.push_back(pq.top().second); pq.pop();
        }
        //printf("\tgetNewsFeed, returning\n");

        return ans;
    }
    
    /** Follower follows a followee. If the operation is invalid, it should be a no-op. */
    void follow(int followerId, int followeeId) {
        //printf("follow, followerId:%d, followeeId\n", followerId, followeeId);
        followees[followerId].insert(followerId);
        followees[followerId].insert(followeeId);
        //printf("\tfollow, returning\n");
    }
    
    /** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
    void unfollow(int followerId, int followeeId) {
        //printf("unfollow, followerId:%d, followeeId\n", followerId, followeeId);
        if (followerId == followeeId) return;
        followees[followerId].erase(followeeId);
        //printf("\tunfollow, returning\n");
    }
};

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter* obj = new Twitter();
 * obj->postTweet(userId,tweetId);
 * vector<int> param_2 = obj->getNewsFeed(userId);
 * obj->follow(followerId,followeeId);
 * obj->unfollow(followerId,followeeId);
 */
