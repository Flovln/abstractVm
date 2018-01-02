#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <list>
#include <string>
#include <string.h>
#include <algorithm>
#include <iterator>

#ifndef LEXER_HPP
# define LEXER_HPP

//http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html
#define INSTRUCTION "Instruction"
#define LEXICAL_ERROR "LexicalError"
#define UNKNOWN_INSTRUCTION "UnknownInstruction"
#define INT8 "Int8"
#define INT16 "Int16"
#define INT32 "Int32"
#define FLOAT "Float"
#define DOUBLE "Double"

class Lexer {
  public:
    enum TokenType
    {
      Instruction, // push | pop | dump | assert | add | sub | mul | div | mod | print | exit
      /* Operands */
      Int8,
      Int16, 
      Int32,
      Float,
      Double,
      /* Errors */
      LexicalError,
      UnknownInstruction
    };

    struct token {
      std::string type;
      std::string value;
//      token(std::string t, std::string v) : type(t), value(v) {}
    };

    Lexer(void);
    Lexer(Lexer const &obj);
    ~Lexer(void);

    Lexer &operator=(Lexer const &rhs);

    void  readFromFile(char *file);
    void  readFromStdin(std::string line);
    void  analysis(void);
    void  getChunks(std::string str);
    void  tokenizeChunks(void);

    /* Utilities */
    void  displayVectorContent(void);
    void  displayTokensList(void);

    std::vector<token> tokens;

  private:
    std::vector<std::string> _buff;
    std::vector<std::string> _chunks;
//    std::vector<std::tuple<std::string, int> > _tokens;

};

std::ostream & operator<<(std::ostream & o, Lexer const &obj);

#endif