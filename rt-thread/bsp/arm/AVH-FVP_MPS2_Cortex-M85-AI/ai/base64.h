
#ifndef __BASE64_H__
#define __BASE64_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define BASE64_SUCCESS              0
#define BASE64_ERROR                -1
#define BASE64_ERR_BASE64_BAD_MSG   -2

int base64_encode(const uint8_t *in, uint16_t in_len, char *out);

int base64_encode_tail(const uint8_t *in, uint16_t in_len, char *out);

int base64_decode(const char *in, uint8_t *out, uint16_t *out_len);

#endif
