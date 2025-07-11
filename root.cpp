#include <iostream>
#include <vector>
#include <string>
#include <limits> // Required for std::numeric_limits in flushing inputs
#include <iomanip> // needed for setw() only
#include <conio.h> // needed for getch() only
#include <algorithm> // needed sort() only
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
vector<VideoNode> allVideos; // Holds all the videos from every channel. Every time I added a video in a channel, I append the video here too. Important for sorting.
int CHANNEL_ID = 500; // starting value of the ids. I will increment them every channel or video created.
int VIDEO_ID = 500;
bool ascending = true; //global flag. Will be used when user is prompted Descending or Ascending in Menus. and we will base the sorting according to this value.

void flushInput(); // Clears input buffer

// create
void addChannel(); 
void addVideoToChannel(); 
void addPredefinedChannels();

// search helper
int findChannelIndexById(int id); // maps every element in 'channels'
int findVideoIndexById(int id); // used binary search. find video's index in allVideos vector.
int findChannelIndexByName(const string& name);    // Finds a channel's index in 'channels' vector by its name, or returns -1 if not found.
int findVideoIndexInChannelById(const ChannelNode& channel, int videoId); // Finds a video's index within the given channel's video list by videoId, or returns -1.
void searchVideosByTitle();

// update
void updateVideoTitle();
void updateChannelName();

//delete
void deleteVideo();
void deleteChannel();
 
// display datas in table format
void displayAll(); 
void displayChannelAndVideos();
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
void displayUpdateMenu();
void displayDeleteMenu();

// helper functions
void displayRowHeader();
void displayRowValues(const VideoNode &v);
void displayAvailableChannels();
void addVideosHelper(ChannelNode& c, VideoNode v);

// sort logic for std::sort by algorithm library
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
        cout << "1. Register New Channel\n";
        cout << "2. Add Video to Channel\n";
        cout << "3. View/Manage Data\n";
        cout << "0. Exit Application\n";
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
    return str; // returns all lowercased letter of a word (str)
} 

// menus
void displayMenu() {
	int choice;
    do {
    	cout<<"\t --- Content Management ---\n";
        cout << "\t Display: \n";
        cout << "\t 1. Show All Content\n";
        cout << "\t 2. Display Channel Videos\n";
        cout << "\t 3. Sort Content\n";
        cout << "\t 4. Find Content\n";
        cout << "\t 5. Update Content\n";
        cout << "\t 6. Delete Content\n";
        cout << "\t 0. Go Back\n";
        cout << "\t\nEnter choice: ";
        cin >> choice;
        flushInput();
		
        switch (choice) {
            case 1: displayAll(); return;
            case 2: displayChannelAndVideos(); return;
            case 3: displaySortOptionMenu(); return;
            case 4: displayFindMenu(); return;
            case 5: displayUpdateMenu(); return;
            case 6: displayDeleteMenu(); return;
            case 0: return;
            default: cout << "\tInvalid choice.\n";
        }
        
    } while (choice != 0);
}

