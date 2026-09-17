#include "bracketparser.h"

BracketParser::BracketParser()
    : mPos(0)
{
}

std::unique_ptr<TreeNode> BracketParser::parse(const QString &content)
{
    mContent = content;
    mPos = 0;
    
    auto root = std::make_unique<TreeNode>("Root");
    
    skipWhitespace();
    
    // Parse the main structure recursively
    parseValue(root.get());
    
    return root;
}

void BracketParser::parseValue(TreeNode *parent)
{
    skipWhitespace();
    
    if (isEOF())
        return;
    
    char c = peek();
    
    if (c == '{') {
        // Start of a new collection/object
        get(); // consume '{'
        
        int elementIndex = 0;
        while (!isEOF() && peek() != '}') {
            skipWhitespace();
            
            if (peek() == '}')
                break;
            
            // Create a node for this element
            QString elementData = parseElement();
            if (!elementData.isEmpty()) {
                auto childNode = std::make_unique<TreeNode>(elementData, parent);
                parent->appendChild(std::move(childNode));
            }
            
            skipWhitespace();
            
            // Check for comma separator
            if (peek() == ',') {
                get(); // consume ','
            }
        }
        
        if (!isEOF() && peek() == '}') {
            get(); // consume '}'
        }
    } else {
        // Simple value - should not happen at top level in valid 1C bracket format
        parseElement();
    }
}

QString BracketParser::parseElement()
{
    skipWhitespace();
    
    if (isEOF())
        return QString();
    
    char c = peek();
    
    if (c == '{') {
        // Nested structure - create a synthetic node and recurse
        get(); // consume '{'
        
        TreeNode tempNode("");
        int itemCount = 0;
        
        while (!isEOF() && peek() != '}') {
            skipWhitespace();
            
            if (peek() == '}')
                break;
            
            QString itemData = parseElement();
            if (!itemData.isEmpty()) {
                auto childNode = std::make_unique<TreeNode>(itemData, &tempNode);
                tempNode.appendChild(std::move(childNode));
                itemCount++;
            }
            
            skipWhitespace();
            
            if (peek() == ',') {
                get(); // consume ','
            }
        }
        
        if (!isEOF() && peek() == '}') {
            get(); // consume '}'
        }
        
        // Return a representation of this nested structure
        return QString("{%1 items}").arg(itemCount);
    } else if (c == '"') {
        return readString();
    } else {
        // Read until comma, closing brace, or EOF
        return readToken();
    }
}

void BracketParser::skipWhitespace()
{
    while (!isEOF() && (mContent[mPos] == ' ' || mContent[mPos] == '\n' || 
                        mContent[mPos] == '\r' || mContent[mPos] == '\t')) {
        mPos++;
    }
}

QString BracketParser::readToken()
{
    QString token;
    
    while (!isEOF()) {
        char c = peek();
        
        if (c == ',' || c == '}' || c == '{' || c == '"' || 
            c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            break;
        }
        
        token += c;
        get();
    }
    
    return token.trimmed();
}

QString BracketParser::readString()
{
    if (peek() != '"')
        return QString();
    
    get(); // consume opening quote
    
    QString result;
    while (!isEOF() && peek() != '"') {
        if (peek() == '\\' && mPos + 1 < mContent.size()) {
            get(); // consume backslash
            if (!isEOF()) {
                result += get();
            }
        } else {
            result += get();
        }
    }
    
    if (!isEOF()) {
        get(); // consume closing quote
    }
    
    return result;
}

QString BracketParser::readUUID()
{
    return readToken();
}

QString BracketParser::readNumber()
{
    QString number;
    
    while (!isEOF()) {
        char c = peek();
        if (c >= '0' && c <= '9') {
            number += c;
            get();
        } else {
            break;
        }
    }
    
    return number;
}

char BracketParser::peek() const
{
    if (mPos >= mContent.size())
        return '\0';
    return mContent[mPos].toLatin1();
}

char BracketParser::get()
{
    if (mPos >= mContent.size())
        return '\0';
    return mContent[mPos++].toLatin1();
}

bool BracketParser::isEOF() const
{
    return mPos >= mContent.size();
}
