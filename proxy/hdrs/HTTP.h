/** @file

  A brief file description

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

#ifndef __HTTP_H__
#define __HTTP_H__


#include <assert.h>
#include "Arena.h"
#include "INK_MD5.h"
#include "MIME.h"
#include "URL.h"

#include "ink_apidefs.h"

#define HTTP_VERSION(a,b)  ((((a) & 0xFFFF) << 16) | ((b) & 0xFFFF))
#define HTTP_MINOR(v)      ((v) & 0xFFFF)
#define HTTP_MAJOR(v)      (((v) >> 16) & 0xFFFF)


enum HTTPStatus
{
  HTTP_STATUS_NONE = 0,

  HTTP_STATUS_CONTINUE = 100,
  HTTP_STATUS_SWITCHING_PROTOCOL = 101,

  HTTP_STATUS_OK = 200,
  HTTP_STATUS_CREATED = 201,
  HTTP_STATUS_ACCEPTED = 202,
  HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION = 203,
  HTTP_STATUS_NO_CONTENT = 204,
  HTTP_STATUS_RESET_CONTENT = 205,
  HTTP_STATUS_PARTIAL_CONTENT = 206,

  HTTP_STATUS_MULTIPLE_CHOICES = 300,
  HTTP_STATUS_MOVED_PERMANENTLY = 301,
  HTTP_STATUS_MOVED_TEMPORARILY = 302,
  HTTP_STATUS_SEE_OTHER = 303,
  HTTP_STATUS_NOT_MODIFIED = 304,
  HTTP_STATUS_USE_PROXY = 305,
  HTTP_STATUS_TEMPORARY_REDIRECT = 307,

  HTTP_STATUS_BAD_REQUEST = 400,
  HTTP_STATUS_UNAUTHORIZED = 401,
  HTTP_STATUS_PAYMENT_REQUIRED = 402,
  HTTP_STATUS_FORBIDDEN = 403,
  HTTP_STATUS_NOT_FOUND = 404,
  HTTP_STATUS_METHOD_NOT_ALLOWED = 405,
  HTTP_STATUS_NOT_ACCEPTABLE = 406,
  HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
  HTTP_STATUS_REQUEST_TIMEOUT = 408,
  HTTP_STATUS_CONFLICT = 409,
  HTTP_STATUS_GONE = 410,
  HTTP_STATUS_LENGTH_REQUIRED = 411,
  HTTP_STATUS_PRECONDITION_FAILED = 412,
  HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE = 413,
  HTTP_STATUS_REQUEST_URI_TOO_LONG = 414,
  HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
  HTTP_STATUS_RANGE_NOT_SATISFIABLE = 416,

  HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
  HTTP_STATUS_NOT_IMPLEMENTED = 501,
  HTTP_STATUS_BAD_GATEWAY = 502,
  HTTP_STATUS_SERVICE_UNAVAILABLE = 503,
  HTTP_STATUS_GATEWAY_TIMEOUT = 504,
  HTTP_STATUS_HTTPVER_NOT_SUPPORTED = 505
};

enum HTTPKeepAlive
{
  HTTP_KEEPALIVE_UNDEFINED = 0,
  HTTP_NO_KEEPALIVE,
  HTTP_KEEPALIVE,
  HTTP_PIPELINE
};

enum HTTPWarningCode
{
  HTTP_WARNING_CODE_NONE = 0,

  HTTP_WARNING_CODE_RESPONSE_STALE = 110,
  HTTP_WARNING_CODE_REVALIDATION_FAILED = 111,
  HTTP_WARNING_CODE_DISCONNECTED_OPERATION = 112,
  HTTP_WARNING_CODE_HERUISTIC_EXPIRATION = 113,
  HTTP_WARNING_CODE_TRANSFORMATION_APPLIED = 114,
  HTTP_WARNING_CODE_MISC_WARNING = 199
};

/* squild log codes */
enum SquidLogCode
{
  SQUID_LOG_EMPTY = '0',
  SQUID_LOG_TCP_HIT = '1',
  SQUID_LOG_TCP_DISK_HIT = '2',
  SQUID_LOG_TCP_MEM_HIT = '.',  // Don't want to change others codes
  SQUID_LOG_TCP_MISS = '3',
  SQUID_LOG_TCP_EXPIRED_MISS = '4',
  SQUID_LOG_TCP_REFRESH_HIT = '5',
  SQUID_LOG_TCP_REF_FAIL_HIT = '6',
  SQUID_LOG_TCP_REFRESH_MISS = '7',
  SQUID_LOG_TCP_CLIENT_REFRESH = '8',
  SQUID_LOG_TCP_IMS_HIT = '9',
  SQUID_LOG_TCP_IMS_MISS = 'a',
  SQUID_LOG_TCP_SWAPFAIL = 'b',
  SQUID_LOG_TCP_DENIED = 'c',
  SQUID_LOG_TCP_WEBFETCH_MISS = 'd',
  SQUID_LOG_TCP_SPIDER_BYPASS = 'e',
  SQUID_LOG_TCP_FUTURE_2 = 'f',
  SQUID_LOG_TCP_HIT_REDIRECT = '[',       // standard redirect
  SQUID_LOG_TCP_MISS_REDIRECT = ']',      // standard redirect
  SQUID_LOG_TCP_HIT_X_REDIRECT = '<',     // extended redirect
  SQUID_LOG_TCP_MISS_X_REDIRECT = '>',    // extended redirect
  SQUID_LOG_UDP_HIT = 'g',
  SQUID_LOG_UDP_WEAK_HIT = 'h',
  SQUID_LOG_UDP_HIT_OBJ = 'i',
  SQUID_LOG_UDP_MISS = 'j',
  SQUID_LOG_UDP_DENIED = 'k',
  SQUID_LOG_UDP_INVALID = 'l',
  SQUID_LOG_UDP_RELOADING = 'm',
  SQUID_LOG_UDP_FUTURE_1 = 'n',
  SQUID_LOG_UDP_FUTURE_2 = 'o',
  SQUID_LOG_ERR_READ_TIMEOUT = 'p',
  SQUID_LOG_ERR_LIFETIME_EXP = 'q',
  SQUID_LOG_ERR_NO_CLIENTS_BIG_OBJ = 'r',
  SQUID_LOG_ERR_READ_ERROR = 's',
  SQUID_LOG_ERR_CLIENT_ABORT = 't',
  SQUID_LOG_ERR_CONNECT_FAIL = 'u',
  SQUID_LOG_ERR_INVALID_REQ = 'v',
  SQUID_LOG_ERR_UNSUP_REQ = 'w',
  SQUID_LOG_ERR_INVALID_URL = 'x',
  SQUID_LOG_ERR_NO_FDS = 'y',
  SQUID_LOG_ERR_DNS_FAIL = 'z',
  SQUID_LOG_ERR_NOT_IMPLEMENTED = 'A',
  SQUID_LOG_ERR_CANNOT_FETCH = 'B',
  SQUID_LOG_ERR_NO_RELAY = 'C',
  SQUID_LOG_ERR_DISK_IO = 'D',
  SQUID_LOG_ERR_ZERO_SIZE_OBJECT = 'E',
  SQUID_LOG_ERR_PROXY_DENIED = 'G',
  SQUID_LOG_ERR_WEBFETCH_DETECTED = 'H',
  SQUID_LOG_ERR_FUTURE_1 = 'I',
  SQUID_LOG_ERR_SPIDER_MEMBER_ABORTED = 'J',
  SQUID_LOG_ERR_SPIDER_PARENTAL_CONTROL_RESTRICTION = 'K',
  SQUID_LOG_ERR_SPIDER_UNSUPPORTED_HTTP_VERSION = 'L',
  SQUID_LOG_ERR_SPIDER_UIF = 'M',
  SQUID_LOG_ERR_SPIDER_FUTURE_USE_1 = 'N',
  SQUID_LOG_ERR_SPIDER_TIMEOUT_WHILE_PASSING = 'O',
  SQUID_LOG_ERR_SPIDER_TIMEOUT_WHILE_DRAINING = 'P',
  SQUID_LOG_ERR_SPIDER_GENERAL_TIMEOUT = 'Q',
  SQUID_LOG_ERR_SPIDER_CONNECT_FAILED = 'R',
  SQUID_LOG_ERR_SPIDER_FUTURE_USE_2 = 'S',
  SQUID_LOG_ERR_SPIDER_NO_RESOURCES = 'T',
  SQUID_LOG_ERR_SPIDER_INTERNAL_ERROR = 'U',
  SQUID_LOG_ERR_SPIDER_INTERNAL_IO_ERROR = 'V',
  SQUID_LOG_ERR_SPIDER_DNS_TEMP_ERROR = 'W',
  SQUID_LOG_ERR_SPIDER_DNS_HOST_NOT_FOUND = 'X',
  SQUID_LOG_ERR_SPIDER_DNS_NO_ADDRESS = 'Y',
  SQUID_LOG_ERR_UNKNOWN = 'Z'
};