void displaySortOptionMenu(){ // when 'sort content' option is selected.
	int choice;
    do {
		cout<<"\t\t --- Select Sort Direction ---\n";	
        cout << "\t\t1. Ascending\n";
        cout << "\t\t2. Descending\n";
        cout << "\t\t0. Go Back\n";
        cout << "\t\t\nEnter choice: ";
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

void displaySortMenu(){ // when either 'ascending' or 'descending' is selected.
int choice;
    do {
		cout<<"\t\t\t--- Select Sort Criterion ---\n";
        cout << "\t\t\t  1. Upload date\n";
        cout << "\t\t\t  2. Views\n";
        cout << "\t\t\t  3. Likes\n";
        cout << "\t\t\t  4. Comments\n";
        cout << "\t\t\t  5. Id\n";
        cout << "\t\t\t  0. Go Back\n";
        cout << "\t\t\t\nEnter choice: ";
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

void displayFindMenu(){ // when "find content" is selected in the menu
	int choice;
    do {
    	cout<<"\t\t--- Search By Options ---\n";
        cout << "\t\t1. Search By Title\n";
        cout << "\t\t2. Search By Video ID\n";
        cout << "\t\t0. Go Back\n";
        cout << "\t\t\nEnter choice: ";
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

void displayUpdateMenu() { // when 'update content' is selected in the menu
    int choice;
    do {
        cout << "\t --- Update Video Attributes ---\n";
        cout << "\t 1. Update Title\n";
        cout << "\t 2. Update Channel Name\n"; 
        cout << "\t 0. Go Back\n";
        cout << "\t\nEnter choice: ";
        cin >> choice;
        flushInput();

        switch (choice) {
            case 1: updateVideoTitle(); return;
            case 2: updateChannelName(); return; 
            case 0: return;
            default: cout << "\tInvalid choice.\n";
        }
    } while (choice != 0);
}

void displayDeleteMenu() { // when 'delete content' is selected in the menu
	int choice;
    do {
        cout << "\t --- Update Video Attributes ---\n";
        cout << "\t 1. Delete Video\n";
        cout << "\t 2. Delete Channel\n"; 
        cout << "\t 0. Go Back\n";
        cout << "\t\nEnter choice: ";
        cin >> choice;
        flushInput();

        switch (choice) {
            case 1: deleteVideo(); return;
            case 2: deleteChannel(); return; 
            case 0: return;
            default: cout << "\tInvalid choice.\n";
        }
    } while (choice != 0);
}

// sort logic for std::sort by algorithm library
bool sortByUploadDate(const VideoNode &a, const VideoNode &b) {
	return ascending ? (a.uploadDate < b.uploadDate) : (a.uploadDate > b.uploadDate); // When this function returns true, "a" comes before "b" in the sorted list
} //

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

// search helper
int findChannelIndexById(int id) { // find channel's index from 'channels' vector
    for (size_t i = 0; i < channels.size(); ++i) {
        if (channels[i].channelId == id) return static_cast<int>(i); // convert i with a data type of size_t to int
    }
    return -1; // id not found
}

int findVideoIndexById(int id) { // find video's index from allVideos vector
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

int findChannelIndexByName(const string& name) {
    
    for (size_t i = 0; i < channels.size(); ++i) { // Iterate through the 'channels' vector
       
        if (channels[i].channelName == name) {
            return static_cast<int>(i); // Found, return the index
        }
    }
    return -1; // Channel not found
}

int findVideoIndexInChannelById(const ChannelNode& channel, int videoId) { // find video's index from channel's video array
    for (size_t i = 0; i < channel.videos.size(); ++i) {
        if (channel.videos[i].videoId == videoId) {
            return static_cast<int>(i);
        }
    }
    return -1; // Video not found in this channel
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

// display datas in table format
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

void displayAll() {
	
	if (channels.empty()){
		cout<<"There are no channels."<<endl;
		return;
	}
	
    for (size_t i = 0; i < channels.size(); ++i) { // Loops every channel
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
        
		
        for (size_t j = 0; j < ch.videos.size(); ++j) { // Loops every channel
            const VideoNode &v = ch.videos[j];
            
            displayRowValues(v); // row values
        }
    }
    cout <<"\n" <<string(100, '=') << "\n";
}

void displayChannelAndVideos() {
	
	displayAvailableChannels();
	
    cout << "\n--- Display Channel and Videos ---\n";
    int channelId;
    cout << "Enter Channel ID to display: ";
    cin >> channelId;
    flushInput();

    int channelIndex = findChannelIndexById(channelId);

    if (channelIndex == -1) {
        cout << "Channel not found!\n";
        return;
    }

    const ChannelNode& ch = channels[channelIndex]; // reference the found channel

    // Display Channel Details
    cout << "\n" << string(100, '=') << "\n";
    cout << "Channel ID     : " << ch.channelId << endl;
    cout << "Channel Name   : \033[31m" << ch.channelName << "\033[0m" << endl;
    cout << "Owner Name     : " << ch.ownerName << endl;

    // Display Videos for the Channel
    if (ch.videos.empty()) {
        cout << "No videos uploaded to this channel.\n";
    } else {
        cout << "\nVideos:\n";
        displayRowHeader(); // Display table header
        for (size_t j = 0; j < ch.videos.size(); ++j) {
            const VideoNode& v = ch.videos[j];
            displayRowValues(v); // Display each video's row values
        }
    }
    cout << "\n" << string(100, '=') << "\n";
}

// update content
void updateVideoTitle() {
    cout << "\n--- Update Video Title ---\n";
    int id;
    cout << "Enter Video ID to update title: ";
    cin >> id;
    flushInput();

    int allVideosIndex = findVideoIndexById(id); // get the videos' index from global allVideos vector.

    if (allVideosIndex == -1) {
        cout << "Video not found!\n";
        return;
    }

    // Get a reference to the video in the global allVideos vector
    VideoNode& videoToUpdateGlobal = allVideos[allVideosIndex];

    // Find the corresponding video in its channel's video list
    int channelIdx = findChannelIndexByName(videoToUpdateGlobal.channelName);
    int videoInChannelIdx = -1;
    if (channelIdx != -1) {
        videoInChannelIdx = findVideoIndexInChannelById(channels[channelIdx], id);
    }

    if (channelIdx == -1 || videoInChannelIdx == -1) {
        cout << "Error: Video found in global list but not in its channel. Data inconsistency.\n";
        return; // Should ideally not happen with consistent data :)
    }
    
    // Get a reference to the video in its channel's video vector
    VideoNode& videoToUpdateInChannel = channels[channelIdx].videos[videoInChannelIdx];

    cout << "Current Title: " << videoToUpdateGlobal.title << endl;
    cout << "Enter New Title: "; // No option to leave blank if you want a change
    string newTitle;
    getline(cin, newTitle);

    if (newTitle.empty()) {
        cout << "Title cannot be empty. No update performed.\n";
        return;
    }

    videoToUpdateGlobal.title = newTitle; // updates the video from allVideos
    videoToUpdateInChannel.title = newTitle; // updates the video from the channel

    cout << "Video title updated successfully!\n";
}

void updateChannelName() {
	
	if (channels.empty()){
		cout<<"There are no channels."<<endl;
		return;
	}
	
	displayAvailableChannels();
    
    cout << "\n--- Update Channel Name ---\n";
    int channelIdToUpdate;
    cout << "Enter Channel ID to update: ";
    cin >> channelIdToUpdate;
    flushInput();

    int channelIndex = findChannelIndexById(channelIdToUpdate);

    if (channelIndex == -1) {
        cout << "Channel not found!\n";
        return;
    }

    // Get a reference to the channel being updated
    ChannelNode& channelToUpdate = channels[channelIndex];
    string oldChannelName = channelToUpdate.channelName;

    cout << "Current Channel Name: " << oldChannelName << endl;
    cout << "Enter New Channel Name: ";
    string newChannelName;
    getline(cin, newChannelName);

    if (newChannelName.empty()) {
        cout << "New channel name cannot be empty. No update performed.\n";
        return;
    }

    // Check if the new name already exists for another channel
    for (size_t i = 0; i < channels.size(); ++i) {
        if (i != channelIndex && channels[i].channelName == newChannelName) {
            cout << "Error: Channel name '" << newChannelName << "' already exists for another channel (ID: " << channels[i].channelId << ").\n";
            return;
        }
    }

    // Update the channel name in the ChannelNode itself
    channelToUpdate.channelName = newChannelName;

    // Update the channel name for all videos associated with this channel in the 'allVideos' list
    for (size_t i = 0; i < allVideos.size(); ++i) {
        if (allVideos[i].channelName == oldChannelName) {
            allVideos[i].channelName = newChannelName;
        }
    }

    // Update the channel name for all videos directly stored within this channel's 'videos' vector
    for (size_t i = 0; i < channelToUpdate.videos.size(); ++i) {
        channelToUpdate.videos[i].channelName = newChannelName;
    }

    cout << "Channel name updated successfully from '" << oldChannelName << "' to '" << newChannelName << "'!\n";
}

// delete content
void deleteVideo() {
    cout << "\n--- Delete Video ---\n";
    int idToDelete;
    cout << "Enter Video ID to delete: ";
    cin >> idToDelete;
    flushInput();

    // Find the video in the global 'allVideos' list
    int allVideosIndex = findVideoIndexById(idToDelete);

    if (allVideosIndex == -1) {
        cout << "Video not found!\n";
        return;
    }

    // Get the channel name before removing from allVideos
    string channelNameOfVideo = allVideos[allVideosIndex].channelName;

    // Remove the video from the 'allVideos' global list
    allVideos.erase(allVideos.begin() + allVideosIndex);

    // Find the parent channel
    int channelIndex = findChannelIndexByName(channelNameOfVideo);

    // Find the video within its parent channel's video list
    ChannelNode& parentChannel = channels[channelIndex]; // Get reference to modify
    int videoInChannelIndex = findVideoIndexInChannelById(parentChannel, idToDelete);

    // Remove the video from the channel's video list
    parentChannel.videos.erase(parentChannel.videos.begin() + videoInChannelIndex);

    cout << "Video with ID " << idToDelete << " deleted successfully." <<endl;
}

void deleteChannel() {
	
	if (channels.empty()){
		cout<<"There are no channels."<<endl;
		return;
	}
	
	displayAvailableChannels();
	
    cout << "\n--- Delete Channel ---\n";
    int idToDelete;
    cout << "Enter Channel ID to delete: ";
    cin >> idToDelete;
    flushInput();

    int channelIndex = findChannelIndexById(idToDelete);

    if (channelIndex == -1) {
        cout << "Channel not found!\n";
        return;
    }

    // Get the name of the channel being deleted (needed for video cleanup)
    string channelNameOfDeletedChannel = channels[channelIndex].channelName;

    // Remove the channel from the 'channels' global list
    channels.erase(channels.begin() + channelIndex);

    // 2. Remove all videos associated with this channel from the 'allVideos' global list
    for (int i = static_cast<int>(allVideos.size()) - 1; i >= 0; --i) {
        if (allVideos[i].channelName == channelNameOfDeletedChannel) {
            allVideos.erase(allVideos.begin() + i);
        }
    }

    cout << "Channel ID " << idToDelete << " (named '" << channelNameOfDeletedChannel << "') and all its videos deleted successfully!\n";
}

// helper function
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

void displayAvailableChannels(){
	
	if (channels.empty()){
		cout<<"There are no channels."<<endl;
		return;
	}
	
	// Display available channels
    cout << "\nAvailable Channels:\n";
    cout << string(50, '-') << "\n";
    for (size_t i = 0; i < channels.size(); ++i) {
        cout << "ID: " << channels[i].channelId 
             << " | Name: " << channels[i].channelName 
             << " | Owner: " << channels[i].ownerName << "\n";
    }
    cout << string(50, '-') << "\n";
}

// add content
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

// add predefined channels
void addPredefinedChannels() {
    // Channel 1
    ChannelNode ch1;
    ch1.channelId = 1;
    ch1.channelName = "Tech Talk";
    ch1.ownerName = "Alice";

    addVideosHelper(ch1, {101, "Intro to C++", "2025-06-01", 1000, 250, 40, ch1.channelName}); // Argument 1 is the ChannelNode
    addVideosHelper(ch1, {102, "OOP Concepts Explained", "2025-06-03", 800, 210, 30, ch1.channelName}); // Argument 2 is the VideoNode and also the cause of the extended initializer warning.
    addVideosHelper(ch1, {103, "Data Structures Fundamentals", "2025-06-05", 1200, 300, 45, ch1.channelName}); 
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

