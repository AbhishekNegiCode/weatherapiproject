#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>

// Callback function to handle curl response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to get weather data
void getWeatherData(const std::string& apiKey, const std::string& city) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Parse the JSON response
            Json::Value jsonData;
            Json::Reader jsonReader;
            if (jsonReader.parse(readBuffer, jsonData)) {
                std::cout << "Weather in " << city << ":\n";
                std::cout << "Temperature: " << jsonData["main"]["temp"].asFloat() - 273.15 << "°C\n";
                std::cout << "Humidity: " << jsonData["main"]["humidity"].asInt() << "%\n";
                std::cout << "Description: " << jsonData["weather"][0]["description"].asString() << "\n";
            } else {
                std::cerr << "Error parsing JSON\n";
            }
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    std::string apiKey = "e3049faa6f3dd478b41ada24b33b494d";  // Replace with your OpenWeatherMap API key
    std::string city;

    std::cout << "Enter city name: ";
    std::cin >> city;

    getWeatherData(apiKey, city);
    return 0;
}