/* squid hieratchy codes */
enum SquidHierarchyCode
{
  SQUID_HIER_EMPTY = '0',
  SQUID_HIER_NONE = '1',
  SQUID_HIER_DIRECT = '2',
  SQUID_HIER_SIBLING_HIT = '3',
  SQUID_HIER_PARENT_HIT = '4',
  SQUID_HIER_DEFAULT_PARENT = '5',
  SQUID_HIER_SINGLE_PARENT = '6',
  SQUID_HIER_FIRST_UP_PARENT = '7',
  SQUID_HIER_NO_PARENT_DIRECT = '8',
  SQUID_HIER_FIRST_PARENT_MISS = '9',
  SQUID_HIER_LOCAL_IP_DIRECT = 'a',
  SQUID_HIER_FIREWALL_IP_DIRECT = 'b',
  SQUID_HIER_NO_DIRECT_FAIL = 'c',
  SQUID_HIER_SOURCE_FASTEST = 'd',
  SQUID_HIER_SIBLING_UDP_HIT_OBJ = 'e',
  SQUID_HIER_PARENT_UDP_HIT_OBJ = 'f',
  SQUID_HIER_PASSTHROUGH_PARENT = 'g',
  SQUID_HIER_SSL_PARENT_MISS = 'h',
  SQUID_HIER_INVALID_CODE = 'i',
  SQUID_HIER_TIMEOUT_DIRECT = 'j',
  SQUID_HIER_TIMEOUT_SIBLING_HIT = 'k',
  SQUID_HIER_TIMEOUT_PARENT_HIT = 'l',
  SQUID_HIER_TIMEOUT_DEFAULT_PARENT = 'm',
  SQUID_HIER_TIMEOUT_SINGLE_PARENT = 'n',
  SQUID_HIER_TIMEOUT_FIRST_UP_PARENT = 'o',
  SQUID_HIER_TIMEOUT_NO_PARENT_DIRECT = 'p',
  SQUID_HIER_TIMEOUT_FIRST_PARENT_MISS = 'q',
  SQUID_HIER_TIMEOUT_LOCAL_IP_DIRECT = 'r',
  SQUID_HIER_TIMEOUT_FIREWALL_IP_DIRECT = 's',
  SQUID_HIER_TIMEOUT_NO_DIRECT_FAIL = 't',
  SQUID_HIER_TIMEOUT_SOURCE_FASTEST = 'u',
  SQUID_HIER_TIMEOUT_SIBLING_UDP_HIT_OBJ = 'v',
  SQUID_HIER_TIMEOUT_PARENT_UDP_HIT_OBJ = 'w',
  SQUID_HIER_TIMEOUT_PASSTHROUGH_PARENT = 'x',
  SQUID_HIER_TIMEOUT_TIMEOUT_SSL_PARENT_MISS = 'y',
  SQUID_HIER_INVALID_ASSIGNED_CODE = 'z'
};

