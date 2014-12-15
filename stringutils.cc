/*
 * File: stringutils.cpp
 * ---------------------
 * This file contains implementations of utility functions related to strings.
 * See stringutils.h for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2014/10/31
 * - fixed bug in string trimToHeight function
 * @since 2014/03/01
 */

#include "stringutils.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>



namespace stringutils {
    int charsDifferent(std::string s1, std::string s2) {
        size_t length = std::min(s1.length(), s2.length());
        int count = 0;
        for (size_t i = 0; i < length; i++) {
            if (s1[i] != s2[i]) {
                count++;
            }
        }
        return count;
    }
    
    std::string collapseSpaces(std::string s) {
        std::ostringstream result;
        char last = '\0';
        for (int i = 0; i < (int) s.length(); i++) {
            if (s[i] == ' ' && last == ' ') {
                continue;
            }
            last = s[i];
            result << (char) last;
        }
        return result.str();
    }

    std::vector<std::string> explodeLines(std::string s) {
        std::vector<std::string> result;
        std::string line;
        for (size_t i = 0; i < s.length(); i++) {
            char ch = s[i];
            if (ch == '\n') {
                // end of line
                result.push_back(trimR(line));
                line = "";
            } else if (ch != '\r') {
                line += ch;
            }
        }
        if ((int) line.length() > 0 || s.empty()) {
            result.push_back(line);
        }
        return result;
    }
    
    int height(std::string s) {
        std::string line;
        int height = 0;
        for (size_t i = 0; i < s.length(); i++) {
            char ch = s[i];
            if (ch == '\n') {
                // end of line
                height++;
                line = "";
            } else if (ch != '\r') {
                line += ch;
            }
        }
        if ((int) line.length() > 0) {
            height++;
        }
        return height;
    }
    
    std::string implode(const std::vector<std::string>& v, std::string delimiter) {
        if (v.size() == 0) {
            return "";
        } else {
            std::string result = v[0];
            for (int i = 1; i < v.size(); i++) {
                result += delimiter;
                result += v[i];
            }
            return result;
        }
    }
    
    std::string indent(std::string s, int spaces) {
        std::vector<std::string> lines = explodeLines(s);
        std::vector<std::string> newLines;
        std::string indentStr = "";
        for (int i = 0; i < spaces; i++) {
            indentStr += " ";
        }
        for (std::string line : lines) {
            newLines.push_back(indentStr + line);
        }
        return implode(newLines);
    }
    
    std::string makeSloppy(std::string s) {
        s = removeBlankLines(s);
        s = collapseSpaces(s);
        return s;
    }
    
   
    
    std::string removeBlankLines(std::string s) {
        std::vector<std::string> lines = explodeLines(s);
        std::vector<std::string> newLines;
        for (std::string line : lines) {
            line = trimR(line);
            if (line.length() > 0) {
                newLines.push_back(line);
            }
        }
        return implode(newLines, "\n");
    }
    
    std::string stripWhitespace(std::string s) {
        std::ostringstream result;
        for (size_t i = 0; i < s.size(); i++) {
            if (!isspace(s[i])) {
                result << tolower(s[i]);
            }
        }
        return result.str();
    }
    
    std::string toLowerCase(std::string s) {
        std::string result;
        for (size_t i = 0; i < s.length(); i++) {
            result += tolower(s[i]);
        }
        return result;
    }
    
    std::string toPrintable(int ch) {
        if (ch == '\n') {
            return "'\\n'";
        } else if (ch == '\t') {
            return "'\\t'";
        } else if (ch == '\r') {
            return "'\\r'";
        } else if (ch == '\f') {
            return "'\\f'";
        } else if (ch == '\b') {
            return "'\\b'";
        } else if (ch == '\0') {
            return "'\\0'";
        } else if (ch == ' ') {
            return "' '";
        } else if (!isgraph(ch)) {
            return "???";
        } else {
            return std::string("'") + (char) ch + std::string("'");
        }
    }
    
    std::string trimR(std::string s) {
        while (s.length() > 0 && isspace(s[s.length() - 1])) {
            s = s.substr(0, s.length() - 1);
        }
        return s;
    }
    
    std::string trimToHeight(std::string s, int height, std::string suffix) {
        std::vector<std::string> lines = explodeLines(s);
        size_t lineCount = lines.size();
        bool wasTooTall = lineCount > height;
        while (lines.size() > height) {
            lines.erase(lines.begin() + lines.size() - 1);
        }
        if (wasTooTall && !suffix.empty()) {
            lines.push_back(suffix + " (" + std::to_string(lineCount - height) + " line(s) truncated)");
        }
        return implode(lines);
    }
    
    std::string trimToWidth(std::string s, int width, std::string suffix) {
        std::vector<std::string> lines = explodeLines(s);
        for (int i = 0; i < lines.size(); i++) {
            if ((int) lines[i].length() > width) {
                lines[i] = lines[i].substr(0, width);
                if (!suffix.empty()) {
                    lines[i] += suffix;
                }
            }
        }
        return implode(lines);
    }
    
    // truncate string with ... between
    std::string truncate(std::string s, int length) {
        int slength = (int) s.length();
        if (slength > length) {
            // s = s.substr(0, length/2) + " ... " + s.substr(slength - length/2);
            s = s.substr(0, length) + " ...";
        }
        return s;
    }
    
    int width(std::string s) {
        std::string line;
        size_t width = 0;
        for (size_t i = 0; i < s.length(); i++) {
            char ch = s[i];
            if (ch == '\n') {
                // end of line
                if (line.length() > width) {
                    width = line.length();
                }
                line = "";
            } else if (ch != '\r') {
                line += ch;
            }
        }
        if ((int) line.length() > 0) {
            if (line.length() > width) {
                width = line.length();
            }
        }
        return (int)width;
    }
}
