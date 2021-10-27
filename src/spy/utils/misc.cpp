#include <spy/utils/misc.hpp>

string read_file(string path_to_file) {
  ifstream ifs;
  string raw_code = "";
  ifs.open(path_to_file);
  if (ifs.is_open())
    while (ifs)
      raw_code.push_back(ifs.get());

  ifs.close();
  return raw_code;
}

char *str_to_chr(string s) {
  char *chrs = (char *)malloc(sizeof(char) * (s.length() + 1));
  for (int i = 0; i < s.length(); i++)
    chrs[i] = s[i];
  chrs[s.length()] = '\0';
  return chrs;
}
