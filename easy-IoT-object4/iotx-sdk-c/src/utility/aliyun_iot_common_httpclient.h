/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef ALIYUN_IOT_COMMON_HTTPCLIENT_H
#define ALIYUN_IOT_COMMON_HTTPCLIENT_H

#include "aliyun_iot_platform_datatype.h"
#include "aliyun_iot_common_error.h"
#include "aliyun_iot_common_net.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup HttpClient
 * @{
 * HttpClient API implements the client-side of HTTP/1.1. It provides base interfaces to execute an HTTP request on a given URL. It also supports HTTPS (HTTP over TLS) to provide secure communication.\n
 * @section HttpClient_Usage_Chapter How to use this module
 * In this release, MediaTek provides two types of APIs: high level APIs and low level APIs.\n
 * - \b The \b high \b level \b APIs
 *  - Enables to execute a single HTTP request on a given URL.
 *  - Call #httpclient_get(), #httpclient_post(), #httpclient_put() or #httpclient_delete() to get, post, put or delete and HTTP request.\n
 * - \b The \b low \b level \b APIs
 *  - Enables to execute more than one HTTP requests during a Keep-Alive connection. Keep-alive is the idea of using a single TCP connection to send and receive multiple HTTP requests/responses, as opposed to opening a new connection for every single request/response pair.
 *  - Step1: Call #httpclient_connect() to connect to a remote server.
 *  - Step2: Call #httpclient_send_request() to send an HTTP request to the server.
 *  - Step3: Call #httpclient_recv_response() to receive an HTTP response from the server.
 *  - Step4: Repeat Steps 2 and 3 to execute more requests.
 *  - Step5: Call #httpclient_close() to close the connection.
 *  - Sample code: Please refer to the example under <sdk_root>/project/mt7687_hdk/apps/http_client/http_client_keepalive folder.
 */

/** @defgroup httpclient_define Define
 * @{
 */
/** @brief   This macro defines the HTTP port.  */
#define HTTP_PORT   80

/** @brief   This macro defines the HTTPS port.  */
#define HTTPS_PORT 443
/**
 * @}
 */

/** @defgroup httpclient_enum Enum
 *  @{
 */
/** @brief   This enumeration defines the HTTP request type.  */
typedef enum
{
    HTTPCLIENT_GET,
    HTTPCLIENT_POST,
    HTTPCLIENT_PUT,
    HTTPCLIENT_DELETE,
    HTTPCLIENT_HEAD
} HTTPCLIENT_REQUEST_TYPE;


/** @defgroup httpclient_struct Struct
 * @{
 */
/** @brief   This structure defines the httpclient_t structure.  */
typedef struct
{
    //intptr_t handle; /**< handle of TCP connection or SSL connection. */
    int remote_port; /**< HTTP or HTTPS port. */                         //�˿ں�
    Network_t net;
    int response_code; /**< Response code. */                          //��Ӧ��
    char *header; /**< Custom header. */                              //�Զ���ͷ��
    char *auth_user; /**< Username for basic authentication. */       //���������֤���û�����
    char *auth_password; /**< Password for basic authentication. */  //���������֤���롣
} httpclient_t;

/** @brief   This structure defines the HTTP data structure.  */
typedef struct
{
    bool is_more; /**< Indicates if more data needs to be retrieved. */
    bool is_chunked; /**< Response data is encoded in portions/chunks.*/
    int retrieve_len; /**< Content length to be retrieved. */
    uint32_t response_content_len; /**< Response content length. */
    uint32_t post_buf_len; /**< Post data length. */
    uint32_t response_buf_len; /**< Response buffer length. */
    char *post_content_type; /**< Content type of the post data. */
    char *post_buf; /**< User data to be posted. */
    char *response_buf; /**< Buffer to store the response data. */
} httpclient_data_t;



/**
 * @brief            This function executes a POST request on a given URL. It blocks until completion.
 * @param[in]        client is a pointer to the #httpclient_t.
 * @param[in]        url is the URL to run the request.
 * @param[in]        port is #HTTP_PORT or #HTTPS_PORT.
 * @param[in, out]   client_data is a pointer to the #httpclient_data_t instance to collect the data returned by the request. It also contains the data to be posted.
 * @return           Please refer to #HTTPCLIENT_RESULT.
 * @par              HttpClient Post Example
 * @code
 *                   char *url = "https://api.mediatek.com/mcs/v2/devices/D0n2yhrl/datapoints.csv";
 *                   char *header = "deviceKey:FZoo0S07CpwUHcrt\r\n";
 *                   char *content_type = "text/csv";
 *                   char *post_data = "1,,I am string!";
 *                   httpclient_t client = {0};
 *                   httpclient_data_t client_data = {0};
 *                   char *buf = NULL;
 *                   buf = pvPortMalloc(BUF_SIZE);
 *                   if (buf == NULL) {
 *                       printf("Malloc failed.\r\n");
 *                       return;
 *                   }
 *                   memset(buf, 0, sizeof(buf));
 *                   client_data.response_buf = buf;  //Sets a buffer to store the result.
 *                   client_data.response_buf_len = BUF_SIZE;  //Sets the buffer size.
 *                   httpclient_set_custom_header(&client, header);  //Sets the custom header if needed.
 *                   client_data.post_buf = post_data;  //Sets the user data to be posted.
 *                   client_data.post_buf_len = strlen(post_data);  //Sets the post data length.
 *                   client_data.post_content_type = content_type;  //Sets the content type.
 *                   httpclient_post(&client, url, HTTPS_PORT, &client_data);
 *                   printf("Data received: %s\r\n", client_data.response_buf);
 * @endcode
 */
aliot_err_t aliyun_iot_common_post(
        httpclient_t *client,
        char *url,
        int port,
        char *ca_crt,
        httpclient_data_t *client_data);

#define HTTPCLIENT_MIN(x,y) (((x)<(y))?(x):(y))
#define HTTPCLIENT_MAX(x,y) (((x)>(y))?(x):(y))

#define HTTPCLIENT_AUTHB_SIZE     128

#define HTTPCLIENT_CHUNK_SIZE     512
#define HTTPCLIENT_SEND_BUF_SIZE  512

#define HTTPCLIENT_MAX_HOST_LEN   64
#define HTTPCLIENT_MAX_URL_LEN    512

#if defined(MBEDTLS_DEBUG_C)
#define DEBUG_LEVEL 2
#endif
aliot_err_t httpclient_recv_response_content(httpclient_t *client, httpclient_data_t *client_data);
int httpclient_send_header(httpclient_t *client, char *url, int method, httpclient_data_t *client_data);
int httpclient_parse_host(char *url, char *host, uint32_t maxhost_len);
int httpclient_parse_url(const char *url, char *scheme, uint32_t max_scheme_len, char *host, uint32_t maxhost_len, int *port, char *path, uint32_t max_path_len);
int httpclient_tcp_send_all(int sock_fd, char *data, int length);
int httpclient_conn(httpclient_t *client);
int httpclient_recv(httpclient_t *client, char *buf, int min_len, int max_len, int *p_read_len);
int httpclient_retrieve_content(httpclient_t *client, char *data, int len, httpclient_data_t *client_data);
int httpclient_response_parse(httpclient_t *client, char *data, int len, httpclient_data_t *client_data);
aliot_err_t httpclient_connect(httpclient_t *client);
void httpclient_close(httpclient_t *client);

#ifdef __cplusplus
}
#endif

#endif /* __HTTPCLIENT_H__ */

