#pragma once

#include <string>

class CurlWrapper {
public:
  CurlWrapper();
  ~CurlWrapper();

  std::string fetch(const std::string &url);

private:
  static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
};
