#include "Util.h"
#include "Log.h"
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>

namespace Util {

    std::string getSources(const std::string &source) {
        /*
        LOG_DEBUG << "source:" << source << "\n";
        if (source.empty())
            return std::string("");
        std::string mustIndex;
        std::string sources;
        bool valid = true;
        for (int i = 0; i < source.size(); ++i) {
            if (source[i] == '1')
                mustIndex.append()
        }
         */
    }

    void split(const std::string &query, const std::string &separator, std::vector<std::string>& array) {
        array.clear();
        if (query.empty())
            return;
        if (separator.empty()) {
            array.push_back(query);
            return;
        }
        Poco::StringTokenizer splitTokenizer(query, separator, Poco::StringTokenizer::TOK_TRIM);
        for (int i = 0; i < splitTokenizer.count(); ++i)
            array.push_back(splitTokenizer[i]);
        return;
    }

    std::string getUUIDFromName(const std::string &uri) {
        Poco::UUIDGenerator& uuidGenerator = Poco::UUIDGenerator::defaultGenerator();
        Poco::UUID uuid(uuidGenerator.createFromName(Poco::UUID::uri(), uri));

        return uuid.toString();
    }
}