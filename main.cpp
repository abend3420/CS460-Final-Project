#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <algorithm>
#include <random>
#include <limits>

struct Advertisement { // advertisement structure for marketer-inputted advertisements
    std::string name;
    std::string category;
    std::string brand;
    double price;
    bool onSale;
    bool sponsored;
    int priority;
};
struct Item { // item structure for user-clicked advertisements
    std::string name;
    std::string category;
    std::string brand;
};

// method to calculate total scores for advertisements
int calculateRelevanceScore(const Advertisement& ad, const Item& lastClickedItem, bool hasClicked) {
    int score = ad.priority;
    if (!hasClicked) { // calculates default ranking of the advertisements before the user begins clicking
        if (ad.onSale) {
        score += 5;
        }
        if (ad.sponsored) {
            score += 10;
        }
        return score;
    }
    // calculates score of the advertisements based on the user's last click
    if (lastClickedItem.category == ad.category) { 
        score += 20;
    }
    if (lastClickedItem.brand == ad.brand) {
        score += 15;
    }
    // extra points unrelated to the user's click (external marketing factors)
    if (ad.onSale) {
        score += 5;
    }
    if (ad.sponsored) {
        score += 10;
    }
    return score;  
}
void rankAds(const std::vector<Advertisement>& ads, const Item& lastClickedItem, bool hasClicked) { // ranks and displays advertisements in order
    std::priority_queue<std::pair<int, int>> pq; // utilizes a priority queue
    for (int i = 0; i < ads.size(); i++) { // calculates the score for each advertisement and pushes them into the priority queue
        int score = calculateRelevanceScore(ads[i], lastClickedItem, hasClicked);
        pq.push({score, i});
    }
    std::cout << "\nRecommended Ads:\n";
    int adRank = 1;
    while (!pq.empty() && adRank <= 5) { // displays the advertisements in order
        auto [score, idx] = pq.top();
        pq.pop();
        
        std::cout << adRank << ". " << ads[idx].name << " (Score: " << score << ")\n";
        adRank++;
    }
}
Advertisement getTopAd(const std::vector<Advertisement>& ads, const Item& lastClickedItem, bool hasClicked) { // helper to return the top advertisement
    std::priority_queue<std::pair<int, int>> pq;
    for (int i = 0; i < ads.size(); i++) {
        int score = calculateRelevanceScore(ads[i], lastClickedItem, hasClicked);
        pq.push({score, i});
    }
    return ads[pq.top().second];
}
void displayItems(const std::vector<Item>& items) { // helper to display the list of items that can be clicked
    std::cout << "\n Clickable Items\n";
    for (const auto& item : items) {
        std::cout << "- " << item.name << "\n";
    }
}
bool processClick(const std::string& input, const std::vector<Item>& items, Item& lastClickedItem) { // checks if the user selected an item
    for (const auto& item : items) {
        if (item.name == input) {
            lastClickedItem = item;
            return true;
        }
    }
    return false;
}
bool processAdClick(const std::string& input, const std::vector<Advertisement>& ads, const Advertisement*& clickedAd) { // checks if the user selected the ad
    for (const auto& ad : ads) {
        if (ad.name == input) {
            clickedAd = &ad;
            return true;
        }
    }
    return false;
}

int main() {
    std::srand(std::time(nullptr));
    int numAds;
    
    // marketer simulation
    std::cout << "Marketer, enter the number of ads to generate: ";
    std::cin >> numAds; // marketer determines the amount of ads in advance
    std::vector<Advertisement> ads;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int i = 0; i < numAds; ++i) { // marketer inserts the features of every advertisement
        Advertisement ad;
        std::cout << "Enter ad name: ";
        std::getline(std::cin, ad.name);
        std::cout << "Enter ad category: ";
        std::getline(std::cin, ad.category);
        std::cout << "Enter ad brand: ";
        std::getline(std::cin, ad.brand);
        std::cout << "Enter ad price: ";
        std::cin >> ad.price;
        std::cout << "Is the ad on sale? (1 for yes, 0 for no): ";
        std::cin >> ad.onSale;
        std::cout << "Is the ad sponsored? (1 for yes, 0 for no): ";
        std::cin >> ad.sponsored;
        std::cout << "Enter ad priority (higher number means higher priority): ";
        std::cin >> ad.priority;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        ads.push_back(ad);
    }
    std::vector<Item> items = { // items available for purchase by the user
        {"Laptop", "Electronics", "Sony"},
        {"Running Shoes", "Footwear", "Nike"},
        {"Smartwatch", "Electronics", "Apple"},
        {"Headphones", "Electronics", "Bose"},
        {"Backpack", "Accessories", "North Face"},
        {"Sunglasses", "Accessories", "Ray-Ban"},
        {"Smartphone", "Electronics", "Samsung"},
        {"Fitness Tracker", "Electronics", "Fitbit"},
        {"Tablet", "Electronics", "Microsoft"},
        {"Camera", "Electronics", "Canon"}
    };

    Item lastClickedItem;
    bool hasClicked = false;
    Advertisement topAd = getTopAd(ads, lastClickedItem, hasClicked);
    displayItems(items); 
    rankAds(ads, lastClickedItem, hasClicked);
    // customer simulation
    std::cout << "\nUser's viewing simulation in real time':\n";
    while (true) { // user shopping session
        std::cout << "\nType your selection (type STOP to stop shopping):";
        std::string input; // user types the item they want to click on next
        std::getline(std::cin, input);
        const Advertisement* clickedAd = nullptr;
        bool clickedItem = processClick(input, items, lastClickedItem);
        bool clickedAdvertisement = processAdClick(input, ads, clickedAd);
        if (input == "STOP" || input == "stop") { // the user types "STOP" if they are done shopping
            std::cout << "Shopping session ended.\n";
            break;
        } 
        if (clickedAdvertisement && clickedAd->name == topAd.name) { // if the top ranked advertisement was clicked, a success message is outputted to indicate that the algorithm was successful
            std::cout << "SUCCESS! Top ranking advertisement was clicked!\n";
        }
        else if (clickedItem) { // otherwise, the ads are reranked based on the user's last click
            hasClicked = true;
            std::cout << "Clicked: " << lastClickedItem.name << "\n";
            rankAds(ads, lastClickedItem, hasClicked);
        }
        else { // edge case for an invalid item entered
            std::cout << "Invalid item.\n";
        }
    }
    return 0;
}