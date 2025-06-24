#include <iostream>
#include <vector>
#include <string>
#include <limits> // Required for std::numeric_limits in flushing inputs
#include <iomanip> // for setw() only
#include <conio.h> // for getch() only
#include <algorithm> // for sort() only
using namespace std;

// video properties
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
    vector<VideoNode> videos; // An array of videos with the property of VideoNode
}; 

vector<ChannelNode> channels; // Holds all channel entries in memory.
vector<VideoNode> allVideos; // Holds all the videos from every channel.
int CHANNEL_ID = 500; // starting value of the ids. I will increment them every channel or video created.
int VIDEO_ID = 500;
bool ascending = true; //global flag. Will be used when user is prompted Descending or Ascending in Menus. and we will base the sorting according to this value.

void flushInput(); // Clears input buffer

void addChannel(); 
void addVideoToChannel(); 
void addPredefinedChannels();
 
// display datas in table format
void displayAll(); 
void displaySortedByUploadDate();
void displaySortedByViews();
void displaySortedByLikes();
void displaySortedByComments();
void displaySortedById();
void displayVideo();

// menus
void displayMenu();
void displaySortOptionMenu();
void displaySortMenu();
void displayFindMenu();

// search
int findChannelIndexById(int id); // used mapping for every channel
int findVideoIndexById(int id); // used binary search

// helper functions
void displayRowHeader();
void displayRowValues(const VideoNode &v);
void addVideosHelper(ChannelNode& c, VideoNode v);

// sort logic
bool sortByUploadDate(const VideoNode &a, const VideoNode &b);
bool sortByViews(const VideoNode &a, const VideoNode &b);
bool sortByLikes(const VideoNode &a, const VideoNode &b);
bool sortByComments(const VideoNode &a, const VideoNode &b);
bool sortById(const VideoNode &a, const VideoNode &b);

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
            case 3: displayMenu(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
        
        cout << "\n\t > OK. Press any key to continue. ";
    	getch();
        
    } while (choice != 0);

    return 0;
}

// FUNCTIONSSSSSS
void flushInput() {
    cin.clear(); // useful when cin fails (when users enter letter when a number is expected).
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // This prevents leftover input such as newline. Basta just use this after using cin>>
}

string toLower(string str) {
    for (size_t i = 0; i < str.length(); ++i)
        str[i] = tolower(str[i]);
    return str; // returns all lowercased letter of a word
}

bool sortByUploadDate(const VideoNode &a, const VideoNode &b) {
	return ascending ? (a.uploadDate < b.uploadDate) : (a.uploadDate > b.uploadDate); // When this function returns true, "a" comes before "b" in the sorted list
} 

bool sortByViews(const VideoNode &a, const VideoNode &b) {
	return ascending ? (a.views < b.views) : (a.views > b.views);
} 

bool sortByLikes(const VideoNode &a, const VideoNode &b) {
	return ascending ? (a.likes < b.likes) : (a.likes > b.likes);
} 

bool sortByComments(const VideoNode &a, const VideoNode &b) {
	return ascending ? (a.comments < b.comments) : (a.comments > b.comments);
} 

bool sortById(const VideoNode &a, const VideoNode &b) {
	return ascending ? (a.videoId  < b.videoId) : (a.videoId > b.videoId);
} 

int findChannelIndexById(int id) {
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].channelId == id) return static_cast<int>(i); // convert i with a data type of size_t to int
    }
    return -1; // id not found
}

int findVideoIndexById(int id) {
	ascending = true;
	sort(allVideos.begin(), allVideos.end(), sortById); // Sorts allVideos using sortById to compare items.
	
	int left = 0;
	int right = allVideos.size() - 1;
	
	while(left <= right){
		int middle = (left+right) / 2;
		
		if(allVideos[middle].videoId == id) 
			return middle;
		else if(allVideos[middle].videoId > id) 
			right = middle - 1;
		else 
			left = middle + 1;
	}
	return -1; // id not found
}

void searchVideosByTitle() {
	
    string input;
    cout << "Enter title keyword to search: ";
    getline(cin, input);

    string keyword = toLower(input); // var keyword is lowercased input
    bool found = false;
    
	displayRowHeader();
	
    for (size_t i = 0; i < allVideos.size(); ++i) {
    
        string videoTitleLower = toLower(allVideos[i].title); // current video's video title is lowercased
        
        if (videoTitleLower.find(keyword) != string::npos) { // check if keyword exists in the video title
            found = true;
            displayRowValues(allVideos[i]); // will cout the video's properties using displayRowValues()
        }
    }

    if (!found) {
        cout << "No videos matched your search.\n";
    }
}

void displayVideo(){ // displays standalone video. needs an id
	
	int id;
	cout<<"Enter Video ID: ";
	cin>>id;
	flushInput();
	
	int index = findVideoIndexById(id);
	
	if (index == -1) {
        cout << "Video not found!\n";
        return;
    }
    
    VideoNode v = allVideos[index];
    displayRowHeader();
    displayRowValues(v);
}

void displayMenu() {
	int choice;
    do {
        cout << "\tDisplay: \n";
        cout << "\t1. Display All\n";
        cout << "\t2. Sort\n";
        cout << "\t3. Find\n";
        cout << "\t0. Exit\n";
        cout << "\tEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: displayAll(); return;
            case 2: displaySortOptionMenu(); return;
            case 3: displayFindMenu(); return;
            case 0: return;
            default: cout << "\tInvalid choice.\n";
        }
        
    } while (choice != 0);
}

