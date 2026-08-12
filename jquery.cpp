#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static bool nextObject(const std::string& json, size_t& pos, std::string& out) {
    size_t start = json.find('{', pos);
    if (start == std::string::npos)
        return false;

    int depth = 0;
    bool inString = false;
    for (size_t i = start; i < json.size(); ++i) {
        char c = json[i];
        if (inString) {
            if (c == '\\') {
                ++i;
            } else if (c == '"') {
                inString = false;
            }
        } else {
            if (c == '"') {
                inString = true;
            } else if (c == '{') {
                ++depth;
            } else if (c == '}') {
                --depth;
                if (depth == 0) {
                    out = json.substr(start, i - start + 1);
                    pos = i + 1;
                    return true;
                }
            }
        }
    }
    return false;
}

static std::string extractString(const std::string& obj, const std::string& key) {
    std::string search = '\"' + key + '\"';
    size_t pos = obj.find(search);
    if (pos == std::string::npos)
        return "";

    pos += search.length();
    while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t' || obj[pos] == '\n' || obj[pos] == '\r' || obj[pos] == ':')) {
        ++pos;
    }
    if (pos >= obj.length() || obj[pos] != '\"')
        return "";

    ++pos; // skip opening quote
    std::string value;
    while (pos < obj.length()) {
        char c = obj[pos];
        if (c == '\\') {
            if (pos + 1 < obj.length()) {
                value += obj[pos + 1];
                pos += 2;
            } else {
                break;
            }
        } else if (c == '\"') {
            break;
        } else {
            value += c;
            ++pos;
        }
    }
    return value;
}

static double extractNumber(const std::string& obj, const std::string& key) {
    std::string search = '\"' + key + '\"';
    size_t pos = obj.find(search);
    if (pos == std::string::npos)
        return 0.0;

    pos += search.length();
    while (pos < obj.length() && (obj[pos] == ' ' || obj[pos] == '\t' || obj[pos] == '\n' || obj[pos] == '\r' || obj[pos] == ':')) {
        ++pos;
    }
    if (pos >= obj.length())
        return 0.0;

    std::string numStr;
    while (pos < obj.length() && (obj[pos] == '-' || (obj[pos] >= '0' && obj[pos] <= '9') || obj[pos] == '.')) {
        numStr += obj[pos];
        ++pos;
    }
    if (numStr.empty())
        return 0.0;

    return std::stod(numStr);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <json_file_path>" << std::endl;
        return 1;
    }

    try {
        std::string json = readFile(argv[1]);

        size_t pos = json.find("\"data\"");
        if (pos == std::string::npos) {
            std::cerr << "No 'data' array found in JSON" << std::endl;
            return 1;
        }

        pos = json.find('[', pos);
        if (pos == std::string::npos) {
            std::cerr << "Invalid JSON: no array after 'data'" << std::endl;
            return 1;
        }
        ++pos;

        std::cout << "Filtered results:" << std::endl;
        std::cout << "=================" << std::endl;
        bool found = false;

        std::string obj;
        while (nextObject(json, pos, obj)) {
            std::string type = extractString(obj, "type");
            std::string tijd = extractString(obj, "tijd");
            double vrijMeters = extractNumber(obj, "vrijMeters");

            if (type == "V" && tijd < "15:00" && vrijMeters >= 5) {
                found = true;
                std::cout << "  tijd: " << tijd
                         /* << ", type: " << type */
                          << ", schip: " << extractString(obj, "schip")
                          << ", vrijMeters: " << static_cast<int>(vrijMeters)
                         /* << ", vrijPersonen: " << static_cast<int>(extractNumber(obj, "vrijPersonen")) */
                          << std::endl;
            }
        }

        if (!found) {
            std::cout << "(no matching entries)" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
