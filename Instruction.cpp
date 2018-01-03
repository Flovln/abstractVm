#include "./includes/Instruction.hpp"
#include "./includes/Vm.hpp"

Instruction::Instruction(void) {}

Instruction::Instruction(Instruction const &obj) {
  *this = obj;
}

Instruction::~Instruction(void) {}

Instruction & Instruction::operator=(Instruction const &rhs) {
  if (this != &rhs) {
    //this->name = rhs.name;
    //...
  }

  return (*this);
}

void  Instruction::createChunks(std::string str)
{
  /* split line on whitespaces to only keep strings and chars */
  std::stringstream lineStream(str);
  std::string       chunk;

  while (lineStream >> chunk)
    this->_chunks.push_back(chunk) ;
}

void  Instruction::lexicalAnalysis(std::vector<std::string> buff, int source) {
  std::string lastElement =  buff.back();

  if (source == 0)
  {
    if (lastElement != "exit")
      throw Vm::SyntaxException("No instruction ending the program found.");
  }
  else if (source == 1)
  {
    if (lastElement != ";;")
      throw Vm::SyntaxException("No instruction ending the program found.");
  }

  /* Remove last element from vector aka exit command or ";;" */
  buff.pop_back();

  std::vector<std::string>::iterator iter = buff.begin();
  std::vector<std::string>::iterator end = buff.end();  

  /* Go through file content line by line to remove comments and tonekize chunks */
  while (iter != end)
  {
    if (!this->_chunks.empty())
      this->_chunks.clear();
    
    this->createChunks(*iter);
    
    if (!this->_commentsRemoved.empty())
      this->_commentsRemoved.clear();
    
    this->removeComments();
    this->tokenizer();
    ++iter;
  }
 
}

void  Instruction::removeComments(void)
{
  std::vector<std::string>::iterator iter = this->_chunks.begin();
  std::vector<std::string>::iterator end = this->_chunks.end();

  this->_markedAsComment = false;

  while (iter != end)
  {
    for (size_t i = 0; i < (*iter).length(); ++i)
    {
      if ((*iter)[i] == ';')
      {
        if (i > 0)
          this->_commentsRemoved.push_back((*iter).substr(0, i));
        this->_markedAsComment = true;
        break;
      }
    }

    if (this->_markedAsComment != true)
      this->_commentsRemoved.push_back(*iter);

    ++iter;
  }
}

void  Instruction::tokenizeSimple(std::string chunk)
{
  std::regex simpleInstructions("pop|dump|add|sub|mul|div|mod|print");
  std::regex complexInstructions("push|assert");

  if (std::regex_match(chunk, simpleInstructions) == true)
  {
    this->_markAsUnknownInstruction = true;
    this->_tokens.push_back({Token::Instruction, "", chunk});
  }
  else if (std::regex_match(chunk, complexInstructions) == true)
    this->_tokens.push_back({Token::Instruction, "", chunk});
  else if (this->_markAsUnknownInstruction == false)
  {
    this->_markAsUnknownInstruction = true;
    this->_tokens.push_back({Token::UnknownInstruction, "", chunk});
  }
}

void  Instruction::tokenizeComplex(std::string chunk)
{
  for (size_t i = 0; i < chunk.length(); ++i)
  {
    if (chunk[i] == '(' && i > 0)
    {
      int   count;
      bool  closed;

      count = 0;
      closed = false;
      for (int j = i; chunk[j] != '\0'; j++)
      {
        if (chunk[j] == ')')
        {
          closed = true;
          break;
        }
        count++;
      }

      /* Get content before and between parenthesis */
      this->_tokens.push_back({Token::Operand, chunk.substr(0, i), chunk.substr(i + 1, count - 1)});

      if (closed == false)
        this->_tokens.push_back({Token::LexicalError, "", "Missing closing parenthesis"});

      if (i + count + 1 < chunk.length())
      {
        this->_tokens.push_back({Token::LexicalError, "", chunk.substr(i + count + 1, chunk.length())});
        this->_markAsLexicalError = true;
      }
  
      this->_markAsUnknownInstruction = true;
    }
  }
}

void  Instruction::tokenizer(void)
{
  std::vector<std::string>::iterator iter = this->_commentsRemoved.begin();
  std::vector<std::string>::iterator end = this->_commentsRemoved.end();
  this->_markAsLexicalError = false;
  this->_markAsUnknownInstruction = false;

  while (iter != end)
  {
    if (this->_markAsUnknownInstruction == true)
      this->_tokens.push_back({Token::LexicalError, "", *iter});
    else if (this->_markAsLexicalError == false)
    {
      this->tokenizeComplex(*iter);
      this->tokenizeSimple(*iter);
    }
    else
      this->_tokens.push_back({Token::LexicalError, "",*iter});

    ++iter;
  }
}

std::list<Token>  Instruction::parser(void)
{
  for (auto &iter : this->_tokens)
  {
    std::cout << "Token: " << "{ " << iter.type << ", " << iter.valueType << ", " << iter.value << " }" << std::endl;

    if (iter.type == Token::Instruction)
    {
        //std::next(iter, 1);
        this->_next = *(&iter + 1);
        std::cout << "Next Token: " << "{ " << this->_next.type << ", " << this->_next.valueType << ", " << this->_next.value << " }" << std::endl;
    }
  }

  return this->_instructions;
}

/* Development tools */

void  Instruction::displayVectorContent(std::vector<std::string> buff) {
  std::cout << "--- Vector content in Instruction ---" << std::endl;

  std::vector<std::string>::iterator iter = buff.begin();
  std::vector<std::string>::iterator end = buff.end();

  while (iter != end)
  {
    std::cout << (*iter) << std::endl;
    ++iter;
  }

  std::cout << "---------" << std::endl;
}

void  Instruction::displayTokensListWithoutComments(void)
{
  std::vector<std::string>::iterator iter = this->_commentsRemoved.begin();
  std::vector<std::string>::iterator end = this->_commentsRemoved.end();

  std::cout << "--- Tokens list without comments ---" << std::endl;
  while (iter != end)
  {
    std::cout << "Token: " << *iter << std::endl;
    ++iter;
  }
  std::cout << "--- --- ---" << std::endl;
}

void  Instruction::displayTokensList(void)
{
  std::vector<Token>::iterator iter = this->_tokens.begin();
  std::vector<Token>::iterator end = this->_tokens.end();

  std::cout << "--- Tokens list Instruction ---" << std::endl;
  while (iter != end)
  {
    std::cout << "Token: " << "{ " << iter[0].type << ", " << iter[0].valueType << ", " << iter[0].value << " }" << std::endl;
    ++iter;
  }
  std::cout << "--- --- ---" << std::endl;
}

/* Non member function */

std::ostream & operator<<(std::ostream & o, Instruction const &obj )
{
  (void)obj;
  o << "operator overload" << std::endl;
  return o;
}