/* squid hit/miss codes */
enum SquidHitMissCode
{
  SQUID_HIT_RESERVED = '0',
  SQUID_HIT_LEVEL_1 = '1',
  SQUID_HIT_LEVEL_2 = '2',
  SQUID_HIT_LEVEL_3 = '3',
  SQUID_HIT_LEVEL_4 = '4',
  SQUID_HIT_LEVEL_5 = '5',
  SQUID_HIT_LEVEL_6 = '6',
  SQUID_HIT_LEVEL_7 = '7',
  SQUID_HIT_LEVEL_8 = '8',
  SQUID_HIT_LEVEl_9 = '9',
  SQUID_MISS_NONE = '1',
  SQUID_MISS_ICP_AUTH = '2',
  SQUID_MISS_HTTP_NON_CACHE = '3',
  SQUID_MISS_ICP_STOPLIST = '4',
  SQUID_MISS_HTTP_NO_DLE = '5',
  SQUID_MISS_HTTP_NO_LE = '6',
  SQUID_MISS_HTTP_CONTENT = '7',
  SQUID_MISS_PRAGMA_NOCACHE = '8',
  SQUID_MISS_PASS = '9',
  SQUID_MISS_PRE_EXPIRED = 'a',
  SQUID_MISS_ERROR = 'b',
  SQUID_MISS_CACHE_BYPASS = 'c',
  SQUID_HIT_MISS_INVALID_ASSIGNED_CODE = 'z'
};


enum HTTPType
{
  HTTP_TYPE_UNKNOWN,
  HTTP_TYPE_REQUEST,
  HTTP_TYPE_RESPONSE
};

struct HTTPHdrImpl:public HdrHeapObjImpl
{
  // HdrHeapObjImpl is 4 bytes
  HTTPType m_polarity;          // request or response or unknown
  int32_t m_version;              // cooked version number
  // 12 bytes means 4 bytes padding here on 64-bit architectures
  union
  {
    struct
    {
      URLImpl *m_url_impl;
      const char *m_ptr_method;
      uint16_t m_len_method;
      int16_t m_method_wks_idx;
    } req;

    struct
    {
      const char *m_ptr_reason;
      uint16_t m_len_reason;
      int16_t m_status;
    } resp;
  } u;

  MIMEHdrImpl *m_fields_impl;

  // Marshaling Functions
  int marshal(MarshalXlate *ptr_xlate, int num_ptr, MarshalXlate *str_xlate, int num_str);
  void unmarshal(intptr_t offset);
  void move_strings(HdrStrHeap *new_heap);

  // Sanity Check Functions
  void check_strings(HeapCheck *heaps, int num_heaps);
};

struct HTTPValAccept
{
  char *type;
  char *subtype;
  double qvalue;
};


struct HTTPValAcceptCharset
{
  char *charset;
  double qvalue;
};


struct HTTPValAcceptEncoding
{
  char *encoding;
  double qvalue;
};


struct HTTPValAcceptLanguage
{
  char *language;
  double qvalue;
};


struct HTTPValFieldList
{
  char *name;
  HTTPValFieldList *next;
};


struct HTTPValCacheControl
{
  const char *directive;

  union
  {
    int delta_seconds;
    HTTPValFieldList *field_names;
  } u;
};


struct HTTPValRange
{
  int start;
  int end;
  HTTPValRange *next;
};


struct HTTPValTE
{
  char *encoding;
  double qvalue;
};


struct HTTPParser
{
  bool m_parsing_http;
  MIMEParser m_mime_parser;
};


extern const char *HTTP_METHOD_CONNECT;
extern const char *HTTP_METHOD_DELETE;
extern const char *HTTP_METHOD_GET;
extern const char *HTTP_METHOD_HEAD;
extern const char *HTTP_METHOD_ICP_QUERY;
extern const char *HTTP_METHOD_OPTIONS;
extern const char *HTTP_METHOD_POST;
extern const char *HTTP_METHOD_PURGE;
extern const char *HTTP_METHOD_PUT;
extern const char *HTTP_METHOD_TRACE;
extern const char *HTTP_METHOD_PUSH;

extern int HTTP_WKSIDX_CONNECT;
extern int HTTP_WKSIDX_DELETE;
extern int HTTP_WKSIDX_GET;
extern int HTTP_WKSIDX_HEAD;
extern int HTTP_WKSIDX_ICP_QUERY;
extern int HTTP_WKSIDX_OPTIONS;
extern int HTTP_WKSIDX_POST;
extern int HTTP_WKSIDX_PURGE;
extern int HTTP_WKSIDX_PUT;
extern int HTTP_WKSIDX_TRACE;
extern int HTTP_WKSIDX_PUSH;
extern int HTTP_WKSIDX_METHODS_CNT;


