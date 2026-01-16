#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <Arduino.h>
#include <vector>

namespace StringUtils {

    // Split un String Arduino par un séparateur
    std::vector<int> splitIds(const String& str, char separator = ',');

    // Trim et conversion en int
    int toInt(const String& str);

}

#endif
