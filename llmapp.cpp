#include <iostream>
#include <string>
#include "nlohmann/json.hpp" // For json handling
#include "cpr/cpr.h"         // For HTTP requests

using json = nlohmann::json;

std::string get_api_key()
{
    // loading env file
    const char *api_key = std::getenv("GEMINI_API_KEY");
    if (api_key)
    {
        return std::string(api_key);
    }

    // If not in env, take user input :
    std::string key;
    std::cout << "Enter your Gemini API Key: ";
    std::getline(std::cin, key);
    return key;
}

std::string generate_content(const std::string &api_key, const std::string &prompt)
{
    // request URL
    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + api_key;

    // JSON payload
    json payload = {
        {"contents", {{{"parts", {{"text", prompt}}}}}}};

    // POST :
    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{payload.dump()});

    // if success :
    if (r.status_code == 200)
    {
        // Parse the response
        json response = json::parse(r.text);

        // extract response :
        try
        {
            return response["candidates"][0]["content"]["parts"][0]["text"];
        }
        catch (...)
        {
            return "Error parsing response: " + r.text;
        }
    }
    else
    {
        return "HTTP Error " + std::to_string(r.status_code) + ": " + r.text;
    }
}

void start_chat(const std::string &api_key)
{
    std::string prompt;
    std::cout << "Welcome to Gemini Chat! (Type 'exit' to quit)\n";

    while (true)
    {
        std::cout << "\nYou: ";
        std::getline(std::cin, prompt);

        // exit when input == "exit"
        if (prompt == "exit")
        {
            break;
        }

        // generating response
        std::string response = generate_content(api_key, prompt);
        std::cout << "\nGemini: " << response << std::endl;
    }

    std::cout << "Thank You for using Gemini Chat !" << std::endl;
}

int main()
{
    try
    {
        std::string api_key = get_api_key();
        start_chat(api_key);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}