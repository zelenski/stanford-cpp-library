/*
 * File: server.h
 * --------------
 * This file exports a set of functions that implement a simple HTTP server
 * that can listen for connections.
 * 
 * @version 2016/03/16
 * - initial version
 */

#ifndef _server_h
#define _server_h

#include <iostream>
#include <string>
#include "gevents.h"

namespace HttpServer {
const int DEFAULT_PORT = 8080;
const int HTTP_ERROR_OK = 200;

std::string getContentType(const std::string& extension);
std::string getErrorMessage(int httpErrorCode);
std::string getUrlExtension(const std::string& url);
bool isRunning();
void sendResponse(const GServerEvent& event, const std::string& responseText,
                  const std::string& contentType = "");
void sendResponseError(const GServerEvent& event, int httpErrorCode,
                  const std::string& errorMessage = "");
void sendResponseFile(const GServerEvent& event, const std::string& responseFilePath,
                      const std::string& contentType = "" /* auto */);
void startServer(int port = DEFAULT_PORT);
void stopServer();
} // namespace HttpServer

#endif
