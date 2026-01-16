#include "StringUtils.h"

namespace StringUtils {

    std::vector<int> splitIds(const String& str, char separator) {
        std::vector<int> result;
        int start = 0;
        int index = str.indexOf(separator, start);

        while (index != -1) {
            String token = str.substring(start, index);
            result.push_back(token.toInt());
            start = index + 1;
            index = str.indexOf(separator, start);
        }

        // dernier élément
        const String token = str.substring(start);
        result.push_back(token.toInt());

        return result;
    }

    int toInt(const String& str) {
        return str.toInt();
    }

}
