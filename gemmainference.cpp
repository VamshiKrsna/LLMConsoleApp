#include<stdlib.h>
#include<curl/curl.h>
#include<nlohmann/json.hpp>
#include<string.h>

using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string* userp){
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string send_prompt(CURL *curl, const std::string& base_url, const std::string& send_prompt){
    if (!curl) 
    return "Error : CURL Not initialized";

    std::string response_string;
    struct curl_slist* headers = NULL;

    json payload = {
        {"messages", {{
            {"role", "user"},
            {"content", prompt}
        }}},
        {"model", "gemma-2b"},  
        {"stream", false}
    };

    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, base_url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        return "Error: " + std::string(curl_easy_strerror(res));
    }

    try {
        json response_json = json::parse(response_string);
        return response_json["choices"][0]["message"]["content"];
    } catch (const std::exception& e) {
        return "Error parsing response: " + std::string(e.what());
    }
}

int main() {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
    
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return 1;
    }

    const std::string base_url = "http://127.0.0.1:1234";
    std::string input;
    
    std::cout << "Gemma 2 CLI (Type 'exit' to quit)\n";
    std::cout << "--------------------------------\n";

    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, input);

        if (input == "exit") break;
        
        std::cout << "\nGemma: ";
        std::string response = send_prompt(curl, base_url, input);
        std::cout << response << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}