extern int HTTP_LEN_CONNECT;
extern int HTTP_LEN_DELETE;
extern int HTTP_LEN_GET;
extern int HTTP_LEN_HEAD;
extern int HTTP_LEN_ICP_QUERY;
extern int HTTP_LEN_OPTIONS;
extern int HTTP_LEN_POST;
extern int HTTP_LEN_PURGE;
extern int HTTP_LEN_PUT;
extern int HTTP_LEN_TRACE;
extern int HTTP_LEN_PUSH;

extern const char *HTTP_VALUE_BYTES;
extern const char *HTTP_VALUE_CHUNKED;
extern const char *HTTP_VALUE_CLOSE;
extern const char *HTTP_VALUE_COMPRESS;
extern const char *HTTP_VALUE_DEFLATE;
extern const char *HTTP_VALUE_GZIP;
extern const char *HTTP_VALUE_IDENTITY;
extern const char *HTTP_VALUE_KEEP_ALIVE;
extern const char *HTTP_VALUE_MAX_AGE;
extern const char *HTTP_VALUE_MAX_STALE;
extern const char *HTTP_VALUE_MIN_FRESH;
extern const char *HTTP_VALUE_MUST_REVALIDATE;
extern const char *HTTP_VALUE_NONE;
extern const char *HTTP_VALUE_NO_CACHE;
extern const char *HTTP_VALUE_NO_STORE;
extern const char *HTTP_VALUE_NO_TRANSFORM;
extern const char *HTTP_VALUE_ONLY_IF_CACHED;
extern const char *HTTP_VALUE_PRIVATE;
extern const char *HTTP_VALUE_PROXY_REVALIDATE;
extern const char *HTTP_VALUE_PUBLIC;
extern const char *HTTP_VALUE_S_MAXAGE;
extern const char *HTTP_VALUE_NEED_REVALIDATE_ONCE;

extern int HTTP_LEN_BYTES;
extern int HTTP_LEN_CHUNKED;
extern int HTTP_LEN_CLOSE;
extern int HTTP_LEN_COMPRESS;
extern int HTTP_LEN_DEFLATE;
extern int HTTP_LEN_GZIP;
extern int HTTP_LEN_IDENTITY;
extern int HTTP_LEN_KEEP_ALIVE;
extern int HTTP_LEN_MAX_AGE;
extern int HTTP_LEN_MAX_STALE;
extern int HTTP_LEN_MIN_FRESH;
extern int HTTP_LEN_MUST_REVALIDATE;
extern int HTTP_LEN_NONE;
extern int HTTP_LEN_NO_CACHE;
extern int HTTP_LEN_NO_STORE;
extern int HTTP_LEN_NO_TRANSFORM;
extern int HTTP_LEN_ONLY_IF_CACHED;
extern int HTTP_LEN_PRIVATE;
extern int HTTP_LEN_PROXY_REVALIDATE;
extern int HTTP_LEN_PUBLIC;
extern int HTTP_LEN_S_MAXAGE;
extern int HTTP_LEN_NEED_REVALIDATE_ONCE;

/* Private */
void http_hdr_adjust(HTTPHdrImpl *hdrp, int32_t offset, int32_t length, int32_t delta);

/* Public */
void http_init();

inkcoreapi HTTPHdrImpl *http_hdr_create(HdrHeap *heap, HTTPType polarity);
void http_hdr_init(HdrHeap *heap, HTTPHdrImpl *hh, HTTPType polarity);
HTTPHdrImpl *http_hdr_clone(HTTPHdrImpl *s_hh, HdrHeap *s_heap, HdrHeap *d_heap);
void http_hdr_copy_onto(HTTPHdrImpl *s_hh, HdrHeap *s_heap, HTTPHdrImpl *d_hh, HdrHeap *d_heap, bool inherit_strs);

inkcoreapi int http_hdr_print(HdrHeap *heap, HTTPHdrImpl *hh, char *buf, int bufsize, int *bufindex, int *dumpoffset);

void http_hdr_describe(HdrHeapObjImpl *obj, bool recurse = true);

int http_hdr_length_get(HTTPHdrImpl *hh);
// HTTPType               http_hdr_type_get (HTTPHdrImpl *hh);

// int32_t                  http_hdr_version_get (HTTPHdrImpl *hh);
inkcoreapi void http_hdr_version_set(HTTPHdrImpl *hh, int32_t ver);

const char *http_hdr_method_get(HTTPHdrImpl *hh, int *length);
inkcoreapi void http_hdr_method_set(HdrHeap *heap, HTTPHdrImpl *hh,
                                    const char *method, int16_t method_wks_idx, int method_length, bool must_copy);

void http_hdr_url_set(HdrHeap *heap, HTTPHdrImpl *hh, URLImpl *url);

// HTTPStatus             http_hdr_status_get (HTTPHdrImpl *hh);
void http_hdr_status_set(HTTPHdrImpl *hh, HTTPStatus status);
const char *http_hdr_reason_get(HTTPHdrImpl *hh, int *length);
void http_hdr_reason_set(HdrHeap *heap, HTTPHdrImpl *hh, const char *value, int length, bool must_copy);
const char *http_hdr_reason_lookup(unsigned status);

