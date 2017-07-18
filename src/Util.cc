#include "Util.h"
#include "Log.h"
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/MD5Engine.h>
#include <Poco/DigestEngine.h>
#include <Poco/DigestStream.h>
#include <Poco/StreamCopier.h>
#include <sstream>

namespace Util {

    std::string getSources(const std::string &source) {
        //Todo 未实现
        return std::string();
    }

    void split(const std::string &query, const std::string &separator, std::vector<std::string> &array) {
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
        Poco::UUIDGenerator &uuidGenerator = Poco::UUIDGenerator::defaultGenerator();
        Poco::UUID uuid(uuidGenerator.createFromName(Poco::UUID::uri(), uri));

        return uuid.toString();
    }

    std::string genMD5(const std::string &data) {
        Poco::MD5Engine md5Engine;
        Poco::DigestOutputStream digestOutputStream(md5Engine);
        std::istringstream istringstream(data);
        Poco::StreamCopier::copyStream(istringstream, digestOutputStream);
        digestOutputStream.close();
        return Poco::DigestEngine::digestToHex(md5Engine.digest());
    }

    void int2Bytes(long value, Poco::UInt8 *bytes) {
        bytes[0] = (Poco::UInt8) (0xff & value);
        bytes[1] = (Poco::UInt8) ((0xff00 & value) >> 8);
        bytes[2] = (Poco::UInt8) ((0xff0000 & value) >> 16);
        bytes[3] = (Poco::UInt8) ((0xff000000 & value) >> 24);
    }

}