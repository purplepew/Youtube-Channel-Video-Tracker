#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>
using namespace std;

struct VideoNode {
    int videoId;
    string title;
    string description;
    string tags;
    string uploadDate;
    int views;
    int likes;
    int comments;
};

struct ChannelNode {
    int channelId;
    string channelName;
    string ownerName;
    vector<VideoNode> videos;
};

vector<ChannelNode> channels;
int CHANNEL_ID = 100;
int VIDEO_ID = 100;

// Navigation system variables and functions
int currentSelection = 0;
const int NUM_MENU_OPTIONS = 4;
string menuOptions[] = {"Add Channel", "Add Video to Channel", "Display All", "Exit"};

void handleArrowKeys(char key, int& ctr, int size) {
    if (key == 80 && ctr == size - 1)      // Down arrow at last item
        ctr = 0;
    else if (key == 72 && ctr == 0)        // Up arrow at first item
        ctr = size - 1;
    else if (key == 80)                    // Down arrow
        ctr = ctr + 1;
    else if (key == 72)                    // Up arrow
        ctr = ctr - 1;
}

void displayMenu(int selectedOption) {
    system("cls");
    cout << "\n=== YouTube Channel Video Tracker ===\n\n";
    cout << "Use Arrow Keys to navigate, Enter to select, Q to quit\n\n";
    
    for (int i = 0; i < NUM_MENU_OPTIONS; i++) {
        if (i == selectedOption) {
            cout << "-> " << menuOptions[i] << " <-\n";
        } else {
            cout << "   " << menuOptions[i] << "\n";
        }
    }
    cout << "\nSelection: " << selectedOption + 1 << "/" << NUM_MENU_OPTIONS;
}

int getMenuSelection() {
    char key;
    
    while (true) {
        displayMenu(currentSelection);
        key = getch();
        
        // Handle arrow keys
        if (key == 72 || key == 80) {  // Up or Down arrow
            handleArrowKeys(key, currentSelection, NUM_MENU_OPTIONS);
        }
        // Handle Enter key (selection)
        else if (key == 13) {  // Enter key
            return currentSelection + 1;  // Return 1-based selection
        }
        // Handle quit
        else if (key == 'q' || key == 'Q') {
            return 0;  // Exit
        }
    }
}

void waitForKeyPress() {
    cout << "\n--> Press any key to continue...";
    getch();
}

void showSuccessMessage(const string& message) {
    cout << "\n" << string(50, '=') << "\n";
    cout << "SUCCESS: " << message << "\n";
    cout << string(50, '=') << "\n";
}

void showErrorMessage(const string& message) {
    cout << "\n" << string(50, '=') << "\n";
    cout << "ERROR: " << message << "\n";
    cout << string(50, '=') << "\n";
}

// Algorithm-related functions
string toLowerCase(const string& str);
int calculateEngagementScore(const string& title, const string& description, const string& tags);
void generateVideoAttributes(VideoNode& video);
bool containsKeyword(const string& text, const string& keyword);

// Core functions
void flushInput();
void addChannel();
int findChannelIndexById(int id);
void addVideoToChannel();
void displayAll();
void addPredefinedChannels();

int main() {
    srand(static_cast<unsigned int>(time(0)));
    addPredefinedChannels();
    
    int choice;
    
    while (true) {
        choice = getMenuSelection();
        
        system("cls");
        
        switch (choice) {
            case 1: 
                cout << "\n=== ADD CHANNEL ===\n";
                addChannel(); 
                waitForKeyPress();
                break;
            case 2: 
                cout << "\n=== ADD VIDEO TO CHANNEL ===\n";
                addVideoToChannel(); 
                waitForKeyPress();
                break;
            case 3: 
                cout << "\n=== DISPLAY ALL CHANNELS ===\n";
                displayAll(); 
                waitForKeyPress();
                break;
            case 0: 
                system("cls");
                cout << "\n=== GOODBYE! ===\n";
                cout << "Thank you for using YouTube Channel Video Tracker!\n";
                waitForKeyPress();
                return 0;
            default: 
                showErrorMessage("Invalid selection!");
                waitForKeyPress();
        }
    }
}

