#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <conio.h>
using namespace std;

struct VideoNode {
    int videoId;
    string title;
    string uploadDate;
    int views;
    int likes;
    int comments;
}; // Represents a video with its properties.

struct ChannelNode {
    int channelId;
    string channelName;
    string ownerName;
    vector<VideoNode> videos; // This is an array with VideoNode type.
}; // Represents a YouTube channel and its associated videos

vector<ChannelNode> channels; // Holds all channel entries in memory.
int CHANNEL_ID = 500; // starting value of the ids. I will increment them every channel or video created.
int VIDEO_ID = 500;

void flushInput(); // Clears input buffer

void addChannel(); 
void addVideoToChannel(); 
void addPredefinedChannels(); 
void displayAll(); 
int findChannelIndexById(int id); 

// ADD MORE CRUD OPERATIONS TOMORROW. GOODNIGHT.

int main() {
	addPredefinedChannels(); // Load initial channels and videos
	
    int choice;
    do {
    	system("cls");
        cout << "\n--- YouTube Channel Video Tracker ---\n";
        cout << "1. Add Channel\n";
        cout << "2. Add Video to Channel\n";
        cout << "3. Display All\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: addChannel(); break;
            case 2: addVideoToChannel(); break;
            case 3: displayAll(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
        
      
        cout << "--> Press any key to continue. ";
    	getch();
        
    } while (choice != 0);

    return 0;
}

                // FUNCTIONSSSSSS

void flushInput() {
    cin.clear(); // useful when cin fails ( when users enter letter when a number is expected).
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // This prevents leftover input such as newline. Basta just use this after using cin>>
}

void addChannel() {
	
    ChannelNode ch;
    
    ch.channelId = CHANNEL_ID++;
    
    cout << "Enter Channel Name: ";
    getline(cin, ch.channelName);
    cout << "Enter Owner Name: ";
    getline(cin, ch.ownerName);
    
    channels.push_back(ch);
    cout << "Channel added successfully!\n";
    
}

int findChannelIndexById(int id) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].channelId == id) return static_cast<int>(i); // convert i with a data type of size_t to int
    }
    return -1;
}

void addVideoToChannel() {
	
	if (channels.empty()) {
       cout<<"No channels available! Please add a channel first."<<endl;
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
        cout << "Channel not found!\n";
        return;
    }

    VideoNode v;
    
    v.videoId = VIDEO_ID++;
  
    cout << "Enter Title: ";
    getline(cin, v.title);
    cout << "Enter Upload Date 'yyyy-mm-dd': ";
    getline(cin, v.uploadDate);
    cout << "Enter Views: ";
    cin >> v.views;
    cout << "Enter Likes: ";
    cin >> v.likes;
    cout << "Enter Number of Comments: ";
    cin >> v.comments;
    flushInput();

    channels[index].videos.push_back(v);
    cout << "Video added to channel successfully!\n";
 
}

void displayAll() {
    for (size_t i = 0; i < channels.size(); ++i) {
        const ChannelNode &ch = channels[i];  
        cout <<"\n" <<string(71, '=') << "\n";
        cout << "Channel ID   : " << ch.channelId << endl;
        cout << "Channel Name : \033[31m" << ch.channelName <<"\033[0m"<< endl;  // "\033[31m" color text to red. "\033[0m" reset the color
        cout << "Owner Name   : " << ch.ownerName << endl;

        if (ch.videos.empty()) {
            cout << "No videos uploaded.\n";
            continue;
        }

		 // Display videos
		 // row header
        cout << "\nVideos:\n";
        cout << left       // align the string to the left
        	 << setw(2) << "-"     // setw(num characters wide column) 
             << setw(8)  << "ID"
             << setw(25) << "Title"
             << setw(12) << "Upload Date"
             << setw(8)  << "Views"
             << setw(8)  << "Likes"
             << setw(10) << "Comments" << endl;

        cout << string(71, '-') << endl;
        
		// row values
        for (size_t j = 0; j < ch.videos.size(); ++j) {
            const VideoNode &v = ch.videos[j];
            cout << left
            	 << setw(2) << "-"
                 << setw(8)  << v.videoId
                 << setw(25) << v.title
                 << setw(12) << v.uploadDate
                 << setw(8)  << v.views
                 << setw(8)  << v.likes
                 << setw(10) << v.comments << endl;
        }
    }
    cout <<"\n" <<string(71, '=') << "\n";
}

void addPredefinedChannels() {
    // Channel 1
    ChannelNode ch1;
    ch1.channelId = 1;
    ch1.channelName = "Tech Talk";
    ch1.ownerName = "Alice";

    VideoNode v1a = {101, "Intro to C++", "2025-06-01", 1000, 250, 40};
    VideoNode v1b = {102, "OOP Concepts", "2025-06-03", 800, 210, 30};
    ch1.videos.push_back(v1a);
    ch1.videos.push_back(v1b);
    channels.push_back(ch1);

    // Channel 2
    ChannelNode ch2;
    ch2.channelId = 2;
    ch2.channelName = "Cooking Mastery";
    ch2.ownerName = "Ben";

    VideoNode v2a = {201, "Perfect Pasta", "2025-05-22", 2300, 560, 90};
    VideoNode v2b = {202, "Knife Skills", "2025-06-10", 1900, 470, 75};
    ch2.videos.push_back(v2a);
    ch2.videos.push_back(v2b);
    channels.push_back(ch2);

    // Channel 3
    ChannelNode ch3;
    ch3.channelId = 3;
    ch3.channelName = "Fitness Focus";
    ch3.ownerName = "Cara";

    VideoNode v3a = {301, "Morning Routine", "2025-06-05", 1500, 390, 55};
    VideoNode v3b = {302, "10-Min Home Workout", "2025-06-09", 1800, 500, 60};
    ch3.videos.push_back(v3a);
    ch3.videos.push_back(v3b);
    channels.push_back(ch3);
}


