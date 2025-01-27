/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 *
 * Modified by Recep Aslantas (github @recp)
 */

#include "base64.h"
#include <string.h>

static const unsigned char base64_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64_encode - Base64 encode
 * @src: Data to be encoded
 * @len: Length of the data to be encoded
 * @out_len: Pointer to output length variable, or %NULL if not used
 * Returns: Allocated buffer of out_len bytes of encoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer. Returned buffer is
 * nul terminated to make it easier to use as a C string. The nul terminator is
 * not included in out_len.
 */
_assetkit_hide
unsigned char*
base64_encode(AkHeap              * __restrict heap,
              void                * __restrict memparent,
              const unsigned char * __restrict src,
              size_t                           len,
              size_t              * __restrict out_len) {
	unsigned char       *out, *pos;
	const unsigned char *end, *in;
	size_t               olen;
	int                  line_len;

	olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	olen += olen / 72; /* line feeds */
	olen++; /* nul termination */
  
	if (olen < len)
		return NULL; /* integer overflow */
	out = ak_heap_alloc(heap, memparent, olen);
	if (out == NULL)
		return NULL;

	end = src + len;
	in = src;
	pos = out;
	line_len = 0;
	while (end - in >= 3) {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
		in += 3;
		line_len += 4;
		if (line_len >= 72) {
			*pos++ = '\n';
			line_len = 0;
		}
	}

	if (end - in) {
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		} else {
			*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
		line_len += 4;
	}

	if (line_len)
		*pos++ = '\n';

	*pos = '\0';
	if (out_len)
		*out_len = pos - out;
	return out;
}


/**
 * base64_decode - Base64 decode
 * @src: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * Returns: Allocated buffer of out_len bytes of decoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
_assetkit_hide
unsigned char*
base64_decode(AkHeap              * __restrict heap,
              void                * __restrict memparent,
              const unsigned char * __restrict src,
              size_t                           len,
              size_t              * __restrict out_len) {
	unsigned char dtable[256], *out, *pos, block[4], tmp;
	size_t        i, count, olen;
	int           pad = 0;

	memset(dtable, 0x80, 256);

	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (unsigned char) i;

	dtable['='] = 0;

	count = 0;
	for (i = 0; i < len; i++) {
		if (dtable[src[i]] != 0x80)
			count++;
	}

	if (count == 0 || count % 4)
		return NULL;

	olen = count / 4 * 3;
	pos  = out = ak_heap_alloc(heap, memparent, olen);
	if (out == NULL)
		return NULL;

	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[src[i]];
		if (tmp == 0x80)
			continue;

		if (src[i] == '=')
			pad++;
		block[count] = tmp;
		count++;
		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) | block[3];
			count = 0;
			if (pad) {
				if (pad == 1)
					pos--;
				else if (pad == 2)
					pos -= 2;
				else {
					/* Invalid padding */
					ak_free(out);
					return NULL;
				}
				break;
			}
		}
	}

	*out_len = pos - out;
	return out;
}

_assetkit_hide
void
base64_buff(const char * __restrict b64,
            size_t                  len,
            AkBuffer   * __restrict buff) {
  const char *b64Data;

  b64Data = strchr(b64, ';');
  if (b64Data) {
    if (strncmp(b64Data, ";base64,", strlen(";base64,")) == 0) {
      b64Data = strchr(b64, ',') + 1;
      
      buff->data = base64_decode(ak_heap_getheap(buff),
                                 buff,
                                 (const unsigned char *)b64Data,
                                 len - (uintptr_t)(b64Data - b64),
                                 &buff->length);
    }
  }
}