void http_parser_init(HTTPParser *parser);
void http_parser_clear(HTTPParser *parser);
MIMEParseResult http_parser_parse_req(HTTPParser *parser, HdrHeap *heap,
                                      HTTPHdrImpl *hh, const char **start,
                                      const char *end, bool must_copy_strings, bool eof);
MIMEParseResult http_parser_parse_resp(HTTPParser *parser, HdrHeap *heap,
                                       HTTPHdrImpl *hh, const char **start,
                                       const char *end, bool must_copy_strings, bool eof);
HTTPStatus http_parse_status(const char *start, const char *end);
int32_t http_parse_version(const char *start, const char *end);


/*
HTTPValAccept*         http_parse_accept (const char *buf, Arena *arena);
HTTPValAcceptCharset*  http_parse_accept_charset (const char *buf, Arena *arena);
HTTPValAcceptEncoding* http_parse_accept_encoding (const char *buf, Arena *arena);
HTTPValAcceptLanguage* http_parse_accept_language (const char *buf, Arena *arena);
HTTPValCacheControl*   http_parse_cache_control (const char *buf, Arena *arena);
const char*            http_parse_cache_directive (const char **buf);
HTTPValRange*          http_parse_range (const char *buf, Arena *arena);
*/
HTTPValTE *http_parse_te(const char *buf, int len, Arena *arena);


class HTTPVersion
{
public:
  HTTPVersion();
  HTTPVersion(int32_t version);
  HTTPVersion(int ver_major, int ver_minor);

  void set(HTTPVersion ver);
  void set(int ver_major, int ver_minor);

  HTTPVersion & operator =(const HTTPVersion & hv);
  int operator ==(const HTTPVersion & hv);
  int operator !=(const HTTPVersion & hv);
  int operator >(const HTTPVersion & hv);
  int operator <(const HTTPVersion & hv);
  int operator >=(const HTTPVersion & hv);
  int operator <=(const HTTPVersion & hv);

public:
    int32_t m_version;
};

class IOBufferReader;

class HTTPHdr: public MIMEHdr
{
public:
  HTTPHdrImpl *m_http;
  // This is all cached data and so is mutable.
  mutable URL m_url_cached;
  mutable MIMEField *m_host_mime;
  mutable int m_host_length; ///< Length of hostname.
  mutable int m_port; ///< Target port.
  mutable bool m_target_cached; ///< Whether host name and port are cached.
  mutable bool m_target_in_url; ///< Whether host name and port are in the URL.
  /// Set if the port was effectively specified in the header.
  /// @c true if the target (in the URL or the HOST field) also specified
  /// a port. That is, @c true if whatever source had the target host
  /// also had a port, @c false otherwise.
  mutable bool m_port_in_header;

  HTTPHdr();
  ~HTTPHdr();

  int valid() const;

  void create(HTTPType polarity, HdrHeap *heap = NULL);
  void clear();
  void reset();
  void copy(const HTTPHdr *hdr);
  void copy_shallow(const HTTPHdr *hdr);

  int unmarshal(char *buf, int len, RefCountObj *block_ref);

  int print(char *buf, int bufsize, int *bufindex, int *dumpoffset);

  int length_get();

  HTTPType type_get() const;

  HTTPVersion version_get();
  void version_set(HTTPVersion version);

  const char *method_get(int *length);
  int method_get_wksidx();
  void method_set(const char *value, int length);

  URL *url_create(URL *url);

  URL *url_get() const;
  URL *url_get(URL *url);
  /** Get a string with the effective URL in it.
      If @a length is not @c NULL then the length of the string
      is stored in the int pointed to by @a length.

      Note that this can be different from getting the @c URL
      and invoking @c URL::string_get if the host is in a header
      field and not explicitly in the URL.
   */
  char* url_string_get(
    Arena* arena = 0, ///< Arena to use, or @c malloc if NULL.
    int* length = 0 ///< Store string length here.
  );

  void url_set(URL *url);
  void url_set_as_server_url(URL *url);
  void url_set(const char *str, int length);

  /** Get the target host name.
      The length is returned in @a length if non-NULL.
      @note The results are cached so this is fast after the first call.
      @return A pointer to the host name.
  */
  char const* host_get(int* length = 0);

  /** Get the target port.
      If the target port is not found then it is adjusted to the
      default port for the URL type.
      @note The results are cached so this is fast after the first call.
      @return The canonicalized target port.
  */
  int port_get();

  /// Check location of target host.
  /// @return @c true if the host was in the URL, @c false otherwise.
  /// @note This returns @c false if the host is missing.
  bool is_target_in_url() const;

  /// Check if a port was specified in the target.
  /// @return @c true if the port was part of the target.
  bool is_port_in_header() const;

  /// If the target is in the fields and not the URL, copy it to the @a url.
  /// If @a url is @c NULL the cached URL in this header is used.
  /// @note In the default case the copy is avoided if the cached URL already
  /// has the target. If @a url is non @c NULL the copy is always performed.
  void set_url_target_from_host_field(URL* url = 0);

  /// Mark the target cache as invalid.
  /// @internal Ugly but too many places currently that touch the
  /// header internals, they must be able to do this.
  void mark_target_dirty() const;

  HTTPStatus status_get();
  void status_set(HTTPStatus status);

  const char *reason_get(int *length);
  void reason_set(const char *value, int length);

  MIMEParseResult parse_req(HTTPParser *parser, const char **start, const char *end, bool eof);
  MIMEParseResult parse_resp(HTTPParser *parser, const char **start, const char *end, bool eof);

