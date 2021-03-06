
#ifndef _USUAL_TLS_COMPAT_H_
#define _USUAL_TLS_COMPAT_H_

#include <usual/tls/tls.h>

#ifdef USUAL_LIBSSL_FOR_TLS

#include <usual/string.h>
#include <usual/socket.h>

#include <openssl/ssl.h>

#if OPENSSL_VERSION_NUMBER < 0x10100000L || defined(LIBRESSL_VERSION_NUMBER)
#define USE_LIBSSL_INTERNALS
#endif

/* ecdh_auto is broken - ignores main EC key */
#undef SSL_CTX_set_ecdh_auto

/* dh_auto seems fine, but use ours to get DH info */
#undef SSL_CTX_set_dh_auto

#ifndef SSL_CTX_set_dh_auto
long SSL_CTX_set_dh_auto(SSL_CTX *ctx, int onoff);
#endif

#ifndef SSL_CTX_set_ecdh_auto
long SSL_CTX_set_ecdh_auto(SSL_CTX *ctx, int onoff);
#endif

#ifndef HAVE_SSL_CTX_USE_CERTIFICATE_CHAIN_MEM
int SSL_CTX_use_certificate_chain_mem(SSL_CTX *ctx, void *buf, int len);
#endif

#ifndef HAVE_SSL_CTX_LOAD_VERIFY_MEM
int SSL_CTX_load_verify_mem(SSL_CTX *ctx, void *buf, int len);
#endif

#ifdef OPENSSL_IS_BORINGSSL
#define SSL_CTX_set_tlsext_status_cb(a,b) (1)
#define SSL_set_tlsext_status_type(a,b) (1)
#endif

#endif /* USUAL_LIBSSL_FOR_TLS */

#endif /* _USUAL_TLS_COMPAT_H_ */

