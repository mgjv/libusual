/* $OpenBSD: tls_internal.h,v 1.11 2015/02/22 14:50:41 jsing Exp $ */
/*
 * Copyright (c) 2014 Jeremie Courreges-Anglas <jca@openbsd.org>
 * Copyright (c) 2014 Joel Sing <jsing@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HEADER_TLS_INTERNAL_H
#define HEADER_TLS_INTERNAL_H

#include <openssl/ssl.h>

#define HTTPS_PORT "443"

#define _PATH_SSL_CA_FILE USUAL_TLS_CA_FILE

/*
 * Anything that is not completely broken.
 *
 * Also fixes 3DES ordering bug in older OpenSSLs.
 */
#define TLS_CIPHERS_COMPAT	"HIGH:+3DES:!aNULL"

/*
 * TLSv1.2+AEAD+ECDHE/DHE.  CBC modes are dubious due to spec bugs in TLS.
 */
#define TLS_CIPHERS_DEFAULT	"HIGH+EECDH:HIGH+EDH:!SSLv3:!SHA384:!SHA256:!DSS:!aNULL"

/*
 * Prefer performance if it does not affect security.
 *
 * ECDH > DH > RSA, AESGCM > CBC, TLSv1.2 > TLSv1.0, AES128 > AES256.
 */
#define TLS_CIPHERS_FAST	"HIGH+EECDH:HIGH+EDH:HIGH+RSA:+AES256:+SHA256:+SHA384:+SSLv3:+EDH:+RSA:!3DES:!CAMELLIA:!DSS:!aNULL"

struct tls_config {
	const char *ca_file;
	const char *ca_path;
	char *ca_mem;
	size_t ca_len;
	const char *cert_file;
	char *cert_mem;
	size_t cert_len;
	const char *ciphers;
	int dheparams;
	int ecdhecurve;
	const char *key_file;
	char *key_mem;
	size_t key_len;
	uint32_t protocols;
	int verify_cert;
	int verify_depth;
	int verify_name;
};

#define TLS_CLIENT		(1 << 0)
#define TLS_SERVER		(1 << 1)
#define TLS_SERVER_CONN		(1 << 2)
#define TLS_CONNECTING		(1 << 3)
#define TLS_ESTABLISHED		(1 << 4)
#define TLS_ABORT		(1 << 5)

#define TLS_KEEP_FLAGS		(TLS_CLIENT | TLS_SERVER | TLS_SERVER_CONN)

struct tls {
	struct tls_config *config;
	uint64_t flags;

	int err;
	char *errmsg;

	int socket;

	SSL *ssl_conn;
	SSL_CTX *ssl_ctx;
};

struct tls *tls_new(void);
struct tls *tls_server_conn(struct tls *ctx);

int tls_check_servername(struct tls *ctx, X509 *cert, const char *servername);
int tls_configure_keypair(struct tls *ctx);
int tls_configure_server(struct tls *ctx);
int tls_configure_ssl(struct tls *ctx);
int tls_configure_verify(struct tls *ctx);
int tls_host_port(const char *hostport, char **host, char **port);
int tls_set_error(struct tls *ctx, char *fmt, ...)
    __attribute__((__format__ (printf, 2, 3)))
    __attribute__((__nonnull__ (2)));
int tls_ssl_error(struct tls *ctx, SSL *ssl_conn, int ssl_ret,
    const char *prefix);

#endif /* HEADER_TLS_INTERNAL_H */
