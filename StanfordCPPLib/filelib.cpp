/*
 * File: filelib.cpp
 * -----------------
 * This file implements the filelib.h interface.  All platform dependencies
 * are managed through the platform interface.
 */

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "filelib.h"
#include "foreach.h"
#include "platform.h"
#include "strlib.h"
#include "vector.h"
using namespace std;

static Platform *pp = getPlatform();

/* Prototypes */

static void splitPath(string path, Vector<string> list);
static bool recursiveMatch(string str, int sx, string pattern, int px);

/* Implementations */

bool openFile(ifstream & stream, string filename) {
   stream.clear();
   stream.open(expandPathname(filename).c_str());
   return !stream.fail();
}

bool openFile(ofstream & stream, string filename) {
   stream.clear();
   stream.open(expandPathname(filename).c_str());
   return !stream.fail();
}

string promptUserForFile(ifstream & stream, string prompt) {
   while (true) {
      cout << prompt;
      string filename;
      getline(cin, filename);
      openFile(stream, filename);
      if (!stream.fail()) return filename;
      stream.clear();
      cout << "Unable to open that file.  Try again." << endl;
      if (prompt == "") prompt = "Input file: ";
   }
}

string promptUserForFile(ofstream & stream, string prompt) {
   while (true) {
      cout << prompt;
      string filename;
      getline(cin, filename);
      openFile(stream, filename);
      if (!stream.fail()) return filename;
      stream.clear();
      cout << "Unable to open that file.  Try again." << endl;
      if (prompt == "") prompt = "Output file: ";
   }
}

string openFileDialog(ifstream & stream) {
   return openFileDialog(stream, "Open File", "");
}

string openFileDialog(ifstream & stream, string title) {
   return openFileDialog(stream, title, "");
}

string openFileDialog(ifstream & stream, string title, string path) {
   string filename = pp->openFileDialog(title, "load", path);
   if (filename == "") return "";
   stream.open(filename.c_str());
   return (stream.fail()) ? "" : filename;
}

string openFileDialog(ofstream & stream) {
   return openFileDialog(stream, "Open File", "");
}

string openFileDialog(ofstream & stream, string title) {
   return openFileDialog(stream, title, "");
}

string openFileDialog(ofstream & stream, string title, string path) {
   string filename = pp->openFileDialog(title, "save", path);
   if (filename == "") return "";
   stream.open(filename.c_str());
   return (stream.fail()) ? "" : filename;
}

void readEntireFile(istream & is, Vector<string> & lines) {
   lines.clear();
   while (true) {
      string line;
      getline(is, line);
      if (is.fail()) break;
      lines.add(line);
   }
}

void readEntireFile(istream & is, vector<string> & lines) {
   lines.clear();
   while (true) {
      string line;
      getline(is, line);
      if (is.fail()) break;
      lines.push_back(line);
   }
}

string getRoot(string filename) {
   int dot = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '.') dot = i;
      if (ch == '/' || ch == '\\') dot = -1;
   }
   if (dot == -1) {
      return filename;
   } else {
      return filename.substr(0, dot);
   }
}

string getExtension(string filename) {
   int dot = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '.') dot = i;
      if (ch == '/' || ch == '\\') dot = -1;
   }
   if (dot == -1) {
      return "";
   } else {
      return filename.substr(dot);
   }
}

string getHead(string filename) {
   int slash = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '/' || ch == '\\') slash = i;
   }
   if (slash < 0) {
      return "";
   } else if (slash == 0) {
      return "/";
   } else {
      return filename.substr(0, slash);
   }
}

string getTail(string filename) {
   int slash = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '/' || ch == '\\') slash = i;
   }
   if (slash < 0) {
      return filename;
   } else {
      return filename.substr(slash + 1);
   }
}

string defaultExtension(string filename, string ext) {
   bool force = (ext[0] == '*');
   if (force) ext = ext.substr(1);
   int dot = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '.') dot = i;
      if (ch == '/' || ch == '\\') dot = -1;
   }
   if (dot == -1) {
      force = true;
      dot = len;
   }
   if (force) {
      return filename.substr(0, dot) + ext;
   } else {
      return filename;
   }
}

