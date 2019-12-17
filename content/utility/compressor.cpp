// WIP, nothing works, doesnt compile.

#include <string>
#include <map>
#include <cassert>
#include <vector>
#include <cstdio>
#include <iostream>
#include <fstream>

class token {
public:
    int id;
    virtual string to_string() const = 0;
};

// represents single token
// or macro, since they are impossible to affect.
// since preprocessor goes only once
// Example:
// #include or 'for'
struct simple_token : public token {
    string content;
    string to_string() const override {
        return content;
    }
};

// Example:
// a.b.c.d.e
class comma_token : public token {
    vector <token&> tokens;
public:
    string to_string() const override {
        string res;
        for (size_t i = 0; i < tokens.size(); ++i) {
            res += tokens[i].to_string();
            if (i != 0) {
                res += '.';
            }
        }
        return res;
    }
};

// Example:
// core(inner1, inner2)
class functor_token : public token {
    const token& core;
    const vector<token&> inner;
public:
    functor_token(
        token& core,
        vector <token&> inner
    )   : core(std::move(core))
        , inner(std::move(inner)) 
    {}

    string to_string() const override {
        string res;
        res += core.to_string();
        res += '(';
        for (size_t i = 0; i < inner.size(); ++i) {
            res += inner[i].to_string();
            if (i != 0) {
                res += ',';
            }
        }
        res += ')';
        return res;
    }
};

// Example:
// core(inner1, inner2)
class bracket_functor_token : public token {
    const token& core;
    const vector<token&> inner;
public:
    bracket_functor_token(
        token& core,
        vector <token&> inner
    ) : core(std::move(core))
        , inner(std::move(inner)) {
    }

    string to_string() const override {
        string res;
        res += core.to_string();
        res += '[';
        for (size_t i = 0; i < inner.size(); ++i) {
            res += inner[i].to_string();
            if (i != 0) {
                res += ',';
            }
        }
        res += ']';
        return res;
    }
};

// nested block
// Example:
// { some_code }
class code_token : public token {
    vector <token&> inner_tokens;
    string to_string() const override {
        string res;
        res += '{';
        for (auto x : inner_tokens) {
            res += x.to_string();
        }
        res += '}';
        return res;
    }
};
using namespace std;


// CLI tool which
// Compresses file into something significantly shorter 
// which unpacks during precompilation

bool readFile(const char* const filename, string& outStr) {
    std::ifstream t(filename);
    if (!t.is_open()) {
        return false;
    }

    t.seekg(0, std::ios::end);
    outStr.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    outStr.assign((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    return true;
}

// if some error occured during runtime
int error(string s) {
    cout << s << endl;
    exit(1);
    return 1;
}

bool isWhitespace(const char x) {
    return
        x == ' ' ||
        x == '\t' ||
        x == '\r' ||
        x == '\n';
}

map <int, token> tokenCache;
map <string, token> tokenCacheStr;
vector <token&> tokenize(std::string content);
vector <token&> split_tokenize(std::string content);
token& extract_token(const string& s) {
    // should not contain no brackets or any of such stuff.
}

vector <token>& append(vector <token>& target, const vector <token>& source) {
    target.reserve(target.size() + source.size());
    for (auto x : source) {
        target.push_back(x);
    }
    return target;
}

bool is_open_bracket(char c) {
    return c == '[' || c == '(' || c == '{';
}

bool is_close_bracket(char c) {
    return c == ']' || c == ')' || c == '}';
}

bool are_paired(char a, char b) {
    if (a == '(') {
        return b == ')';
    }
    if (a == '[') {
        return b == ']';
    }
    if (a == '{') {
        return b == '}';
    }
    return false;
}

vector <token&> tokenize(std::string content) {
    vector <token&> tokens;
    const size_t n = content.size();
    bool line_comment = false;
    bool segment_comment = false;
    for (size_t l = 0; l < n;) {
        while (l < n && isWhitespace(content[l]) || line_comment) {
            if (content[l] == '\n') {
                line_comment = false;
            }
            ++l;
        }
        string token_content;
        if (!line_comment && !segment_comment) {
            if (content[l] == '/') {
                if (content[l + 1] == '/') {
                    line_comment = true;
                    l += 2;
                    continue;
                }
                if (content[l + 1] == '*') {
                    segment_comment = true;
                    l += 2;
                    continue;
                }
            }
        }
        if (content[l] == '*') {
            if (content[l + 1] == '/') {
                segment_comment = false;
                l += 2;
                continue;
            }
        }
        if (segment_comment) {
            ++l;
            continue;
        }
        for (size_t r = l; r < n; ++r) {
            char ch = content[r];
            vector <size_t> open_brackets;
            vector <char> brackets;
            // given code is correct I don't really need to keep chars
            // but for the purposes of testing it is very useful
            if (is_open_bracket(ch)) {
                brackets.push_back(ch);
                open_brackets.push_back(r);
                for (size_t i = r + 1; i < n; ++i) {
                    if (is_open_bracket(ch)) {
                        brackets.push_back(ch);
                        open_brackets.push_back(r);
                        continue;
                    } 
                    if (is_close_bracket(ch)) {
                        assert(are_paired(brackets.back(), ch));
                        if (brackets.size() == 1) {
                            size_t l_b = open_brackets.back() + 1;
                            size_t r_b = i - 1;
                            if (content[l_b - 1] == '(') {
                                tokens.back() =
                                    std::move(
                                        functor_token(
                                            tokens.back(),
                                            tokenize(content.substr(l_b, r_b))
                                        )
                                    );
                            } else {
                                tokens.back() =
                                    std::move(
                                        bracket_functor_token(
                                            tokens.back(),
                                            tokenize(content.substr(l_b, r_b))
                                        )
                                    );
                            }
                            l = r_b + 1;
                        }
                        brackets.pop_back();
                        open_brackets.pop_back();
                    }
                }
                continue;
            }
            if (isWhitespace(r)) {
                tokens.push_back(extract_token(content.substr(l, r - l + 1)));
            }
        }
    }
    return tokens;

}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return error("Not enough arguments passed. Pass file as a first argument.");
    }
    string file_content;
    if (!readFile(argv[1], file_content)) {
        return error("Could not file");
    }
    int n = file_content.size();

    // token for example <string> can be expanded using one of macros
    // but only one of them.
    // keep it mind that macros could be nested, but preprocessor only goes one way, so
    // #include cannot be macroed
    vector <string> outtokens;
    for (auto& token : tokens) {
        outtokens.push_back(token);
    }
    return 0;
}
