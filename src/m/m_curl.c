#include "m_curl.h"

#define PBLEN 40

static int progress_bar(curl_off_t curr, curl_off_t total)
{
  int done = (int)(((double)curr / (double)total) * PBLEN), i, j;
  int remain = PBLEN - done;
  char buffer[PBLEN + 3];

  strcpy(buffer, "");

  strcat(buffer, "[");

  for (i = 0; i < done; i++)
    strcat(buffer, "=");

  if (done < PBLEN)
    strcat(buffer, ">");

  for (j = 0; j < remain; j++)
    strcat(buffer, " ");

  strcat(buffer, "]");

  printf("\r%s ", buffer);
  fflush(stdout);
  return 0;
}

static size_t write_file(void *data, size_t size, size_t nmemb, struct download_data *dd)
{
  size_t written = fwrite(data, size, nmemb, dd->file);
  dd->currl += written;
  if (dd->totall == 0)
    curl_easy_getinfo(dd->curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &dd->totall);
  progress_bar(dd->currl, dd->totall);
  return written;
}

static size_t write_get_res(void *data, size_t size, size_t nmemb,
                            struct get_res *res)
{
  size_t new_len = res->len + size * nmemb;
  res->ptr = realloc(res->ptr, new_len + 1);

  if (res->ptr == NULL)
  {
    perror("Could not re-allocate memory\n");
    return 1;
  }

  memcpy(res->ptr + res->len, data, size * nmemb);
  res->ptr[new_len] = '\0';
  res->len = new_len;

  return size * nmemb;
}

static int init_curl_handler(CURL *curl_handle, const char *url)
{
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "mopm");
  return 0;
}

int send_http_get(CURL *curl_handle, const char *url, struct get_res *res)
{
  CURLcode perform_res;
  curl_off_t size;

  init_curl_handler(curl_handle, url);

  res->len = 0;
  res->ptr = malloc(res->len + 1);
  if (res->ptr == NULL)
  {
    perror("Could not allocate memory");
    return 1;
  }
  res->ptr[0] = '\0';

  curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_get_res);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, res);

  perform_res = curl_easy_perform(curl_handle);
  curl_easy_reset(curl_handle);
  return 0;
}

int download_to_file(CURL *curl_handle, char *url, char *file_dir)
{
  CURLcode perform_res;
  struct download_data dd;

  init_curl_handler(curl_handle, url);
  
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_file);

  dd.totall = 0;
  dd.currl = 0;
  dd.curl_handle = curl_handle;
  dd.file = fopen(file_dir, "wb");

  if (dd.file == NULL)
  {
    fclose(dd.file);
    return -1;
  }

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &dd);
  perform_res = curl_easy_perform(curl_handle);
  printf("\n");

  fclose(dd.file);
  curl_easy_reset(curl_handle);
  return perform_res;
}