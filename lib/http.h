/*
copyright: Boaz segev, 2016
license: MIT

Feel free to copy, use and enjoy according to the license provided.
*/
#ifndef HTTP_COLLECTED_H
#define HTTP_COLLECTED_H

/**
This header file simply collects all the http libraries required to start an
HTTP server and defines simple marcros to start an HTTP server using lib-server.
*/
#include "lib-server.h"
#include "http-response.h"
#include "http-protocol.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

/**
This macro allows an easy start for an HTTP server, using the lib-server library
and the HTTP library helpers.

The macro accepts variable arguments. The first two (required) arguments are the
on_request callback for the HTTP protocol and an optional (can be NULL) public
folder for serving static files. The rest of the optional (variable) arguments
are any valid `struct ServerSettings` fields.

i.e. use:

    void on_request(struct HttpRequest request) {
      // ...
    }

    int main()
    {
      char * public_folder = NULL
      start_http_server(on_request, public_folder, .threads = 16);
    }
*/

#define start_http_server(on_request_callback, http_public_folder, ...)        \
  do {                                                                         \
    struct HttpProtocol* protocol = HttpProtocol.new();                        \
    if ((on_request_callback))                                                 \
      protocol->on_request = (on_request_callback);                            \
    if ((http_public_folder))                                                  \
      protocol->public_folder = (http_public_folder);                          \
    HttpResponse.create_pool();                                                \
    Server.listen((struct ServerSettings){                                     \
        .timeout = 5, __VA_ARGS__, .protocol = (struct Protocol*)(protocol)}); \
    HttpResponse.destroy_pool();                                               \
    HttpProtocol.destroy(protocol);                                            \
  } while (0);

#endif /* HTTP_COLLECTED_H */