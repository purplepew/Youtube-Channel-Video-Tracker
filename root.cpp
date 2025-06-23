#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <conio.h>
#include <algorithm>
using namespace std;

// Represents a video with its properties.
struct VideoNode {
    int videoId;
    string title;
    string uploadDate;
    int views;
    int likes;
    int comments;
	string channelName;
}; 

// Represents a YouTube channel and its associated videos
struct ChannelNode {
    int channelId;
    string channelName;
    string ownerName;
    vector<VideoNode> videos; // An array of VideoNode
}; 

vector<ChannelNode> channels; // Holds all channel entries in memory.
vector<VideoNode> allVideos; // Holds all the videos in every channel.
int CHANNEL_ID = 500; // starting value of the ids. I will increment them every channel or video created.
int VIDEO_ID = 500;
bool ascending = true; //global flag. Will be used when user is prompted Descending or Ascending in Menus. and we will base the sorting according to this value.

void flushInput(); // Clears input buffer

void addChannel(); 
void addVideoToChannel(); 
void addVideosHelper(ChannelNode& c, VideoNode v);
void addPredefinedChannels();
void menuDisplay();
void menuDisplaySort();
void menuDisplaySortAscending();
void displaySortUploadDate();
void displayAll(); 
int findChannelIndexById(int id); 
void displayRowHeader();
void displayRowValues(const VideoNode &v);
bool sortByUploadDate(const VideoNode &a, const VideoNode &b);

                     // MAIN
int main() {
	addPredefinedChannels(); // Load initial channels and videos
	
    int choice;
    do {
    	system("cls");
        cout << "\n--- YouTube Channel Video Tracker ---\n";
        cout << "1. Add Channel\n";
        cout << "2. Add Video to Channel\n";
        cout << "3. Display\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: addChannel(); break;
            case 2: addVideoToChannel(); break;
            case 3: menuDisplay(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
        
      
        cout << "--> OK. Press any key to continue. ";
    	getch();
        
    } while (choice != 0);

    return 0;
}

                  // FUNCTIONSSSSSS

bool sortByUploadDate(const VideoNode &a, const VideoNode &b) {
	   return ascending ? (a.uploadDate < b.uploadDate) : (a.uploadDate > b.uploadDate);
} 

void flushInput() {
    cin.clear(); // useful when cin fails (when users enter letter when a number is expected).
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
    allVideos.push_back(v);
    cout << "Video added to channel successfully!\n";
 
}

void menuDisplay() {
	int choice;
    do {
        cout << "\tDisplay: \n";
        cout << "\t1. Display All\n";
        cout << "\t2. Sorted\n";
        cout << "\t0. Exit\n";
        cout << "\tEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: displayAll(); return;
            case 2: menuDisplaySort(); return;
            case 0: return;
            default: cout << "\tInvalid choice.\n";
        }
        
    } while (choice != 0);
}
void menuDisplaySort(){ // I QUIT HERE. DO THE SORTED DESCENDING LATER.
	int choice;
    do {

        cout << "\t\t1.Sorted Ascending\n";
        cout << "\t\t0. Exit\n";
        cout << "\t\tEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: menuDisplaySortAscending(); return;
            case 0: return;
            default: cout << "\t\tInvalid choice.\n";
        }
        
    } while (choice != 0);
}
void menuDisplaySortAscending(){
int choice;
    do {

        cout << "\t\t\t1.By Upload date\n";
        cout << "\t\t\t0. Exit\n";
        cout << "\t\t\tEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: displaySortUploadDate(); return;
            case 0: return;
            default: cout << "\t\t\tInvalid choice.\n";
        }
        
    } while (choice != 0);	
}
void displaySortUploadDate(){
	
	if (allVideos.empty()) {
            cout << "Videos empty.\n";
         return;
    }
    
	sort(allVideos.begin(), allVideos.end(), sortByUploadDate);
	
	displayRowHeader();
	
	for (size_t i = 0; i < allVideos.size(); ++i) {
		const VideoNode &v = allVideos[i];
		
		displayRowValues(v);
	}
	
}

void displayRowHeader(){
		 // row header
        cout << "\nVideos Sorted Ascending Order By Upload Date:\n";
        cout << left       // align the strings to the left
        	 << setw(2) << "-"     // setw(num characters wide column) 
             << setw(8)  << "ID"
             << setw(40) << "Title"
             << setw(12) << "Upload Date"
             << setw(8)  << "Views"
             << setw(8)  << "Likes"
             << setw(10) << "Comments" 
			 << setw(10) << "Channel Name" << endl;

        cout << string(88, '-') << endl;
}

void displayRowValues(const VideoNode &v){
	cout << left
            	 << setw(2) << "-"
                 << setw(8)  << v.videoId
                 << setw(40) << (v.title.length() > 35 ? v.title.substr(0, 35) + "..." : v.title) // Title with more than 35 will be sliced
                 << setw(12) << v.uploadDate
                 << setw(8)  << v.views
                 << setw(8)  << v.likes
                 << setw(10) << v.comments
                 << setw(10) << v.channelName  << endl;
}

void displayAll() {
	
    for (size_t i = 0; i < channels.size(); ++i) {
        const ChannelNode &ch = channels[i];  
        cout <<"\n" <<string(88, '=') << "\n";
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
        
        displayRowHeader();
        
		// row values
        for (size_t j = 0; j < ch.videos.size(); ++j) {
            const VideoNode &v = ch.videos[j];
            
            displayRowValues(v);
        }
    }
    cout <<"\n" <<string(88, '=') << "\n";
}

