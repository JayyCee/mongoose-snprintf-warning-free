// Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>
// Copyright (c) 2013 Cesanta Software Limited
// All rights reserved
//
// This library is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this library under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this library under a commercial
// license, as set out in <http://cesanta.com/products.html>.
//
// NOTE: Detailed API documentation is at http://cesanta.com/docs.html

#ifndef MONGOOSE_HEADER_INCLUDED
#define  MONGOOSE_HEADER_INCLUDED

#include <stdio.h>      // required for FILE
#include <stddef.h>     // required for size_t

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// This structure contains information about HTTP request.
struct mg_connection {
  const char *request_method; // "GET", "POST", etc
  const char *uri;            // URL-decoded URI
  const char *http_version;   // E.g. "1.0", "1.1"
  const char *query_string;   // URL part after '?', not including '?', or NULL

  char remote_ip[48];         // Max IPv6 string length is 45 characters
  int remote_port;            // Client's port

  int num_headers;            // Number of HTTP headers
  struct mg_header {
    const char *name;         // HTTP header name
    const char *value;        // HTTP header value
  } http_headers[30];

  char *content;              // POST (or websocket message) data, or NULL
  int content_len;            // content length

  int is_websocket;           // Connection is a websocket connection
  int status_code;            // HTTP status code for HTTP error handler
  unsigned char wsbits;       // First byte of the websocket frame
  void *server_param;         // Parameter passed to mg_add_uri_handler()
  void *connection_param;     // Placeholder for connection-specific data
};

struct mg_server; // Opaque structure describing server instance
typedef int (*mg_handler_t)(struct mg_connection *);

// Server management functions
struct mg_server *mg_create_server(void *server_param);
void mg_destroy_server(struct mg_server **);
const char *mg_set_option(struct mg_server *, const char *opt, const char *val);
void mg_poll_server(struct mg_server *, int milliseconds);
void mg_add_uri_handler(struct mg_server *, const char *uri, mg_handler_t);
void mg_set_error_handler(struct mg_server *, mg_handler_t);
void mg_set_log_handler(struct mg_server*, mg_handler_t);
const char **mg_get_valid_option_names(void);
const char *mg_get_option(const struct mg_server *server, const char *name);

// Connection management functions
int mg_write(struct mg_connection *, const void *buf, int len);
const char *mg_get_header(const struct mg_connection *, const char *name);
const char *mg_get_mime_type(const char *file_name);
int mg_get_var(const struct mg_connection *conn,
               const char *var_name, char *dst, size_t dst_len);
int mg_get_cookie(const char *cookie, const char *var_name,
                  char *buf, size_t buf_len);

// Utility functions
int mg_start_thread(void *(*func)(void *), void *param);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED