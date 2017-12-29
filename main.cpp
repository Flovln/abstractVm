#include "Lexer.hpp"
#include "Parser.hpp"

int main(int ac, char **av)
{
  if (ac < 3)
  {
    Lexer lexer;

    if (ac == 2)
      lexer.readFromFile(av[1]);
    else
    {
      std::string line;
  
      while (1)
      {
        std::cin >> line;
        lexer.readFromStdin(line);
        
        if (line == ";;")
          break;
      }
    }

    lexer.analysis();
    Parser parser;

    //parser.mainFilter(lexer._tokens);
    // call factory method
  }
  else
    std::cout << "AbstractVm runs programs from a file and from the standard input." << std::endl;

  return (0);
}