void displaySortOptionMenu(){ // when sort option is selected.
	int choice;
    do {

        cout << "\t\t1. Ascending\n";
        cout << "\t\t2. Descending\n";
        cout << "\t\t0. Exit\n";
        cout << "\t\tEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: 
				ascending = true;
				displaySortMenu(); 
				return;
			case 2:
				ascending = false;
				displaySortMenu();
				return;
            case 0:
				 return;
            default: cout << "\t\tInvalid choice.\n";
        }
        
    } while (choice != 0);
}

void displaySortMenu(){ // when either ascending or descending is selected.
int choice;
    do {

        cout << "\t\t\t1.By Upload date\n";
        cout << "\t\t\t2.By Views\n";
        cout << "\t\t\t3.By Likes\n";
        cout << "\t\t\t4.By Comments\n";
        cout << "\t\t\t5.By Id\n";
        cout << "\t\t\t0. Exit\n";
        cout << "\t\t\tEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: displaySortedByUploadDate(); return;
            case 2: displaySortedByViews(); return;
            case 3: displaySortedByLikes(); return;
            case 4: displaySortedByComments(); return;
            case 5: displaySortedById(); return;
            case 0: return;
            default: cout << "\t\t\tInvalid choice.\n";
        }
        
    } while (choice != 0);	
}

void displayFindMenu(){ // when "find" is selected in the menu
	int choice;
    do {
        cout << "\t\t1. Find By Title\n";
        cout << "\t\t2. Find By ID\n";
        cout << "\t\t0. Exit\n";
        cout << "\t\tEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: searchVideosByTitle(); return;
            case 2: displayVideo(); return;
            case 0: return;
            default: cout << "\t\tInvalid choice.\n";
        }
        
    } while (choice != 0);
}

void displaySortedByUploadDate(){
	if (allVideos.empty()) {
            cout << "Videos empty.\n";
         return;
    } 
	sort(allVideos.begin(), allVideos.end(), sortByUploadDate); // will sort base on global variable "ascending"
	
	displayRowHeader();
	
	for (size_t i = 0; i < allVideos.size(); ++i) { //maps every videos
		const VideoNode &v = allVideos[i]; // stored inside a readonly variable named "v".
										// "&" directly points at VideoNode object located at index "i". so not creating a new object
		displayRowValues(v); // cout video properties in an awesome table row
	}	
}

void displaySortedByViews(){
	if (allVideos.empty()) {
            cout << "Videos empty.\n";
         return;
    }
	sort(allVideos.begin(), allVideos.end(), sortByViews);
	
	displayRowHeader();
	
	for (size_t i = 0; i < allVideos.size(); ++i) {
		const VideoNode &v = allVideos[i];
		
		displayRowValues(v);
	}	
}

void displaySortedByLikes(){
	if (allVideos.empty()) {
            cout << "Videos empty.\n";
         return;
    }
	sort(allVideos.begin(), allVideos.end(), sortByLikes);
	
	displayRowHeader();
	
	for (size_t i = 0; i < allVideos.size(); ++i) {
		const VideoNode &v = allVideos[i];
		
		displayRowValues(v);
	}	
}

void displaySortedByComments(){
	if (allVideos.empty()) {
            cout << "Videos empty.\n";
         return;
    }
	sort(allVideos.begin(), allVideos.end(), sortByComments);
	
	displayRowHeader();
	
	for (size_t i = 0; i < allVideos.size(); ++i) {
		const VideoNode &v = allVideos[i];
		
		displayRowValues(v);
	}	
}

void displaySortedById(){
	if (allVideos.empty()) {
            cout << "Videos empty.\n";
         return;
    }
	sort(allVideos.begin(), allVideos.end(), sortById);
	
	displayRowHeader();
	
	for (size_t i = 0; i < allVideos.size(); ++i) {
		const VideoNode &v = allVideos[i];
		
		displayRowValues(v);
	}	
}

void displayRowHeader(){
        cout << left       // align the strings to the left
        	 << setw(2) << "-"     // setw(num characters wide column) 
             << setw(8)  << "ID"
             << setw(40) << "Title"
             << setw(12) << "Upload Date"
             << setw(8)  << "Views"
             << setw(8)  << "Likes"
             << setw(10) << "Comments" 
			 << setw(10) << "Channel Name" << endl;
        cout << string(100, '-') << endl;
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
        cout <<"\n" <<string(100, '=') << "\n";
        cout << "Channel ID   : " << ch.channelId << endl;
        cout << "Channel Name : \033[31m" << ch.channelName <<"\033[0m"<< endl;  // "\033[31m" color text to red. "\033[0m" reset the color
        cout << "Owner Name   : " << ch.ownerName << endl;

        if (ch.videos.empty()) {
            cout << "No videos uploaded.\n";
            continue;
        }

	
        cout << "\nVideos:\n";
        
        displayRowHeader(); //row header
        
		
        for (size_t j = 0; j < ch.videos.size(); ++j) {
            const VideoNode &v = ch.videos[j];
            
            displayRowValues(v); // row values
        }
    }
    cout <<"\n" <<string(100, '=') << "\n";
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
    
    v.channelName = channels[index].channelName;

    channels[index].videos.push_back(v);
    allVideos.push_back(v);
    cout << "Video added to channel successfully!\n";
 
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