string toLowerCase(const string& str) {
    string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = static_cast<char>(tolower(result[i]));
    }
    return result;
}

bool containsKeyword(const string& text, const string& keyword) {
    string lowerText = toLowerCase(text);
    string lowerKeyword = toLowerCase(keyword);
    return lowerText.find(lowerKeyword) != string::npos;
}

int calculateEngagementScore(const string& title, const string& description, const string& tags) {
    int score = 100; // Base score
    
    // High-appeal keywords for 18-22 year old men
    vector<string> highAppealKeywords;
    highAppealKeywords.push_back("gaming");
    highAppealKeywords.push_back("game");
    highAppealKeywords.push_back("sports");
    highAppealKeywords.push_back("football");
    highAppealKeywords.push_back("basketball");
    highAppealKeywords.push_back("tech");
    highAppealKeywords.push_back("technology");
    highAppealKeywords.push_back("fitness");
    highAppealKeywords.push_back("workout");
    highAppealKeywords.push_back("car");
    highAppealKeywords.push_back("cars");
    highAppealKeywords.push_back("music");
    highAppealKeywords.push_back("rap");
    highAppealKeywords.push_back("hip hop");
    highAppealKeywords.push_back("meme");
    highAppealKeywords.push_back("funny");
    highAppealKeywords.push_back("comedy");
    highAppealKeywords.push_back("tutorial");
    highAppealKeywords.push_back("review");
    highAppealKeywords.push_back("unboxing");
    
    // Medium-appeal keywords
    vector<string> mediumAppealKeywords;
    mediumAppealKeywords.push_back("lifestyle");
    mediumAppealKeywords.push_back("travel");
    mediumAppealKeywords.push_back("food");
    mediumAppealKeywords.push_back("movie");
    mediumAppealKeywords.push_back("film");
    mediumAppealKeywords.push_back("education");
    mediumAppealKeywords.push_back("science");
    mediumAppealKeywords.push_back("programming");
    mediumAppealKeywords.push_back("coding");
    
    // Low-appeal keywords for this demographic
    vector<string> lowAppealKeywords;
    lowAppealKeywords.push_back("cooking");
    lowAppealKeywords.push_back("recipe");
    lowAppealKeywords.push_back("fashion");
    lowAppealKeywords.push_back("makeup");
    lowAppealKeywords.push_back("beauty");
    lowAppealKeywords.push_back("crafts");
    lowAppealKeywords.push_back("diy");
    lowAppealKeywords.push_back("gardening");
    
    string combinedText = title + " " + description + " " + tags;
    
    // Check for high-appeal keywords
    for (size_t i = 0; i < highAppealKeywords.size(); ++i) {
        if (containsKeyword(combinedText, highAppealKeywords[i])) {
            score += 50;
        }
    }
    
    // Check for medium-appeal keywords
    for (size_t i = 0; i < mediumAppealKeywords.size(); ++i) {
        if (containsKeyword(combinedText, mediumAppealKeywords[i])) {
            score += 25;
        }
    }
    
    // Check for low-appeal keywords (reduce score)
    for (size_t i = 0; i < lowAppealKeywords.size(); ++i) {
        if (containsKeyword(combinedText, lowAppealKeywords[i])) {
            score -= 30;
        }
    }
    
    // Bonus for trending words/phrases
    if (containsKeyword(combinedText, "viral") || 
        containsKeyword(combinedText, "trending") ||
        containsKeyword(combinedText, "epic") ||
        containsKeyword(combinedText, "insane") ||
        containsKeyword(combinedText, "crazy")) {
        score += 40;
    }
    
    // Ensure minimum score
    if (score < 50) score = 50;
    
    return score;
}

