#ifndef MNIARGS_H
#define MNIARGS_H

/* MINC tools spell long options with a single dash ("-help", "-column x"),
   which cxxopts reads as a group of short flags.  These helpers rewrite the
   command line into the form cxxopts expects -- "-opt" becomes "--opt" -- so
   the historical command lines keep working.  An option taking several values
   ("-vertstats_output <file> <value>") is expanded into one "--opt=value" per
   value, and is read back as a vector<string>. */

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <cxxopts.hpp>

namespace mniArgs {

/*! Number of values consumed by each multi-valued option, by option name.
 *  Options not listed take the usual single value. */
typedef std::map<std::string, int> MultiValueMap;

/*! Rewrites the command line from MINC single-dash form into cxxopts form. */
inline std::vector<std::string>
rewrite(int argc, char *argv[], const MultiValueMap &multiValue) {
  std::vector<std::string> out;
  bool endOfOptions = false;

  for (int i = 0; i < argc; ++i) {
    std::string token(argv[i]);

    // program name, "-", "--", and negative numbers pass through untouched,
    // as do options already written in cxxopts' "--opt" form and everything
    // following "--"
    if (i == 0 || endOfOptions || token.size() < 2 || token[0] != '-' ||
        !std::isalpha(static_cast<unsigned char>(token[1]))) {
      endOfOptions = endOfOptions || token == "--";
      out.push_back(token);
      continue;
    }

    std::string name(token, 1);
    MultiValueMap::const_iterator multi = multiValue.find(name);

    if (multi == multiValue.end()) {
      out.push_back("--" + name);
      continue;
    }

    for (int value = 0; value < multi->second; ++value) {
      if (i + 1 >= argc)
        throw cxxopts::exceptions::exception("Too few arguments for option " +
                                             name);
      out.push_back("--" + name + "=" + argv[++i]);
    }
  }

  return out;
}

/*! Parses a MINC-style command line.
 *
 * Prints the usage screen and exits on -help, on a malformed command line, or
 * when a mandatory positional argument is missing - as the tools using this
 * did back when they returned straight out of main().
 *
 * \param opts the cxxopts options, positionals included
 * \param positional names of the mandatory positional arguments, in order
 * \param multiValue options taking more than one value
 */
inline cxxopts::ParseResult
parse(cxxopts::Options &opts, int argc, char *argv[],
      const std::vector<std::string> &positional,
      const MultiValueMap &multiValue = MultiValueMap()) {
  opts.parse_positional(positional);

  try {
    const std::vector<std::string> rewritten =
        rewrite(argc, argv, multiValue);

    std::vector<const char *> cargv;
    for (size_t i = 0; i < rewritten.size(); ++i)
      cargv.push_back(rewritten[i].c_str());

    cxxopts::ParseResult result =
        opts.parse(static_cast<int>(cargv.size()), cargv.data());

    if (result.count("help")) {
      std::cerr << opts.help() << std::endl;
      exit(0);
    }

    for (size_t i = 0; i < positional.size(); ++i) {
      if (!result.count(positional[i])) {
        std::cerr << opts.program() << " error: Too few arguments." << std::endl;
        std::cerr << opts.help() << std::endl;
        exit(1);
      }
    }

    return result;
  } catch (const cxxopts::exceptions::exception &e) {
    std::cerr << opts.program() << " error: " << e.what() << "." << std::endl;
    std::cerr << opts.help() << std::endl;
    exit(1);
  }
}

} // namespace mniArgs

#endif // MNIARGS_H
