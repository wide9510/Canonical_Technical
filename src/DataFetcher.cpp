#include "DataFetcher.hpp"
#include "CurlWrapper.hpp"
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <sstream>
#include <algorithm>

void DataFetcher::fetchData() {
  CurlWrapper curl;
  std::string response = curl.fetch(_url);

  _doc.Parse(response.c_str());
  if (_doc.HasParseError()) {
    throw std::runtime_error("Failed to parse JSON");
  }
}

std::shared_ptr<std::vector<std::string>> DataFetcher::getSupportedReleases() {
  auto releases = std::make_shared<std::vector<std::string>>();

  if (_doc.HasMember("products") && _doc["products"].IsObject()) {
    const auto &products = _doc["products"];

    for (auto &productIter : products.GetObject()) {
      std::string product_name = productIter.name.GetString();
      const auto &product_data = productIter.value;

      // Only return the amd64 versions as specified by the assignment
      if (product_name.find("amd64") != std::string::npos &&
          product_data.HasMember("version") &&
          product_data["version"].IsString()) {
        releases->push_back(product_data["version"].GetString());
      }
    }
  }
  // Return the list with the latest versions first
  std::reverse(releases->begin(), releases->end());
  return releases;
}

std::string DataFetcher::getCurrentLTSVersion() {
  if (!_doc.HasMember("products") || !_doc["products"].IsObject()) {
    throw std::runtime_error("Missing or invalid 'products' object");
  }

  const auto &products = _doc["products"];
  if (products.MemberCount() == 0) {
    throw std::runtime_error("No products available");
  }

  // Access the last element (which will be the latest release)
  auto iter = products.MemberEnd();
  --iter;

  if (!iter->value.IsObject() || !iter->value.HasMember("version") ||
      !iter->value["version"].IsString()) {
    throw std::runtime_error("Last product has no valid version");
  }

  return iter->value["version"].GetString();
}

std::string DataFetcher::getChecksum(const std::string &imageName) {
  std::vector<std::string> tokens;
  std::stringstream ss(imageName);
  std::string token;

  // imageName is expected to be of form:
  // ubuntu-<codename>-<version#>-<plaform>-server-<yearmonthday>
  // e.g (ubuntu-maverick-10.10-amd64-server-20120410)
  while (std::getline(ss, token, '-')) {
    tokens.push_back(std::move(token));
  }

  if (tokens.size() < 6) {
    throw std::runtime_error(
        "Invalid image name: expected name in the form of "
        "ubuntu-<codename>-<version#>-<plaform>-server-<yearmonthday>");
  }

  const std::string key =
      "com.ubuntu.cloud:server:" + tokens[2] + ":" + tokens[3];

  const auto &products = _doc["products"];
  if (!products.HasMember(key.c_str())) {
    throw std::runtime_error("Product key not found: " + key);
  }

  const auto &product = products[key.c_str()];
  const auto versionKey = tokens.back();

  if (!product.HasMember("versions") ||
      !product["versions"].HasMember(versionKey.c_str())) {
    throw std::runtime_error("Version not found: " + versionKey);
  }

  const auto &version = product["versions"][versionKey.c_str()];

  if (!version.HasMember("items") || !version["items"].HasMember("disk1.img")) {
    throw std::runtime_error("Missing disk1.img item in version: " +
                             versionKey);
  }

  const auto &item = version["items"]["disk1.img"];

  if (!item.HasMember("sha256") || !item["sha256"].IsString()) {
    throw std::runtime_error("Missing or invalid SHA256 hash for disk1.img");
  }

  return item["sha256"].GetString();
}
