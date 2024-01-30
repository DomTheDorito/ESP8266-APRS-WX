/*
Private Open Source License 1.0
Copyright 2024 Dominic Hord

https://github.com/DomTheDorito/Private-Open-Source-License

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the “Software”), to deal in the 
Software without limitation the rights to personally use, copy, modify, distribute,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

1. The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

2. The source code shall not be used for commercial purposes, including but not limited to sale of the Software, or use in products 
intended for sale, unless express writen permission is given by the source creator.

3. Attribution to source work shall be made plainly available in a reasonable manner.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

THIS LICENSE MAY BE UPDATED OR REVISED, WITH NOTICE ON THE POS LICENSE REPOSITORY.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>  // Include the <ctime> header for working with time
#include <curl/curl.h>
#include <json/json.h>

// Callback function to handle cURL response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    // Set the URL to fetch JSON data
    std::string url = "YOURESPIP";

    // Initialize cURL
    CURL* curl = curl_easy_init();

    if (curl) {
        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Store JSON data in a string
        std::string jsonData;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &jsonData);

        // Perform the cURL request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Parse JSON data using the JsonCpp library
            Json::Value root;
            Json::CharReaderBuilder reader;
            std::istringstream jsonStream(jsonData);
            Json::parseFromStream(reader, jsonStream, &root, nullptr);

            // Extract values from JSON
            double humidity = root["humidity"].asDouble();
            double temp = root["temp"].asDouble();
            double pressure = root["pressure"].asDouble();

            // Get current time
            std::time_t currentTime = std::time(nullptr);
            char timestamp[20];
            std::strftime(timestamp, sizeof(timestamp), "%b %d %Y %H:%M", std::localtime(&currentTime));



            // Convert temp to F
            int fTemp = ((temp * 1.8) + 32);
            // Convert KPA to aprs mbar
            double mbar = pressure * 100;
            // Convert humidty to int, and check for 100% case
            int iHum = humidity;
            std::string fHum;
            if (iHum == 100)
                fHum = "0" + std::to_string(0);
            else
                fHum = std::to_string(iHum);

            


            if (pressure != 0 && humidity != 0)
            {
            // Write values and timestamp to a text file
            std::ofstream outFile("wxnow.txt");
            outFile << timestamp << std::endl;

            if (fTemp >= 100)
            {
                outFile << "000/000g000t" << fTemp << "r000p000P000h" << fHum << "b" << std::setprecision(4) << mbar;
            }
            else if (fTemp >= 10 || fTemp <= 99)
            {
                outFile << "000/000g000t0" << fTemp << "r000p000P000h" << fHum << "b" << std::setprecision(4) << mbar;
            }
            else if (fTemp > 0 || fTemp <= 9)
            {
                outFile << "000/000g000t00" << fTemp << "r000p000P000h" << fHum << "b" << std::setprecision(4) << mbar;
            }
            else
            {
                outFile << "000/000g000t000r000p000P000h" << fHum << "b" << std::setprecision(4) << mbar;
            }


            //outFile << "Light: " << light << std::endl;
            //outFile << "Humidity: " << humidity << std::endl;
            //outFile << "Temperature: " << temp << std::endl;
            //outFile << "Pressure: " << pressure << std::endl;

            std::cout << "Sensor data written to wxnow.txt" << std::endl;
            }
            else
            std::cout << "Error parsing sensor data, maybe the values are zero?" << std::endl;
        }

        // Cleanup cURL
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize cURL." << std::endl;
    }

    return 0;
}