void addVideosHelper(ChannelNode& c, VideoNode v){ // "&" means pass by reference -- does not re-copy the entire array. 
    c.videos.push_back(v);
    allVideos.push_back(v);
}

void addPredefinedChannels() {
    // Channel 1
    ChannelNode ch1;
    ch1.channelId = 1;
    ch1.channelName = "Tech Talk";
    ch1.ownerName = "Alice";

    addVideosHelper(ch1, {101, "Intro to C++", "2025-06-01", 1000, 250, 40, ch1.channelName}); // Argument 1 is the ChannelNode
    addVideosHelper(ch1, {102, "OOP Concepts Explained", "2025-06-03", 800, 210, 30, ch1.channelName}); // Argument 2 is the VideoNode and also the cause of the extended initializer warning.
    addVideosHelper(ch1, {103, "Data Structures Fundamentals", "2025-06-05", 1200, 300, 45, ch1.channelName}); // still uses it because it is cleaner
    addVideosHelper(ch1, {104, "Algorithms for Beginners", "2025-06-07", 950, 230, 35, ch1.channelName});
    addVideosHelper(ch1, {105, "Web Dev Basics with HTML/CSS", "2025-06-09", 1100, 280, 42, ch1.channelName});
    addVideosHelper(ch1, {106, "Introduction to Python", "2025-06-11", 1300, 320, 50, ch1.channelName});
    addVideosHelper(ch1, {107, "Understanding Cloud Computing", "2025-06-13", 1400, 350, 55, ch1.channelName});
    addVideosHelper(ch1, {108, "Machine Learning 101", "2025-06-15", 1600, 400, 60, ch1.channelName});
    addVideosHelper(ch1, {109, "Cybersecurity Essentials", "2025-06-17", 900, 200, 25, ch1.channelName});
    addVideosHelper(ch1, {110, "DevOps Principles", "2025-06-19", 1050, 260, 38, ch1.channelName});
    channels.push_back(ch1);

    // Channel 2
    ChannelNode ch2;
    ch2.channelId = 2;
    ch2.channelName = "Cooking Mastery";
    ch2.ownerName = "Ben";

    addVideosHelper(ch2, {201, "Perfect Pasta from Scratch", "2025-05-22", 2300, 560, 90, ch2.channelName});
    addVideosHelper(ch2, {202, "Essential Knife Skills", "2025-06-10", 1900, 470, 75, ch2.channelName});
    addVideosHelper(ch2, {203, "Baking Artisan Bread at Home", "2025-06-12", 2500, 600, 100, ch2.channelName});
    addVideosHelper(ch2, {204, "Quick & Easy Weeknight Stir-Fry", "2025-06-14", 1800, 450, 70, ch2.channelName});
    addVideosHelper(ch2, {205, "Mastering Classic French Sauces", "2025-06-16", 2100, 520, 85, ch2.channelName});
    addVideosHelper(ch2, {206, "Delicious Vegetarian Dishes", "2025-06-18", 2000, 490, 80, ch2.channelName});
    addVideosHelper(ch2, {207, "Simple Desserts for Beginners", "2025-06-20", 2200, 540, 95, ch2.channelName});
    addVideosHelper(ch2, {208, "Grilling Techniques for Summer", "2025-06-22", 1700, 430, 68, ch2.channelName});
    addVideosHelper(ch2, {209, "Healthy Smoothies & Juices", "2025-06-24", 1500, 380, 60, ch2.channelName});
    addVideosHelper(ch2, {210, "Homemade Pizza Perfection", "2025-06-26", 2400, 580, 92, ch2.channelName});
    channels.push_back(ch2);

    // Channel 3
    ChannelNode ch3;
    ch3.channelId = 3;
    ch3.channelName = "Fitness Focus";
    ch3.ownerName = "Cara";

    addVideosHelper(ch3, {301, "Energizing Morning Routine", "2025-06-05", 1500, 390, 55, ch3.channelName});
    addVideosHelper(ch3, {302, "10-Min Full Body Home  Workout", "2025-06-09", 1800, 500, 60, ch3.channelName});
    addVideosHelper(ch3, {303, "Gentle Yoga for Beginners", "2025-06-11", 1600, 420, 58, ch3.channelName});
    addVideosHelper(ch3, {304, "Strength Training Essentials (No Gym)", "2025-06-13", 1700, 450, 62, ch3.channelName});
    addVideosHelper(ch3, {305, "High-Intensity Cardio Blaster", "2025-06-15", 1900, 480, 65, ch3.channelName});
    addVideosHelper(ch3, {306, "Nutrition & Healthy Eating Tips", "2025-06-17", 1400, 350, 50, ch3.channelName});
    addVideosHelper(ch3, {307, "Mindfulness Meditation for Relaxation", "2025-06-19", 1300, 320, 48, ch3.channelName});
    addVideosHelper(ch3, {308, "Running Form & Injury Prevention", "2025-06-21", 1200, 300, 40, ch3.channelName});
    addVideosHelper(ch3, {309, "Flexibility & Stretching Guide", "2025-06-23", 1100, 280, 35, ch3.channelName});
    addVideosHelper(ch3, {310, "Bodyweight Workout Challenge", "2025-06-25", 2000, 510, 70, ch3.channelName});
    channels.push_back(ch3);
}