  MIMEParseResult parse_req(HTTPParser *parser, IOBufferReader *r, int *bytes_used, bool eof);
  MIMEParseResult parse_resp(HTTPParser *parser, IOBufferReader *r, int *bytes_used, bool eof);

public:
  // Utility routines
  bool is_cache_control_set(const char *cc_directive_wks);
  bool is_pragma_no_cache_set();

protected:
  /** Load the target cache.
      @see m_host, m_port, m_target_in_url
  */
  void _fill_target_cache() const;
  /** Test the cache and fill it if necessary.
      @internal In contrast to @c _fill_target_cache, this method
      is inline and checks whether the cache is already filled.
      @ _fill_target_cache @b always does a cache fill.
  */
  void _test_and_fill_target_cache() const;

private:
  // No gratuitous copies!
  HTTPHdr(const HTTPHdr & m);
  HTTPHdr & operator =(const HTTPHdr & m);
};


/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPVersion::HTTPVersion()
:m_version(HTTP_VERSION(0, 9))
{
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPVersion::HTTPVersion(int32_t version)
:m_version(version)
{
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPVersion::HTTPVersion(int ver_major, int ver_minor)
  :
m_version(HTTP_VERSION(ver_major, ver_minor))
{
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPVersion::set(HTTPVersion ver)
{
  m_version = ver.m_version;
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPVersion::set(int ver_major, int ver_minor)
{
  m_version = HTTP_VERSION(ver_major, ver_minor);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPVersion &
HTTPVersion::operator =(const HTTPVersion & hv)
{
  m_version = hv.m_version;

  return *this;
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPVersion::operator ==(const HTTPVersion & hv)
{
  return (m_version == hv.m_version);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPVersion::operator !=(const HTTPVersion & hv)
{
  return (m_version != hv.m_version);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPVersion::operator >(const HTTPVersion & hv)
{
  return (m_version > hv.m_version);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPVersion::operator <(const HTTPVersion & hv)
{
  return (m_version < hv.m_version);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPVersion::operator >=(const HTTPVersion & hv)
{
  return (m_version >= hv.m_version);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPVersion::operator <=(const HTTPVersion & hv)
{
  return (m_version <= hv.m_version);
}


/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline
HTTPHdr::HTTPHdr()
  : MIMEHdr(), m_http(NULL), m_url_cached(), m_target_cached(false)
{ }


/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/
inline
HTTPHdr::~HTTPHdr()
{                               /* nop */
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPHdr::valid() const
{
  return (m_http && m_mime && m_heap);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::create(HTTPType polarity, HdrHeap *heap)
{
  if (heap) {
    m_heap = heap;
  } else if (!m_heap) {
    m_heap = new_HdrHeap();
  }

  m_http = http_hdr_create(m_heap, polarity);
  m_mime = m_http->m_fields_impl;
}

inline void
HTTPHdr::clear()
{

  if (m_http && m_http->m_polarity == HTTP_TYPE_REQUEST) {
    m_url_cached.clear();
  }
  this->HdrHeapSDKHandle::clear();
  m_http = NULL;
  m_mime = NULL;
}

inline void
HTTPHdr::reset()
{
  m_heap = NULL;
  m_http = NULL;
  m_mime = NULL;
  m_url_cached.reset();
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::copy(const HTTPHdr *hdr)
{
  ink_debug_assert(hdr->valid());

  if (valid()) {
    http_hdr_copy_onto(hdr->m_http, hdr->m_heap, m_http, m_heap, (m_heap != hdr->m_heap) ? true : false);
  } else {
    m_heap = new_HdrHeap();
    m_http = http_hdr_clone(hdr->m_http, hdr->m_heap, m_heap);
    m_mime = m_http->m_fields_impl;
  }
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::copy_shallow(const HTTPHdr *hdr)
{
  ink_debug_assert(hdr->valid());

  m_heap = hdr->m_heap;
  m_http = hdr->m_http;
  m_mime = hdr->m_mime;

  if (hdr->type_get() == HTTP_TYPE_REQUEST && m_url_cached.valid())
    m_url_cached.copy_shallow(&hdr->m_url_cached);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPHdr::print(char *buf, int bufsize, int *bufindex, int *dumpoffset)
{
  ink_debug_assert(valid());
  return http_hdr_print(m_heap, m_http, buf, bufsize, bufindex, dumpoffset);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPHdr::length_get()
{
  ink_debug_assert(valid());
  return http_hdr_length_get(m_http);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::_test_and_fill_target_cache() const {
  if (!m_target_cached) this->_fill_target_cache();
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline char const*
HTTPHdr::host_get(int* length)
{
  this->_test_and_fill_target_cache();
  if (m_target_in_url) {
    return url_get()->host_get(length);
  } else if (m_host_mime) {
    if (length) *length = m_host_length;
    return m_host_mime->m_ptr_value;
  }

  if (length) *length = 0;
  return NULL;
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int
HTTPHdr::port_get()
{
  this->_test_and_fill_target_cache();
  return m_port;
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline bool
HTTPHdr::is_target_in_url() const
{
  this->_test_and_fill_target_cache();
  return m_target_in_url;
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline bool
HTTPHdr::is_port_in_header() const
{
  this->_test_and_fill_target_cache();
  return m_port_in_header;
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::mark_target_dirty() const
{
  m_target_cached = false;
}
/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPType
http_hdr_type_get(HTTPHdrImpl *hh)
{
  return (hh->m_polarity);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPType
HTTPHdr::type_get() const
{
  ink_debug_assert(valid());
  return http_hdr_type_get(m_http);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline int32_t
http_hdr_version_get(HTTPHdrImpl *hh)
{
  return (hh->m_version);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPVersion
HTTPHdr::version_get()
{
  ink_debug_assert(valid());
  return HTTPVersion(http_hdr_version_get(m_http));
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::version_set(HTTPVersion version)
{
  ink_debug_assert(valid());
  http_hdr_version_set(m_http, version.m_version);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline const char *
HTTPHdr::method_get(int *length)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  return http_hdr_method_get(m_http, length);
}


inline int
HTTPHdr::method_get_wksidx()
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  return (m_http->u.req.m_method_wks_idx);
}


/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::method_set(const char *value, int length)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  int method_wks_idx = hdrtoken_tokenize(value, length);
  http_hdr_method_set(m_heap, m_http, value, method_wks_idx, length, true);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline URL *
HTTPHdr::url_create(URL *u)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  u->set(this);
  u->create(m_heap);
  return (u);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline URL *
HTTPHdr::url_get() const
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  // It's entirely possible that someone changed URL in our impl
  // without updating the cached copy in the C++ layer.  Check
  // to see if this happened before handing back the url

  URLImpl *real_impl = m_http->u.req.m_url_impl;
  if (m_url_cached.m_url_impl != real_impl) {
    m_url_cached.set(this);
    m_url_cached.m_url_impl = real_impl;
    this->mark_target_dirty();
  }
  return (&m_url_cached);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline URL *
HTTPHdr::url_get(URL *url)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  url->set(this);               // attach refcount
  url->m_url_impl = m_http->u.req.m_url_impl;
  return (url);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::url_set(URL *url)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  URLImpl *url_impl = m_http->u.req.m_url_impl;
  ::url_copy_onto(url->m_url_impl, url->m_heap, url_impl, m_heap, true);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::url_set_as_server_url(URL *url)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  URLImpl *url_impl = m_http->u.req.m_url_impl;
  ::url_copy_onto_as_server_url(url->m_url_impl, url->m_heap, url_impl, m_heap, true);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::url_set(const char *str, int length)
{
  URLImpl *url_impl;

  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  url_impl = m_http->u.req.m_url_impl;
  ::url_clear(url_impl);
  ::url_parse(m_heap, url_impl, &str, str + length, true);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPStatus
http_hdr_status_get(HTTPHdrImpl *hh)
{
  ink_debug_assert(hh->m_polarity == HTTP_TYPE_RESPONSE);
  return (HTTPStatus) hh->u.resp.m_status;
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline HTTPStatus
HTTPHdr::status_get()
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_RESPONSE);

  return (NULL == m_http) ? HTTP_STATUS_NONE : http_hdr_status_get(m_http);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::status_set(HTTPStatus status)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_RESPONSE);

  http_hdr_status_set(m_http, status);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline const char *
HTTPHdr::reason_get(int *length)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_RESPONSE);

  return http_hdr_reason_get(m_http, length);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline void
HTTPHdr::reason_set(const char *value, int length)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_RESPONSE);

  http_hdr_reason_set(m_heap, m_http, value, length, true);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline MIMEParseResult
HTTPHdr::parse_req(HTTPParser *parser, const char **start, const char *end, bool eof)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_REQUEST);

  return http_parser_parse_req(parser, m_heap, m_http, start, end, true, eof);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline MIMEParseResult
HTTPHdr::parse_resp(HTTPParser *parser, const char **start, const char *end, bool eof)
{
  ink_debug_assert(valid());
  ink_debug_assert(m_http->m_polarity == HTTP_TYPE_RESPONSE);

  return http_parser_parse_resp(parser, m_heap, m_http, start, end, true, eof);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline bool
HTTPHdr::is_cache_control_set(const char *cc_directive_wks)
{
  ink_debug_assert(valid());
  ink_debug_assert(hdrtoken_is_wks(cc_directive_wks));

  HdrTokenHeapPrefix *prefix = hdrtoken_wks_to_prefix(cc_directive_wks);
  ink_debug_assert(prefix->wks_token_type == HDRTOKEN_TYPE_CACHE_CONTROL);

  uint32_t cc_mask = prefix->wks_type_specific.u.cache_control.cc_mask;
  if (get_cooked_cc_mask() & cc_mask)
    return (true);
  else
    return (false);
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

inline bool
HTTPHdr::is_pragma_no_cache_set()
{
  ink_debug_assert(valid());
  return (get_cooked_pragma_no_cache());
}

/*-------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

enum
{
  CACHE_ALT_MAGIC_ALIVE = 0xabcddeed,
  CACHE_ALT_MAGIC_MARSHALED = 0xdcbadeed,
  CACHE_ALT_MAGIC_DEAD = 0xdeadeed
};

// struct HTTPCacheAlt
struct HTTPCacheAlt
{
  HTTPCacheAlt();
  void copy(HTTPCacheAlt *to_copy);
  void destroy();

  uint32_t m_magic;

  // Writeable is set to true is we reside
  //  in a buffer owned by this structure.
  // INVARIENT: if own the buffer this HttpCacheAlt
  //   we also own the buffers for the request &
  //   response headers
  int32_t m_writeable;
  int32_t m_unmarshal_len;

  int32_t m_id;
  int32_t m_rid;

  int32_t m_object_key[4];
  int32_t m_object_size[2];

  HTTPHdr m_request_hdr;
  HTTPHdr m_response_hdr;

  time_t m_request_sent_time;
  time_t m_response_received_time;

  // With clustering, our alt may be in cluster
  //  incoming channel buffer, when we are
  //  destroyed we decrement the refcount
  //  on that buffer so that it gets destroyed
  // We don't want to use a ref count ptr (Ptr<>)
  //  since our ownership model requires explict
  //  destroys and ref count pointers defeat this
  RefCountObj *m_ext_buffer;
};

class HTTPInfo
{
public:
  HTTPCacheAlt *m_alt;

  HTTPInfo()
    : m_alt(NULL)
  { }

  ~HTTPInfo()
  {
    clear();
  }

  void clear() { m_alt = NULL; }
  bool valid() const { return (m_alt != NULL); }

  void create();
  void destroy();

  void copy(HTTPInfo *to_copy);
  void copy_shallow(HTTPInfo *info) { m_alt = info->m_alt; }
  HTTPInfo & operator =(const HTTPInfo & m);

  inkcoreapi int marshal_length();
  inkcoreapi int marshal(char *buf, int len);
  static int unmarshal(char *buf, int len, RefCountObj *block_ref);
  void set_buffer_reference(RefCountObj *block_ref);
  int get_handle(char *buf, int len);

  int32_t id_get() const { return m_alt->m_id; }
  int32_t rid_get() { return m_alt->m_rid; }

  void id_set(int32_t id) { m_alt->m_id = id; }
  void rid_set(int32_t id) { m_alt->m_rid = id; }

  INK_MD5 object_key_get();
  void object_key_get(INK_MD5 *);
  bool compare_object_key(const INK_MD5 *);
  int64_t object_size_get();

  void request_get(HTTPHdr *hdr) { hdr->copy_shallow(&m_alt->m_request_hdr); }
  void response_get(HTTPHdr *hdr) { hdr->copy_shallow(&m_alt->m_response_hdr); }

  HTTPHdr *request_get() { return &m_alt->m_request_hdr; }
  HTTPHdr *response_get() { return &m_alt->m_response_hdr; }

  URL *request_url_get(URL *url = NULL) { return m_alt->m_request_hdr.url_get(url); }

  time_t request_sent_time_get() { return m_alt->m_request_sent_time; }
  time_t response_received_time_get() { return m_alt->m_response_received_time; }

  void object_key_set(INK_MD5 & md5);
  void object_size_set(int64_t size);

  void request_set(const HTTPHdr *req) { m_alt->m_request_hdr.copy(req); }
  void response_set(const HTTPHdr *resp) { m_alt->m_response_hdr.copy(resp); }

  void request_sent_time_set(time_t t) { m_alt->m_request_sent_time = t; }
  void response_received_time_set(time_t t) { m_alt->m_response_received_time = t; }

  // Sanity check functions
  static bool check_marshalled(char *buf, int len);

private:
  HTTPInfo(const HTTPInfo & h);
};

inline void
HTTPInfo::destroy()
{
  if (m_alt) {
    if (m_alt->m_writeable) {
      m_alt->destroy();
    } else if (m_alt->m_ext_buffer) {
      if (m_alt->m_ext_buffer->refcount_dec() == 0) {
        m_alt->m_ext_buffer->free();
      }
    }
  }
  clear();
}

inline HTTPInfo &
HTTPInfo::operator =(const HTTPInfo & m)
{
  m_alt = m.m_alt;
  return *this;
}

inline INK_MD5
HTTPInfo::object_key_get()
{
  INK_MD5 val;
  int32_t* pi = reinterpret_cast<int32_t*>(&val);

  pi[0] = m_alt->m_object_key[0];
  pi[1] = m_alt->m_object_key[1];
  pi[2] = m_alt->m_object_key[2];
  pi[3] = m_alt->m_object_key[3];

  return val;
}

inline void
HTTPInfo::object_key_get(INK_MD5 *md5)
{
  int32_t* pi = reinterpret_cast<int32_t*>(md5);
  pi[0] = m_alt->m_object_key[0];
  pi[1] = m_alt->m_object_key[1];
  pi[2] = m_alt->m_object_key[2];
  pi[3] = m_alt->m_object_key[3];
}

inline bool
HTTPInfo::compare_object_key(const INK_MD5 *md5)
{
  int32_t const* pi = reinterpret_cast<int32_t const*>(md5);
  return ((m_alt->m_object_key[0] == pi[0]) &&
          (m_alt->m_object_key[1] == pi[1]) &&
          (m_alt->m_object_key[2] == pi[2]) &&
          (m_alt->m_object_key[3] == pi[3])
         );
}

inline int64_t
HTTPInfo::object_size_get()
{
  int64_t val;
  int32_t* pi = reinterpret_cast<int32_t*>(&val);

  pi[0] = m_alt->m_object_size[0];
  pi[1] = m_alt->m_object_size[1];
  return val;
}

inline void
HTTPInfo::object_key_set(INK_MD5 & md5)
{
  int32_t* pi = reinterpret_cast<int32_t*>(&md5);
  m_alt->m_object_key[0] = pi[0];
  m_alt->m_object_key[1] = pi[1];
  m_alt->m_object_key[2] = pi[2];
  m_alt->m_object_key[3] = pi[3];
}

inline void
HTTPInfo::object_size_set(int64_t size)
{
  int32_t* pi = reinterpret_cast<int32_t*>(&size);
  m_alt->m_object_size[0] = pi[0];
  m_alt->m_object_size[1] = pi[1];
}


#endif /* __HTTP_H__ */