string openOnPath(ifstream & stream, string path, string filename) {
   Vector<string> paths;
   splitPath(path, paths);
   foreach (string dir in paths) {
      string pathname = dir + "/" + filename;
      if (openFile(stream, pathname)) return pathname;
   }
   return "";
}

string openOnPath(ofstream & stream, string path, string filename) {
   Vector<string> paths;
   splitPath(path, paths);
   foreach (string dir in paths) {
      string pathname = dir + "/" + filename;
      if (openFile(stream, pathname)) return pathname;
   }
   return "";
}

string findOnPath(string path, string filename) {
   ifstream stream;
   string result = openOnPath(stream, path, filename);
   if (result != "") stream.close();
   return result;
}

void deleteFile(string filename) {
   remove(expandPathname(filename).c_str());
}

void renameFile(string oldname, string newname) {
   oldname = expandPathname(oldname);
   newname = expandPathname(newname);
   rename(oldname.c_str(), newname.c_str());
}

void createDirectoryPath(string path) {
   size_t cp = 1;
   if (path == "") return;
   while ((cp = path.find('/', cp + 1)) != string::npos) {
      createDirectory(path.substr(0, cp - 1));
   }
   createDirectory(path);
}

bool matchFilenamePattern(string filename, string pattern) {
   return recursiveMatch(filename, 0, pattern, 0);
}

bool fileExists(string filename) {
   return pp->fileExists(filename);
}

bool isFile(string filename) {
   return pp->isFile(filename);
}

bool isSymbolicLink(string filename) {
   return pp->isSymbolicLink(filename);
}

bool isDirectory(string filename) {
   return pp->isDirectory(filename);
}

void setCurrentDirectory(string path) {
   return pp->setCurrentDirectory(path);
}

string getCurrentDirectory() {
   return pp->getCurrentDirectory();
}

void createDirectory(string path) {
   return pp->createDirectory(path);
}

string getDirectoryPathSeparator() {
   return pp->getDirectoryPathSeparator();
}

string getSearchPathSeparator() {
   return pp->getSearchPathSeparator();
}

string expandPathname(string filename) {
   return pp->expandPathname(filename);
}

void listDirectory(string path, Vector<string> & list) {
   vector<string> vec;
   listDirectory(path, vec);
   list.clear();
   foreach (string file in vec) {
      list.add(file);
   }
}

void listDirectory(string path, vector<string> & list) {
   return pp->listDirectory(path, list);
}

/* Private functions */

static void splitPath(string path, Vector<string> list) {
   char sep = (path.find(';') == string::npos) ? ':' : ';';
   path += sep;
   size_t start = 0;
   while (true) {
      size_t finish = path.find(sep, start);
      if (finish == string::npos) break;
      if (finish > start + 1) {
         list.add(path.substr(start, finish - start - 1));
      }
      start = finish + 1;
   }
}

static bool recursiveMatch(string str, int sx, string pattern, int px) {
   int slen = str.length();
   int plen = pattern.length();
   if (px == plen) return (sx == slen);
   char pch = pattern[px];
   if (pch == '*') {
      for (int i = sx; i <= slen; i++) {
         if (recursiveMatch(str, i, pattern, px + 1)) return true;
      }
      return false;
   }
   if (sx == slen) return false;
   char sch = str[sx];
   if (pch == '[') {
      bool match = false;
      bool invert = false;
      px++;
      if (px == plen) {
         error("matchFilenamePattern: missing ]");
      }
      if (pattern[px] == '^') {
         px++;
         invert = true;
      }
      while (px < plen && pattern[px] != ']') {
         if (px + 2 < plen && pattern[px + 1] == '-') {
            match |= (sch >= pattern[px] && sch <= pattern[px + 2]);
            px += 3;
         } else {
            match |= (sch == pattern[px]);
            px++;
         }
      }
      if (px == plen) {
         error("matchFilenamePattern: missing ]");
      }
      if (match == invert) return false;
   } else if (pch != '?') {
      if (pch != sch) return false;
   }
   return recursiveMatch(str, sx + 1, pattern, px + 1);
}
