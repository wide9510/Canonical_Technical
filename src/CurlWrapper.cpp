#include "CurlWrapper.hpp"
#include <curl/curl.h>
#include <stdexcept>

CurlWrapper::CurlWrapper() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

CurlWrapper::~CurlWrapper() {
    curl_global_cleanup();
}

size_t CurlWrapper::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string CurlWrapper::fetch(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL error: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    return response;
}

