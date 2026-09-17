#ifndef BRACKETPARSER_H
#define BRACKETPARSER_H

#include <QString>
#include <memory>
#include "treenode.h"

class BracketParser {
public:
    BracketParser();
    
    std::unique_ptr<TreeNode> parse(const QString &content);
    
private:
    void skipWhitespace();
    QString readToken();
    QString readString();
    QString readUUID();
    QString readNumber();
    char peek() const;
    char get();
    bool isEOF() const;
    
    QString mContent;
    int mPos;
};

#endif // BRACKETPARSER_H
