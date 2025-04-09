#pragma once
#include <rapidjson/document.h>
#include <memory>
#include <vector>

class DataFetcher {
public:
  std::shared_ptr<std::vector<std::string>> getSupportedReleases();
  std::string getCurrentLTSVersion();
  std::string getChecksum(const std::string& imageName);
  void fetchData();

private:
  std::string _url = "https://cloud-images.ubuntu.com/releases/streams/v1/"
                     "com.ubuntu.cloud:released:download.json";
  rapidjson::Document _doc;
};
