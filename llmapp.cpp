#include<iostream>
#include<fstream> //Filestreams
#include<string> 
#include<cstdlib> //ENV handling
#include<cpr/cpr.h>
#include<nlohmann/json.hpp>
// #include <vcpkg/packages/cpr_x64-windows/include/cpr/cpr.h> //HTTP Client
// #include<vcpkg/packages/nlohmann-json_x64-windows/include/nlohmann/json.hpp> //JSON Parsing
// #include<curl/curl.h>

using json = nlohmann::json; // namespace for json

// loading api key : 
std::string get_api_key(){
    const char* api_key = std::getenv("GEMINI_API_KEY");
    if (!api_key){
        throw std::runtime_error("API Key not found, please set GEMINI_API_KEY in a dotenv file.");
    }
    return std::string(api_key);
}

// method for interaction with gemini pro : 
std::string interact_gemini(const std::string& prompt){
    std::string api_key = get_api_key();
    std::string api_url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key="+api_key;
    return api_url;

}

int main(){
    std::cout << "Hello" << std::endl;
    return 0;
    std::cout << get_api_key;
    std::cout << interact_gemini;
}