void generateVideoAttributes(VideoNode& video) {
    int engagementScore = calculateEngagementScore(video.title, video.description, video.tags);
    
    // Base views calculation (influenced by engagement score)
    int baseViews = 500 + (engagementScore * 10);
    int randomFactor = rand() % 1000 + 1; // 1-1000
    video.views = baseViews + randomFactor;
    
    // Likes calculation (typically 3-8% of views for this demographic)
    double likeRate = 0.03 + (engagementScore / 10000.0); // 3% base + bonus
    if (likeRate > 0.08) likeRate = 0.08; // Cap at 8%
    video.likes = static_cast<int>(video.views * likeRate);
    
    // Comments calculation (typically 0.5-2% of views)
    double commentRate = 0.005 + (engagementScore / 20000.0); // 0.5% base + bonus
    if (commentRate > 0.02) commentRate = 0.02; // Cap at 2%
    video.comments = static_cast<int>(video.views * commentRate);
    
    // Add some randomness to make it more realistic
    video.views += (rand() % 200) - 100; // +/- 100 views
    video.likes += (rand() % 20) - 10;   // +/- 10 likes
    video.comments += (rand() % 10) - 5; // +/- 5 comments
    
    // Ensure minimum values
    if (video.views < 100) video.views = 100;
    if (video.likes < 5) video.likes = 5;
    if (video.comments < 1) video.comments = 1;
    
    cout << "\n" << string(40, '-') << "\n";
    cout << "ALGORITHM ANALYSIS RESULTS\n";
    cout << string(40, '-') << "\n";
    cout << "Engagement Score: " << engagementScore << "/300\n";
    cout << "Generated Views: " << video.views << "\n";
    cout << "Generated Likes: " << video.likes << "\n";
    cout << "Generated Comments: " << video.comments << "\n";
    cout << "Like Rate: " << fixed << setprecision(2) << (double(video.likes)/video.views)*100 << "%\n";
    cout << "Comment Rate: " << fixed << setprecision(2) << (double(video.comments)/video.views)*100 << "%\n";
    cout << string(40, '-') << "\n";
}

void flushInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void addChannel() {
    ChannelNode ch;
    ch.channelId = CHANNEL_ID++;
    
    cout << "Enter Channel Name: ";
    getline(cin, ch.channelName);
    cout << "Enter Owner Name: ";
    getline(cin, ch.ownerName);
    
    channels.push_back(ch);
    showSuccessMessage("Channel '" + ch.channelName + "' added successfully!");
    cout << "Channel ID: " << ch.channelId << "\n";
}

int findChannelIndexById(int id) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].channelId == id) return static_cast<int>(i);
    }
    return -1;
}

void addVideoToChannel() {
    if (channels.empty()) {
        showErrorMessage("No channels available! Please add a channel first.");
        return;
    }
    
    // Display available channels
    cout << "Available Channels:\n";
    cout << string(50, '-') << "\n";
    for (size_t i = 0; i < channels.size(); ++i) {
        cout << "ID: " << channels[i].channelId 
             << " | Name: " << channels[i].channelName 
             << " | Owner: " << channels[i].ownerName << "\n";
    }
    cout << string(50, '-') << "\n";
    
    int chId;
    cout << "Enter Channel ID to add video to: ";
    cin >> chId;
    flushInput();

    int index = findChannelIndexById(chId);
    if (index == -1) {
        showErrorMessage("Channel not found!");
        return;
    }

    VideoNode v;
    v.videoId = VIDEO_ID++;
    
    cout << "\nEnter video details:\n";
    cout << "Title: ";
    getline(cin, v.title);
    cout << "Description: ";
    getline(cin, v.description);
    cout << "Tags (comma-separated): ";
    getline(cin, v.tags);
    cout << "Upload Date: ";
    getline(cin, v.uploadDate);
    
    // Generate attributes using the algorithm
    cout << "\nAnalyzing content for target demographic (18-22 year old men)...\n";
    generateVideoAttributes(v);

    channels[index].videos.push_back(v);
    showSuccessMessage("Video '" + v.title + "' added to channel '" + channels[index].channelName + "'!");
}

void displayAll() {
    if (channels.empty()) {
        showErrorMessage("No channels available!");
        return;
    }
    
    for (size_t i = 0; i < channels.size(); ++i) {
        const ChannelNode &ch = channels[i];
        cout << "\n" << string(80, '=') << "\n";
        cout << "Channel ID   : " << ch.channelId << endl;
        cout << "Channel Name : \033[31m" << ch.channelName << "\033[0m" << endl;
        cout << "Owner Name   : " << ch.ownerName << endl;
        cout << "Total Videos : " << ch.videos.size() << endl;

        if (ch.videos.empty()) {
            cout << "\nNo videos uploaded yet.\n";
            continue;
        }

        cout << "\nVideos:\n";
        cout << string(80, '-') << "\n";
        cout << left
             << setw(2) << "#"
             << setw(8) << "ID"
             << setw(25) << "Title"
             << setw(15) << "Upload Date"
             << setw(8) << "Views"
             << setw(8) << "Likes"
             << setw(10) << "Comments" << endl;
        cout << string(80, '-') << "\n";

        for (size_t j = 0; j < ch.videos.size(); ++j) {
            const VideoNode &v = ch.videos[j];
            cout << left
                 << setw(2) << j+1
                 << setw(8) << v.videoId
                 << setw(25) << (v.title.length() > 24 ? v.title.substr(0, 21) + "..." : v.title)
                 << setw(15) << v.uploadDate
                 << setw(8) << v.views
                 << setw(8) << v.likes
                 << setw(10) << v.comments << endl;
            
            // Show description and tags if they exist
            if (!v.description.empty()) {
                cout << "    Desc: " << (v.description.length() > 60 ? v.description.substr(0, 57) + "..." : v.description) << endl;
            }
            if (!v.tags.empty()) {
                cout << "    Tags: " << v.tags << endl;
            }
            
            // Calculate and show engagement rates
            double likeRate = (double(v.likes) / v.views) * 100;
            double commentRate = (double(v.comments) / v.views) * 100;
            cout << "    Engagement: " << fixed << setprecision(1) 
                 << likeRate << "% likes, " << commentRate << "% comments\n";
            cout << endl;
        }
    }
}

void addPredefinedChannels() {
    // Channel 1 - Tech content (high appeal)
    ChannelNode ch1;
    ch1.channelId = 1;
    ch1.channelName = "Tech Talk";
    ch1.ownerName = "Alice";

    VideoNode v1a = {101, "Gaming PC Build Guide", "Complete tutorial for building the ultimate gaming PC for 2025", "gaming,tech,pc,build,tutorial", "2025-06-01", 0, 0, 0};
    VideoNode v1b = {102, "Best Programming Languages", "Top programming languages every developer should know", "programming,coding,tech,tutorial", "2025-06-03", 0, 0, 0};
    
    generateVideoAttributes(v1a);
    generateVideoAttributes(v1b);
    
    ch1.videos.push_back(v1a);
    ch1.videos.push_back(v1b);
    channels.push_back(ch1);

    // Channel 2 - Cooking content (lower appeal for target demographic)
    ChannelNode ch2;
    ch2.channelId = 2;
    ch2.channelName = "Cooking Mastery";
    ch2.ownerName = "Ben";

    VideoNode v2a = {201, "Perfect Pasta Recipe", "Learn how to make restaurant-quality pasta at home", "cooking,recipe,pasta,food", "2025-05-22", 0, 0, 0};
    VideoNode v2b = {202, "Knife Skills Tutorial", "Master basic knife techniques for cooking", "cooking,tutorial,knife,skills", "2025-06-10", 0, 0, 0};
    
    generateVideoAttributes(v2a);
    generateVideoAttributes(v2b);
    
    ch2.videos.push_back(v2a);
    ch2.videos.push_back(v2b);
    channels.push_back(ch2);

    // Channel 3 - Fitness content (high appeal)
    ChannelNode ch3;
    ch3.channelId = 3;
    ch3.channelName = "Fitness Focus";
    ch3.ownerName = "Cara";

    VideoNode v3a = {301, "Epic Morning Workout", "Insane 20-minute morning routine to get ripped fast", "fitness,workout,morning,epic,muscle", "2025-06-05", 0, 0, 0};
    VideoNode v3b = {302, "Home Gym Setup", "Build the perfect home gym on any budget", "fitness,gym,home,equipment,workout", "2025-06-09", 0, 0, 0};
    
    generateVideoAttributes(v3a);
    generateVideoAttributes(v3b);
    
    ch3.videos.push_back(v3a);
    ch3.videos.push_back(v3b);
    channels.push_back(ch3);
}
